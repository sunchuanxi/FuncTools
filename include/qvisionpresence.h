#ifndef QVISIONPRESENCE_H
#define QVISIONPRESENCE_H

#include "qvisioninterface.h"

class QVisionPresencePrivate;
class ELVISIONSHARED_EXPORT QVisionPresence : public QVisionInterface {
    Q_OBJECT

    Q_PROPERTY(bool resultExists READ resultExists CONSTANT)

public:
    explicit QVisionPresence(const QString&, QObject *parent = 0);
    ~QVisionPresence();

    void clear();

protected:
    Catalog family() const;

    void setResultExists(bool);

private:
    QVisionPresencePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QVisionPresence)
    Q_DISABLE_COPY(QVisionPresence)

    bool resultExists() const;
};

#endif // QVISIONPRESENCE_H
