#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "main.h"
#include <iostream>
using namespace std;
using namespace cv;

int Stamp::delStamp(int thresh, const char* argv){
	Mat src, hsv, result;
	Mat imgH;
	Mat imgS;
	Mat imgV;
	vector<Mat> hsvChs(3);
	int imgCol, imgRow;

	//读取图片
	src = imread(argv);
	//将原始图片转换到HSV空间
	cvtColor(src, hsv, COLOR_BGR2HSV);
	//获取图片尺寸
	imgCol = src.cols;
	imgRow = src.rows;

	//RGB法：
	vector<Mat> rgb;
	Mat imgR;
	Mat imgG;
	Mat imgB;
	Mat blank_ch, img_R, img_G, img_B;

	blank_ch = Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC1);

	split(src, rgb);
	imgB = rgb.at(2);
	threshold(imgB, imgB, 0, thresh, THRESH_OTSU);

	//图像增强
	//1. 直方图均衡化
	//equalizeHist(imgB, imgB);
	//2. 拉普拉斯图像锐化
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	filter2D(imgB, imgB, CV_8UC1, kernel);

	Mat imageGamma(imgB.size(), CV_8UC1);
	for (int i = 0; i < imgB.rows; i++)
	{
		for (int j = 0; j < imgB.cols; j++)
		{
			imageGamma.at<uchar>(i, j) = (imgB.at<uchar>(i, j))*(imgB.at<uchar>(i, j))*(imgB.at<uchar>(i, j));
		}
	}
	//归一化到0~255  
	normalize(imageGamma, imageGamma, 0, 255, NORM_MINMAX);
	//转换成8bit图像显示  
	convertScaleAbs(imageGamma, imageGamma);

	
	std::string s;
	s = argv;
	s = s.substr(0, s.rfind("."));
	imwrite(s + "Result.jpg", imageGamma);
	
    return 0;
}