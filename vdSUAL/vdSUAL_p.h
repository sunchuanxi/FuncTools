#ifndef VDSUALPRIVATE_H
#define VDSUALPRIVATE_H
#include <QPixmap>
#include <cpp/HalconCpp.h>
#include "../include/qmvtoolplugin.h"
class QvdSUAL;
class QvdSUALPrivate {
public:
    QvdSUALPrivate(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QvdSUAL* parent);
protected:
    void run(const Halcon::HImage&, const Halcon::HRegion&, const QByteArray&);
private:
    Q_DECLARE_PUBLIC(QvdSUAL)
    QvdSUAL* const q_ptr;
    const quint8 station;
    const quint8 camera;
    const quint8 threadID;
    const quint8 productID;
    QMVToolPlugin::ShowType showType;
    QString resultStr;
    int resultStatus;
    int resultOutport;
    QPixmap resultPixmap;
    QFixture resultFixture;
    Halcon::HRegionArray resultRegions;
    QList<QColor> resultRegionColors;
};

#endif // VDSUALPRIVATE_H
