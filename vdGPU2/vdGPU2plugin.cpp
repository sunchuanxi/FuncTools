#include "vdGPU2Plugin.h"
#include "vdGPU2.h"

QvdGPU2Plugin::QvdGPU2Plugin(QObject *parent)
    : QMVToolPlugin(parent)
{
}

QString QvdGPU2Plugin::version() const
{
    return "1.0.0";
}

QString QvdGPU2Plugin::pluginName() const
{
    return tr("GPU2");
}

QString QvdGPU2Plugin::description() const
{
    return tr("Describe");
}


QObject* QvdGPU2Plugin::create(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent) const
{
    return new QvdGPU2(station, camera, threadID, productID, parent);
}
