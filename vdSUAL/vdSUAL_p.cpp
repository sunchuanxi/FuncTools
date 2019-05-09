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
	_font.setPointSize(image.Height() / 100);
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
		bool flagResult = true;
		//写图片
		imgPath_folder = imgPath_folder.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
		timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").remove(":").remove("-").remove(" ").remove("2019");
		imgPath = imgPath_folder + timestamp;
		image.WriteImage("bmp", 255, imgPath.toStdString().c_str());

		//写Input.txt		
		InputPath = InputPath.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");
		FILE* isOK = fopen(InputPath.append("Input.txt").toStdString().c_str(), "w");
		fclose(isOK);
		OutPath_1 = OutPath.append(QString::number(q_ptr->m_calibration_type + 1)).append("/");

		char output[100];
		int fontsize = image.Height() / 50;
		_font.setPointSize(fontsize);
		_paint.setFont(_font);
		short textLine = 1;
		std::string temp;
		int lineNum = 0;
		float a;
		int b;
		Sleep(100);
		result.open(OutPath.append("output.txt").toStdString().c_str(), ios::binary);
		size_t index = 2;
		string strLineAnsi;
		wstring wstrLine;
		size_t iLine = 0;
		vector<string> data;//存储第三行（包括第三行）以后的数据
		while (!result.eof()){
			result.seekg(index, ios::beg);
			wchar_t wch;
			result.read((char*)(&wch), 2);
			if (wch == 0x000D){
				strLineAnsi = ws2s(wstrLine);
				wstrLine.erase(0, wstrLine.size() + 1);
				iLine++;
				index += 4;
				//qWarning() << "line" << iLine  << ": "<< strLineAnsi.c_str();
				data.push_back(strLineAnsi);
			}
			else{
				wstrLine.append(1, wch);
				index += 2;
			}
		}

		//如果文件最后一行不是空行，下面代码存储最后一行的字符串
		//如果文件最后一行是空行，则不必
		strLineAnsi = ws2s(wstrLine);
		wstrLine.erase(0, wstrLine.size() + 1);
		iLine++;
		index += 4;
		//qWarning() << "line" << iLine << ": " << strLineAnsi.c_str();
		data.push_back(strLineAnsi);

		//qWarning() << __LINE__ << "Total line number:" << iLine;

		//如果文件只有3行，为OK
		double AreaOrContrast;
		int dtype, x, y, len, wid;
		bool portFlag1 = false;
		bool portFlag2 = false;
		bool portFlag3 = false;
		
		if (iLine == 2){
			_paint.setPen(Qt::green);
			resultStatus = QMVToolPlugin::OK;
			resultStr = QvdSUAL::tr("OK");
			_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
			result.close();
		}
		else{		
			_paint.setPen(Qt::red);
			vector<string> res;
			vector<string> res1;
			string tmp;
			//qWarning() << __LINE__;
			int lineIndex = 0;
			for (vector<string>::iterator iter = data.begin(); iter != data.end(); ++iter){
				lineIndex++;
				if (lineIndex > 2 && ((*iter).c_str()!=NULL)){
					//qWarning() << __LINE__ << (*iter).c_str();
					res = split(*iter, "  ");
					//qWarning() << __LINE__;
					int index = 0;
					int index1 = 0;
					for (vector<string>::iterator iter1 = res.begin(); iter1 != res.end(); ++iter1){
						if (index == 1){
							AreaOrContrast = stod(*iter1);
							//qWarning() << __LINE__ << AreaOrContrast;
						}
						else{
							res1 = split(*iter1, ",");
							for (vector<string>::const_iterator iter2 = res1.begin(); iter2 != res1.end(); ++iter2){
								if (index1 == 0){
									dtype = stoi(*iter2);
								}
								if (index1 == 1){
									x = stoi(*iter2);
								}
								if (index1 == 2){
									y = stoi(*iter2);
								}
								if (index1 == 3){
									len = stoi(*iter2);
								}
								if (index1 == 4){
									wid = stoi(*iter2);
								}
								index1++;
							}
						}
						index++;
					}

					QString type;
					if (dtype == 0){
						portFlag1 = true;
						type = QvdSUAL::tr("diaojiao");
					}
						
					if (dtype == 1){
						portFlag2 = true;
						type = QvdSUAL::tr("huahen");
					}
					if (dtype == 2){
						portFlag3 = true;
						type = QvdSUAL::tr("zangwu");
					}
						
					resultStr = QvdSUAL::tr("Defect Type: ") + type;
					_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
					resultStr = QvdSUAL::tr("Area/Contrast") + QString("%1: %2").arg(lineIndex - 2).arg(AreaOrContrast, 0, 'f', 3);
					_paint.drawText(image.Height() / 20 + 300, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
					resultStr = QvdSUAL::tr("Length") + QString("%1: %2").arg(lineIndex - 2).arg(len);
					_paint.drawText(image.Height() / 20 + 800, image.Height() / 20 + (fontsize*1.5*textLine), resultStr);
					resultStr = QvdSUAL::tr("Width") + QString("%1: %2").arg(lineIndex - 2).arg(wid);
					_paint.drawText(image.Height() / 20 + 1100, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);

					if (portFlag1)
						resultOutport = q_ptr->m_edgePort - 1;
					if (!portFlag1 && portFlag2)
						resultOutport = q_ptr->m_scratchPort - 1;
					if (!portFlag1 && !portFlag2 && portFlag3)
						resultOutport = q_ptr->m_mudgePort - 1;
					flagResult = false;			
				}
				else
					continue;
			}

	     }
		
		if (result.is_open())
			result.close();
		if (!flagResult){
			_paint.setPen(Qt::red);
			resultStr = QvdSUAL::tr("NG");
			_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
		}
		else{
			_paint.setPen(Qt::green);
			resultStr = QvdSUAL::tr("OK");
			_paint.drawText(image.Height() / 20, image.Height() / 20 + (fontsize*1.5*textLine++), resultStr);
		}
		//remove(OutPath.toStdString().c_str());
		return;
	}
	catch (const Halcon::HException& e) {
		_paint.setPen(Qt::yellow);
		resultStatus = QMVToolPlugin::VAGUE;
		qWarning() << "vdSUAL.dll:" << e.message;
		return;
	}
}