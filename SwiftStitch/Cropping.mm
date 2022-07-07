//
//  Cropping.m
//  SwiftStitch
//
//  Created by chengbin on 2022/7/6.
//  Copyright © 2022 ellipsis.com. All rights reserved.
//

#import "Cropping.h"

const float CUTBLACKTHREASHOLD = 0.01; // 0.05

@implementation Cropping

+ (bool) cropWithMat: (const cv::Mat &)src andResult: (cv::Mat &)dest {
    cv::Mat gray;
    cvtColor(src, gray, cv::COLOR_RGB2GRAY); //convert src to gray
    
    cv::Rect roiRect(0,0,gray.cols,gray.rows); // start as the source image frame
    
    while (true) {
//        NSLog(@"%d %d %d %d",roiRect.x,roiRect.y,roiRect.width,roiRect.height);
        
        bool isTopNotBlack = checkBlackRow(gray, roiRect.y,roiRect);
        bool isLeftNotBlack = checkBlackColumn(gray, roiRect.x,roiRect);
        bool isBottomNotBlack = checkBlackRow(gray, roiRect.y+roiRect.height-1,roiRect);
        bool isRightNotBlack = checkBlackColumn(gray, roiRect.x+roiRect.width-1,roiRect);
        
        if(isTopNotBlack && isLeftNotBlack && isBottomNotBlack && isRightNotBlack) {
            cv::Mat imageReference = src(roiRect);//get the pixels in roiRect
            imageReference.copyTo(dest);//copy pixels to dest
            return true;
        }
        // If not, scale ROI down
        // if x is increased, width has to be decreased to compensate
        if(!isLeftNotBlack) {
            roiRect.x++;
            roiRect.width--;
        }
        // same is valid for y
        if(!isTopNotBlack) {
            roiRect.y++;
            roiRect.height--;
        }
        if(!isRightNotBlack) {
            roiRect.width--;
        }
        if(!isBottomNotBlack) {
            roiRect.height--;
        }
        if(roiRect.width <= 0 || roiRect.height <= 0) {
            return false;
        }
    }
}

//check the pure dark pixel in row y whether less than threashold
bool checkBlackRow(const cv::Mat& roi, int y, const cv::Rect &rect) {
    int zeroCount = 0;
    for(int x = rect.x; x < rect.width; x++) {
        if(roi.at<uchar>(y, x) == 0) {
            zeroCount++;
        }
    }
    if((zeroCount/(float)roi.cols) > CUTBLACKTHREASHOLD) {
        return false;
    }
    return true;
}

//check the pure dark pixel in column x whether less than threashold
bool checkBlackColumn(const cv::Mat& roi, int x,const cv::Rect &rect) {
    int zeroCount = 0;
    for(int y = rect.y; y < rect.height; y++) {
        if(roi.at<uchar>(y, x) == 0) {
            zeroCount++;
        }
    }
    if((zeroCount / (float)roi.rows) > CUTBLACKTHREASHOLD) {
        return false;
    }
    return true;
}
@end
