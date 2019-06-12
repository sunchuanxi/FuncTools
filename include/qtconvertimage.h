#ifndef QTCONVERTIMAGE_H
#define QTCONVERTIMAGE_H

#include <cpp/HalconCpp.h>
#include <opencv2/opencv.hpp>

#include "elparty_global.h"

struct ELPARTYSHARED_EXPORT QConvert2HImage {
    Halcon::HImage operator()(const cv::Mat& mat) const;
};

struct ELPARTYSHARED_EXPORT QConvert2Mat {
    cv::Mat operator()(const Halcon::HImage& himage) const;
};

#endif // QTCONVERTIMAGE_H
