#include "../include/project_pkg/Imu.h"

bool imu_on = false;
float roll = 0, pitch = 0, yaw = 0, imu_x = 0, imu_y = 0, imu_z = 0, init_x = 0, init_y = 0, init_z = 0, init_roll = 0, init_yaw = 0, init_pitch = 0;

ImuSensor::ImuSensor(ros::NodeHandle n) :
    status(0),
    n_(n)
{
    //imu_sub = n_.subscribe("/imu/data", 1, &ImuSensor::Callback, this);
    imu_sub2 = n_.subscribe("/imu_data_str", 1, &ImuSensor::Callback2, this);
}

bool first_time = false;

void ImuSensor::Callback (const sensor_msgs::Imu::ConstPtr& imu_msg)
{
    pthread_mutex_lock(&mutexI);
    if(syncronize_devices == true)
    {
        std::cout << std::endl << "Imu waiting!" << std::endl << std::endl;
        pthread_cond_wait(&cond2,&mutexI);
    }
    dadosImu.orientation.x = imu_msg->orientation.x;
    dadosImu.orientation.y = imu_msg->orientation.y;
    dadosImu.orientation.z = imu_msg->orientation.z;
    dadosImu.orientation.w = imu_msg->orientation.w;
    dadosImu.angular_vel.x = imu_msg->angular_velocity.x;
    dadosImu.angular_vel.y = imu_msg->angular_velocity.y;
    dadosImu.angular_vel.z = imu_msg->angular_velocity.z;
    dadosImu.linear_vel.x  = imu_msg->linear_acceleration.x;
    dadosImu.linear_vel.y  = imu_msg->linear_acceleration.y;
    dadosImu.linear_vel.z  = imu_msg->linear_acceleration.z;
	
	if(first_time == false){
		init_x = dadosImu.orientation.x;
		init_y = dadosImu.orientation.y;
		init_z = dadosImu.orientation.z;
		first_time = true;	
	}
	
	imu_x = dadosImu.orientation.x;
	imu_y = dadosImu.orientation.y;
	imu_z = dadosImu.orientation.z;
	imu_on = true;
	//    printf("\n Inicial p-> x:%f  y:%f  z:%f",init_x,init_y,init_z);
    //printf("\n Orientação-> x:%f  y:%f  z:%f",(init_x - imu_x)*180/M_PI,(init_y - imu_y)*180/M_PI,(init_z - imu_z)*180/M_PI);
    if(syncronize_devices == true)
    {
        pthread_cond_signal(&cond3);
    }
pthread_mutex_unlock(&mutexI);
}

bool first_time2 = false;

void ImuSensor::Callback2 (const std_msgs::String::ConstPtr& imu_msg)
{

    pthread_mutex_lock(&mutexI);
    if(syncronize_devices == true)
    {
        std::cout << std::endl << "Imu waiting!" << std::endl << std::endl;
        pthread_cond_wait(&cond2,&mutexI);
    }
    std::string aux = (std::string)imu_msg->data;
    std::string r = "", p = "", y = "";
    int i;
    for(i = 45; i < 74; i ++){
    if((aux[i] <= '9' && aux[i] >= '0') || aux[i] == '.' || aux[i] == '-' )
        y += aux[i];
    }
    for(i = 76; i < 97; i ++){
    if((aux[i] <= '9' && aux[i] >= '0') || aux[i] == '.' || aux[i] == '-' )
        r += aux[i];
    }
    for(i = 107; i < aux.length()-2; i ++){
    if((aux[i] <= '9' && aux[i] >= '0') || aux[i] == '.' || aux[i] == '-' )
        p += aux[i];
    }
    //std::cout << y << std::endl ;

	if(first_time2 == false){
		init_roll = std::stod(r.c_str());
		init_pitch = std::stod(p.c_str());
		init_yaw = std::stod(y.c_str());
		first_time2 = true;	
	}
	
    dadosImu.yaw = std::stod(y.c_str());
    dadosImu.roll = std::stod(r.c_str());
    dadosImu.pitch = std::stod(p.c_str());
	
	roll = ceil(dadosImu.roll*pow(10,2)) / pow(10,2) ;
	pitch = ceil(dadosImu.pitch*pow(10,2)) / pow(10,2);
	yaw = ceil(dadosImu.yaw*pow(10,2)) / pow(10,2);
		
//	std::cout << "Yaw: " << yaw << std::endl;
//	std::cout << "Roll: " << dadosImu.roll << std::endl;
//	std::cout << "Pitch: " << pitch << std::endl;
		
	imu_on = true;
	
    if(syncronize_devices == true)
    {
        pthread_cond_signal(&cond3);
    }
	pthread_mutex_unlock(&mutexI);
	
	if(getStatus() == 1){
   		this->imu_sub2.shutdown();
   		imu_on = false;
   	}
}

void ImuSensor::SaveFile(){
    int k;
    k = pthread_create(&fileThread,NULL,fileThreadFunc,(void*)this);
    if(k)
	std::cout << std::endl <<"Falha imu" << std::endl; 
}

void* ImuSensor::fileThreadFunc(void* arg)
{
    ImuSensor* I;
    I = (ImuSensor*)arg;
    std::ofstream arq;
    int i;

    arq.open("imu_data.txt");				  

              			 
        arq << I->dadosImu.orientation.x << "  ";
        arq << I->dadosImu.orientation.y << "  ";
        arq << I->dadosImu.orientation.z << "  ";
        arq << I->dadosImu.orientation.w << "  ";			 
        arq << std::endl;
    
    std::cout << std::endl << std::endl << "Imu File Created!" << std::endl << std::endl;
    arq.close();
    pthread_exit(NULL);
}

int ImuSensor::getStatus()
{
	return status;
}

void ImuSensor::setStatus(int val)
{
	status = val;
}
