#ifndef PGMVCTRL_GLOBAL_H
#define PGMVCTRL_GLOBAL_H

#include <QMetaType>
#include <QtCore/qglobal.h>

#if defined(PGMVCTRL_LIBRARY)
#  define PGMVCTRLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PGMVCTRLSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QMap>
#include <QString>

#include <windows.h>
#if defined(_WIN32) && defined(UNICODE)
#define FindText FindTextA
#endif
#include <cpp/HalconCpp.h>

struct QImageGrabbedPacket {
    quint8 station;
    quint8 camera;
    quint16 tag;
    QString lots;
    QString product;
    qint64 msecsSinceEpoch;

    Halcon::HImage image;

    quint16 upm      = 0;
    quint16 sampling = 0;

    QImageGrabbedPacket copy() const
    {
        QImageGrabbedPacket _result;
        _result.station         = station;
        _result.camera          = camera;
        _result.tag             = tag;
        _result.lots            = lots;
        _result.product         = product;
        _result.msecsSinceEpoch = msecsSinceEpoch;
        try {
            _result.image       = image.CopyImage();
        } catch (const Halcon::HException&) {

        }
        _result.upm             = upm;  //每个相机运行速度
        _result.sampling        = sampling;

        return _result;
    }
};

Q_DECLARE_METATYPE(Halcon::HImage)
Q_DECLARE_METATYPE(Halcon::HRegion)
Q_DECLARE_METATYPE(Halcon::HRegionArray)
Q_DECLARE_METATYPE(QImageGrabbedPacket)

///////////////////////////////////////////////////////////////////////////////

#include <QImage>
#include <QPixmap>

struct QVideoPacket {
    quint8 station = 0xFF;
    quint8 camera  = 0xFF;
    quint8 status  = 0xFF;

    quint16 tag      = 0;
    quint16 elapsed  = 0;

    quint16 upm      = 0;
    quint16 sampling = 0;
    quint64 failed   = 0;
    quint64 succeed  = 0;
    quint64 vague    = 0;
    qint64 msecsSinceEpoch = 0;

    QMap<QString, quint64> groupFail;

    QString lots;
    QString defect;
    QString product;

    QImage image;
    QPixmap pixmap;
};

Q_DECLARE_METATYPE(QVideoPacket)

///////////////////////////////////////////////////////////////////////////////

#include "qmvtoolplugin.h"

struct QROIRectangle {
    QPointF center = QPointF(0., 0.);
    double width   = 400.;
    double height  = 200.;
    double angle   = 0.;

    QFixture fixture;
};

struct QROICircle {
    QPointF center    = QPointF(0., 0.);
    double radius     = 200.;
    double startAngle = 0.;
    double spanAngle  = 360.;

    QFixture fixture;
};

struct QROIAnnulus {
    QPointF center     = QPointF(0., 0.);
    double innerRadius = 100.;
    double outerRadius = 200.;
    double startAngle  = 0.;
    double spanAngle   = 360.;

    QFixture fixture;
};

struct QROIPolygon {
    QPointF         center     = QPointF(0., 0.);
    double          angle      = 0.;
    int             length     = 0;
    QPolygonF       pointGroup;

    QFixture fixture;
};

struct QROIRoundedRectangleRing {
    int     type       = QMVToolPlugin::Surface;
    int     direction  = 0;
    int     lineNumber = 10;
    QPointF center     = QPointF(0., 0.);
    double  width      = 400.;
    double  height     = 200.;
    double  thickness  = 50.;
    double  radius     = 50.;
    double  angle      = 0.;

    QFixture fixture;
};

struct QROIEllipseRing {
    int     type       = QMVToolPlugin::Surface;
    int     direction  = 0;
    int     lineNumber = 10;
    QPointF center     = QPointF(0., 0.);
    double  width      = 400.;
    double  height     = 200.;
    double  startAngle = 0.;
    double  spanAngle  = 360.;
    double  thickness  = 50.;
    double  angle      = 0.;

    QFixture fixture;
};

PGMVCTRLSHARED_EXPORT QDataStream& operator << (QDataStream& stream, const QROIRectangle& rect);
PGMVCTRLSHARED_EXPORT QDataStream& operator >> (QDataStream& stream, QROIRectangle& rect);

//hwf delete PGMVCTRLSHARED_EXPORT QDataStream& operator << (QDataStream& stream, const QROICircle& circle);
//hwf delete PGMVCTRLSHARED_EXPORT QDataStream& operator >> (QDataStream& stream, QROICircle& circle);

PGMVCTRLSHARED_EXPORT QDataStream& operator << (QDataStream& stream, const QROIAnnulus& annulus);
PGMVCTRLSHARED_EXPORT QDataStream& operator >> (QDataStream& stream, QROIAnnulus& annulus);

PGMVCTRLSHARED_EXPORT QDataStream& operator << (QDataStream& stream, const QROIPolygon& polygon);
PGMVCTRLSHARED_EXPORT QDataStream& operator >> (QDataStream& stream, QROIPolygon& polygon);

Halcon::HRegion toRegion(const QROIRectangle& rect);
Halcon::HRegion toRegion(const QROICircle& circle);
Halcon::HRegion toRegion(const QROIAnnulus& annulus);
Halcon::HRegion toRegion(const QROIPolygon& polygon);
Halcon::HRegion toRegion(const QROIRoundedRectangleRing& roundedRectangleRing);
Halcon::HRegion toRegion(const QROIEllipseRing& ellipse);

///////////////////////////////////////////////////////////////////////////////

class PGMVCTRLSHARED_EXPORT QConvert2QImage {
public:
    QImage operator()(const Halcon::HImage&) const;
};

///////////////////////////////////////////////////////////////////////////////

class PGMVCTRLSHARED_EXPORT QConvert2HImage {
public:
    Halcon::HImage operator()(const QImage&) const;
};

#endif // PGMVCTRL_GLOBAL_H
