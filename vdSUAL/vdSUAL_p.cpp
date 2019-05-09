#include <QtGui>
#include "vdSUAL.h"
#include "vdSUAL_p.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 
#include <iomanip>
#include <vector>

using namespace std;

QvdSUALPrivate::QvdSUALPrivate(quint8 station, quint8 camera, quint8 threadID, quint16 productID, QvdSUAL* parent)
	: q_ptr(parent)
	, station(station)
	, camera(camera)
	, threadID(threadID)
	, productID(productID)
{
}

vector<string> split(string str, string pattern)
{
	vector<string> ret;
	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);
	while (index != str.npos)
	{
		if (start != index)
			ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}
	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}


std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}


void QvdSUALPrivate::run(const Halcon::HImage& image, const Halcon::HRegion& roi, const QByteArray& roiData = QByteArray())
{
	q_ptr->initResultMeasure();
	showType = QMVToolPlugin::ShowFail;
	resultPixmap = QPixmap(image.Width(), image.Height());
	resultPixmap.fill(Qt::transparent);
	QPainter _paint(&resultPixmap);
	QFont _font;
	_font.setPointSize(image.Height() /100);
	_paint.setFont(_font);
	resultRegions = Halcon::HRegionArray();
	
	try{
		QString rootPath = "D:/CommunicationDirectory";
		QString imgPath_folder = rootPath;
		QString src;
		QString imgPath;
		QString timestamp;
		QString InputPath = rootPath;
		std::ifstream result;
		QString OutPath = rootPath;
		QString OutPath_1;
		
		const char *OKPath;
		
		//if (q_ptr->flagCellSize)
		//{		
			//写图片
			imgPath_folder = imgPath_folder.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
			timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").remove(":").remove("-").remove(" ").remove("2019");
			imgPath = imgPath_folder + timestamp;
			image.WriteImage("bmp", 255, imgPath.toStdString().c_str());
			
			//写Input.txt		
			InputPath = InputPath.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
			//std::ofstream isOK(InputPath.append("Input.txt").toStdString().c_str());
			FILE* isOK = fopen(InputPath.append("Input.txt").toStdString().c_str(), "w");
			//if (isOK.is_open())
			//	isOK.close();
			//if (!isOK){
				fclose(isOK);
			//}
			//读取output.txt
			OutPath_1 = OutPath.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
			//q_ptr->flagCellSize = false;
		//}

		qWarning() << q_ptr->m_calibration_type;
		
		char output[100];
		int fontsize = image.Height() / 50;
		_font.setPointSize(fontsize);
		_paint.setFont(_font);
		short textLine = 1;
		std::string temp;
		int lineNum = 0;
		float a;
		int b;	
		//Sleep(30);
		//result.open(OutPath.append("output.txt").toStdString().c_str());	
		//while (1){
		Sleep(1000);	
	    result.open(OutPath.append("output.txt").toStdString().c_str());
		//bool qq = result.is_open();
		//while (!qq){
		//	qq = result.is_open();
		//}

			while (getline(result, temp)){
				lineNum++;
			}
			//如果文件只有3行，为OK
			int  num = 0;
			char* linedata;
			std::vector<std::string> lines; //存储每行数据
			std::string data;
			const int LINE_LENGTH = 512;
			char str[LINE_LENGTH];
			qWarning() << lineNum;
			if (lineNum == 2 || !result.is_open()){
				_paint.setPen(Qt::green);
				resultStatus = QMVToolPlugin::OK;
				resultStr = QvdSUAL::tr("OK");
				_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
				result.close();
			}
			else{
				_paint.setPen(Qt::red);
				resultStatus = QMVToolPlugin::FAIL;
				//将output.txt数据写入
				//QString OutPath_2 = "D:/Result";
				//OutPath_2 = OutPath_2.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
				//OutPath_2 = OutPath_2.append(__TIME__).remove(":").append(".txt");
				//std::ofstream result_file(OutPath_2.toStdString().c_str());
				//char* data;
				//while (!result.eof()){
				//	result >> data;
				//	result_file << data;
				//}
				//result_file << result.rdbuf();
				//result_file.close();

				result.close();
				result.open(OutPath.toStdString().c_str());

				int NumofLine = 0;
				size_t index = 2;
				string strLine;
				size_t iLine = 0;
				int a1, a2, a3, a4;
				string strLineAnsi;
				vector<string> res;
				int i = 0;
				int defect_x, defect_y, defect_length, defect_width;
				double defect_area;
				bool portFlag1 = false;
				bool portFlag2 = false;
				bool portFlag3 = false;
				int dType = -1;
				while (getline(result, strLine)){				
					NumofLine++;
					int j = 0;					
					string tmp;
					vector<string> data;
					stringstream input(strLine);
					if (NumofLine == 1){
						double a;
						input >> a;
						//cout << setiosflags(ios::fixed) << a;
					}
					if (NumofLine == 2){
						//cout << strLine;
					}

					if (NumofLine > 2){
						while (getline(input, tmp, ',')) {
							data.push_back(tmp);
						}

						int k = 0;

						for (auto s : data){
							if (k == 0){
								dType = stoi(s);
								//qWarning() << defect_x;
								//cout << "x = " << stoi(s) << ",";
							}
							if (k == 1){
								defect_x = stoi(s);
								//qWarning() << defect_x;
								//cout << "x = " << stoi(s) << ",";
							}

							if (k == 2){
								defect_y = stoi(s);
								//qWarning() << stoi(s);
								//cout << "y = " << stoi(s) << ",";
							}

							if (k == 3){
								defect_length = stoi(s);
								//qWarning() << stoi(s);
								//cout << "length = " << stoi(s) << ",";
							}

							if (k == 4){
								defect_width = stoi(s);
								//qWarning() << stoi(s);
								//cout << "width = " << stoi(s) << ",";
							}
							k++;
						}
					}

					if (NumofLine > 2){
						//res = split(strLine, "  ");
						res = split(strLine, "  ");
						for (vector<string>::iterator iter = res.begin(); iter != res.end(); iter++){
							if (j == 1){
								//cout << "area=" << setiosflags(ios::fixed) << stod(*iter);
								defect_area = stod(*iter);
							}
							j++;
						}
						//qWarning() << defect_y;
						//qWarning() << defect_x;
						//qWarning() << defect_length;
						//qWarning() << defect_width;
						//qWarning() << defect_area;
						//Halcon::HRegion::GenCircle(defect_y, defect_x, 10);
						if (dType == 0)
							portFlag1 = true;
						if (dType == 1)
							portFlag2 = true;
						if (dType == 2)
							portFlag3 = true;
						resultStr = QvdSUAL::tr("Defect Type: ") + QString("%1").arg(dType);
						_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
						resultStr = QvdSUAL::tr("Area/Contrast") + QString("%1: %2").arg(NumofLine - 2).arg(defect_area, 0, 'f', 3);
						_paint.drawText(image.Height() / 20 + 300, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
						resultStr = QvdSUAL::tr("Length") + QString("%1: %2").arg(NumofLine - 2).arg(defect_length);
						_paint.drawText(image.Height() / 20 + 800, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
						resultStr = QvdSUAL::tr("Width") + QString("%1: %2").arg(NumofLine - 2).arg(defect_width);
						_paint.drawText(image.Height() / 20 + 1100, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
					}
					//cout << endl;
				}

				//while (!result.eof()){
				//	//按行读取				
				//	std::getline(result, data);
				//	lines.clear();

				//	//解析单行数据
				//	//std::stringstream stringin(str);
				//	//stringin >> data;
				//	qWarning() << __LINE__;
				//	std::cout << data << std::endl;
				//}
				result.close();

				resultStr = QvdSUAL::tr("NG");
				_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
				if (portFlag1)
					resultOutport = q_ptr->m_edgePort - 1;
				if (!portFlag1 && portFlag2)
					resultOutport = q_ptr->m_scratchPort - 1;
				if (!portFlag1 && !portFlag2 && portFlag3)
					resultOutport = q_ptr->m_mudgePort - 1;
			}
			if (result.is_open())
				result.close();

	    //remove(OutPath.toStdString().c_str());
		//qWarning() << resultOutport;
		return;
	}
	catch (const Halcon::HException& e) {
		_paint.setPen(Qt::yellow);
		resultStatus = QMVToolPlugin::VAGUE;
		qWarning() << "vdSUAL.dll:" << e.message;
		return;
	}
}