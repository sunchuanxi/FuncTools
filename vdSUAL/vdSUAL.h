#ifndef VDSUAL_H
#define VDSUAL_H
#include <QPixmap>
#include <cpp/HalconCpp.h>
#include <QtSql/QtSql>
#include "../include/qmvtoolplugin.h"
///////////////////////////////////////////////////////////////////////////////

struct QDictItem {
	QString name = "";
	double  value;
	qint32  level;
};
struct InspectResult {
	double  mvalue = 0.0;
	double  mstavalue = 0.0;
	double  mupTole = 0.0;
	double  mloTole = 0.0;
	qint32  mstatus = -1;
	//qint32  moutput    = -1;
};
enum{ sortDefect, sortMeasure };
class QvdSUALPrivate;
class QvdSUAL : public QObject {
    Q_OBJECT
    Q_PROPERTY(int calibration_type MEMBER m_calibration_type NOTIFY calibration_typeChanged)
	Q_PROPERTY(int edgePort MEMBER m_edgePort NOTIFY edgePortChanged)
	Q_PROPERTY(int scratchPort MEMBER m_scratchPort NOTIFY scratchPortChanged)
	Q_PROPERTY(int mudgePort MEMBER m_mudgePort NOTIFY mudgePortChanged)

    Q_PROPERTY(QMVToolPlugin::ShowType showType READ showType CONSTANT)
    Q_PROPERTY(QString resultStr READ resultStr CONSTANT)
    Q_PROPERTY(int resultStatus READ resultStatus CONSTANT)
    Q_PROPERTY(int resultOutport READ resultOutport CONSTANT)
    Q_PROPERTY(QPixmap resultPixmap READ resultPixmap CONSTANT)
    Q_PROPERTY(QFixture resultFixture READ resultFixture CONSTANT)
    Q_PROPERTY(Halcon::HRegionArray resultRegions READ resultRegions CONSTANT)
    Q_PROPERTY(QList<QColor> resultRegionColors READ resultRegionColors CONSTANT)
public:
    QvdSUAL(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent = Q_NULLPTR);
    ~QvdSUAL();
signals:
	void calibration_typeChanged(int calibration_type);
	void edgePortChanged(int edgePort);
	void scratchPortChanged(int scratchPort);
	void mudgePortChanged(int mudgePort);

    void valueChanged(const QString& key, const QVariant& value);
public slots:
    void run(const Halcon::HImage&, const Halcon::HRegion&, const QByteArray&);
	void calibration(const QVariant&);
protected:
    QMVToolPlugin::ShowType showType() const;
    QString resultStr() const;
    int resultStatus() const;
    int resultOutport() const;
    QPixmap resultPixmap() const;
    QFixture resultFixture() const;
    Halcon::HRegionArray resultRegions() const;
    QList<QColor> resultRegionColors() const;
private:
    Q_DECLARE_PRIVATE(QvdSUAL)
    Q_DISABLE_COPY(QvdSUAL)
    QvdSUALPrivate* const d_ptr;

	QMap<QString, QDictItem> m_dictDefect;
	QMap<QString, InspectResult> initResult;
	QMap<QString, InspectResult> inspectResult;
	void initDictDefect();
	void initResultMeasure();

	int    m_calibration_type = 0;
	int    m_edgePort    = 0;
	int    m_scratchPort = 0;
	int    m_mudgePort   = 0;
	bool   flagCellSize = true;
};
#endif // VDSUAL_H
