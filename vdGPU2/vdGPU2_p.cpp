#include <QtGui>
#include "vdGPU2.h"
#include "vdGPU2_p.h" 
#include "opencv2\opencv.hpp"
#include "SuaKITRuntime.h"
#include "spdhelper.hpp"
using namespace std;

SuaKIT::API::SegmentationEvaluator* citieSegmentationEvaluator[COLOR_CLASSIFICATION_MAX_MODEL_NUM];
int index  = 0;
int imgNum = 0;

QvdGPU2Private::QvdGPU2Private(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QvdGPU2* parent)
	: q_ptr(parent)
	, station(station)
	, camera(camera)
	, threadID(threadID)
	, productID(productID)
{
	if (index == 0)
	{
		int networkH = 700;
		int networkW = 700;
		int networkC = 1; // we currently support 1channel or BGR 3channel or BGRA 4channel 
		SuaKIT::API::DeviceDescriptorArray deviceDescArray;
		SuaKIT::API::DeviceDescriptor::GetAllGPUDevices(deviceDescArray);
		

		wstring netWrokPathList[6] = {L"./model2/quejiao.net", L"./model2/zangwu.net" }; //, L"./model1/yiwu.net" };
		// Init Evaluator
		for (int i = 0; i < COLOR_CLASSIFICATION_MAX_MODEL_NUM; i++)
		{
			//qWarning() << __LINE__;
			//qWarning() << deviceDescArray.GetLength();
			citieSegmentationEvaluator[i] = new SuaKIT::API::SegmentationEvaluator(netWrokPathList[i].c_str(), deviceDescArray.GetAt(0), networkH, networkW, networkC);
			//qWarning() << __LINE__;
			if (citieSegmentationEvaluator[i]->GetStatus() != SuaKIT::API::SUCCESS)
			{
				citieSegmentationEvaluator[i]->Destroy();
				delete citieSegmentationEvaluator[i];
				citieSegmentationEvaluator[i] = nullptr; 
			}
		}
		if (index==0)
		  qWarning() << __LINE__;
	}
	bc::spdhelper::LOG_INIT("vdGPU2.log", "./log", false, true);
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

void QvdGPU2Private::ProcessImage(cv::Mat img, std::vector<csInfo>& defectList, string csFileName, double dirtyThresh, double dirtyArea, double scratchLen, double losingEdgeArea)
{
	//try
	{
		imgNum++;
		if (citieSegmentationEvaluator[1] == nullptr || citieSegmentationEvaluator[0] == nullptr){
			return;
		}
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
			SuaKIT::API::Status status;

			bool isNG = true;
			bool isDent = false;
			bool isHuahen = false;
			bool isZangwu = false;
			img.copyTo(srcImg);//
			/*cv::Mat huahen;
			srcImg.copyTo(huahen);
			cv::cvtColor(huahen, huahen, CV_GRAY2BGR);*/
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

			//
			srcImg.copyTo(srcImg);
			SuaKIT::API::Rect roi(318, 154, 700, 700);

			cv::Mat imgGray;
			srcImg.copyTo(imgGray);
			cv::Mat pic_clear;
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
			int sizeofvec = vecContours.size();
			for (int i = 0; i < sizeofvec; ++i)
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

			//qWarning() << "相机" << (q_ptr->m_calibration_type + 1) << "GPUID= " << citieSegmentationEvaluator[0]->GetDeviceId();
			//qWarning() << "相机" << (q_ptr->m_calibration_type + 1) << "GPUID= " << citieSegmentationEvaluator[1]->GetDeviceId();
			//qWarning() << "相机" << (q_ptr->m_calibration_type + 1) << "GPUID= " << citieSegmentationEvaluator[2]->GetDeviceId();
			//检测缺角
			if (isNG)
			{
				size_t numClass = citieSegmentationEvaluator[0]->GetClassTotalNum();
				SuaKIT::API::SizeArray minDefectSizes(numClass);
				for (int i = 0; i < numClass; ++i)
				{
					SuaKIT::API::Size curSize1 = { 8, 8 };	// user can choose the min defect size for each class.
					SuaKIT::API::Size curSize2 = { 10, 10 };
					SuaKIT::API::Size curSize3 = { 0, 0 };
					if (i == 0)
						minDefectSizes.SetAt(i, curSize1);
					else if (i == 1)
						minDefectSizes.SetAt(i, curSize2);
					else
						minDefectSizes.SetAt(i, curSize3);
				}
				//SuaKIT::API::Rect roi1(318, 154, 700, 700);
				SuaKIT::API::ImageData curImg1(bin.data, bin.step, bin.cols, bin.rows, bin.channels(), roi);
				status = citieSegmentationEvaluator[0]->Evaluate(curImg1, resultImg1, resultRects1, minDefectSizes);
				if (status != SuaKIT::API::Status::SUCCESS)
				{
					csInfo info;
					info.status = false;
					defectList.push_back(info);
					return;
				}
				cv::Mat resultMat1(resultImg1.GetHeight(), resultImg1.GetWidth(), CV_MAKE_TYPE(CV_8U, resultImg1.GetChannel()), resultImg1.GetDataPtr());
				cv::Mat img = ~resultMat1;
				if (resultRects1.GetLength() > 0)
				{
					int wid = boundRect.x + boundRect.width;
					int hei = boundRect.y + boundRect.height;
					int x = boundRect.x - 5;
					int y = boundRect.y - 5;
					if (x < 0)
					{
						x = 0;
					}
					if (y < 0)
					{
						y = 0;
					}

					int dentnum = 0;

					for (int jIndex = 0; jIndex < resultRects1.GetLength(); ++jIndex)
					{
						SuaKIT::API::Rect temp = resultRects1.GetAt(jIndex);
						if (temp.classNumber == 0)
						{
							dentnum++;
						}
					}

					for (int jIndex = 0; jIndex < resultRects1.GetLength(); ++jIndex)
					{
						SuaKIT::API::Rect temp = resultRects1.GetAt(jIndex);
						if (temp.classNumber == 0)
						{
							if (temp.x > x&&temp.y > y&& temp.x < wid + 10 && temp.y < hei + 10)
							{
								int counts = 0;
								for (int m = temp.x; m < temp.x + temp.width; ++m)
								{
									for (int n = temp.y; n < temp.y + temp.height; ++n)
									{
										if (img.at<uchar>(n, m) == 255)
											counts++;
									}
								}
								int defectX, defectY, defectW, defectH;
								defectX = temp.x + roi.x;
								defectY = temp.y + roi.y;
								defectW = temp.width;
								defectH = temp.height;
								cv::Rect  defectRect(defectX, defectY, defectW, defectH);
								csInfo info;
								info.name = csFileName;
								info.isOK = "NG";
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
								if (dentnum > 1)
								{
									isNG = false;
									isDent = true;
									result = ("NG");
									//cv::rectangle(huahen, defectRect, cv::Scalar(0, 255, 0), 1, 1, 0);
									defectList.push_back(info);
								}
								else
								{
									if (info.value >= losingEdgeArea)
									{
										isNG = false;
										isDent = true;
										result = ("NG");
										//cv::rectangle(huahen, defectRect, cv::Scalar(0, 255, 0), 1, 1, 0);
										defectList.push_back(info);
									}
								}
							}
						}     //缺角
						else   //划痕
						{
							if (isNG)
							{
								std::vector<std::vector<cv::Point>> vecContour;
								findContours(img.clone(), vecContour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
								cv::Rect rest2;
								cv::RotatedRect roRect;
								std::vector<double> lengths;

								for (int i = 0; i < vecContour.size(); ++i)
								{
									rest2 = boundingRect(cv::Mat(vecContour[i]));
									if (rest2.x > x&&rest2.y > y&& rest2.x < wid + 10 && rest2.y < hei + 10)
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
										if (info.value >= scratchLen)
										{
											isNG = false;
											isHuahen = true;
											result = ("NG");
											//cv::rectangle(huahen, cv::Rect(info.x, info.y, info.width, info.height), cv::Scalar(0, 0, 255), 1, 1, 0);
											defectList.push_back(info);
										}
									}
								}
							}
						}
					}
				}
				if (isDent || isHuahen)
				{
					cv::imwrite("D:\\Camera2\\" + csFileName + "_NG.bmp", srcImg);
					LOGI("imgNum:{}", imgNum);
				}
			}

			//检测脏污
			if (isNG)
			{
				SuaKIT::API::ImageData curImg(bin.data, bin.step, bin.cols, bin.rows, bin.channels(), roi);
				size_t numClass2 = citieSegmentationEvaluator[1]->GetClassTotalNum();
				SuaKIT::API::SizeArray minDefectSizes2(numClass2);
				for (int i = 0; i < numClass2; ++i)
				{
					SuaKIT::API::Size curSize1 = { 7, 7 };	// user can choose the min defect size for each class.
					SuaKIT::API::Size curSize2 = { 0, 0 };
					if (i == 0)
						minDefectSizes2.SetAt(i, curSize1);
					else
						minDefectSizes2.SetAt(i, curSize2);
				}
				status = citieSegmentationEvaluator[1]->Evaluate(curImg, resultImg, resultRects, minDefectSizes2);
				if (status != SuaKIT::API::Status::SUCCESS)
				{
					csInfo info;
					info.status = false;
					defectList.push_back(info);
					return;
				}
				cv::Mat resultMat(resultImg.GetHeight(), resultImg.GetWidth(), CV_MAKE_TYPE(CV_8U, resultImg.GetChannel()), resultImg.GetDataPtr());
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
					cv::Mat masks;
					cv::Mat temp;
					int wid = boundRect.x + boundRect.width;
					int hei = boundRect.y + boundRect.height;
					sizeofvec = vecContour.size();
					for (int i = 0; i < sizeofvec; ++i)
					{
						int x = boundRect.x - 5;
						int y = boundRect.y - 5;
						if (x < 0)
						{
							x = 0;
						}
						if (y < 0)
						{
							y = 0;
						}
						rest2 = boundingRect(cv::Mat(vecContour[i]));
						if (rest2.x > x && rest2.y > y && rest2.x < wid + 10 && rest2.y < hei + 10)
						{
							//int counts = 0;
							if (srcImg.at<uchar>(rest2.y + rest2.height / 2, rest2.x + rest2.width / 2) > 60)
							{
								masks = cv::Mat(srcImg.rows, srcImg.cols, srcImg.type(), cv::Scalar());
								drawContours(masks, vecContour, i, cv::Scalar(1), CV_FILLED, 8);
								temp = srcImg.mul(masks);
								defectImg.push_back(temp.clone());
								drawContours(mask, vecContour, i, cv::Scalar(0), CV_FILLED, 8);

								defectPointTemp.push_back(rest2);
							}
						}
					}
					int num = 0;
					//dst1 = srcImg.mul(mask);
					for (int i = boundRect.x; i < wid; ++i)
					{
						for (int j = boundRect.y; j < hei; ++j)
						{
							if (mask.at<uchar>(j, i) != 0)
								num++;
						}
					}
					double res = srcImg.dot(mask);
					double bacM = res / num;
					int count = 0;
					double bac = bacM - 50;
					cv::Mat defectImgTemp;
					for (int k = 0; k < defectImg.size(); ++k)
					{
						defectImgTemp = defectImg[k].clone();
						rest2 = defectPointTemp[k];
						for (int i = rest2.x; i < rest2.x + rest2.width; ++i)
						{
							for (int j = rest2.y; j < rest2.y + rest2.height; ++j)
							{
								if (defectImg[k].at<uchar>(j, i) > bac || defectImg[k].at<uchar>(j, i) == 0)
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
						roiArea.push_back(count);
						rest2.x += roi.x;
						rest2.y += roi.y;
						defectPoint.push_back(rest2);
					}
					sizeofvec = meanGray.size();
					csInfo info;
					int RoiArea = 0;
					double a = 0;
					double radio = 0;
					for (int i = 0; i < sizeofvec; ++i)
					{

						info.name = csFileName;//时间戳
						info.isOK = "NG";
						info.defectKind = 2;
						RoiArea = roiArea[i];
						info.x = defectPoint[i].x;
						info.y = defectPoint[i].y;
						info.width = defectPoint[i].width;
						info.height = defectPoint[i].height;
						a = bacM - meanGray[i];
						if (a < 0)
							a = -a;
						info.value = a / bacM;
						info.zangwu = (int)(info.value*100.0) / 100.0;
						info.huahen = 0;
						info.quejiao = 0;
						info.index = imgNum;
						radio = RoiArea / totalArea;
						if (info.value >= dirtyThresh || radio >= dirtyArea)
						{
							isNG = false;
							isZangwu = true;
							result = "NG";
							//cv::rectangle(huahen, cv::Rect(info.x, info.y, info.width, info.height), cv::Scalar(255, 0, 0), 1, 1, 0);
							defectList.push_back(info);
						}
					}
					//}
				}
				if (isZangwu)
				{
					LOGI("imgNum:{}", imgNum);
					cv::imwrite("D:\\Camera2\\" + csFileName + "_NG.bmp", srcImg);
				}
			}
			if (isNG)
			{
				csInfo info;
				info.name = csFileName;
				info.isOK = "OK";
				info.index = imgNum;
				result = ("OK");
				defectList.push_back(info);
				LOGI("imgNum:{}", imgNum);
				LOGI("defectkind: {}", info.defectKind);
				cv::imwrite("D:\\Camera2\\" + csFileName + "_OK.bmp", srcImg);
			}

		}
	}
}
BOOL QvdGPU2Private::SaveStatisticsExcel(string fileName, string type, double quejiao, double huahen, double zangwu, int i, int total, int index)
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

bool QvdGPU2Private::UpdateNumberOfDefect(vector<csInfo>  *defectList1)
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
		QvdGPU2Private::SaveStatisticsExcel(fileName, type, quejiao, huahen, zangwu, i, total, index);
		i++;
	}
	return 0L;
}

void QvdGPU2Private::run(const Halcon::HImage& image, const Halcon::HRegion& roi, const QByteArray& roiData = QByteArray())
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
	resultOutport  = -1;

	try{
		resultOutport = -1;

		QString type;
		cv::Mat srcImg;
		srcImg = HImage2Mat(image);
		QString timestamp;
		timestamp = QDateTime::currentDateTime().toString("MM-dd hh:mm:ss.z").remove(":").remove("-").remove(" ");
		string imgPath;
		QString myPath = "D:/origalImg";
		myPath = myPath.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
		QDir path(myPath);
		if (!path.exists()){
			path.mkdir(myPath);
		}
		//OK图片文件夹
		std::string OKFolderName = myPath.toStdString() + "OK" + "/";
		QDir OKFolder(OKFolderName.c_str());
		if (!OKFolder.exists())
			OKFolder.mkdir(OKFolderName.c_str());
		//NG0(掉角)图片文件夹
		std::string NG0FolderName = myPath.toStdString() + "NG0" + "/";
		QDir NG0Folder(NG0FolderName.c_str());
		if (!NG0Folder.exists())
			NG0Folder.mkdir(NG0FolderName.c_str());
		//NG12（脏污和划痕）图片文件夹
		std::string NG12FolderName = myPath.toStdString() + "NG12" + "/";
		QDir NG12Folder(NG12FolderName.c_str());
		if (!NG12Folder.exists())
			NG12Folder.mkdir(NG12FolderName.c_str());
			
		const char *fileName = myPath.toStdString().c_str(), *tag;
		imgPath = myPath.toStdString() + timestamp.toStdString();
		vector<csInfo> result;
		ProcessImage(srcImg, result, timestamp.toStdString(), q_ptr->m_dirtyThresh, q_ptr->m_dirtyArea, q_ptr->m_scratchLength, q_ptr->m_losingAngle);
		
		for (vector<csInfo>::iterator i = result.begin(); i != result.end(); i++){
			if ((*i).isOK == "NG"){
				flagResult = false;
				resultStatus = QMVToolPlugin::FAIL;
				Halcon::HRegion resReg;
				resReg = Halcon::HRegion::GenRectangle1((*i).y, (*i).x, (*i).y + (*i).height, (*i).x + (*i).width);
				resultRegions.Append(resReg);

				if ((*i).defectKind == 0)
				{
					portFlag1 = true;
					type = QvdGPU2::tr("diaojiao");
				}

				if ((*i).defectKind == 1)
				{
					portFlag2 = true;
					type = QvdGPU2::tr("huahen");
				}
				
				if ((*i).defectKind == 2)
				{
					portFlag3 = true;
					type = QvdGPU2::tr("zangwu");
				}
				
				if ((*i).status == false){
					type = QvdGPU2::tr("detect failed");
					resultStr = QvdGPU2::tr("VAGUE");
					_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
					resultStatus = QMVToolPlugin::VAGUE;
					return;
				}
			}
		}
		
		//1:尺寸不良; 
		//2:缺角; 
		//3:脏污/划伤; 
		//4:合格; 
		//5:待定。
		if (portFlag1){
			resultOutport = q_ptr->m_edgePort - 1;
		}
		
		if (q_ptr->m_scratchPort != q_ptr->m_mudgePort){
			if (!portFlag1 && portFlag2 && (resultOutport > q_ptr->m_edgePort)){
				resultOutport = q_ptr->m_scratchPort - 1;
			}
			if (!portFlag1 && !portFlag2 && portFlag3 && (resultOutport > q_ptr->m_scratchPort)){
				resultOutport = q_ptr->m_mudgePort - 1;
			}
		}

		if (q_ptr->m_scratchPort == q_ptr->m_mudgePort){
			if (!portFlag1 && (portFlag2 || portFlag3)){
				resultOutport = q_ptr->m_scratchPort - 1;
			}
		}
		
		//生成数据表格
		UpdateNumberOfDefect(&result);

		if (!flagResult){
			if (portFlag1)
				cv::imwrite(NG0FolderName + timestamp.toStdString() + "_NG0" + ".bmp", srcImg);
			if (!portFlag1 && (portFlag2 || portFlag3))
				cv::imwrite(NG12FolderName + timestamp.toStdString() + "_NG12" + ".bmp", srcImg);
			_paint.setPen(Qt::red);
			resultStatus = QMVToolPlugin::FAIL;
			resultStr = QvdGPU2::tr("NG");
			_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
		}
		else{
			cv::imwrite(OKFolderName + timestamp.toStdString() + "_OK" + ".bmp", srcImg);
			_paint.setPen(Qt::green);
			resultStatus = QMVToolPlugin::OK;
			resultStr = QvdGPU2::tr("OK");
			_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
		}
		return;
	}
	catch (const Halcon::HException& e){
		_paint.setPen(Qt::yellow);
		resultStatus = QMVToolPlugin::VAGUE;
		qWarning() << "vdGPU2.dll:" << e.message; 
		return;
	}
}