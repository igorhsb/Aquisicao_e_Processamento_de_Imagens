#ifndef _ZED_H
#define _ZED_H

#include <QMainWindow>
#ifndef Q_MOC_RUN
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <pcl/common/common_headers.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#endif
#include <thread>
#include <sl/Camera.hpp>
#include "sync.h"

class ZedDevice{
public:
    ZedDevice();
    ~ZedDevice();
    cv::Mat depthView, rgbZ,pcd;
    sl::Mat rgb,depth,pointCloud;
    float* dataPC;
    void Init();
    int StreamData();
    void Stop();
    int getStatus();
    int cont;

    sl::Camera zed;
    sl::RuntimeParameters runtime_parameters;
    void convertSl2Cv(int i, sl::Mat& input);
    void cv2file();
    std::string errorCode;
    static void* streamFunc(void*);
    void SaveFile();
    static void* fileThreadFunc(void*);
    pthread_t fileThread;
	float *points; 
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr zedCloud;
	void cloud2pcl(sl::Mat& input);
	float convertColor(float);
	


private:
    int status;
    pthread_t streamThread;  
};

#endif
