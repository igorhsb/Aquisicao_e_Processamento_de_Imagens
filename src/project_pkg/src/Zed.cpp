#include "../include/project_pkg/Zed.h"
#include <pcl/common/common_headers.h>

bool zed_new_pointcloud = false;

ZedDevice::ZedDevice():
    status(0),
    cont(0),
  	zedCloud(new pcl::PointCloud<pcl::PointXYZRGB>)	
{

}
ZedDevice::~ZedDevice(){
    Stop();
}

void ZedDevice::Init(){
    errorCode.clear();
    sl::InitParameters init_params;
    init_params.sdk_verbose = true;
    init_params.depth_mode = sl::DEPTH_MODE_PERFORMANCE;
    init_params.coordinate_units = sl::UNIT_METER;
    init_params.camera_resolution = sl::RESOLUTION_HD720;
    init_params.coordinate_system = sl::COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP;
    init_params.camera_fps = 15;

    sl::ERROR_CODE err = zed.open(init_params);
    errorCode.append(sl::errorCode2str(err));
    if (err == sl::SUCCESS) {
        runtime_parameters.sensing_mode = sl::SENSING_MODE_STANDARD;
        status = 1;
		situation.append("ZED sucefull connect!\n");
	    zedCloud->points.resize(1280*720);

		situation_change = true;
    }
    else
    {
        std::cout << std::endl << "Unable to init the ZED:" << errorCode << std::endl;
		situation.append("Unable to init the ZED \n ");
		//situation.append(errorCode);
		situation_change = true;
    }
    
}

int ZedDevice::StreamData(){
    int k;
    k = pthread_create(&streamThread,NULL,streamFunc,(void*)this);
    return k;
}

void* ZedDevice::streamFunc(void* arg)
{
	std::cout << "213124" << std::endl;
    bool first = true;
    ZedDevice* zedDevice;
    zedDevice = (ZedDevice*)arg;
/*    while(zed_new_pointcloud = true){
        
        /*if(syncronize_devices == true)
        {
            pthread_mutex_lock(&mutexZ);
            if (!first){
                //    pthread_cond_wait(&cond3,&mutex);
            }
        }

        if (zedDevice->zed.retrieveMeasure(zedDevice->pointCloud,sl::MEASURE_XYZRGBA) == sl::SUCCESS)
        {
   //         zedDevice->convertSl2Cv(2,zedDevice->pointCloud);
            std::cout << "adasdasdasd" << std::endl;
            //zed_new_pointcloud = false;
            
        }
 /*       
        if(syncronize_devices == true)
        {
            pthread_cond_signal(&cond1);
            first = false;
            pthread_mutex_unlock(&mutexZ);
        }
        
    }*/
}


void ZedDevice::Stop(){
    status = 0;
    pthread_exit(&streamThread);
    zed.close();
}

bool hehe = true;

void ZedDevice::convertSl2Cv(int i,sl::Mat &input){
    int cv_type = -1;
    switch (input.getDataType()) {
    case sl::MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
    case sl::MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
    case sl::MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
    case sl::MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
    case sl::MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
    case sl::MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
    case sl::MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
    case sl::MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
    default: break;
    }
    if(i == 0)
        rgbZ = cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM_GPU));
    else if(i == 1)
        depthView = cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM_GPU));
    else
        pcd = cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM_CPU)); 
	if(hehe == true){
		//cv::imshow("Image", rgbZ);
		//cv::imwrite("sync1.jpg",rgbZ);
		hehe = false;	
		std::cout << std::endl <<  "Done " << std::endl;
	}

}

int ZedDevice::getStatus(){
    return status;
}

void ZedDevice::SaveFile(){
	pcl::io::savePCDFileASCII("zed_cloud.pcd", *zedCloud);
}

void* ZedDevice::fileThreadFunc(void* arg)
{
    ZedDevice* Z;
    Z = (ZedDevice*)arg;
    std::ofstream arq,arq2;
    int i,j;

    arq.open("zed_depth_data.txt");
    arq2.open("zed_rgb_data.txt");

    for (i = 0 ; i < 1280; i++) {
        for(j = 0; j < 720; j++){
            arq << Z->depthView.at<float>(i,j)<<"  ";
            arq2 << Z->rgbZ.at<float>(i,j)<<"  ";
        }
        arq << std::endl;
        arq2 << std::endl;
    }
    std::cout << std::endl << std::endl << "Zed Files Created!" << std::endl << std::endl;
	situation.clear();
	situation.append("Zed Files Created!");
	situation_change = true;
}

void ZedDevice::cv2file()
{
    CvMat cm = rgbZ;
    //cvSave("zed_rgb.xml",&cm);
    CvMat cm2 = depthView;
    //cvSave("zed_depth.xml",&cm2);
    CvMat cm3 = pcd;
    //cvSave("zed_pcd.xml",&cm3);
    //std::cout << "Arquivos salvos!" << std::endl << std::endl << std::endl;
}

void ZedDevice::cloud2pcl(sl::Mat& input){
	float *p_data_cloud = input.getPtr<float>();
    int index = 0;

    // Check and adjust points for PCL format
    for (auto &it : zedCloud->points) {
        float X = p_data_cloud[index];
        if (!isValidMeasure(X)) // Checking if it's a valid point
            it.x = it.y = it.z = it.rgb = 0;
        else {
            it.x = X;
            it.y = p_data_cloud[index + 1];
            it.z = p_data_cloud[index + 2];
            it.rgb = convertColor(p_data_cloud[index + 3]); // Convert a 32bits float into a pcl .rgb format
        }
		index += 4;
	}
	zedCloud->width = 1280;
	zedCloud->height = 720; 
}

float ZedDevice::convertColor(float colorIn){
    uint32_t color_uint = *(uint32_t *)&colorIn;
    unsigned char *color_uchar = (unsigned char *)&color_uint;
    color_uint = ((uint32_t)color_uchar[0] << 16 | (uint32_t)color_uchar[1] << 8 | (uint32_t)color_uchar[2]);
    return *reinterpret_cast<float *>(&color_uint);
}

