#ifndef QVISIONDEFECT_H
#define QVISIONDEFECT_H

#include <QList>
#include <QVariant>

#include "qvisioninterface.h"

class QVisionDefectPrivate;
class ELVISIONSHARED_EXPORT QVisionDefect : public QVisionInterface {
    Q_OBJECT

    Q_PROPERTY(QList<QVariant> resultDefects READ resultDefects CONSTANT)

public:
    explicit QVisionDefect(const QString&, QObject *parent = 0);
    ~QVisionDefect();

    void clear();

protected:
    Catalog family() const;

    void setResultDefects(const QList<QVariant>&);

private:
    QVisionDefectPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QVisionDefect)
    Q_DISABLE_COPY(QVisionDefect)

    QList<QVariant> resultDefects() const;
};

#endif // QVISIONDEFECT_H
