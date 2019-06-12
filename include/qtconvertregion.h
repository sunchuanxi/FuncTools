#ifndef QTCONVERTREGION_H
#define QTCONVERTREGION_H

#include <QPolygonF>
#include <cpp/HalconCpp.h>
#include <opencv2/opencv.hpp>
//#include "C:/Program Files/OpenCV/OpenCV243/include/opencv.hpp"

#include "elparty_global.h"

struct ELPARTYSHARED_EXPORT QConvert2Polygon {
    QPolygonF operator()(const Halcon::HRegion& region) const;
};

struct ELPARTYSHARED_EXPORT QConvert2HRegion {
    Halcon::HRegion operator()(const cv::Mat& mask);
};

#endif // QCONVERTREGION_H
