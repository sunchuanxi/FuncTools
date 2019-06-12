#ifndef QCONVERTIMAGE_H
#define QCONVERTIMAGE_H

#include <QImage>
#include <opencv2/opencv.hpp>

#include "elvision_global.h"

struct ELVISIONSHARED_EXPORT QConvert2Mat {
    cv::Mat operator()(const QImage& image) const;
    cv::Mat operator()(const QPainterPath&, const QSize&) const;
    cv::Mat operator()(const QPainterPath&, const cv::Size&) const;
};

struct ELVISIONSHARED_EXPORT QConvert2Image {
    QImage operator()(const cv::Mat& mat) const;
};

#endif // QCONVERTIMAGE_H
