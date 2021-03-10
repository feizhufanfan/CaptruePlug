#include "CaptruePlug.h"

#include <ActiveQt/QAxFactory>

CaptruePlug::CaptruePlug(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	Init_Object();
	init_connect();
}

QAXFACTORY_DEFAULT(CaptruePlug,
    "{95c1ec7f-cec0-492a-83f5-e2c2d4397da2}",
    "{f33206ef-65e1-4032-9bba-11d1bf04e680}",
    "{568880b7-b7da-4f0b-861d-aaaebf4faa4c}",
    "{0d878aad-341e-42e0-9944-7f7678627563}",
    "{4dadf630-ea0c-4dfd-a75f-55cb8aeb3eab}"
)

void CaptruePlug::slots_out_close()
{
	cap.release();
}

QString CaptruePlug::slots_out_Screenshot()
{
	iFlasgs = 0;
	std::string pic = Mat2Base64(cvMatdst,"png");
	iFlasgs = 1;
	//QString sr;
	return QString::fromStdString(pic);
}
void CaptruePlug::init_connect()
{
	connect(timer, &QTimer::timeout, this,&CaptruePlug::slots_TimerEvent);

}
void CaptruePlug::Init_Object()
{
	timer = new QTimer;
	timer->setInterval(50);
	timer->start();
}
void CaptruePlug::slots_out_open() {
	cap = cv::VideoCapture(0);
	iFlasgs = 1;
}

void CaptruePlug::slots_TimerEvent() {
	cap >> cvMatSrc;
	if (!cvMatSrc.data)
	{
		return;
	}
	if (iFlasgs)
	{
		cvMatdst = cvMatSrc.clone();
	}
	cv::Mat Rgb;
	QImage Img;
	if (cvMatSrc.channels() == 3)//RGB Img
	{
		cv::cvtColor(cvMatSrc, Rgb, CV_BGR2RGB);//ÑÕÉ«¿Õ¼ä×ª»»
		Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
	}
	else//Gray Img
	{
		Img = QImage((const uchar*)(cvMatSrc.data), cvMatSrc.cols, cvMatSrc.rows, cvMatSrc.cols*cvMatSrc.channels(), QImage::Format_Indexed8);
	}
	ui.label->setPixmap(QPixmap::fromImage(Img));

}
std::string CaptruePlug::base64Decode(const char* Data, int DataByte)
{
	// Decoding table
		const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	//return value 
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
				}
			}
			i += 4;
		}
		else// carriage return, skip
		{
			Data++;
		i++;
		}
	}
	return strDecode;
}


std::string CaptruePlug::base64Encode(const unsigned char* Data, int DataByte)
{
	// code table
		const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//return value 
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	// Encode the remaining data
		int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}


	return strEncode;
}


std::string CaptruePlug::Mat2Base64(const cv::Mat &img, std::string imgType)
{
	//Mat to base64
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, img, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return img_data;
}


cv::Mat CaptruePlug::Base2Mat(std::string &base64_data)
{
	cv::Mat img;
	std::string s_mat;
	s_mat = base64Decode(base64_data.data(), base64_data.size());
	std::vector<char> base64_img(s_mat.begin(), s_mat.end());
	img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
	return img;
}