/*
#Author       : xyoung
#Date         : 2024-05-27 17:10:07
#LastEditors  : kuai le jiu shi hahaha
#LastEditTime : 2024-05-30 09:01:21
*/
#include "yuv_converter.h"
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    std::string yuv_path = "./src0_avm_5120x720_1.yuv";
    YUV2RGB yuv_converter = YUV2RGB();
    yuv_converter.YUV_Init(0.5);
    yuv_converter.read_yuv(yuv_path);
    cv::Mat yuyv_data = yuv_converter.uyvy_img;

    yuv_converter.convertSplit(yuyv_data);
    yuv_converter.splitConvert(yuyv_data);
    yuv_converter.scaleConvert(yuyv_data);
    return 0;
}
