#ifndef DELSTAMP_H
#define DELSTAMP_H
#pragma once
#define DllExport  __declspec(dllexport)//宏定义
#include <iostream> 
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


class DllExport Stamp
{
public:
	int delStamp(int thresh, const char* argv);
};

#endif