/*
#Author       : xyoung
#Date         : 2024-05-29 18:56:51
#LastEditors  : kuai le jiu shi hahaha
#LastEditTime : 2024-05-30 08:59:20
*/
#include "libyuv.h"
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

class YUV2RGB
{
public:
    YUV2RGB() = default;
    ~YUV2RGB();

    void YUV_Init(float s);
    void convertSplit(cv::Mat yuv_data);
    void splitConvert(cv::Mat yuv_data);
    void read_yuv(const std::string yuv_path);
    void scaleConvert(cv::Mat yuv_data);

private:
    float yuv2rgb(const cv::Mat &yuv_data, cv::Mat &rgb_img);
    float yuv2rgb5(cv::Mat &yuv_data, cv::Mat &rgb_img);
    float yuv2rgb_list2(std::vector<cv::Mat> &yuv_list, std::vector<cv::Mat> &rgb_list);
    void image_split(cv::Mat &yuv_data);
    float nv12_process(const cv::Mat &yuyv, cv::Mat &rgb);
    void save_image(int index);

private:
    int IMAGE_WIDTH = 1280;
    int IMAGE_WIDTH5 = 5120;

    int IMAGE_HEIGHT = 720;
    int iter_number = 100;

    int dst_w;
    int dst_h;

    uint8_t *nv12_y;
    int nv12_y_stride;

    uint8_t *nv12_uv;
    int nv12_uv_stride;

    uint8_t *dst_y_scale;
    int dst_y_stride;

    uint8_t *dst_uv_scale;
    int dst_uv_stride;

    cv::Mat scale_argb_mat;
    std::vector<cv::Mat> yuv_images;
    std::vector<cv::Mat> rgb_images;
    std::vector<cv::Mat> rgb_scale_images;

    float scale_value = 1;

public:
    cv::Mat uyvy_img;
};