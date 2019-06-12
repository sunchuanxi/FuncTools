#ifndef QVISIONGEOMETRY_H
#define QVISIONGEOMETRY_H

#include "qvisioninterface.h"

class QVisionGeometryPrivate;
class ELVISIONSHARED_EXPORT QVisionGeometry : public QVisionInterface {
    Q_OBJECT

    Q_PROPERTY(QPainterPath resultGeometry READ resultGeometry CONSTANT)

public:
    explicit QVisionGeometry(const QString&, QObject *parent = 0);
    ~QVisionGeometry();

    void clear();

protected:
    Catalog family() const;

    void setResultGeometry(const QPainterPath&);

private:
    QVisionGeometryPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QVisionGeometry)
    Q_DISABLE_COPY(QVisionGeometry)

    QPainterPath resultGeometry() const;
};

#endif // QVISIONGEOMETRY_H
