#ifndef QVISIONLOCATION_H
#define QVISIONLOCATION_H

#include "qvisioninterface.h"

class QVisionLocationPrivate;
class ELVISIONSHARED_EXPORT QVisionLocation : public QVisionInterface {
    Q_OBJECT

    Q_PROPERTY(double resultX READ resultX CONSTANT)
    Q_PROPERTY(double resultY READ resultY CONSTANT)
    Q_PROPERTY(double resultAngle READ resultAngle CONSTANT)
    Q_PROPERTY(double resultScale READ resultScale CONSTANT)
    Q_PROPERTY(double resultScore READ resultScore CONSTANT)

public:
    QVisionLocation(const QString&, QObject *parent = 0);
    ~QVisionLocation();

    void clear();

protected:
    Catalog family() const;

    void setResultX(double);
    void setResultY(double);
    void setResultAngle(double);
    void setResultScale(double);
    void setResultScore(double);

private:
    QVisionLocationPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QVisionLocation)
    Q_DISABLE_COPY(QVisionLocation)

    double resultX() const;
    double resultY() const;
    double resultAngle() const;
    double resultScale() const;
    double resultScore() const;
};

#endif // QVISIONLOCATION_H
