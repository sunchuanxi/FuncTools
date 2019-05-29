#include <QtGui>
#include "vdSUAL.h"
#include "vdSUAL_p.h" 
#include "opencv2\opencv.hpp"
using namespace std;
SuaKIT::API::SegmentationEvaluator* citieSegmentationEvaluator[COLOR_CLASSIFICATION_MAX_MODEL_NUM];
int index = 0;
int imgNum = 0;
QvdSUALPrivate::QvdSUALPrivate(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QvdSUAL* parent)
	: q_ptr(parent)
	, station(station)
	, camera(camera)
	, threadID(threadID)
	, productID(productID)
{
	if (index == 0){
		int networkH = 700;
		int networkW = 700;
		int networkC = 1; // we currently support 1channel or BGR 3channel or BGRA 4channel 
		SuaKIT::API::DeviceDescriptorArray deviceDescArray;
		SuaKIT::API::DeviceDescriptor::GetAvailableGPUDevices(deviceDescArray);
		qWarning() << __LINE__;

		wstring netWrokPathList[6] = { L"./model/huahen.net", L"./model/quejiao.net", L"./model/zangwu.net" };
		// Init Evaluator
		for (int i = 0; i < COLOR_CLASSIFICATION_MAX_MODEL_NUM; i++)
		{
			qWarning() << __LINE__;
			qWarning() << deviceDescArray.GetLength();
			citieSegmentationEvaluator[i] = new SuaKIT::API::SegmentationEvaluator(netWrokPathList[i].c_str(), deviceDescArray.GetAt(0), networkH, networkW, networkC);
			qWarning() << __LINE__;
			if (citieSegmentationEvaluator[i]->GetStatus() != SuaKIT::API::SUCCESS)
			{
				citieSegmentationEvaluator[i]->Destroy();
				delete citieSegmentationEvaluator[i];
				citieSegmentationEvaluator[i] = nullptr;
			}
		}
	}
	index++;
}



cv::Mat  HImage2Mat(const Halcon::HImage& hImage){
	Hlong htChannels = 0;
	Halcon::HTuple   cType;
	Halcon::HTuple   width, height;
	cv::Mat          matImage;

	htChannels = hImage.CountChannels();
	if (htChannels == 1){
		Hlong ptr = NULL;
		ptr = hImage.GetImagePointer1(&cType, &width, &height);//得到Halcon数据指针
		matImage.create(height[0].I(), width[0].I(), CV_8UC(1));
		for (int i = 0; i < height[0].I(); i++){
			memcpy(matImage.ptr() + matImage.step * i, (void *)(ptr + width[0].I()*i), width[0].I());
		}
	}
	if (htChannels == 3){
		Hlong ptrRed, ptrGreen, ptrBlue;
		Halcon::HTuple ptrGreenT, ptrBlueT;
		long  imgWidth, imgHeight;
		ptrRed = hImage.GetImagePointer3(&ptrGreenT, &ptrBlueT, &cType, &width, &height);
		imgWidth = width[0].I();
		imgHeight = height[0].I();
		ptrGreen = ptrGreenT[0].L();
		ptrBlue = ptrBlueT[0].L();
		cv::Mat imgR(imgHeight, imgWidth, CV_8UC(1));
		cv::Mat imgG(imgHeight, imgWidth, CV_8UC(1));
		cv::Mat imgB(imgHeight, imgWidth, CV_8UC(1));

		for (int i = 0; i < imgHeight; i++){
			memcpy(imgR.ptr() + imgR.step*i, (void *)(ptrRed + imgWidth*i), imgWidth);
			memcpy(imgG.ptr() + imgG.step*i, (void *)(ptrGreen + imgWidth*i), imgWidth);
			memcpy(imgB.ptr() + imgB.step*i, (void *)(ptrBlue + imgWidth*i), imgWidth);
		}
		//imwrite("R.jpeg", imgR);
		//imwrite("G.jpeg", imgG);
		//imwrite("B.jpeg", imgB);
		std::vector<cv::Mat> mgImage;
		mgImage.push_back(imgB);
		mgImage.push_back(imgG);
		mgImage.push_back(imgR);
		merge(mgImage, matImage);
	}

	return matImage;
}

void ProcessImage(cv::Mat img, std::vector<csInfo>& defectList, string csFileName)
{
	imgNum++;
	if (citieSegmentationEvaluator[2] == nullptr || citieSegmentationEvaluator[1] == nullptr){
		return;
	}
	//qWarning() << __LINE__ << "test";
	cv::Mat rstImg;
	{
		string result;
		double totalArea = 0.00;

		cv::Mat srcImg;
		cv::Mat dst;
		cv::Mat originalImg;//init

		//read image
		SuaKIT::API::ImageData resultImg;
		SuaKIT::API::RectArray resultRects;
		SuaKIT::API::ImageData resultImg1;
		SuaKIT::API::RectArray resultRects1;
		SuaKIT::API::ImageData resultImg2;
		SuaKIT::API::RectArray resultRects2;
		bool isNG = true;
		img.copyTo(srcImg);//
		cv::Mat huahen;
		srcImg.copyTo(huahen);
		cv::cvtColor(huahen, huahen, CV_GRAY2BGR);
		cv::Mat bin;
		srcImg.copyTo(bin);
		cv::Rect ROIrect(318, 154, 700, 700);
		if (srcImg.cols > 700 && srcImg.rows > 700)
		{
			srcImg = srcImg(ROIrect);
		}
		else
		{
			return;
		}
		//qWarning() << __LINE__ << "test";

		//
		srcImg.copyTo(srcImg);
		SuaKIT::API::Rect roi(318, 154, 700, 700);
		SuaKIT::API::ImageData curImg(bin.data, bin.step, bin.cols, bin.rows, bin.channels(), roi);

		//检测脏污
		//qWarning() << __LINE__ << "test";
		size_t numClass2 = citieSegmentationEvaluator[2]->GetClassTotalNum();
		//qWarning() << __LINE__ << "test";
		SuaKIT::API::SizeArray minDefectSizes2(numClass2);
		for (int i = 0; i < numClass2; ++i)
		{
			SuaKIT::API::Size curSize1 = { 8, 8 };	// user can choose the min defect size for each class.
			SuaKIT::API::Size curSize2 = { 0, 0 };
			if (i == 0)
				minDefectSizes2.SetAt(i, curSize1);
			else
				minDefectSizes2.SetAt(i, curSize2);
		}
		citieSegmentationEvaluator[2]->Evaluate(curImg, resultImg, resultRects, minDefectSizes2);
		cv::Mat resultMat(resultImg.GetHeight(), resultImg.GetWidth(), CV_MAKE_TYPE(CV_8U, resultImg.GetChannel()), resultImg.GetDataPtr());
		cv::Mat imgGray;
		srcImg.copyTo(imgGray);
		cv::Mat binary_image, pic_clear;
		adaptiveThreshold(imgGray, pic_clear, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 177, -4);//图片二值化
		std::vector<std::vector<cv::Point>> vecContours;
		cv::Mat mask(srcImg.rows, srcImg.cols, srcImg.type(), cv::Scalar());
		if (pic_clear.empty())
		{
			return;
		}
		findContours(pic_clear, vecContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		cv::Rect boundRect;//定义cv::Rect类型的vector容器boundRect存放正外接矩形，初始化大小为轮廓个数
		int iFin = 0;
		int widthMax = 0;
		int heightMax = 0;
		for (int i = 0; i < vecContours.size(); i++)
		{
			boundRect = boundingRect(cv::Mat(vecContours[i]));
			if (boundRect.width < 200 || boundRect.height < 200)
				continue;
			rectangle(imgGray, boundRect, cv::Scalar(255, 0, 0), 1, 8);
			if (widthMax < boundRect.width&&heightMax < boundRect.height)
			{
				widthMax = boundRect.width;
				heightMax = boundRect.height;
				iFin = i;
			}
		}
		boundRect = boundingRect(cv::Mat(vecContours[iFin]));//目标轮廓
		totalArea = cv::contourArea(vecContours[iFin]);
		drawContours(mask, vecContours, iFin, cv::Scalar(1), CV_FILLED, 8);
		cv::Mat dst1(srcImg.rows, srcImg.cols, srcImg.type(), cv::Scalar());
		if (resultRects.GetLength() != 0)
		{
			cv::Mat img = ~resultMat;
			std::vector<std::vector<cv::Point>> vecContour;
			findContours(img.clone(), vecContour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			cv::Rect rest2;
			std::vector<double> meanGray;
			std::vector<int> roiArea;
			std::vector<cv::Rect> defectPoint, defectPointTemp;
			std::vector<cv::Mat> defectImg;
			for (int i = 0; i < vecContour.size(); i++)
			{
				rest2 = boundingRect(cv::Mat(vecContour[i]));
				if (rest2.x > boundRect.x - 5 && rest2.y > boundRect.y - 5 && rest2.x < boundRect.x + boundRect.width + 10 && rest2.y < boundRect.y + boundRect.height + 10)
				{
					int counts = 0;
					cv::Mat temp;
					cv::Mat masks(srcImg.rows, srcImg.cols, srcImg.type(), cv::Scalar());
					drawContours(masks, vecContour, i, cv::Scalar(1), CV_FILLED, 8);
					temp = srcImg.mul(masks);
					defectImg.push_back(temp);
					drawContours(mask, vecContour, i, cv::Scalar(0), CV_FILLED, 8);
					for (int i = rest2.x; i < rest2.x + rest2.width; i++)
					{
						for (int j = rest2.y; j < rest2.y + rest2.height; j++)
						{
							if (img.at<uchar>(j, i) != 0)
								counts++;
						}
					}
					roiArea.push_back(counts);
					defectPointTemp.push_back(rest2);
				}
			}
			int num = 0;
			dst1 = srcImg.mul(mask);
			for (int i = boundRect.x; i < boundRect.x + boundRect.width; i++)
			{
				for (int j = boundRect.y; j < boundRect.y + boundRect.height; j++)
				{
					if (mask.at<uchar>(j, i) != 0)
						num++;
				}
			}
			double res = srcImg.dot(mask);
			double bacM = res / num;
			int count = 0;
			for (int k = 0; k < defectImg.size(); k++)
			{
				cv::Mat defectImgTemp;
				defectImgTemp = defectImg[k].clone();
				rest2 = defectPointTemp[k];
				for (int i = rest2.x; i < rest2.x + rest2.width; i++)
				{
					for (int j = rest2.y; j < rest2.y + rest2.height; j++)
					{
						if (defectImg[k].at<uchar>(j, i) > bacM - 30 || defectImg[k].at<uchar>(j, i) == 0)
						{
							defectImgTemp.at<uchar>(j, i) = 0;
						}
						else
						{
							defectImgTemp.at<uchar>(j, i) = 1;
							count++;
						}
					}
				}
				double areas = defectImgTemp.dot(srcImg);
				double grayM = areas / count;
				meanGray.push_back(grayM);
				rest2.x += roi.x;
				rest2.y += roi.y;
				defectPoint.push_back(rest2);
			}

			for (int i = 0; i < meanGray.size(); i++)
			{
				csInfo info;
				info.name = csFileName;//时间戳
				info.isOK = "NG";
				info.defectKind = 2;
				int RoiArea = roiArea[i];
				info.x = defectPoint[i].x;
				info.y = defectPoint[i].y;
				info.width = defectPoint[i].width;
				info.height = defectPoint[i].height;
				double a = bacM - meanGray[i];
				if (a < 0)
					a = -a;
				info.value = a / bacM;
				info.zangwu = (int)(info.value*100.0) / 100.0;
				info.huahen = 0;
				info.quejiao = 0;
				info.index = imgNum;
				double radio = RoiArea / totalArea;
				if (info.value >= 0.25 || radio >= 0.45)
				{
					isNG = false;
					result = "NG";
					cv::rectangle(huahen, cv::Rect(info.x, info.y, info.width, info.height), cv::Scalar(255, 0, 0), 1, 1, 0);
					defectList.push_back(info);
				}
			}
			//}
		}
		//qWarning() << __LINE__ << "test";

		//检测缺角
		size_t numClass = citieSegmentationEvaluator[1]->GetClassTotalNum();
		SuaKIT::API::SizeArray minDefectSizes(numClass);
		for (int i = 0; i < numClass; ++i)
		{
			SuaKIT::API::Size curSize1 = { 8, 8 };	// user can choose the min defect size for each class.
			SuaKIT::API::Size curSize2 = { 0, 0 };
			if (i == 0)
				minDefectSizes.SetAt(i, curSize1);
			else
				minDefectSizes.SetAt(i, curSize2);
		}
		SuaKIT::API::Rect roi1(318, 154, 700, 700);
		SuaKIT::API::ImageData curImg1(bin.data, bin.step, bin.cols, bin.rows, bin.channels(), roi1);
		citieSegmentationEvaluator[1]->Evaluate(curImg1, resultImg1, resultRects1);// , minDefectSizes);
		cv::Mat resultMat1(resultImg1.GetHeight(), resultImg1.GetWidth(), CV_MAKE_TYPE(CV_8U, resultImg1.GetChannel()), resultImg1.GetDataPtr());
		cv::Mat img = ~resultMat1;
		if (resultRects1.GetLength() > 0)
		{
			for (int jIndex = 0; jIndex<resultRects1.GetLength(); jIndex++)
			{
				SuaKIT::API::Rect temp = resultRects1.GetAt(jIndex);
				if (temp.x > boundRect.x&&temp.y > boundRect.y&& temp.x < boundRect.x + boundRect.width&&temp.y < boundRect.y + boundRect.height)
				{
					int counts = 0;
					for (int m = temp.x; m < temp.x + temp.width; m++)
					{
						for (int n = temp.y; n < temp.y + temp.height; n++)
						{
							if (img.at<uchar>(n, m) == 255)
								counts++;
						}
					}
					int defectX, defectY, defectW, defectH;
					defectX = temp.x + roi1.x;
					defectY = temp.y + roi1.y;
					defectW = temp.width;
					defectH = temp.height;
					cv::Rect  defectRect(defectX, defectY, defectW, defectH);
					csInfo info;
					info.name = csFileName;
					info.isOK = "NG";
					result = ("NG");
					info.defectKind = 0;
					info.x = defectX;
					info.y = defectY;
					info.width = defectW;
					info.height = defectH;
					double dArea;
					dArea = counts*0.01483*0.01483;
					info.value = dArea;
					info.quejiao = (int)(dArea*100.0) / 100.0;
					info.huahen = 0;
					info.zangwu = 0;
					info.index = imgNum;
					if (info.value >= 0.09)
					{
						isNG = false;
						result = ("NG");
						cv::rectangle(huahen, defectRect, cv::Scalar(0, 255, 0), 1, 1, 0);
						defectList.push_back(info);
					}
				}
			}
		}
		//qWarning() << __LINE__ << "test";

		//检测划痕
		SuaKIT::API::ImageData curImg2(bin.data, bin.step, bin.cols, bin.rows, bin.channels(), roi);
		citieSegmentationEvaluator[0]->Evaluate(curImg2, resultImg2, resultRects2);
		if (resultRects2.GetLength() != 0)
		{
			cv::Mat dstImg(resultImg2.GetHeight(), resultImg2.GetWidth(), CV_MAKE_TYPE(CV_8U, resultImg2.GetChannel()), resultImg2.GetDataPtr(), resultImg2.GetStep());
			cv::Mat img = ~dstImg;

			std::vector<std::vector<cv::Point>> vecContour;
			findContours(img.clone(), vecContour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			cv::Rect rest2;
			cv::RotatedRect roRect;
			std::vector<double> lengths;
			for (int i = 0; i < vecContour.size(); i++)
			{
				rest2 = boundingRect(cv::Mat(vecContour[i]));
				if (rest2.x > boundRect.x&&rest2.y > boundRect.y&& rest2.x < boundRect.x + boundRect.width&&rest2.y < boundRect.y + boundRect.height)
				{

					roRect = minAreaRect(cv::Mat(vecContour[i]));
					cv::Point2f pts[4] = { 0 };
					roRect.points(pts);
					int x1 = pts[1].x - pts[0].x;
					int y1 = pts[1].y - pts[0].y;
					int x2 = pts[3].x - pts[0].x;
					int y2 = pts[3].y - pts[0].y;
					double L1 = sqrt(x1*x1 + y1*y1);
					double L2 = sqrt(x2*x2 + y2*y2);
					double length = 0;
					if (L1 < L2)
					{
						length = L2*0.01483;
					}
					else
					{
						length = L1*0.01483;
					}
					csInfo info;
					info.name = csFileName;
					info.isOK = "NG";
					info.defectKind = 1;

					info.x = rest2.x + roi.x;
					info.y = rest2.y + roi.y;
					info.width = rest2.width;
					info.height = rest2.height;
					info.value = length;
					info.huahen = (int)(length*100.0) / 100.0;
					info.quejiao = 0;
					info.zangwu = 0;
					info.index = imgNum;
					if (info.value >= 5)
					{
						isNG = false;
						result = ("NG");
						cv::rectangle(huahen, cv::Rect(info.x, info.y, info.width, info.height), cv::Scalar(0, 0, 255), 1, 1, 0);
						defectList.push_back(info);
					}
				}
			}
			//saveResult
		}
		if (isNG)
		{
			csInfo info;
			info.name = csFileName;
			info.isOK = "OK";
			info.index = imgNum;
			result = ("OK");
			defectList.push_back(info);
		}

		csInfo info;
		info.name = csFileName;
		info.isOK = result;
		srcImg.copyTo(rstImg);
	}
}

BOOL QvdSUALPrivate::SaveStatisticsExcel(string fileName, string type, double quejiao, double huahen, double zangwu, int i, int total, int index)
{
	SYSTEMTIME st;
	string defectLogFile, statisticsLogFile, statisticsLogFile1;
	GetLocalTime(&st);
	//statisticsLogFile1.Format(("D:\\SuakitDetectCsv\\%d%d%d_statistics.csv"), st.wYear, st.wMonth, st.wDay);
	//QDir path = R"(D:\SuakitDetectCsv\)";
	QString myPath = "D:/SuakitDetectCsv";
	myPath = myPath.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
	QDir path(myPath);
	if (!path.exists()){
		path.mkdir(myPath);
	}
	statisticsLogFile1 = myPath.append(QDateTime::currentDateTime().toString("yyyy-MM-dd").remove(":").remove("-").remove(" ")) \
			.append("_statistics.csv").toStdString();

	bool file_status;
	ifstream iFile(statisticsLogFile1, ios::in);
	file_status = iFile.is_open();
	iFile.close();

	std::string sz11 = fileName;// .GetBuffer());
	std::string sz21 = type;// .GetBuffer());
	const char* sz1 = sz11.c_str();
	const char* sz2 = sz21.c_str();

	ofstream oFile(statisticsLogFile1, ios::out | ios::app);

	if (file_status)
	{
		if (i != (total - 1))
		{
			if (i == 0)
			{
				if (type == "NG")
					oFile << index << "," << sz1 << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "," << endl;
				else
					oFile << index << "," << sz1 << "," << sz2 << "," << " " << "," << " " << "," << " " << "," << endl;
			}
			else
			{
				oFile << " " << "," << " " << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "," << endl;
			}
		}
		else
		{
			if (i == 0)
			{
				if (type == "NG")
					oFile << index << "," << sz1 << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "\n" << endl;
				else
					oFile << index << "," << sz1 << "," << sz2 << "," << " " << "," << " " << "," << " " << "\n" << endl;
			}

			else
				oFile << " " << "," << " " << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "," << "\n" << endl;
		}
	}
	else
	{
		//oFile.open(statisticsLogFile1, ios::out | ios::app);    // 这样就很容易的输出一个需要的excel 文件  
		oFile << "No" << "," << "Timing" << "," << "Result" << "," << "Dent(mm*mm)" << "," << "Scratch(mm)" << "," << "Dirt(%)" << endl;
		if (i != (total - 1))
		{
			if (i == 0)
			{
				if (type == ("NG"))
					oFile << index << "," << sz1 << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "," << endl;
				else
					oFile << index << "," << sz1 << "," << sz2 << "," << " " << "," << " " << "," << " " << "," << endl;
			}
			else
			{
				oFile << " " << "," << " " << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "," << endl;
			}
		}
		else
		{
			if (i == 0)
			{
				if (type == ("NG"))
					oFile << index << "," << sz1 << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "\n" << endl;
				else
					oFile << index << "," << sz1 << "," << sz2 << "," << " " << "," << " " << "," << " " << "\n" << endl;
			}

			else
				oFile << " " << "," << " " << "," << sz2 << "," << quejiao << "," << huahen << "," << zangwu * 100 << "," << "\n" << endl;
		}

	}
	oFile.close();
	return true;
}

//BOOL SaveStatisticsExcel(string fileName, string type, double quejiao, double zangwu, double huahen, int i, int total, int index)
//{
//	SYSTEMTIME st;
//	string defectLogFile, statisticsLogFile, statisticsLogFile1;
//	GetLocalTime(&st);

//QDir path = R"(D:\SuakitDetectCsv\)";
//QString myPath = R"(D:\SuakitDetectCsv\)";
//if (!path.exists()){
//	path.mkdir(myPath);
//}
//	statisticsLogFile1 = name1.append(QDateTime::currentDateTime().toString("yyyy-MM-dd").remove(":").remove("-").remove(" ")) \
//		.append("_statistics.csv").toStdString();
//
//	bool file_status;
//	ifstream iFile(statisticsLogFile1, ios::in);
//	file_status = iFile.is_open();
//	iFile.close();
//
//	std::string sz1 = fileName;
//	std::string sz2 = type;
//	//std::string sz3 = defectKind;
//
//	ofstream oFile(statisticsLogFile1, ios::out | ios::app);
//
//	if (file_status)
//	{
//		if (i != (total - 1))
//			oFile << sz1 << "," << sz2 << "," << sz3 << "," << value << "," << endl;
//		else
//			oFile << sz1 << "," << sz2 << "," << sz3 << "," << value << "," << "\n" << endl;
//	}
//	else
//	{
//		oFile << "文件名" << "," << "检测结果" << "," << "缺陷类型" << "," << "值" << endl;
//		if (i != (total - 1))
//			oFile << sz1 << "," << sz2 << "," << sz3 << "," << value << "," << endl;
//		else
//			oFile << sz1 << "," << sz2 << "," << sz3 << "," << value << "," << "\n" << endl;
//	}
//	oFile.close();
//	return true;
//}
//
bool QvdSUALPrivate::UpdateNumberOfDefect(vector<csInfo>  *defectList1)
{
	size_t total = defectList1->size();
	int i = 0;
	for (vector<csInfo>::iterator it = defectList1->begin(); it != defectList1->end(); it++)
	{
		string fileName, type, defectKind;
		//int ngNum;
		double value,zangwu,quejiao,huahen;
		int index;
		fileName = it->name;
		type = it->isOK;
		defectKind = it->defectKind;
		value = it->value;
		zangwu = it->zangwu;
		quejiao = it->quejiao;
		huahen = it->huahen;
		index = it->index;
		QvdSUALPrivate::SaveStatisticsExcel(fileName, type, quejiao, huahen, zangwu, i, total, index);
		i++;
	}
	return 0L;
}

void QvdSUALPrivate::run(const Halcon::HImage& image, const Halcon::HRegion& roi, const QByteArray& roiData = QByteArray())
{
	q_ptr->initResultMeasure();
	showType = QMVToolPlugin::ShowFail;
	resultPixmap = QPixmap(image.Width(), image.Height());
	resultPixmap.fill(Qt::transparent);
	QPainter _paint(&resultPixmap);
	QFont _font;
	_font.setPointSize(image.Height() / 100);
	_paint.setFont(_font);
	resultRegions = Halcon::HRegionArray();
	bool flagResult = true;
	bool flag = true;
	int fontsize = image.Height() / 20;
	short textLine = 0;
	bool portFlag1 = false;
	bool portFlag2 = false;
	bool portFlag3 = false;

	try{
		QString type;

		cv::Mat srcImg;
		srcImg = HImage2Mat(image);
		QString timestamp;
		timestamp = QDateTime::currentDateTime().toString("MM-dd hh:mm:ss.z").remove(":").remove("-").remove(" ");
		//timestamp.toStdString();
		string imgPath;
		//CreateDirectory((LPCWSTR)(imgPath.c_str()), NULL);
		//system("md D:\\origalImg\\");
		//QDir path = R"(D:\origalImg\)";
		QString myPath = "D:/origalImg";
		myPath = myPath.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
		QDir path(myPath);
		if (!path.exists()){
			path.mkdir(myPath);
			//system("md D:\\origalImg\\");
		}
			
		const char *fileName = myPath.toStdString().c_str(), *tag;
		imgPath = myPath.toStdString() + timestamp.toStdString();
		//cv::imwrite(imgPath, srcImg);
		vector<csInfo> result;
		//qWarning() << __LINE__ << " : test";
		ProcessImage(srcImg, result, timestamp.toStdString());
		
		for (vector<csInfo>::iterator i = result.begin(); i != result.end(); i++){
			/*qWarning() << __LINE__;
			cout << "OKorNG: "<< (*i).isOK << endl;
			cout << "name: " << (*i).name << endl;
			cout << "zangwu: " << (*i).zangwu << endl;
			cout << "quejiao: " << (*i).quejiao << endl;
			cout << "huahen: " << (*i).huahen << endl;
			cout << "height: " << (*i).height << endl;
			cout << "width: " << (*i).width << endl;
			cout << "x: " << (*i).x << endl;
			cout << "y: " << (*i).y << endl;
			cout << "value: " << (*i).value << endl;
			cout << "index: " << (*i).index << endl;*/
			if ((*i).isOK == "NG"){
				flagResult = false;
				resultStatus = QMVToolPlugin::FAIL;
				Halcon::HRegion resReg;
				resReg = Halcon::HRegion::GenRectangle1((*i).y, (*i).x, (*i).y + (*i).height, (*i).x + (*i).width);
				resultRegions.Append(resReg);
			}
			
			if ((*i).defectKind == 0){
				portFlag1 = true;
				type = QvdSUAL::tr("diaojiao");
			}

			if ((*i).defectKind == 1){
				portFlag2 = true;
				type = QvdSUAL::tr("huahen");
			}
			if ((*i).defectKind == 2){
				portFlag3 = true;
				type = QvdSUAL::tr("zangwu");
			}

			//resultStr = QvdSUAL::tr("Defect Type: ") + type;
			//_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
			//resultStr = QvdSUAL::tr("Area/Contrast") + QString("%1: %2").arg(lineIndex - 2).arg(AreaOrContrast, 0, 'f', 3);
			//_paint.drawText(image.Height() / 20 + 300, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
			//resultStr = QvdSUAL::tr("Length") + QString("%1: %2").arg(lineIndex - 2).arg(len);
			//_paint.drawText(image.Height() / 20 + 800, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
			//resultStr = QvdSUAL::tr("Width") + QString("%1: %2").arg(lineIndex - 2).arg(wid);
			//_paint.drawText(image.Height() / 20 + 1100, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
		}
		
		if (portFlag1){
			resultOutport = q_ptr->m_edgePort - 1;
			//flagResult = false;
		}
		if (!portFlag1 && portFlag2){
			resultOutport = q_ptr->m_scratchPort - 1;
			//flagResult = false;
		}
		if (!portFlag1 && !portFlag2 && portFlag3){
			resultOutport = q_ptr->m_mudgePort - 1;
			//flagResult = false;
		}
		
		//生成数据表格
		UpdateNumberOfDefect(&result);
		
		if (!flagResult){
			imgPath = imgPath + "_NG" + ".bmp";
			cv::imwrite(imgPath, srcImg);
			//cv::imwrite(imgPath, srcImg);
			_paint.setPen(Qt::red);
			resultStatus = QMVToolPlugin::FAIL;
			resultStr = QvdSUAL::tr("NG");
			_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
		}
		else{
			imgPath = imgPath + "_OK" + ".bmp";
			cv::imwrite(imgPath, srcImg);
			_paint.setPen(Qt::green);
			resultStatus = QMVToolPlugin::OK;
			resultStr = QvdSUAL::tr("OK");
			_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
		}
		
		return;
	}
	catch (const Halcon::HException& e) {
		_paint.setPen(Qt::yellow);
		resultStatus = QMVToolPlugin::VAGUE;
		qWarning() << "vdSUAL.dll:" << e.message;
		return;
	}
}


