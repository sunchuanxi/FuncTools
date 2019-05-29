#include <QtCore>
#include "vdSUAL.h"
#include "vdSUAL_p.h"
#include <iostream>

QvdSUAL::QvdSUAL(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent)
    : QObject(parent)
    , d_ptr(new QvdSUALPrivate(station, camera, threadID, productID, this))
{
	//std::cout << "test";
    if (threadID == (std::numeric_limits<quint8>::max)()) {
    }
	initDictDefect();
	m_dictDefect.clear();
}

void QvdSUAL::calibration(const QVariant&)
{
	flagCellSize = true;
}
QvdSUAL::~QvdSUAL()
{
    delete d_ptr;
}

void QvdSUAL::run(const Halcon::HImage& image, const Halcon::HRegion& roi, const QByteArray& roiData = QByteArray())
{
    d_func()->run(image, roi, roiData);
}

QMVToolPlugin::ShowType QvdSUAL::showType() const
{
    return d_func()->showType;
}

QString QvdSUAL::resultStr() const
{
    return d_func()->resultStr;
}

int QvdSUAL::resultStatus() const
{
    return d_func()->resultStatus;
}

int QvdSUAL::resultOutport() const
{
    return d_func()->resultOutport;
}

QPixmap QvdSUAL::resultPixmap() const
{
    return d_func()->resultPixmap;
}

QFixture QvdSUAL::resultFixture() const
{
    return d_func()->resultFixture;
}

Halcon::HRegionArray QvdSUAL::resultRegions() const
{
    return d_func()->resultRegions;
}

QList<QColor> QvdSUAL::resultRegionColors() const
{
    return d_func()->resultRegionColors;
}

void QvdSUAL::initResultMeasure(){
}

void QvdSUAL::initDictDefect(){
}
