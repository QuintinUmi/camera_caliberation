#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <stdlib.h>

#include "opencv2/opencv.hpp"  
// #include "apriltag/apriltag.h"    
#include "opencv2/aruco/charuco.hpp"  


#include <yaml-cpp/yaml.h>

#include "image_transport/image_transport.h"

#include "camera_calibration_tool.h"
#include "drawing_tool.h"
#include "aruco_tool.h"
#include "param_code.h"


using namespace std;
using namespace cct;
using namespace drt;

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "Aruco_Ext_Calib");
    ros::NodeHandle rosHandle;
    
    cv::String yamlPath;
    std::string imageFormat;
    cv::String imageSavePath;

    cv::Size chessboardSize;
    double squareSize;

    rosHandle.param("yaml_save_path", yamlPath, cv::String("~/"));
    rosHandle.param("image_save_path", imageSavePath, cv::String("~/"));
    rosHandle.param("image_format", imageFormat, std::string("png"));

    int dictionaryName;
    vector<int> ids;
    int markerSize;
    rosHandle.param("dictionary_name", dictionaryName, 10);
    rosHandle.param("selected_ids", ids, vector<int>{});
    rosHandle.param("aruco_marker_size", markerSize, 500);
    

    cv::String intrinsicsPath = yamlPath + "calibration_param.yaml";
    cv::FileStorage fs(intrinsicsPath, cv::FileStorage::READ);
    int image_width{0}, image_height{0};
    fs["imageWidth"] >> image_width;
    fs["imageHeight"] >> image_height;

    cv::Size image_size = cv::Size(image_width, image_height);

    cv::Mat cameraMatrix, distCoeffs, newCameraMatrix, newDisCoffes;
    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;
    fs["newCameraMatrix"] >> newCameraMatrix;
    fs["newDisCoffes"] >> newDisCoffes;
    fs.release();
    std::cout << cameraMatrix << std::endl;
    std::cout << distCoeffs << std::endl;
    std::cout << image_size << std::endl;

    cv::aruco::DICT_6X6_250;
    ArucoM arucoMarker(dictionaryName, ids);
    vector<cv::Mat> arucoMarkerImgs;

    arucoMarkerImgs = arucoMarker.generate_aruco_marker(markerSize);

    arucoMarker.aruco_marker_save(imageSavePath, imageFormat, arucoMarkerImgs, dictionaryName, true);
    
    return 0;
}