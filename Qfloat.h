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
	QFloat();
	QFloat(string Binary);

	unsigned int* getData();

	void setData(int viTrim, unsigned int Data);
};

//Chuyển số thực Dec dạng string qua bool* nhị phân theo dạng   ±1.F * 2^E
unsigned long long int NguyenSangSo(string x);

bool* DecToBin15Bit(unsigned long long int Data15Bit, int &size, bool want);

bool* StrDecToBin(string Dec);

//Chuyển đổi số Qfloat nhị phân sang thập phân: 
QFloat BinToDec(bool *bit);

//Hàm chuyển đổi số Qfloat thập phân sang nhị phân :
//bool *DecToBin(QFloat x);

#endif