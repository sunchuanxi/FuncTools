#include <QtCore>
#include "vdGPU2.h"
#include "vdGPU2_p.h"
#include <iostream>

QvdGPU2::QvdGPU2(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent)
    : QObject(parent)
    , d_ptr(new QvdGPU2Private(station, camera, threadID, productID, this))
{
	//std::cout << "test";
    if (threadID == (std::numeric_limits<quint8>::max)()) {
    }
	initDictDefect();
	m_dictDefect.clear();
}

void QvdGPU2::calibration(const QVariant&)
{
	flagCellSize = true;
}
QvdGPU2::~QvdGPU2()
{
    delete d_ptr;
}

void QvdGPU2::run(const Halcon::HImage& image, const Halcon::HRegion& roi, const QByteArray& roiData = QByteArray())
{
    d_func()->run(image, roi, roiData);
}

QMVToolPlugin::ShowType QvdGPU2::showType() const
{
    return d_func()->showType;
}

QString QvdGPU2::resultStr() const
{
    return d_func()->resultStr;
}

int QvdGPU2::resultStatus() const
{
    return d_func()->resultStatus;
}

int QvdGPU2::resultOutport() const
{
    return d_func()->resultOutport;
}

QPixmap QvdGPU2::resultPixmap() const
{
    return d_func()->resultPixmap;
}

QFixture QvdGPU2::resultFixture() const
{
    return d_func()->resultFixture;
}

Halcon::HRegionArray QvdGPU2::resultRegions() const
{
    return d_func()->resultRegions;
}

QList<QColor> QvdGPU2::resultRegionColors() const
{
    return d_func()->resultRegionColors;
}

void QvdGPU2::initResultMeasure(){
}

void QvdGPU2::initDictDefect(){
}
