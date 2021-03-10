#pragma once

#include <QtWidgets/QWidget>
#include <ActiveQt/QAxBindable>
#include <opencv2/opencv.hpp>
#include "ui_CaptruePlug.h"
#include <qtimer.h>
class CaptruePlug : public QWidget, public QAxBindable
{
    Q_OBJECT

public:
    CaptruePlug(QWidget *parent = NULL);

public slots:
	void slots_TimerEvent();
	void slots_out_open();
	void slots_out_close();
	QString slots_out_Screenshot();
	

private:
	void init_connect();
	void Init_Object();

	std::string base64Decode(const char * Data, int DataByte);

	std::string base64Encode(const unsigned char * Data, int DataByte);

	std::string Mat2Base64(const cv::Mat & img, std::string imgType);

	cv::Mat Base2Mat(std::string & base64_data);

private:
    Ui::CaptruePlugClass ui;
	cv::VideoCapture cap;
	cv::Mat cvMatSrc,cvMatdst;
	int iFlasgs;
	QTimer *timer;
};
