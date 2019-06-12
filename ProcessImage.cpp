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
