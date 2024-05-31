/*
#Author       : xyoung
#Date         : 2024-05-29 18:57:07
#LastEditors  : kuai le jiu shi hahaha
#LastEditTime : 2024-05-30 09:04:24
*/

#include "yuv_converter.h"

void YUV2RGB::YUV_Init(float s)
{
    scale_value = s;

    uyvy_img = cv::Mat(IMAGE_HEIGHT, IMAGE_WIDTH5, CV_8UC2);

    dst_w = (int)IMAGE_WIDTH * scale_value;
    dst_h = (int)IMAGE_HEIGHT * scale_value;
    scale_argb_mat = cv::Mat(dst_h, dst_w, CV_8UC4);

    nv12_y = new uint8_t[IMAGE_WIDTH * IMAGE_HEIGHT];
    nv12_uv = new uint8_t[IMAGE_WIDTH * IMAGE_HEIGHT / 2];

    nv12_y_stride = IMAGE_WIDTH;
    nv12_uv_stride = IMAGE_WIDTH;

    dst_y_scale = new uint8_t[dst_w * dst_h];
    dst_uv_scale = new uint8_t[dst_w * dst_h / 2];
    dst_y_stride = dst_w;
    dst_uv_stride = dst_w;

    for (int i = 0; i < 4; i++) {
        cv::Mat rgb_img_2(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
        cv::Mat scale_rgb_img(dst_h, dst_w, CV_8UC3);
        rgb_images.emplace_back(rgb_img_2);
        rgb_scale_images.emplace_back(scale_rgb_img);
    }
}

YUV2RGB::~YUV2RGB()
{

    delete[] nv12_y;
    delete[] nv12_uv;

    delete[] dst_y_scale;
    delete[] dst_uv_scale;
}

void YUV2RGB::read_yuv(const std::string yuv_path)
{
    // cv::Mat uyvy_img(IMAGE_HEIGHT, IMAGE_WIDTH5, CV_8UC2);

    FILE *yuyv_fid = fopen(yuv_path.c_str(), "rb");
    int framesize = IMAGE_HEIGHT * IMAGE_WIDTH5 * 2;
    fread(uyvy_img.data, framesize * sizeof(uint8_t), 1, yuyv_fid);
    fclose(yuyv_fid);
}

float YUV2RGB::yuv2rgb5(cv::Mat &yuv_data, cv::Mat &rgb_img)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    cv::Mat rgba_mat = cv::Mat(IMAGE_HEIGHT, IMAGE_WIDTH5, CV_8UC4);

    libyuv::YUY2ToARGB(yuv_data.data, IMAGE_WIDTH5 * 2, rgba_mat.data,
                       IMAGE_WIDTH5 * 4, IMAGE_WIDTH5, IMAGE_HEIGHT);
    libyuv::ARGBToRGB24(rgba_mat.data, IMAGE_WIDTH5 * 4, rgb_img.data,
                        IMAGE_WIDTH5 * 3, IMAGE_WIDTH5, IMAGE_HEIGHT);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time_spend = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return time_spend.count();
}

float YUV2RGB::yuv2rgb(const cv::Mat &yuv_data, cv::Mat &rgb_img)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    cv::Mat rgba_mat = cv::Mat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
    libyuv::YUY2ToARGB(yuv_data.data, IMAGE_WIDTH * 2, rgba_mat.data,
                       IMAGE_WIDTH * 4, IMAGE_WIDTH, IMAGE_HEIGHT);
    libyuv::ARGBToRGB24(rgba_mat.data, IMAGE_WIDTH * 4, rgb_img.data,
                        IMAGE_WIDTH * 3, IMAGE_WIDTH, IMAGE_HEIGHT);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time_spend = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return time_spend.count();
}

/*
#description: :save image
#param {int} index 0 :  convertSplit
                                       1  :  splitConvert
                                       2  :  scaleConvert
#return {*}
*/
void YUV2RGB::save_image(int index)
{
    // save images
    for (int j = 0; j < 4; j++) {
        char name[256];
        cv::Mat temp;
        switch (index) {
        case 0:
            sprintf(name, "%d%d_yuv2rgb_%d_converter.jpg", index, index, j);
            temp = rgb_images.at(j);
            cv::cvtColor(temp, temp, CV_RGB2BGR);
            cv::imwrite(name, temp);
            break;
        case 1:
            temp = rgb_images.at(j);
            // cv::cvtColor(temp, temp, CV_RGB2BGR);
            sprintf(name, "%d%d_yuv2rgb_%d_splitF.jpg", index, index, j);
            cv::imwrite(name, temp);
            break;
        default:
            temp = rgb_scale_images.at(j);
            // cv::cvtColor(temp, temp, CV_RGB2BGR);
            sprintf(name, "%d%d_yuv2rgb_%d_scale.jpg", index, index, j);
            cv::imwrite(name, temp);
            break;
        }
    }
}

void YUV2RGB::image_split(cv::Mat &yuv_data)
{
    yuv_images.clear();
    rgb_images.clear();
    // yuv split
    for (int i = 0; i < 4; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        cv::Rect roi{i * IMAGE_WIDTH, 0, IMAGE_WIDTH, IMAGE_HEIGHT};
        // printf("x = %d, y = %d, w = %d, h =%d\n", roi.x, roi.y, roi.width, roi.height);
        // cv::Mat temp = uyvy_img(roi); // result error
        cv::Mat temp(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC2);
        cv::Mat rgb_img_2(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
        yuv_data(roi).copyTo(temp);

        yuv_images.emplace_back(temp);
        rgb_images.emplace_back(rgb_img_2);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto time_spend = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        printf("split yuv image: %.4f   ms\n", time_spend.count());
    }
}

float YUV2RGB::yuv2rgb_list2(std::vector<cv::Mat> &yuv_list, std::vector<cv::Mat> &rgb_list)
{
    float total_time = 0.;
    for (int k = 0; k < yuv_list.size(); k++) {
        total_time += yuv2rgb(yuv_list.at(k), rgb_list.at(k));
    }
    return total_time;
}

float YUV2RGB::nv12_process(const cv::Mat &yuyv, cv::Mat &rgb)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    // convert to nv12
    libyuv::YUY2ToNV12(
        yuyv.data, IMAGE_WIDTH * 2,
        nv12_y, IMAGE_WIDTH, nv12_uv, IMAGE_WIDTH,
        IMAGE_WIDTH, IMAGE_HEIGHT);

    // NV12Scale
    libyuv::NV12Scale(nv12_y, IMAGE_WIDTH, nv12_uv, IMAGE_WIDTH, IMAGE_WIDTH, IMAGE_HEIGHT,
                      dst_y_scale, dst_w, dst_uv_scale, dst_w, dst_w, dst_h, libyuv::kFilterNone);

    // NV12ToARGB
    libyuv::NV12ToARGB(dst_y_scale, dst_w, dst_uv_scale, dst_w,
                       scale_argb_mat.data, dst_w * 4, dst_w, dst_h);

    // ARGBToRGB24
    libyuv::ARGBToRGB24(scale_argb_mat.data, dst_w * 4,
                        rgb.data, dst_w * 3, dst_w, dst_h);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time_spend = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return time_spend.count();
}

/*
#description: :先将yuv转成rgb, 然后再拆分成rgb小图片
#param {Mat} &uyvy_img
#return {*}
*/
void YUV2RGB::convertSplit(cv::Mat yuv_data)
{
    float total_time = 0.0;
    cv::Mat rgb_img(IMAGE_HEIGHT, IMAGE_WIDTH5, CV_8UC3);
    for (int i = 0; i < iter_number; i++) {
        float per_time = yuv2rgb5(yuv_data, rgb_img);
        total_time += per_time;
    }

    cv::imwrite("yuv2rgb_all.jpg", rgb_img);

    for (int i = 0; i < 4; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        cv::Rect roi{i * IMAGE_WIDTH, 0, IMAGE_WIDTH, IMAGE_HEIGHT};
        // printf("x = %d, y = %d, w = %d, h =%d\n", roi.x, roi.y, roi.width, roi.height);

        cv::Mat temp;
        rgb_img(roi).copyTo(temp);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time_spend = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        printf("split rgb image: %f  ms\n", time_spend.count());

        // cv::cvtColor(temp, temp, CV_RGB2BGR);
        char name[256];
        sprintf(name, "000_yuv2rgb_%d.jpg", i);
        cv::imwrite(name, temp);
    }

    printf("convert_Split  total test number : %d ,total times : %.4f  ms, mean time  : %.4f   ms\n",
           iter_number, total_time, total_time / iter_number);
}

/*
#description: :先将yuv拆分成小图，然后转成rgb格式
#param {Mat} &uyvy_img
#return {*}
*/
void YUV2RGB::splitConvert(cv::Mat yuv_data)
{
    // yuv split
    image_split(yuv_data);

    // yuv convert to rgb
    float total_time = 0.0;
    for (int i = 0; i < iter_number; i++) {
        float per_time = yuv2rgb_list2(yuv_images, rgb_images);
        total_time += per_time;
    }

    printf("splitConvert  total test number : %d ,total times : %.4f   ms, per yuv image mean time  : %.4f    ms\n",
           iter_number, total_time, total_time / (iter_number));

    // save images
    save_image(1);
}

/*
#description: :先将yuv拆分成小图并转成nv12，对nv12进行缩放，
                            最后将缩放后的nv12转成rgb格式
#param {vector<cv::Mat>} &yuv_list
#return {*}
*/
void YUV2RGB::scaleConvert(cv::Mat yuv_data)
{
    float total_time = 0.0;
    image_split(yuv_data);
    for (int i = 0; i < iter_number; i++) {
        for (int k = 0; k < 4; k++) {
            total_time += nv12_process(yuv_images.at(k), rgb_scale_images.at(k));
        }
    }
    printf("scaleConvert  total test number : %d ,total times : %.4f   ms, mean time  : %.4f    ms\n",
           iter_number, total_time, total_time / (iter_number));

    save_image(2);
}
