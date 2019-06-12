#ifndef QVISIONMEASUREMENT_H
#define QVISIONMEASUREMENT_H

#include "qvisioninterface.h"

class QVisionMeasurementPrivate;
class ELVISIONSHARED_EXPORT QVisionMeasurement : public QVisionInterface {
    Q_OBJECT

    Q_PROPERTY(double resultValue READ resultValue CONSTANT)

public:
    explicit QVisionMeasurement(const QString&, QObject *parent = 0);
    ~QVisionMeasurement();

    void clear();

protected:
    Catalog family() const;

    void setResultValue(double);

private:
    QVisionMeasurementPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QVisionMeasurement)
    Q_DISABLE_COPY(QVisionMeasurement)

    double resultValue() const;
};

#endif // QVISIONMEASUREMENT_H
