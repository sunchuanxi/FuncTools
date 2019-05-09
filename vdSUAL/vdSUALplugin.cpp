#include "vdSUALPlugin.h"
#include "vdSUAL.h"

QvdSUALPlugin::QvdSUALPlugin(QObject *parent)
    : QMVToolPlugin(parent)
{
}

QString QvdSUALPlugin::version() const
{
    return "1.0.0";
}

QString QvdSUALPlugin::pluginName() const
{
    return tr("SUAL");
}

QString QvdSUALPlugin::description() const
{
    return tr("Describe");
}


QObject* QvdSUALPlugin::create(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent) const
{
    return new QvdSUAL(station, camera, threadID, productID, parent);
}
