## yuv converte to rgb

### 1. yuv to rgb
将 yuv 转为rgb
### 2. yuv to nv12
将 yuv 先转成 nv12 格式，对 nv12 进行缩放，再转为rgb


## demo
```c++
    std::string yuv_path = "./src0_avm_5120x720_1.yuv";
    YUV2RGB yuv_converter = YUV2RGB();
    yuv_converter.YUV_Init(0.5); // 0.5倍下采样
    yuv_converter.read_yuv(yuv_path);
    cv::Mat yuyv_data = yuv_converter.uyvy_img;

    yuv_converter.convertSplit(yuyv_data);
    yuv_converter.splitConvert(yuyv_data);
    yuv_converter.scaleConvert(yuyv_data);
    return 0;
```



>参考
> https://github.com/youngx123/libyuv_testDemo