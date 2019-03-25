#ifndef __QFLOAT_H__
#define __QFLOAT_H__

#include <iostream>
#include <string>
using namespace std;

class QFloat
{
protected:
	unsigned int Data[4];
public:
	//Khởi tạo mặc định
	QFloat();

	//Khởi tạo với chuỗi string cơ số 2
	QFloat(string Binary);

	//Khởi tạo với một mảng bit
	QFloat(bool* Binary);

	//Hàm lấy dữ liệu
	unsigned int* getData();

	//Hàm set dữ liệu
	void setData(int viTrim, unsigned int Data);
};

//Hàm Nhập
void ScanQfloat(QFloat &x);

//Hàm xuất
void PrintQfloat(QFloat x);

//Hàm bổ trợ cho StrDecToBin
unsigned long long int NguyenSangSo(string x);

//Hàm bổ trợ cho StrDecToBin
bool* DecToBin15Bit(unsigned long long int Data15Bit, int &size, bool want);

//Chuyển số thực Dec dạng string qua bool* nhị phân theo dạng   ±1.F * 2^E
bool* StrDecToBin(string Dec);

//Chuyển đổi số Qfloat nhị phân sang thập phân: 
QFloat BinToDec(bool *bit);

//Hàm chuyển đổi số Qfloat thập phân sang nhị phân :
bool *DecToBin(QFloat x);

//Hàm chuyển từ QFloat sang string hệ 10
string QFloatToDecStr(QFloat x);

#endif
