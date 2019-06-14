#ifndef VDGPU2_H
#define VDGPU2_H
#include <QPixmap>
#include <cpp/HalconCpp.h>
#include <QtSql/QtSql>
#include "vdGPU2_p.h"
#include "../include/qmvtoolplugin.h"
///////////////////////////////////////////////////////////////////////////////
#define COLOR_CLASSIFICATION_MAX_MODEL_NUM 3

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

class QvdGPU2Private;
class QvdGPU2 : public QObject {
    Q_OBJECT
    Q_PROPERTY(int calibration_type MEMBER m_calibration_type NOTIFY calibration_typeChanged)
	Q_PROPERTY(int edgePort MEMBER m_edgePort NOTIFY edgePortChanged)
	Q_PROPERTY(int scratchPort MEMBER m_scratchPort NOTIFY scratchPortChanged)
	Q_PROPERTY(int mudgePort MEMBER m_mudgePort NOTIFY mudgePortChanged)

    Q_PROPERTY(double dirtyArea     MEMBER m_dirtyArea     NOTIFY dirtyAreaChanged)
	Q_PROPERTY(double dirtyThresh   MEMBER m_dirtyThresh   NOTIFY dirtyThreshChanged)
	Q_PROPERTY(double losingAngle   MEMBER m_losingAngle   NOTIFY losingAngleChanged)
	Q_PROPERTY(double scratchLength MEMBER m_scratchLength NOTIFY scratchLengthChanged)

    Q_PROPERTY(QMVToolPlugin::ShowType showType READ showType CONSTANT)
    Q_PROPERTY(QString resultStr READ resultStr CONSTANT)
    Q_PROPERTY(int resultStatus READ resultStatus CONSTANT)
    Q_PROPERTY(int resultOutport READ resultOutport CONSTANT)
    Q_PROPERTY(QPixmap resultPixmap READ resultPixmap CONSTANT)
    Q_PROPERTY(QFixture resultFixture READ resultFixture CONSTANT)
    Q_PROPERTY(Halcon::HRegionArray resultRegions READ resultRegions CONSTANT)
    Q_PROPERTY(QList<QColor> resultRegionColors READ resultRegionColors CONSTANT)
public:
    QvdGPU2(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent = Q_NULLPTR);
	~QvdGPU2();
signals:
	void calibration_typeChanged(int calibration_type);
	void edgePortChanged(int edgePort);
	void scratchPortChanged(int scratchPort);
	void mudgePortChanged(int mudgePort);

    void dirtyAreaChanged(double dirtyArea);
    void dirtyThreshChanged(double dirtyThresh);
    void losingAngleChanged(double losingAngle);
    void scratchLengthChanged(double scratchLength);

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
	Q_DECLARE_PRIVATE(QvdGPU2)
	Q_DISABLE_COPY(QvdGPU2)
	QvdGPU2Private* const d_ptr;

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
    double m_dirtyArea     = 0.15;
    double m_dirtyThresh   = 0.25;
    double m_losingAngle   = 0.1;
    double m_scratchLength = 0.1;
};
#endif // VDSUAL_H
