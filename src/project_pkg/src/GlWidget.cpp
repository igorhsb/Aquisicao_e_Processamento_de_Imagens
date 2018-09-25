#include "../include/project_pkg/GlWidget.h"
#include <QtGui/QMouseEvent>

int dispCB = 0, dispCB2 = 0, vModCB = 0, VModCB2 = 0, saveSignal = 0, exitCond = 0, syncCond = 0;
bool changeBTN = false, syncronize_devices = false;
QString situation;
bool situation_change = false;
clock_t ck1, ck2, ck3, ck4, ck5, ckT;
double time1 = 0, time2 = 0, time3 = 0, time4 = 0, time5 = 0, timeT = 0;

GLWidget::GLWidget(QWidget *parent, int stat) :
    QGLWidget(parent),
    requested_format(FREENECT_VIDEO_RGB),
    freenect_angle(0),
    got_frames(0),
    status(stat),
    viewStatus(0),
    rotate(0),
    modoVideo("RGB"),
    kinectCloud(new sensor_msgs::PointCloud),
    depthData(new uint16_t),
    zedOn(0),
    laserOn(0),
    imuOn(0),
    id(0)
{
    setMouseTracking(true);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);
    
    zed_timer = new QTimer();
    connect(zed_timer, SIGNAL(timeout()), this, SLOT(zedCallback()));
    
    strFragmentShad = ("uniform sampler2D texImage;\n"
                       " void main() {\n"
                       " vec4 color = texture2D(texImage, gl_TexCoord[0].st);\n"
                       " gl_FragColor = vec4(color.b, color.g, color.r, color.a);\n}");
                       
  
  shader_s = //"#version 330 core"
			 //"layout(location = 0) in vec4 in_VertexRGBA;"
			 "uniform float scale_val, dev;"
  			 "attribute vec4 position;"
  			 "attribute vec4 colorVec;"
             "varying vec4 color;"
             "uniform mat4 matrix;"
             "float device = 1*dev;"
			 "vec4 decomposeFloat(const in float value){"
		 	 "   uint rgbaInt = floatBitsToUint(value);"
			 "	uint bIntValue = (rgbaInt / 256U / 256U) % 256U;"
			 "	uint gIntValue = (rgbaInt / 256U) % 256U;"
			 "	uint rIntValue = (rgbaInt) % 256U; "
		 	 "	return vec4(rIntValue / 255.0f, gIntValue / 255.0f, bIntValue / 255.0f, 1.0); "
			"}"
			 "void drawZed(){"
			 "	 device = 1*dev;"
			 "   vec3 pos = vec3(1.0);"
             "   pos = position.xyz*scale_val;"
             // Decompose the 4th channel of the XYZRGBA buffer to retrieve the color of the point (1float to 4uint)
             "   color = decomposeFloat(position.a);"
             "   gl_Position = matrix * vec4(pos, 1.0);"
			 "}"
			 "void drawLaser(){"
			 "	 device = 1*dev;"
			 "   vec3 pos = vec3(1.0);"
             "   pos = position.xyz*scale_val;"
             "   color = vec4(1,0,0,1);"
             "   gl_Position = matrix * vec4(pos, 1.0);"
			 "}"
			 "void drawAstra(){"
			 "	 device = 1*dev;"
			 "   vec3 pos = vec3(1.0);"
             "   pos = position.xyz*scale_val;"
             "   color = decomposeFloat(colorVec.x);"
             //"   color = vec4(1.0);"
             "   gl_Position = matrix * vec4(pos, 1.0);"
			 "}"
             "void main(){"
             "	 device = 1*dev;"
             "	if(scale_val == 1.3f){"
             "		drawZed();"
             "	}"
             "	if(scale_val == 0.3f){"
             "		drawLaser();"
             "	}"
             "	if(scale_val == 0.5f){"
             "		drawAstra();"
             "	}"
             "}";
				
  
  fragment_s = "varying vec4 color;"
               "	float remap(float minval, float maxval, float curval){"
               "		return (curval - minval)/(maxval - minval);"
               "	}"
               "	vec4 hsl(float h, float s, float l)"
               "{"
               "	float v = (l <= 0.5) ? (l * (1.0 + s)) : (l + s - l * s);"
               "	float r = 1.00;"
               "	float g = 1.00;"
               "	float b = 1.00;"
               "	if(v > 0.0)"
               "	{"
               "		float m;"
               "		float sv;"
               "		int sextant;"
               "		float fract, vsf, mid1, mid2;"
               "		m = l + l - v;"
               "		sv = (v - m ) / v;"
               "		h *= 6.0;"
               "		sextant = int(h);"
               "		fract = h - float(sextant);"
               "		vsf = v * sv * fract;"
               "		mid1 = m + vsf;"
               "		mid2 = v - vsf;"
               "		if (sextant == 0)"
               "		{"
               "				r = v;"
               "				g = mid1;"
               "				b = m;"
               "		}"
               "		else if(sextant == 1)"
               "		{"
               "				r = mid2;"
               "				g = v;"
               "				b = m;"
               "		}"
               "		else if(sextant == 2)"
               "		{"
               "				r = m;"
               "				g = v;"
               "				b = mid1;"
               "		}"
               "		else if(sextant == 3)"
               "		{"
               "				r = m;"
               "				g = mid2;"
               "				b = v;"
               "		}"
               "		else if(sextant == 4)"
               "		{"
               "				r = mid1;"
               "				g = m;"
               "				b = v;"
               "		}"
               "		else if(sextant == 5)"
               "		{"
               "				r = v;"
               "				g = m;"
               "				b = mid2;"
               "		}"
               "	}"
               "	return vec4(r, g, b, 1.0);"
               "}	"
               "void main(){"
               "   gl_FragColor = color;"
               //"   gl_FragColor = hsl(remap(0.0, 1.0, color), 1.0, 0.5);"
               "}";
  
    zedLeftImage.free();
    zedDepthImage.free();
    
    dispCB = 0;
    dispCB2 = 0;
    
    
    //std::cout << "Setup the GPIO Pins" << std::endl;

	//redLED = gpio37 ;     // Ouput
    pushButton = gpio36; // Disp1
    pushButton2 = gpio219; // Disp2
    pushButton3 = gpio37;  // Vmod1
    pushButton4 = gpio63;  // Vmod2
    exitButton = gpio186;  //power_off
    saveFButton = gpio184;  //save_file
    syncButton = gpio38;  //sync
    
    // Make the button and led available in user space
    gpioExport(pushButton) ;
    gpioExport(pushButton2) ;
    gpioExport(pushButton3) ;
    gpioExport(pushButton4) ;
    gpioExport(exitButton) ;
    gpioExport(saveFButton) ;
    gpioExport(syncButton) ;
    //gpioExport(redLED) ;
    gpioSetDirection(pushButton,inputPin) ;
    gpioSetDirection(pushButton2,inputPin) ;
    gpioSetDirection(pushButton3,inputPin) ;
    gpioSetDirection(pushButton4,inputPin) ;
    gpioSetDirection(exitButton,inputPin) ;
    gpioSetDirection(saveFButton,inputPin) ;
    gpioSetDirection(syncButton,inputPin) ;
    //gpioSetDirection(redLED,outputPin) ;
    // Reverse the button wiring; this is for when the button is wired
    // with a pull up resistor
    // gpioActiveLow(pushButton, true);
}


GLWidget::~GLWidget(){
    timer->stop();
    Stop();
    pthread_cond_signal(&cond1);
    pthread_cond_signal(&cond2);
    pthread_cond_signal(&cond3);
}

void GLWidget::Set_id(int value)
{
	id = value;
}

void GLWidget::initializeGL()
{
    if(status!=1){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    resizeGL(630, 400);
    GLenum err = glewInit();
   	if (err == GLEW_OK)
  	{
     PCprogram = glCreateProgram();

    const char* shader_src = shader_s.c_str();
    const char* frag_src = fragment_s.c_str();

    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, (const char**) &shader_src, NULL);
    glCompileShader(shader);
    GLint compile_status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) std::cout << std::endl << "Shader compile error" << std::endl;
    //std::cout << std::endl << "Shader: " << compile_status << std::endl;

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char**)&frag_src, NULL);
    glCompileShader(fragment);
    GLint compile_status2 = GL_FALSE;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compile_status2);
    if (compile_status2 != GL_TRUE) std::cout << std::endl << "Fragment compile error" << std::endl;
    //std::cout << std::endl << "Frag:" << compile_status2 << std::endl;

    glAttachShader(PCprogram, shader);
    glAttachShader(PCprogram, fragment);

    glLinkProgram(PCprogram);
    GLint link_status = GL_FALSE;
    glGetProgramiv(PCprogram, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) std::cout << std::endl << "Error Linking!" << std::endl;
    //std::cout << std::endl << "Link: " << link_status << std::endl << std::endl;

    glDeleteShader(shader);
    glDeleteShader(fragment);

    position = glGetAttribLocation(PCprogram, "position");
    matrix = glGetUniformLocation(PCprogram, "matrix");
    scale = glGetUniformLocation(PCprogram, "scale_val");
    device_c = glGetUniformLocation(PCprogram, "dev");
    color_vec = glGetAttribLocation(PCprogram, "colorVec");
    printf("Pos: %d, Mat: %d, Sca: %d, Dev: %d, Color_vec: %d\n", position, matrix, scale, device_c, color_vec);
    
    }else{
    	situation.clear();
		situation.append("GLEW Error!");
		situation_change = true;
    	std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    }
    }
    else if(status == 1)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0);
        glDepthFunc(GL_LESS);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glShadeModel(GL_SMOOTH);
        glGenTextures(1, &gl_depth_tex);
        glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenTextures(1, &gl_rgb_tex);
        glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        resizeGL(630, 400);
    }
}

void GLWidget::updateGL()
{
    glDraw();
}

void GLWidget::paintGL()
{
	testGPIO();
    if(status==0){
        glClear(GL_COLOR_BUFFER_BIT);
       
        glMatrixMode(GL_MODELVIEW);
        glRotated(1,0,1,0);
        glPushMatrix();
       
        /*
        // X-Axis
        glColor3f(0,0.5f,0);
        glBegin(GL_POLYGON); 
			glVertex3f(315, 190, 0); 
			glVertex3f(315, 210, 0); 
			glVertex3f(415, 210, 0); 
			glVertex3f(415, 190, 0); 
			glVertex3f(315, 190, 0);
		
			glVertex3f(315, 190, 20); 
			glVertex3f(315, 210, 20); 
			glVertex3f(415, 210, 20); 
			glVertex3f(415, 190, 20); 
			glVertex3f(315, 190, 20);
		glEnd(); 
		glBegin(GL_POLYGON);
			glVertex3f(415, 180, 0);
			glVertex3f(415, 220, 0);
			glVertex3f(430, 200, 0);
			glVertex3f(415, 180, 0);
			
			glVertex3f(415, 180, 20);
			glVertex3f(415, 220, 20);
			glVertex3f(430, 200, 20);
			glVertex3f(415, 180, 20);
		glEnd();

        // Y-Axis
        glColor3f(0,0,0.5f);
        glBegin(GL_POLYGON); 
			glVertex3f(305, 200, 0); 
			glVertex3f(325, 200, 0); 
			glVertex3f(325, 100, 0); 
			glVertex3f(305, 100, 0); 
			glVertex3f(305, 200, 0);
		
			glVertex3f(305, 200, 20); 
			glVertex3f(325, 200, 20); 
			glVertex3f(325, 100, 20); 
			glVertex3f(305, 100, 20); 
			glVertex3f(305, 200, 20);
		glEnd(); 
		glBegin(GL_POLYGON);
			glVertex3f(295, 100, 0);
			glVertex3f(335, 100, 0);
			glVertex3f(315, 85, 0);
			glVertex3f(295, 100, 0);
			
			glVertex3f(295, 100, 20);
			glVertex3f(335, 100, 20);
			glVertex3f(315, 85, 20);
			glVertex3f(295, 100, 20);
		glEnd();
		
		
		// Z-Axis
        glColor3f(0.5f,0,0);
        glBegin(GL_POLYGON); 
			glVertex3f(305, 165, 0); 
			glVertex3f(305, 185, 0); 
			glVertex3f(325, 185, 0); 
			glVertex3f(325, 165, 0); 
			glVertex3f(305, 165, 0);
		
			glVertex3f(305, 165, 100); 
			glVertex3f(305, 185, 100); 
			glVertex3f(325, 185, 100); 
			glVertex3f(325, 165, 100); 
			glVertex3f(305, 165, 100);
		glEnd(); 
		glBegin(GL_POLYGON);
			glVertex3f(295, 165, 100);
			glVertex3f(335, 165, 100);
			glVertex3f(315, 165, 115);
			glVertex3f(295, 165, 100);
			
			glVertex3f(295, 185, 100);
			glVertex3f(335, 185, 100);
			glVertex3f(315, 185, 115);
			glVertex3f(295, 185, 100);
		glEnd();
		*/
		
		/*glColor3f(0,0.5f,0);
        
        glBegin(GL_POLYGON); // Draw A Quad
		glVertex3f(200, 200, 0); // Top Left
		glVertex3f(200, 300, 0); // Left
		glVertex3f(300, 300, 0); // Bottom Left
		glVertex3f(300, 200, 0); // Top Right
		glEnd(); 
		*/
        glPopMatrix();
    }
    else if(status==1)
        KinectDraw();
    else if(status==2)
        LaserDraw();
    else if(status == 3)
        AstraDraw();
    else if(status == 4)
        ZedDraw();
}

void GLWidget::resizeGL(int Width, int Height)
{
    glViewport(0,0,Width,Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, Width, Height, 0, -50000.0f, 50000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {
   
    if(status==1)
    {
        if (event->key() == Qt::Key_Escape) {
            status = 0;
            printf("\n\nEsc");
        }
        if (event->key() == Qt::Key_1) {
            device->setLed(LED_GREEN);
        }
        if (event->key() == Qt::Key_2) {
            device->setLed(LED_RED);
        }
        if (event->key() == Qt::Key_3) {
            device->setLed(LED_YELLOW);
        }
        if (event->key() == Qt::Key_4) {
            device->setLed(LED_BLINK_GREEN);
        }
        if (event->key() == Qt::Key_5) {
            device->setLed(LED_BLINK_RED_YELLOW);
        }
        if (event->key() == Qt::Key_6) {
            device->setLed(LED_OFF);
        }
        if (event->key() == Qt::Key_M) {
            modoVideo.clear();
            if (requested_format == FREENECT_VIDEO_IR_8BIT){
                requested_format = FREENECT_VIDEO_RGB;
                modoVideo.append("RGB");
            }
            else if (requested_format == FREENECT_VIDEO_RGB){
                requested_format = FREENECT_VIDEO_YUV_RGB;
                modoVideo.append("RGB_YUV");
            }
            else{
                requested_format = FREENECT_VIDEO_IR_8BIT;
                modoVideo.append("8 Bit");
            }
            device->setVideoFormat(requested_format);
        }

        if (event->key() == Qt::Key_W) {
            printf("\n\nW");
            freenect_angle++;
            if (freenect_angle > 30) {
                freenect_angle = 30;
            }
        }
        if (event->key() == Qt::Key_S) {
            freenect_angle = 0;
        }
        if (event->key() == Qt::Key_X) {
            freenect_angle--;
            if (freenect_angle < -30) {
                freenect_angle = -30;
            }
        }
        updateGL();
        device->setTiltDegrees(freenect_angle);
    }
    else if(status == 0)
    {
        if (event->key() == Qt::Key_X){
            printf("\n\nX");
        }
    }
    else if(status == 2)
    {

    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  this->setFocus();
  lastPos = event->pos();
  rotateEnabled = true;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  rotateEnabled = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  if(rotateEnabled == true){
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    xRot += dx;
    yRot += dy;
    xRot = xRot % 360;
    yRot = yRot % 360;
    lastPos = event->pos();
  }
}

void GLWidget::mouseWheelEvent(QWheelEvent *event){
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    	//scale_vec += 0.1*numSteps; 
   // std::cout << std::endl << "Mouse Wheel" << std::endl;
}

void GLWidget::setXRotation(int angle)
{
  if (angle != xRot) {
    xRot = angle;
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    updateGL();
  }
}

void GLWidget::setYRotation(int angle)
{
  if (angle != yRot) {
    yRot = angle;
    glRotatef(yRot / 16.0, 1.0, 0.0, 0.0);
    updateGL();
  }
}

void GLWidget::setZRotation(int angle)
{
  if (angle != zRot) {
    zRot = angle;
    glRotatef(zRot / 16.0, 1.0, 0.0, 0.0);
    updateGL();
  }
}
void GLWidget::SetStatus(int s){
    this->status = s;
    timer->stop();
    if (!timer->isActive()) timer->start(10);
    updateGL();
}

int GLWidget::GetStatus(){
    return this->status;
}

void GLWidget::Start(float val)
{
    timer->start(val);
    updateGL();
}

void GLWidget::Stop()
{
	situation.clear();
	situation.append("Stop Device..");
	situation_change = true;
    if(status == 1){
        device->stopDepth();
        device->stopVideo();
        this->freenect.deleteDevice(0);
    }
    if(status == 4){
    	if(syncCond != 1){
    		zed_timer->stop();
		    zedLeftImage.free();
		    zedDepthImage.free();
		    glDeleteShader(shaderF);
		    glDeleteProgram(program);
		    glBindTexture(GL_TEXTURE_2D, 0);
		    zedOn == 0;
		    free(colorPoints);
        }
    }
    if(status == 2){
    	if(syncCond != 1){
    		laser->setStatus(1);
    	}
    }
    if(status == 3){
    	if(syncCond != 1){
    		astraDevice->setStatus(1);
    	}
    }
    if(status == 5){
    	if(syncCond != 1){
    		imuDevice->setStatus(1);
    	}
    }
    
    SetStatus(0);
    updateGL();
}

// Init Functions ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float *pointsL;

void GLWidget::AstraInit(ros::NodeHandle nodeH){
    makeCurrent();
    if(syncCond == 0){
    	delete astraDevice;
    	astraDevice = new Astra_Camera(nodeH);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &rgbAstra);
    glBindTexture(GL_TEXTURE_2D, rgbAstra);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, rgbAstra);
    
    glGenTextures(1, &depthAstra);
    glBindTexture(GL_TEXTURE_2D, depthAstra);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, depthAstra);

    GLenum err = glewInit();
	   	
    if (err == GLEW_OK)
   	{
		//PointCloud Init
		int w = 640, h = 480; // -w*h = 729
		size = w*h;
		/*pointsL = new float[5*size];
		int k,i=0;
		float _w = 2.0f/w;
		float _h = 2.0f/h;
		float z_inc = 2.0/(w*h);
		float z = -1.0f;

		for(int k = 0; k < h ; k++){
		for(int l = 0; l < w ; l++){
		i = 5*(k*w + l);
			pointsL[i] = (l*_w) - 1.0f;
			pointsL[i + 1] = (k*_h) - 1.0f;
			pointsL[i + 2] = z;
			pointsL[i + 3] = 255*255*255*255;
			pointsL[i + 4] = 12312;
			z += z_inc;
		}
		}   */
		
		scale_vec = 0.5;
		dev_val = 1;
		makeCurrent();       			   
		swapBuffers();
		SetStatus(3);
	}else{
		situation.clear();
		situation.append("GLEW Error!");
		situation_change = true;
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
	}
   
    updateGL();
}

void GLWidget::ImuInit(ros::NodeHandle nodeH){
	//situation.clear();
	//situation.append("IMU");
	//situation_change = true;
    if(syncCond == 0){
    	delete imuDevice;
        imuDevice = new ImuSensor(nodeH);
    }
    SetStatus(5);
    updateGL();
}

void GLWidget::LaserInit(ros::NodeHandle nodeH){
	if(syncCond == 0){
	   delete laser;
	   laser = new LaserS(nodeH);
	}
	GLenum err = glewInit();
	   	
	
	    if (err == GLEW_OK)
	   	{
			//PointCloud Init
			int w = 27, h = 27; // -w*h = 729
			size = 6*w*h;
			/*pointsL = new float[4*size];
			int k,i=0;
			float _w = 2.0f/w;
			float _h = 2.0f/h;
			float z_inc = 2.0/(w*h);
			float z = -1.0f;
	
			for(int k = 0; k < h ; k++){
			for(int l = 0; l < w ; l++){
			i = 4*(k*w + l);
				pointsL[i] = (l*_w) - 1.0f;
				pointsL[i + 1] = (k*_h) - 1.0f;
				pointsL[i + 2] = z;
				pointsL[i + 3] = 255*255*255;
				z += z_inc;
			}
			}
			*/    
			scale_vec = 0.3;
			dev_val = 0;
			makeCurrent();       			   
			swapBuffers();
			SetStatus(2);
		}else{
			situation.clear();
			situation.append("GLEW Error: ");//+glewGetErrorString(err));
			situation_change = true;
			std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		}
}

void GLWidget::ZedInit()
{
	situation.clear();
    zedDevice.Init();
	
	if(syncCond == 1){
		zedOn++;
	}	
	
    if(zedDevice.getStatus() == 1){
      //  printf("\nZED sucefull connect!");
        makeCurrent();
        
       
  		  GLenum err = glewInit();
   		if (err == GLEW_OK)
  		{
        glEnable(GL_TEXTURE_2D);

        // Create and Register OpenGL Texture for Image (RGBA -- 4channels)
        glGenTextures(1, &imageTex);
        glBindTexture(GL_TEXTURE_2D, imageTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        err1 = cudaGraphicsGLRegisterImage(&pcuImageRes, imageTex, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsWriteDiscard);

        // Create and Register a OpenGL texture for Depth (RGBA- 4 Channels)
        glGenTextures(1, &depthTex);
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        err2 = cudaGraphicsGLRegisterImage(&pcuDepthRes, depthTex, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsWriteDiscard);


        if(err1 == 0 && err2 == 0) // 0 = Sucess
        {

            // Create GLSL fragment Shader for future processing (and here flip R/B)
            GLuint shaderF = glCreateShader(GL_FRAGMENT_SHADER); //fragment shader
            const char* pszConstString = strFragmentShad.c_str();
            glShaderSource(shaderF, 1, (const char**) &pszConstString, NULL);

            // Compile the shader source code and check
            glCompileShader(shaderF);
            GLint compile_status = GL_FALSE;
            glGetShaderiv(shaderF, GL_COMPILE_STATUS, &compile_status);
            if (compile_status != GL_TRUE) SetStatus(0);

            // Create the progam for both V and F Shader
            program = glCreateProgram();
            glAttachShader(program, shaderF);

            glLinkProgram(program);
            GLint link_status = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &link_status);
            if (link_status != GL_TRUE) SetStatus(0);

            glUniform1i(glGetUniformLocation(program, "texImage"), 0);
            
        }else{
        	std::cout << "Falha ao desenhar textura RGB e Depth" << std::endl;
			situation.append("Falha ao desenhar textura RGB e Depth");
			situation_change = true;

        }
        //PointCloud Init
        
			int w = 1280, h = 720;
			size = w*h;
		
			scale_vec = 1.3;
			dev_val = 0;
    
            if(zedOn == 0){
                int k = zedDevice.StreamData();
                zedOn = 1;
            }
            else
            {
                printf("Error");
            }
        swapBuffers();
        zed_timer->start(10);
        if(syncCond == 0 || zedOn > 1)
        	SetStatus(4);
	}
    }
    else
    {
		situation.append("Zed nao inicializada!");
		situation_change = true;
        printf("\nZed não inicializada!");
        SetStatus(0);
    }
}

int GLWidget::KinectStart(){
    int dev,create = 0;
    dev = freenect.deviceCount();
    if(dev>0){
        device = &freenect.createDevice<MyFreenectDevice>(0);
        device->startVideo();
        device->startDepth();
        if (!timer->isActive()) timer->start(10);
        updateGL();
    }
    return dev;
}


// Draw Functions -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool first = true;

void GLWidget::zedCallback(){
	
	if(syncCond == 1){
		pthread_mutex_lock(&mutexZ);
		/*if(first != true){
			//pthread_cond_wait(&cond3,&mutexZ);
		}else{
			first == false;
		}*/
	}
	  
	sl::Camera::sticktoCPUCore(2);
	if(zedDevice.zed.grab(zedDevice.runtime_parameters) == 0){
    if (zedDevice.zed.retrieveImage(zedLeftImage, sl::VIEW_LEFT, sl::MEM_GPU) == sl::SUCCESS) {
        cudaArray_t ArrIm;
        cudaGraphicsMapResources(1, &pcuImageRes, 0);
        cudaGraphicsSubResourceGetMappedArray(&ArrIm, pcuImageRes, 0, 0);
        cudaMemcpy2DToArray(ArrIm, 0, 0, zedLeftImage.getPtr<sl::uchar1>(sl::MEM_GPU), zedLeftImage.getStepBytes(sl::MEM_GPU), zedLeftImage.getWidth() * sizeof(sl::uchar4), zedLeftImage.getHeight(), cudaMemcpyDeviceToDevice);
        cudaGraphicsUnmapResources(1, &pcuImageRes, 0);
        zedDevice.convertSl2Cv(0,zedLeftImage);
    }

    // Map GPU Ressource for Depth. Depth image == 8U 4channels
    if (zedDevice.zed.retrieveImage(zedDepthImage, sl::VIEW_DEPTH, sl::MEM_GPU) == sl::SUCCESS) {
        cudaArray_t ArrDe;
        cudaGraphicsMapResources(1, &pcuDepthRes, 0);
        cudaGraphicsSubResourceGetMappedArray(&ArrDe, pcuDepthRes, 0, 0);
        cudaMemcpy2DToArray(ArrDe, 0, 0, zedDepthImage.getPtr<sl::uchar1>(sl::MEM_GPU), zedDepthImage.getStepBytes(sl::MEM_GPU), zedLeftImage.getWidth() * sizeof(sl::uchar4), zedLeftImage.getHeight(), cudaMemcpyDeviceToDevice);
        cudaGraphicsUnmapResources(1, &pcuDepthRes, 0);
        zedDevice.convertSl2Cv(1,zedDepthImage);
    }
    
    if (zedDevice.zed.retrieveMeasure(zedPointCloud, sl::MEASURE_XYZRGBA) == sl::SUCCESS) {
		zed_new_pointcloud = true;
		//zedDevice.convertSl2Cv(2,zedPointCloud);
		zedDevice.cloud2pcl(zedPointCloud);
    }
}
	
	  if(first != true){
	      /*time4 = ((double)( clock() - ck4 )) / CLOCKS_PER_SEC;
	      timeT = ((double)( clock() - ckT )) / CLOCKS_PER_SEC;
    	  
    	  //cout << "Imu ---> Zed" << time4 << " seconds" << endl;
    	  situation.clear();
    	  if(timeT >= time4){
    	  situation.append("Tempo Total (s): ");
    	  situation.append(QString::number(timeT));
    	  situation.append("\n");
    	  situation.append("Imu ---> Zed (s): ");
    	  situation.append(QString::number(time4));
		  situation.append("\n");
    	  situation.append("Zed ---> Laser (s): ");
    	  situation.append(QString::number(time1));
    	  situation.append("\n");
    	  situation.append("Laser ---> Astra (s): ");
    	  situation.append(QString::number(time2));
    	  situation.append("\n");
    	  situation.append("Astra ---> Imu (s): ");
    	  situation.append(QString::number(time3));
    	  situation.append("\n");
    	  }
    	  
		  situation_change = true;
		  */
	  }
	  if(syncCond == 1)
        {
        	ckT = clock();
        	ck1 = clock();
            pthread_cond_signal(&cond1);
            first = false;
            pthread_mutex_unlock(&mutexZ);
        }

}


void GLWidget::ZedDraw()
{
    glDrawBuffer(GL_BACK); // Write to both BACK_LEFT & BACK_RIGHT
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if(viewStatus == 2)
    {
        glBindTexture(GL_TEXTURE_2D, imageTex);

        glUseProgram(program);

        glBegin(GL_TRIANGLE_FAN);
        glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
        glTexCoord2f(0, 0); glVertex3f(0,0,0);
        glTexCoord2f(1, 0); glVertex3f(315,0,0);
        glTexCoord2f(1, 1); glVertex3f(315,400,0);
        glTexCoord2f(0, 1); glVertex3f(0,400,0);
        glEnd();

        glUseProgram(0);

        // Draw depth texture in right part of side by side
        glBindTexture(GL_TEXTURE_2D, depthTex);

        glBegin(GL_TRIANGLE_FAN);
        glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
        glTexCoord2f(0, 0); glVertex3f(315,0,0);
        glTexCoord2f(1, 0); glVertex3f(630,0,0);
        glTexCoord2f(1, 1); glVertex3f(630,400,0);
        glTexCoord2f(0, 1); glVertex3f(315,400,0);
        glEnd();
    }
    else if(viewStatus == 1)
    {
        // Draw depth texture in right part of side by side
        glBindTexture(GL_TEXTURE_2D, depthTex);

        glBegin(GL_TRIANGLE_FAN);
        glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
        glTexCoord2f(0, 0); glVertex3f(0,0,0);
        glTexCoord2f(1, 0); glVertex3f(630,0,0);
        glTexCoord2f(1, 1); glVertex3f(630,400,0);
        glTexCoord2f(0, 1); glVertex3f(0,400,0);
        glEnd();
        
    }
    else if(viewStatus == 0)
    {
        glBindTexture(GL_TEXTURE_2D, imageTex);

        glUseProgram(program);

        glBegin(GL_TRIANGLE_FAN);
        glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
        glTexCoord2f(0, 0); glVertex3f(0,0,0);
        glTexCoord2f(1, 0); glVertex3f(630,0,0);
        glTexCoord2f(1, 1); glVertex3f(630,400,0);
        glTexCoord2f(0, 1); glVertex3f(0,400,0);
        glEnd();

        glUseProgram(0);
    }
    else if(viewStatus == 3)
    {
	    if(zed_new_pointcloud == true){		    
			glUseProgram(PCprogram);
		
			QMatrix4x4 mat;
	  		float rawMat[16];
			glUniform1f(scale, scale_vec);
			glUniform1f(device_c, dev_val);
			mat.perspective(60.0f, (float)width()/(float)height(), 1.0f, 100.0f);
			mat.translate(0, 0, -3);
			mat.rotate(xRot, 0, 1, 0);
			mat.rotate(yRot, 1, 0, 0);
			for(int i = 0; i < 16; i++)
			  rawMat[i] = (float)mat.constData()[i];
			
			glUniformMatrix4fv(matrix, 1, GL_FALSE, rawMat);
  			glVertexAttribPointer(position, 4, GL_FLOAT, false, 0, zedPointCloud.getPtr<float>());
			glUniform1f(scale, scale_vec);
			glUniform1f(device_c, dev_val);
			glEnableVertexAttribArray(position);
			glDrawArrays(GL_POINTS, 0, size);
			glDisableVertexAttribArray(position);
			zed_new_pointcloud = false;
			
		}
		
		glUseProgram(0);
		
   // }
 
    // Swap
    	swapBuffers();
    	}
    	
}


void GLWidget::KinectDraw()
{
    static std::vector<float> depth(640*480*4);
    static std::vector<uint8_t> rgb(640*480*4);

    device->updateState();
    fflush(stdout);

    device->getDepth(depth);
    device->getRGB(rgb);
    got_frames = 0;

    //    printf("%f",depth[(3*320*240)+2]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(0,0,0);
    glTexCoord2f(1, 0); glVertex3f(315,0,0);
    glTexCoord2f(1, 1); glVertex3f(315,400,0);
    glTexCoord2f(0, 1); glVertex3f(0,400,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    if (device->getVideoFormat() == FREENECT_VIDEO_RGB || device->getVideoFormat() == FREENECT_VIDEO_YUV_RGB)
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &rgb[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(315,0,0);
    glTexCoord2f(1, 0); glVertex3f(630,0,0);
    glTexCoord2f(1, 1); glVertex3f(630,400,0);
    glTexCoord2f(0, 1); glVertex3f(315,400,0);
    glEnd();
}

void GLWidget::AstraDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);

    if(viewStatus == 0)
    {
    	if(syncCond == 0){
        	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,640,480,0,GL_BGR,GL_UNSIGNED_BYTE,astraDevice->rgb.data);
        }else{
        	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,640,480,0,GL_BGR,GL_UNSIGNED_BYTE,syncDevs->rgb.data);
        }
        // (Type of texture,Pyramid level - 0 is the top level,Internal colour format,width,height,Border width in pixels,Input image format,Image data type, image data)

        glBegin(GL_TRIANGLE_FAN);
        glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
        glTexCoord2f(0, 0); glVertex3f(0,0,0);
        glTexCoord2f(1, 0); glVertex3f(630,0,0);
        glTexCoord2f(1, 1); glVertex3f(630,400,0);
        glTexCoord2f(0, 1); glVertex3f(0,400,0);
        glEnd();

    }
    else if(viewStatus == 3){
    glUseProgram(PCprogram);
			
			QMatrix4x4 mat;
	  		float rawMat[16];
			glUniform1f(scale, scale_vec);
			glUniform1f(device_c, dev_val);
			mat.perspective(60.0f, (float)width()/(float)height(), 1.0f, 100.0f);
			mat.translate(0, 0.5f, -3);
			mat.rotate(xRot, 0, 1, 0);
			mat.rotate(yRot, 1, 0, 0);
			for(int i = 0; i < 16; i++)
			  rawMat[i] = (float)mat.constData()[i];
			
			
			
			
			glUniformMatrix4fv(matrix, 1, GL_FALSE, rawMat);
  			if(syncCond == 0){
  				colorPoints = astraDevice->points + 4;
  				glVertexAttribPointer(position, 4, GL_FLOAT, false, 0*sizeof(GLfloat), astraDevice->points);//pointsL
  			}else{
  				colorPoints = (float*)syncDevs->astraCloud->points.data() + 4;
  				glVertexAttribPointer(position, 4, GL_FLOAT, false, 0*sizeof(GLfloat), (float*)syncDevs->astraCloud->points.data());//pointsL
  			}
  			glVertexAttribPointer(color_vec, 4, GL_FLOAT, false, 0*sizeof(GLfloat), colorPoints); 
			glUniform1f(scale, scale_vec);
			glUniform1f(device_c, dev_val);
			glEnableVertexAttribArray(position);
			glEnableVertexAttribArray(color_vec);			
			glDrawArrays(GL_POINTS, 0, size);
			glDisableVertexAttribArray(position);
			glDisableVertexAttribArray(color_vec);
					
		glUseProgram(0);
    }
}

void GLWidget::LaserDraw()
{
/*
    int i;
    double x,y,z;
    glClear(GL_COLOR_BUFFER_BIT);
    glRotated(0,0,0,1);
    for(int i = 0; i < 729 ; i++){
        x = (laser->laserData[i].x*315/11.2)+(315/2)+125;
        y = (laser->laserData[i].y*315/11.2)+(315/2)+10;
        z = (laser->laserData[i].z);
        glBegin(GL_POINTS);
        glColor3f(1.0,0.0,0.0);
        glPointSize(100);
        glVertex3f(x,y,z);
        glEnd();
    }*/
	//std::cout << "syncCond " << syncCond << std::endl; 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	      
      	if(laser_on == true){
			glUseProgram(PCprogram);
		
			QMatrix4x4 mat;
	  		float rawMat[16];
			glUniform1f(scale, scale_vec);
			glUniform1f(device_c, dev_val);
			mat.perspective(60.0f, (float)width()/(float)height(), 1.0f, 100.0f);
			mat.translate(0, 0, -3);
			mat.rotate(xRot, 0, 1, 0);
			mat.rotate(yRot, 1, 0, 0);
			for(int i = 0; i < 16; i++)
			  rawMat[i] = (float)mat.constData()[i];
			
			glUniformMatrix4fv(matrix, 1, GL_FALSE, rawMat);
  			if(syncCond == 0){
  				glVertexAttribPointer(position, 3, GL_FLOAT, false, 4*sizeof(GLfloat), (float*)laser->laserCloud->points.data());//pointsL
  				//std::cout << "L" << std::endl;
			}
			else{
				//std::cout << "Laser" << std::endl;
				glVertexAttribPointer(position, 3, GL_FLOAT, false, 4*sizeof(GLfloat), (float*)syncDevs->laserCloud->points.data());//pointsL
			}
			glUniform1f(scale, scale_vec);
			glUniform1f(device_c, dev_val);
			glEnableVertexAttribArray(position);
			glDrawArrays(GL_POINTS, 0, size);
			glDisableVertexAttribArray(position);
		    glUseProgram(0);
    	}		
    // Swap
    	swapBuffers();
    	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GLWidget::ChangeMod(int i)
{
    viewStatus = i;
    updateGL();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GLWidget::Syncronize(ros::NodeHandle nh, ros::NodeHandle nh2)
{
    //syncDevices = new Syncronize_Devices(nh, nh2);
    delete syncDevs;
	SetStatus(0);
	syncDevs = new SyncronizeDevices(nh);
	
   
   	//ros::NodeHandle imuNH;
    //imuDevice = new ImuSensor(imuNH);
    //imuOn = 1;

	zedOn = 0;
    ZedInit();
}

void GLWidget::Syncronize_OFF()
{
	SetStatus(0);
	pthread_mutex_unlock(&mutexZ);
	pthread_mutex_unlock(&mutexL);
	pthread_mutex_unlock(&mutexI);
	std::cout << "He he" << std::endl;
	//delete syncDevs;
    //syncronize_devices = false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GLWidget::saveFiles()
{
	if(syncCond == 0)
	{
		if(zedOn == 1)
		    zedDevice.SaveFile();
		
		if(laser_on == true)
		    laser->SaveFile();

		if(imu_on == true)
		    imuDevice->SaveFile();
		if(astra_on == true)
			astraDevice->saveFiles();
    }
    else
    {
    	syncDevs->SaveFile();
    	zedDevice.SaveFile();
    }
}

void GLWidget::testGPIO(){



    // Flash the LED 5 times
    /*for(int i=0; i<5; i++){
        std::cout << "Setting the LED on" << std::endl;
        gpioSetValue(redLED, on);
        usleep(200000);         // on for 200ms
        std::cout << "Setting the LED off" << std::endl;
        gpioSetValue(redLED, off);
        usleep(200000);         // off for 200ms
    }*/

	unsigned int value = low;
	unsigned int value2 = low;
	unsigned int value3 = low;
	unsigned int value4 = low;
	unsigned int save_value = low;
	unsigned int sync_value = low;
	unsigned int exit_value = low;
    //int ledValue = low ;
    // Turn off the LED
    //gpioSetValue(redLED,low) ;
   // while(getkey() != 27) {
        gpioGetValue(pushButton, &value) ;
        gpioGetValue(pushButton2, &value2) ;
        gpioGetValue(pushButton3, &value3) ;
        gpioGetValue(pushButton4, &value4) ;
        gpioGetValue(exitButton, &exit_value) ;
        gpioGetValue(syncButton, &sync_value) ;
        gpioGetValue(saveFButton, &save_value) ;
        // Useful for debugging
        // cout << "Button " << value << endl;
        if(id == 1){
		    if (value==high) {
		    	dispCB = 1;
			//	std::cout << dispCB << std::endl;
		        changeBTN = true;
		    }
		    if (value3 == high){
		    	vModCB = 1;
		    	changeBTN = true;
		    }
        }
        
        if(id == 2){        
		    if(value2 == high) {
		    	dispCB2 = 1;
		//std::cout << dispCB2 << std::endl;
		        changeBTN = true;
		    }
		    if (value4 == high){
		    	VModCB2 = 1;
		    	changeBTN = true;
		    }
		}
		

        if(exit_value == high){
        	exitCond = 1;
        }
        
        if(save_value == high){
        	saveSignal = 1;
        }
        
        if(sync_value == high){
        	if(syncCond == 0){
        		//std::cout << "0" << std::endl;
        		//syncCond = 1;
        		syncCond = syncCond;
        	}
        	else{
        		//syncCond = 0;
        		//std::cout << "1" << std::endl;
        		syncCond = syncCond;
        	}	
        }
        	
        usleep(1000); // sleep for a millisecond
   // }
        
}

int GLWidget::getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}




