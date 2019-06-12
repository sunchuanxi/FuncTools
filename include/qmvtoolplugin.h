#ifndef QMVTOOLPLUGIN_H
#define QMVTOOLPLUGIN_H

#include <QObject>
#include <QMetaType>

struct QFixture {
    double x = 0.;
    double y = 0.;
    double score = 1.;
    double angle = 0.;
    double scale = 1.;
};

#define MVToolLocationInterface_iid         "com.zigaa.location"
#define MVToolPresenceAbsenceInterface_iid  "com.zigaa.presence_absence"
#define MVToolMeasurementInterface_iid      "com.zigaa.measurement"
#define MVToolCountingInterface_iid         "com.zigaa.counting"
#define MVToolIdentificationInterface_iid   "com.zigaa.identification"
#define MVToolGeometryInterface_iid         "com.zigaa.geometry"
#define MVToolMathLogicInterface_iid        "com.zigaa.math_logic"
#define MVToolPlotInterface_iid             "com.zigaa.plot"
#define MVToolImageFilterInterface_iid      "com.zigaa.image_filter"
#define MVToolDefectDetectionInterface_iid  "com.zigaa.defect_detection"
#define MVToolCalibrationInterface_iid      "com.zigaa.calibration"

#define CONNECT_VALUE_TO_JOB(space, type, key)\
    connect(this, &space::key##Changed, [&](type value){ emit valueChanged(#key, value); })

class QMVToolPluginPrivate;
class QMVToolPlugin : public QObject {
    Q_OBJECT

public:
    enum Catalog { Location, Presence_Absence, Measurement, Counting, Identification, Geometry, Math_Logic,
                   Plot, Image_Filter, Defect_Detection, Calibration };
    enum ROIShape  { ROIRectangle, ROICircle, ROIAnnulus, ROIPolygon, ROIRoundedRectangleRing, ROIEllipseRing };
    /*enum ROIShape  { ROIRectangle, ROICircle, ROIAnnulus, ROIPolygon
                   , ROIPoint, ROILine
                   , ROIRoundedRectangle, ROIEllipse, ROIRectangleRing, ROIRoundedRectangleRing, ROIEllipseRing
                   , ROIScanningLine0, ROIScanningLine1, ROIScanningCircle};*/
    enum ROIType { Point, Line, Surface };
    enum AnnulusDirection   { FI2O, FO2I, Clockwise, AntiClockwise };
    enum RectangleDirection { FT2B, FB2T, FL2R, FR2L };
    enum ResultStatus { NONE, OK, FAIL, VAGUE };
    enum ShowType { HideAll, ShowFail, ShowAll };

    explicit QMVToolPlugin(QObject *parent = Q_NULLPTR);
    ~QMVToolPlugin();

    virtual QString version() const = 0;
    virtual QString pluginName() const = 0;
    virtual QString description() const = 0;

    QObject* create(quint8 station, quint8 camera, quint8 threadID, quint16 productID, const QString& cyphertext, QObject* parent = Q_NULLPTR) const;

protected:
    virtual QObject* create(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QObject* parent = Q_NULLPTR) const = 0;

private:
    QMVToolPluginPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QMVToolPlugin)
    Q_DISABLE_COPY(QMVToolPlugin)
};

Q_DECLARE_METATYPE(QFixture)
Q_DECLARE_METATYPE(QMVToolPlugin::ShowType)

#endif // QMVTOOLPLUGIN_H
