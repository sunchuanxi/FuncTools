#ifndef VDGPU2PRIVATE_H
#define VDGPU2PRIVATE_H
#include <QPixmap>
#include <cpp/HalconCpp.h>
#include "../include/qmvtoolplugin.h"
#include "SuaKITRuntime.h"
#include "stdio.h"
#include <fstream>   
#include <string>  
#include <iostream>  
#include <streambuf> 
#include "opencv2\opencv.hpp"

using namespace std;
struct csInfo
{
	string name;
	string isOK = "OK";
	int index = 0;
	int defectKind = 0;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	double value = 0;
	double zangwu = 0;
	double huahen = 0;
	double quejiao = 0;
};

class QvdGPU2;
class QvdGPU2Private {
public:
	QvdGPU2Private(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QvdGPU2* parent);
protected:
    void run(const Halcon::HImage&, const Halcon::HRegion&, const QByteArray&);
	BOOL SaveStatisticsExcel(string fileName, string type, double quejiao, double huahen, double zangwu, int i, int total, int index);
	bool UpdateNumberOfDefect(vector<csInfo>  *defectList1);
	void ProcessImage(cv::Mat img, std::vector<csInfo>& defectList, string csFileName, double dirtyThresh, double dirtyArea, double scratchLen, double losingEdgeArea);
private:
	Q_DECLARE_PUBLIC(QvdGPU2)
	QvdGPU2* const q_ptr;
    const quint8 station;
    const quint8 camera;
    const quint8 threadID;
    const quint8 productID;
    QMVToolPlugin::ShowType showType;
    QString resultStr;
    int resultStatus;
    int resultOutport = -1;
    QPixmap resultPixmap;
    QFixture resultFixture;
    Halcon::HRegionArray resultRegions;
    QList<QColor> resultRegionColors;
};

#endif // VDSUALPRIVATE_H
