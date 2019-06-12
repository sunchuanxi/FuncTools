#ifndef QVISIONINTERFACE_H
#define QVISIONINTERFACE_H

#include <QMap>
#include <QUrl>
#include <QImage>
#include <QObject>
#include <QVariant>
#include <QPainterPath>
#include <opencv2/opencv.hpp>

#include "elvision_global.h"

class QVisionInterfacePrivate;
class ELVISIONSHARED_EXPORT QVisionInterface : public QObject {
    Q_OBJECT

    Q_ENUMS(Catalog)
    Q_ENUMS(ResultStatus)

    Q_PROPERTY(QUrl qmlfile READ qmlfile CONSTANT)
    Q_PROPERTY(Catalog family READ family CONSTANT)
    Q_PROPERTY(QString leaguer READ leaguer CONSTANT)

    Q_PROPERTY(QMap params READ params WRITE setParams)
    Q_PROPERTY(QMap defaultParams READ defaultParams CONSTANT)

    Q_PROPERTY(QImage resultImage READ resultImage CONSTANT)
    Q_PROPERTY(QString resultTips READ resultTips CONSTANT)
    Q_PROPERTY(QPainterPath resultPath READ resultPath CONSTANT)
    Q_PROPERTY(QString resultMessage READ resultMessage CONSTANT)
    Q_PROPERTY(ResultStatus resultStatus READ resultStatus CONSTANT)

public:
    enum ResultStatus { NIL, OK, NG, UN };
    enum Catalog { Location, Presence_Absence, Measurement, Counting, Identification, Geometry, Math_Logic,
        Image_Filter, Defect_Detection, Calibreation };

    explicit QVisionInterface(const QString&, QObject *parent = 0);
    ~QVisionInterface();

    void exec();

    virtual void clear();
    virtual void setMat(const cv::Mat&) = 0;
    virtual void setMask(const QList<cv::Mat>&) = 0;

signals:
    void valueChanged(const QString& key, const QVariant& value);

protected:
    virtual void run() = 0;

    virtual QUrl qmlfile() const = 0;
    virtual Catalog family() const = 0;
    virtual QString leaguer() const = 0;

    virtual QMap<QString, QVariant> params() const = 0;
    virtual QMap<QString, QVariant> defaultParams() const = 0;

    virtual void setParams(const QMap<QString, QVariant>&) = 0;

    void setResultStatus(ResultStatus);
    void setResultMat(const cv::Mat&);
    void setResultTips(const QString&);
    void setResultMessage(const QString&);
    void setResultPath(const QPainterPath&);

private:
    QVisionInterfacePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QVisionInterface)
    Q_DISABLE_COPY(QVisionInterface)

    ResultStatus resultStatus() const;
    const QImage& resultImage() const;
    const QString& resultTips() const;
    const QString& resultMessage() const;
    const QPainterPath& resultPath() const;
};

Q_DECLARE_METATYPE(QPainterPath)
Q_DECLARE_METATYPE(QVisionInterface::Catalog)
Q_DECLARE_METATYPE(QVisionInterface::ResultStatus)

#define VISION_CONNECT_VALUE_INT(space, name) connect(this, &space::name##Changed, [&](int name){ emit valueChanged(#name, name); })
#define VISION_CONNECT_VALUE_REAL(space, name) connect(this, &space::name##Changed, [&](double name){ emit valueChanged(#name, name); })
#define VISION_CONNECT_VALUE_DATA(space, name) connect(this, &space::name##Changed, [&](const QByteArray& name){ emit valueChanged(#name, name); })

#define VISION_SET_PARAM_INT(name) m_##name = v.value(#name, m_##name).toInt()
#define VISION_SET_PARAM_REAL(name) m_##name = v.value(#name, m_##name).toDouble()
#define VISION_SET_PARAM_DATA(name) m_##name = v.value(#name, m_##name).toByteArray()

#define VISION_GET_PARAM(name) v[#name] = m_##name
#define VISION_DEFAULT_PARAM(name, value) v[#name] = value

#endif // QVISIONINTERFACE_H
