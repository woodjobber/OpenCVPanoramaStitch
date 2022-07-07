//
//  Cropping.h
//  SwiftStitch
//
//  Created by chengbin on 2022/7/6.
//  Copyright © 2022 ellipsis.com. All rights reserved.
//
// https://dji-dev.gitbooks.io/mobile-sdk-tutorials/content/en/iOS/PanoDemo/PanoDemo_en.html#cropping-the-panorama
//

#import <opencv2/opencv.hpp>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface Cropping : NSObject

+ (bool) cropWithMat: (const cv::Mat &)src andResult:(cv::Mat &)dest;

@end

NS_ASSUME_NONNULL_END
