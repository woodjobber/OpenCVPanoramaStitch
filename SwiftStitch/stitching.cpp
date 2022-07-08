/////////////////////////
/*
 
 // stitching.cpp
 // adapted from stitching.cpp sample distributed with openCV source.
 // adapted by Foundry for iOS
 
 */


//#include "stitching.h"
//#include "algorithm"
//#include <iostream>
//#include <fstream>
//#include "opencv2/stitching.hpp"
// https://www.jianshu.com/p/26f76fdac36f?ivk_sa=1024320u

//using namespace std;
//using namespace cv;
//bool try_use_gpu = false;
//vector<Mat> imgs;
//string result_name = "result.jpg";
//int thresh = 100;
//int max_thresh = 255;
//RNG rng(12345);
//
//const int scale = 2;
//void printUsage();
//int parseCmdArgs(int argc, char** argv);
//int getMaxContour(std::vector<vector<cv::Point>> contours);
//int cropBoundingRect(cv::Mat &inputMat, cv::Mat &outputMat);

//cv::Mat stitch (vector<Mat>& images)
//{
//    imgs = images;
//    Mat pano;//拼接图
//    Ptr<Stitcher> stitcher = Stitcher::create();//4.0
//
//    //拼接
//    Stitcher::Status status = stitcher->stitch(imgs, pano);
//
//    if (status != Stitcher::OK)
//    {
//        cout << "Can't stitch images, error code = " << int(status) << endl;
//    }
//
//    return pano;
//}
//int cropBoundingRect(cv::Mat &inputMat, cv::Mat &outputMat)
//{
//    //在全景图四周各添加10像素宽的黑色边框，以确保能够找到全景图的完整轮廓：
//    Mat stitched;//黑色边框轮廓图
//    copyMakeBorder(inputMat, stitched, 10, 10, 10, 10, cv::BORDER_CONSTANT, true);
//    //全景图转换灰度图，并将不为0的像素全置为255
//    //作为前景，其他像素灰度值为0，作为背景。
//    Mat gray;
//    cv::cvtColor(stitched, gray, cv::COLOR_BGR2GRAY);
//
//    //中值滤波，去除黑色边际中可能含有的噪声干扰
//    cv::medianBlur(gray, gray, 7);
//
//    //白色剪影与黑色背景
//    Mat tresh;
//    threshold(gray, tresh, 0, 255, THRESH_BINARY);
//
//    //resize 缩小一半处理
//    resize(tresh, tresh,
//           Size(tresh.cols / scale, tresh.rows / scale),
//           tresh.cols / 2,
//           tresh.rows / 2, INTER_LINEAR);
//
//    //现在有了全景图的二值图，再应用轮廓检测，找到最大轮廓的边界框，
//    vector<vector<Point>> contours; //contours:包含图像中所有轮廓的python列表（三维数组）,每个轮廓是包含边界所有坐标点(x, y)的Numpy数组。
//    vector<Vec4i> hierarchy = vector<cv::Vec4i>();//vec4i是一种用于表示具有4个维度的向量的结构，每个值都小于cc>
//    findContours(tresh.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);//传入参数不一样
//    //计算最大轮廓的边界框
//    int index = getMaxContour(contours);
//    if (index == -1) {
//        return -1;
//    }
//    vector<Point> cnt = contours[index];
//    drawContours(tresh, contours, index, Scalar(255,0,0));
//
//    //蒙板
//    Mat mask = Mat::zeros(tresh.rows, tresh.cols, CV_8UC1); // 0矩阵
//    //依赖轮廓创建矩形
//    Rect cntRect = cv::boundingRect(cnt);
//    rectangle(mask, cntRect, cv::Scalar(255, 0, 0), -1);
//
//    Mat minRect = mask.clone();//minRect的白色区域会慢慢缩小，直到它刚好可以完全放入全景图内部。
//    Mat sub = mask.clone();//sub用于确定minRect是否需要继续减小，以得到满足要求的矩形区域。
//
//    //开始while循环，直到sub中不再有前景像素
//    while (cv::countNonZero(sub) > 0) {
//    //        int zero = cv::countNonZero(sub);
//    //        printf("剩余前景像素 %d \n",zero);
//        cv::erode(minRect, minRect, Mat());
//        cv::subtract(minRect, tresh, sub);
//    }
//
//    //第二次循环
//    cv::Mat minRectClone = minRect.clone();
//
//    cv::resize(minRectClone, minRectClone,
//               cv::Size(minRectClone.cols * scale, minRectClone.rows * scale),
//               (float)minRect.cols / 2, (float)minRect.rows / 2,INTER_LINEAR);
//
//    std::vector<std::vector<Point> > cnts;
//    vector<Vec4i> hierarchyA = vector<cv::Vec4i>();
//    findContours(minRectClone, cnts, hierarchyA, RETR_TREE, CHAIN_APPROX_SIMPLE);
//    int idx = getMaxContour(cnts);
//    if (idx == -1) {
//        return -1;
//    }
//    Rect finalRect = cv::boundingRect(cnts[idx]);
//
//    outputMat = Mat(stitched, finalRect).clone();
//
//    return 0;
//}
//
////循环最大的轮廓边框
//int getMaxContour(std::vector<vector<cv::Point>> contours){
//  double max_area = 0;
//  int index = -1;
//  for (int i = 0; i < contours.size(); i++) {
//     double tempArea = contourArea(contours[i]);
//     if (tempArea > max_area) {
//         max_area = tempArea;
//         index = i;
//     }
//  }
//  return index;
//}
//




#include "stitching.h"
#include <iostream>
#include <fstream>

#include "opencv2/stitching.hpp"


using namespace std;
using namespace cv;

bool try_use_gpu = false;
vector<Mat> imgs;
string result_name = "result.jpg";

void printUsage();
int parseCmdArgs(int argc, char** argv);
cv::Mat sphericalStitch(vector<Mat>& images);

cv::Mat stitch(vector<Mat>& images)
{
    imgs = images;
    Mat pano;
    Ptr<Stitcher> stitcher = Stitcher::create();
    Stitcher::Status status = stitcher->stitch(imgs, pano);

    std::string errorString = "";
    switch (status) {
        case cv::Stitcher::OK:
            break;
        case cv::Stitcher::ERR_NEED_MORE_IMGS:
            errorString += "need more images";
            break;
        case cv::Stitcher::ERR_HOMOGRAPHY_EST_FAIL:
            errorString += "homography failed";
            break;
        case cv::Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL:
            errorString += "camera params adjust failed";
            break;
    }
    if (errorString.length() > 0) {
        throw invalid_argument(errorString);
    } else {
        return pano;
    }
}

//平面曲翘拼接
cv::Mat planeStitch(vector<Mat>& images) {
  imgs = images;
  Mat pano;
  Ptr<Stitcher> stitcher = Stitcher::create();//4.0
  Ptr<PlaneWarper> plane_warper = makePtr<cv::PlaneWarper>();
  stitcher->setWarper(plane_warper);

  Stitcher::Status status = stitcher->stitch(imgs, pano);
  if (status != Stitcher::OK)
  {
      cout << "Can't stitch images, error code = " << int(status) << endl;
      //return 0;
  }
  return pano;
}

//鱼眼拼接
cv::Mat fisheyeStitch(vector<Mat>& images) {
  imgs = images;
  Mat pano;

  Ptr<Stitcher> stitcher = Stitcher::create();//4.0
  Ptr<FisheyeWarper> fisheye_warper = makePtr<cv::FisheyeWarper>();
  stitcher->setWarper(fisheye_warper);
  Stitcher::Status status = stitcher->stitch(imgs, pano);
  if (status != Stitcher::OK)
  {
      cout << "Can't stitch images, error code = " << int(status) << endl;
      //return 0;
  }
  return pano;
}

cv::Mat sphericalStitch(vector<Mat>& images) {
    imgs = images;
    Mat pano;
    Ptr<Stitcher> stitcher = Stitcher::create();//4.0
    Ptr<SphericalWarper> fisheye_warper = makePtr<cv::SphericalWarper>();
    stitcher->setWarper(fisheye_warper);
    Stitcher::Status status = stitcher->stitch(imgs, pano);
    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
        //return 0;
    }
    return pano;
}

