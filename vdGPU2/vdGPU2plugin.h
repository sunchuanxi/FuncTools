#ifndef VDGPU2PLUGIN_H
#define VDGPU2PLUGIN_H

#include "../include/qmvtoolplugin.h"

class QvdGPU2Plugin : public QMVToolPlugin {
    Q_OBJECT

    Q_PLUGIN_METADATA(IID MVToolDefectDetectionInterface_iid FILE "vdGPU2.json")

public:
    QvdGPU2Plugin(QObject *parent = Q_NULLPTR);

    QString version() const;
    QString pluginName() const;
    QString description() const;
protected:
    QObject* create(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent = Q_NULLPTR) const;
};

#endif // VDSUALPLUGIN_H
