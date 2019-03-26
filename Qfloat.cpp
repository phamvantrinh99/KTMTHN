#include "QFloat.h"
#include "QInt.h"

//Khởi tạo mặc định
QFloat::QFloat()
{
}

//Khởi tạo với chuỗi string cơ số 2
QFloat::QFloat(string Binary)
{
	memset(this->Data, 0, 16);
	for (size_t i = 0; i < Binary.size(); i++)
	{
		if (Binary[i] == '1')
		{
			this->Data[i / 32] = this->Data[i / 32] | ((1 << ((127 - i) % 32)));
		}
	}
}

//Khởi tạo với một mảng bit
QFloat::QFloat(bool* Binary)
{
	memset(this->Data, 0, 16);
	for (size_t i = 0; i < 128; i++)
	{
		if (Binary[i] == 1)
		{
			this->Data[i / 32] = this->Data[i / 32] | ((1 << ((127 - i) % 32)));
		}
	}
}

//Hàm lấy dữ liệu
unsigned int* QFloat::getData()
{
	unsigned int* Result = new unsigned int[4];
	for (int i = 0; i < 4; i++)
		Result[i] = this->Data[i];
	return Result;
}

//Hàm set dữ liệu
void QFloat::setData(int viTrim, unsigned int Data)
{
	this->Data[viTrim] = Data;
}

//Hàm Nhập
void ScanQfloat(QFloat &x)
{
	string Dec;
	QFloat result(StrDecToBin(Dec));
	x = result;
}

//Hàm xuất
void PrintQfloat(QFloat x)
{
	cout << QFloatToDecStr(x);
}

//Hàm bổ trợ cho StrDecToBin
bool* DecToBin15Bit(unsigned long long int Data15Bit, int &size, bool want) //ham chuyen dec to bin max la 15 bit
{
	string temp;
	if (want == false)
	{
		unsigned long long int x;
		int i = 0;
		while (Data15Bit > 0)
		{
			x = Data15Bit % 2;
			temp.push_back(x);
			Data15Bit = Data15Bit / 2;
			i++;
		}
		int chay = i;
		size = chay;
		int *binaryNum = new int[temp.size()];
		bool *Res = new bool[temp.size()];
		for (int j = 0; j < chay; j++)
		{
			binaryNum[j] = temp[j];
		}
		for (int j = 0; j < chay; j++)
		{
			Res[i - 1] = binaryNum[j];
			i--;
		}

		return Res;
		delete[]Res;
	}
	else
	{
		bool Res[15];
		int binaryNum[15];

		// counter for binary array 
		int i = 0;
		while (Data15Bit > 0)
		{
			// storing remainder in binary array 
			binaryNum[i] = Data15Bit % 2;
			Data15Bit = Data15Bit / 2;
			i++;
		}
		if (i < 15)
		{
			for (int run = 14; run >= i; run--)
			{
				binaryNum[run] = 0;
			}
		}
		int temp = 14;
		for (int j = 0; j < 15; j++)
		{
			Res[temp] = binaryNum[j];
			temp--;
		}
		return Res;
	}
}

//Hàm bổ trợ cho StrDecToBin
unsigned long long int NguyenSangSo(string x)
{
	int l1 = x.length();
	unsigned long long int num1 = 0;
	for (int i = l1 - 1; i >= 0; --i)
	{
		num1 += (int)(x[i] - '0') * pow(10, l1 - i - 1);
	}
	return num1;
}

//Nhân số dạng string với 2
string StrMultiTwo(string Str)
{
	string tempres;
	int temp = 0;
	bool del = true;
	for (int i = Str.size() - 1; i >= 0; i--)
	{
		if (Str[i] == '.')
		{
			tempres.push_back('.');
			tempres.push_back(temp + '0');
		}
		else
		{
			temp += 2 * (Str[i] - '0');
			if (del)
			{
				if (temp % 10 != 0)
				{
					tempres.push_back((temp % 10) + '0');
					del = false;
				}
				temp /= 10;
			}
			else
			{
				tempres.push_back((temp % 10) + '0');
				temp /= 10;
			}
		}
	}
	string res;
	for (int i = tempres.size() - 1; i >= 0; i--)
		res.push_back(tempres[i]);
	return res;
}

//Chuyển dãy số thập phân dạng 0.xxx qua hệ 2
bool* StrFracToBin(string frac)
{
	bool* res = new bool[128];
	memset(res, 0, 128);
	int i = 0;
	while (!(frac == "1." || i == 128))
	{
		frac.erase(frac.begin());
		frac = StrMultiTwo(frac);
		res[i] = (frac[0] - '0');
		i++;
	}
	return res;
}

//Chuyển số thực Dec dạng string qua bool* nhị phân theo dạng   ±1.F * 2^E
bool* StrDecToBin(string Dec)
{
	bool negative = false;
	if (Dec[0] == '-')
	{
		negative = true;
		Dec.erase(Dec.begin());
	}
	bool* res = new bool[128];
	memset(res, 0, 128);
	string nguyen;
	string frac;
	while (Dec[0] != '.' && Dec[0] != '\0')
	{
		nguyen.push_back(Dec[0]);
		Dec.erase(Dec.begin());
	}
	frac = "0" + Dec;
	bool* nguyenbin = StrQIntDecToBin(nguyen);
	bool* fracbin = StrFracToBin(frac);
	int nguyensize = 0;
	int E = 16383;
	int dichfrac = 0;
	if (nguyen == "0")
	{
		dichfrac = 1;
		while (fracbin[dichfrac - 1] == 0)
			dichfrac++;
		E -= dichfrac;
	}
	else{
		for (int i = 127; i >= 0; i--)
		{
			if (nguyenbin[i] == 1) nguyensize = 128 - i;
		}
		E += nguyensize - 1;
	}
	int i = 1;
	for (; i < 16; i++)
		res[i] = ((E & (1 << (15 - i))) >> (15 - i));
	for (; i < 15 + nguyensize; i++)
	{
		res[i] = nguyenbin[113 - nguyensize + i];
	}
	for (; i < 128; i++)
	{
		if (dichfrac != 0) dichfrac--;
		res[i] = fracbin[dichfrac + i - nguyensize - 15];
	}
	if (negative) res[0] = 1;
	return res;
}

//Chuyển đổi số Qfloat nhị phân sang thập phân: 
QFloat BinToDec(bool *Binary)
{
	return QFloat(Binary);
}

//Hàm chuyển đổi số Qfloat thập phân sang nhị phân :
bool* DecToBin(QFloat x)
{
	bool* res = new bool[128];
	for (size_t i = 0; i < 128; i++)
	{
		res[i] = (x.getData()[3 - (127 - i) / 32] & (1 << ((127 - i) % 32))) >> ((127 - i) % 32);
	}
	return res;
}

//Hàm chuyển từ QFloat sang string hệ 10
string QFloatToDecStr(QFloat x)
{
	bool* Data = DecToBin(x);
	int x15bit = 0, e;
	long double x112bit = 0, m;
	unsigned long long int phanNguyen = 0;
	float phanFrac = 0;
	bool negative = false;
	if (Data[0] != 0)
		negative = true;
	for (int i = 1; i <= 15; i++)
	{
		x15bit = x15bit + Data[i] * pow(2.0, 15 - i);
	}
	e = x15bit - 16383; // so bit dich;
	int oBitDichToi = 16 + e;
	phanNguyen = phanNguyen + pow(2.0, e);
	e--;
	for (int i = 16; i < oBitDichToi; i++) // toi o bit 18
	{
		phanNguyen = phanNguyen + Data[i] * pow(2.0, e);
		e--;
	}
	int run = -1; //so mu cua Frac
	for (int i = oBitDichToi; i < 128; i++)
	{
		phanFrac = phanFrac + Data[i] * pow(2.0, run);
		run--;
	}

	string strNguyen, strFrac;
	string ketQua;
	strNguyen = to_string(phanNguyen);
	strFrac = to_string(phanFrac);
	if (strFrac[0] == '0')
	{
		strFrac.erase(strFrac.begin());
	}
	if (negative == true)
	{
		strNguyen.insert(0, "-");
	}
	ketQua = strNguyen + strFrac;
	int bitCuoi = ketQua.length() - 1;
	while (ketQua[bitCuoi] == '0')
	{
		ketQua.pop_back();
		bitCuoi--;
	}
	return ketQua;


}
