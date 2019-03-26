#include "QInt.h"

QInt::QInt(){};
//Hàm khỏi tạo bằng cách truyền vào chuỗi bit nhị phân.
QInt::QInt(const string&Binary)
{
	memset(this->Data, 0, 16);
	for (size_t i = 0; i < Binary.size(); i++)
	{
		if (Binary[i] == '1')
		{
			this->Data[3 - (Binary.size() - 1 - i) / 32] = this->Data[3 - (Binary.size() - 1 - i) / 32] | ((1 << ((Binary.size() - 1 - i) % 32)));
		}
	}
}
//Hàm khỏi tạo bằng cách truyền vào dãy bit nhị phân.
QInt::QInt(bool* Binary)
{
	memset(this->Data, 0, 16);
	int BinSize = 0;
	while (Binary[BinSize] == 0 || Binary[BinSize] == 1)
		BinSize++;
	for (size_t i = 0; i < BinSize; i++)
	{
		if (Binary[i] == 1)
		{
			this->Data[3 - (BinSize - 1 - i) / 32] = this->Data[3 - (BinSize - 1 - i) / 32] | ((1 << ((BinSize - 1 - i) % 32)));
		}
	}
}

//Hàm hủy dữ liệu của kiểu QInt (Không có xử lý gì đặc biệt).
QInt::~QInt(){}

//---------------------------------NHÓM HÀM GET, SET---------------------------------
//Hàm lấy data .
unsigned int* QInt::getData() const
{
	unsigned int* Result = new unsigned int[4];
	for (int i = 0; i < 4; i++)
		Result[i] = this->Data[i];
	return Result;
}

//---------------------------------NHÓM TOÁN TỬ VÀ PHÉP TOÁN SỐ HỌC---------------------------------
// toan tu *
QInt QInt::operator * (QInt x) const
{
	QInt Result("0");

	QInt Temp = *this;

	QInt One("1"); // 1
	bool Negative = false; //kiem tra so am

	if ((Temp.isNegative() && !x.isNegative()) || (!Temp.isNegative() && x.isNegative()))//kiem tra trai dau
	{
		Negative = true;
	}
	if (x.isNegative())
	{
		x = ~(x - One); //neu am thi chuyen sang dang so duong
	}
	if (Temp.isNegative())
	{
		Temp = ~(Temp - One); //neu am thi chuyen sang dang so duong
	}
	//nhan 2 so
	while (!(x.isNegative() || x.isEqualZero()))
	{
		if (((x & One) - One).isEqualZero()) //x le thi cong ket qua voi Temp
		{
			Result = Result + Temp;
		}

		Temp = Temp << 1; //nhan temp cho 2;
		x = x >> 1; //chia x cho 2.
	}
	// neu 2 so trai dau
	if (Negative == true)
	{
		Result = ~Result + One; //chuyet ket qua ve dang bu 2.
	}

	return Result;
}
//Toán tử /.
QInt QInt::operator / (QInt x) const
{
	QInt Result("0");

	if (this->isEqualZero() || x.isEqualZero())
	{
		return Result;
	}
	else
	{
		QInt One("1");
		if ((x - QInt("1")).isEqualZero())
		{
			Result = *this;
		}
		else
		{
			QInt Temp = *this;
			int k = 128;
			bool Negative = false;

			if ((Temp.isNegative() && !x.isNegative()) || (!Temp.isNegative() && x.isNegative()))//Nếu 2 số trái dấu.
			{
				Negative = true;
			}

			if (x.isNegative())
			{
				x = ~(x - One); //Nếu âm thì chuyển về dạng số dương
			}
			if (Temp.isNegative())
			{
				Temp = ~(Temp - One); //Nếu âm thì chuyển về dạng số dương
			}

			while (k>0)
			{
				Result = Result << 1;
				Result.Data[3] = Result.Data[3] | ((Temp.Data[0] & (1 << 31)) >> 31);
				Temp = Temp << 1;

				Result = Result - x;
				if (Result.isNegative())
				{
					Result = Result + x;
				}
				else
				{
					Temp.Data[3] = Temp.Data[3] | 1;
				}

				--k;
			}


			Result = Temp;
			if (Negative == true)
			{
				Result = ~Result + One;
			}
		}
	}

	return Result;
}
//toan tu +.
QInt QInt::operator + (QInt x) const
{
	QInt Result = *this;
	QInt test1; //kiem tra bit cung 1 cua 2 so

	while (!(x.isEqualZero()))
	{
		//test1 là những bit cùng là 1 giữa 2 số.
		test1 = Result & x;

		//Tổng các bit khác nhau của hai số (0 + 1 = 1).
		Result = Result ^ x;

		//Đưa các bit cùng là 1 bên 1 bit để tiếp tục phép cộng cần thực hiện.
		x = test1 << 1;
	}

	return Result;
}
//toan tu -.
QInt QInt::operator - (QInt x) const
{
	return (*this + x.QInttoTwosComplement());
}

//---------------------------------NHÓM CÁC TOÁN TỬ SO SÁNH VÀ GÁN -----------------------------------
//Toán tử <.
bool  QInt::operator < (QInt x) {
	if (this->isNegative() && !x.isNegative())
		return true;
	if (!this->isNegative() && x.isNegative())
		return false;
	if (this->isNegative() && x.isNegative()){
		QInt One("1");
		x = ~(x - One); //doi ve so duong
		*this = ~(*this - One);
		if (this != &x)
		{
			for (int i = 0; i < 4; i++)
			{
				if (this->Data[i] < x.Data[i])
					return false;
			}
		}
		return true;
	}

	if (!this->isNegative() && !x.isNegative()) {
		if (this != &x)
		{
			for (int i = 0; i < 4; i++)
			{
				if (this->Data[i] > x.Data[i])
					return false;
			}
		}
		return true;
	}
}
//Toán tử >.
bool  QInt::operator > (QInt x) {
	return !(*this <= x);
}
//Toán tử >=.
bool  QInt::operator >= (QInt x) {
	return (*this > x || *this == x);
}
//Toán tử <=
bool  QInt::operator <= (QInt x) {
	return !(*this > x);
}
//Toán tử ==
bool QInt::operator == (QInt x) const
{
	for (int i = 0; i < 4; i++)
	if (Data[i] != x.Data[i])	return false;
	return true;
}
//Toán tử !=
bool QInt::operator != (QInt x) const
{
	return !(this->operator==(x));
}
//Toán tử gán =.
QInt& QInt::operator = (const QInt&x)
{
	for (int i = 0; i < 4; i++)
		this->Data[i] = x.Data[i];
	return *this;
}

//---------------------------------NHÓM TOÁN TỬ VÀ PHÉP TOÁN THAO TÁC TRÊN BIT---------------------------------
//Phép toán dịch phải(Shift Right).
QInt QInt::operator >> (const int&shift)
{
	QInt Result;
	memset(Result.Data, 0, 16);

	unsigned int SignBit = (1 << 31) & this->Data[0];
	Result.Data[3] = this->Data[3] >> shift;
	for (int i = 2; i >= 0; i--)
	{
		//Đưa các bit tràn ra ngoài vào phần block 4 bytes thấp hơn.
		for (int j = 0; j < shift; j++)
		{

			Result.Data[i + 1] = Result.Data[i + 1] | (((1 << j) & this->Data[i]) << (32 - shift));
		}

		Result.Data[i] = this->Data[i] >> shift; //Tiến hành dịch bit.
	}
	for (int i = 0; i < shift; i++)
		Result.Data[0] = Result.Data[0] | (SignBit >> i); //Giữ lại bit dấu.

	return Result;
}
//Phép toán dịch trái(Shift Left).
QInt QInt::operator << (const int&shift)
{
	QInt Result;
	memset(Result.Data, 0, 16);

	unsigned int SignBit = (1 << 31) & this->Data[0];
	Result.Data[0] = this->Data[0] << shift;
	for (int i = 1; i < 4; i++)
	{
		//Đưa các bit tràn ra ngoài vào phần block 4 bytes cao hơn.
		for (int j = 0; j < shift; j++)
		{
			Result.Data[i - 1] = Result.Data[i - 1] | (((1 << (31 - j)) & this->Data[i]) >> (32 - shift));
		}

		Result.Data[i] = this->Data[i] << shift; //Tiến hành dịch bit.
	}

	return Result;
}
//Phép toán &.
QInt QInt::operator & (const QInt&x) const
{
	QInt Result;
	memset(Result.Data, 0, 16);

	for (int i = 0; i < 4; i++)
	{
		Result.Data[i] = this->Data[i] & x.Data[i];
	}

	return Result;
}
//Phép toán |.
QInt QInt::operator | (const QInt&x) const
{
	QInt Result;
	memset(Result.Data, 0, 16);

	for (int i = 0; i < 4; i++)
	{
		Result.Data[i] = this->Data[i] | x.Data[i];
	}

	return Result;
}
//Phép toán ^.
QInt QInt::operator ^ (const QInt&x) const
{
	QInt Result;
	memset(Result.Data, 0, 16);

	for (int i = 0; i < 4; i++)
	{
		Result.Data[i] = this->Data[i] ^ x.Data[i];
	}

	return Result;
}
//Phép toán ~.
QInt& QInt::operator ~ ()
{
	for (int i = 0; i < 4; i++)
	{
		this->Data[i] = ~this->Data[i];
	}

	return *this;
}
//Phép toán xoay trái.
QInt& QInt::RoL()
{
	//Lưu bit bị đẩy ra ngoài.
	unsigned int TempBit = (1 << 31) & this->Data[0];
	*this = *this << 1; //Dịch trái 1 bit.
	this->Data[3] = this->Data[3] | (TempBit >> 31); //Chèn bit vùa bị đẩy ra vào.

	return *this;
}
//Phép toán xoay phải
QInt& QInt::RoR()
{
	int TempBit = 1 & this->Data[3];
	*this = *this >> 1; //Dịch phải 1 bit.
	this->Data[0] = this->Data[0] | (TempBit << 31); //Chèn bit vùa bị đẩy ra vào.

	return *this;
}

//Hàm kiếm tra bằng không.
bool QInt::isEqualZero() const
{
	for (int i = 0; i < 4; i++)
	{
		if (this->Data[i] != 0)
		{
			return false;
		}
	}

	return true;
}

QInt QInt::QInttoTwosComplement()
{
	return (~*this + QInt("1"));
}

//Hàm kiểm tra số âm.
bool QInt::isNegative() const
{
	if (((this->Data[0] & (1 << 31)) >> 31) == 1)
	{
		return true;
	}

	return false;
}

//Hàm Nhập
void ScanQInt(QInt &x)
{
	string Dec;
	cin.ignore();
	getline(cin, Dec);
	QInt result(StrQIntDecToBin(Dec));
	x = result;
}

//Hàm xuất 
void PrintQInt(QInt x)
{
	cout << QInttoDecString(x);
}

//Hàm chuyển đổi số QInt nhị phân sang thập lục phân
string BinToHex(bool *bit)
{
	string tempRes;
	string result;
	bool tempBit[128]; //do 16bytes nen chuoi bit dai nhat 128

	//dem do dai chuoi bit dau vao
	int dem = 0;
	for (int i = 0; i < 128; i++)
	{
		if (bit[i] == true || bit[i] == false)
		{
			dem++;
		}
		else break;
	}

	//lap 128 bit cho de thuc hien
	int tempSize = dem;
	if (tempSize < 128) //neu size cua chuoi bool <128 thi chuan bi chen` so 0 trong tat ca cac bit con lai
	{
		int tempSub = 128 - tempSize;
		for (int i = 127; i >= tempSub; i--) //push nguoc tu cuoi chuoi
		{
			tempBit[i] = bit[tempSize - 1];
			tempSize--;
		}
		for (int i = 0; i < tempSub; i++) // push lap day 28bit
		{
			tempBit[i] = false;
		}
	}
	else
	{
		for (int i = 0; i < 128; i++) // push lap day 28bit
		{
			tempBit[i] = bit[i];
		}
	}

	//chuan bi chuyen tu BIN -> HEX
	for (int i = 0; i < 128; i++)
	{
		if (tempBit[i] == true) //neu bool dung thi push 1
		{
			tempRes.push_back('1');
		}
		else tempRes.push_back('0'); //sai push 0

		if (i % 4 == 3) //lay ra 4bit de xu li
		{
			if (tempRes == "0000")//0
			{
				result.push_back('0');
			}
			else
			{
				if (tempRes == "0001")//1
				{
					result.push_back('1');
				}
				else
				{
					if (tempRes == "0010")//2
					{
						result.push_back('2');
					}
					else
					{
						if (tempRes == "0011")//3
						{
							result.push_back('3');
						}
						else
						{
							if (tempRes == "0100")//4
							{
								result.push_back('4');
							}
							else
							{
								if (tempRes == "0101")//5
								{
									result.push_back('5');
								}
								else
								{
									if (tempRes == "0110")//6
									{
										result.push_back('6');
									}
									else
									{
										if (tempRes == "0111")//7
										{
											result.push_back('7');
										}
										else
										{
											if (tempRes == "1000")//8
											{
												result.push_back('8');
											}
											else
											{
												if (tempRes == "1001")//9
												{
													result.push_back('9');
												}
												else
												{
													if (tempRes == "1010")//10
													{
														result.push_back('A');
													}
													else
													{
														if (tempRes == "1011")//11
														{
															result.push_back('B');
														}
														else
														{
															if (tempRes == "1100")//12
															{
																result.push_back('C');
															}
															else
															{
																if (tempRes == "1101")//13
																{
																	result.push_back('D');
																}
																else
																{
																	if (tempRes == "1110")//14
																	{
																		result.push_back('E');
																	}
																	else
																	{
																		if (tempRes == "1111")//15
																		{
																			result.push_back('F');
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			tempRes.clear();
		}
	}
	//xoa so 0 thua o dau chuoi hex 
	while (result.front() == '0')
	{
		result.erase(0, 1);
	}
	return result;
}

//Hàm chuyển đổi số QInt thập phân sang nhị phân
bool* DecToBin(QInt x)
{
	bool* res = new bool[128];
	for (size_t i = 0; i < 128; i++)
	{
		res[i] = (x.getData()[3 - (127 - i) / 32] & (1 << ((127 - i) % 32))) >> ((127 - i) % 32);
	}
	return res;
}

//Hàm chuyển đổi số QInt thập phân sang thập lục phân:
string DecToHex(QInt x)
{
	string Res;
	bool tempBit[128];
	for (int i = 0; i < 128; i++)
	{
		if (DecToBin(x)[i] == true)
		{
			tempBit[i] = true;
		}
		else
		{
			tempBit[i] = false;
		}
	}
	Res = BinToHex(tempBit);
	return Res;
}

//Chia số nguyên string Str cho 2 lấy phần nguyên
string StrDivTwo(const string &Str)
{
	string Result;
	int Temp = 0;

	for (int i = 0; i < Str.size(); i++)
	{
		Temp = Temp * 10 + (Str[i] - '0');

		if (i > 0 || (i == 0 && Temp >= 2))
		{
			Result.push_back((Temp / 2) + '0');
		}

		Temp = Temp % 2;
	}

	return Result;
}

//Chuyển chuỗi số nguyên he 10 string x sang chuỗi nhị phân kiểu bool
bool* StrQIntDecToBin(string x)
{
	bool Negative = false;
	bool *Result = new bool[128];
	for (int i = 0; i < 128; i++) Result[i] = 0;
	if (x[0] == '-')
	{
		x.erase(x.begin());
		Negative = true;
	}
	int k = 127;
	while (x != "")
	{
		Result[k] = (x[x.size() - 1] - '0') % 2;
		x = StrDivTwo(x);
		k--;
	}
	//Nếu là chuỗi ban đầu là số âm.
	if (Negative == true)
	{
		QInt Result2(Result); //Đưa vài số kiểu QInt.
		Result2 = Result2.QInttoTwosComplement(); //Chuyển sang dạng bù 2.
		Result = DecToBin(Result2); //Đưa lại về nhị phân.
	}
	return Result;
}

//Hàm tính a lũy thừa n (a, n là số nguyên không âm).
string Power(int a, int n)
{
	string Result;

	if (a > -1)
	{
		//Nếu a > 1
		if (a > 1)
		{
			int Pow = n;

			if (n > 31 * 2 / a) //Tránh trường hợp tràn số.
			{
				Pow = 31 * 2 / a;
			}

			Result = to_string(unsigned int(pow(a, Pow)));


			//Phần có khả năng bị tràn số sẽ được nhân theo nhân chuỗi.
			for (int i = Pow + 1; i <= n; i++)
			{
				Result = MultiplyNumberString(Result, to_string(a));
			}
		}
		else
		{
			Result.push_back(a + '0');
		}
	}

	return Result;
}

//Hàm nhân hai chuỗi.
string MultiplyNumberString(string a, string b)
{
	string Result = "0";
	string TempStr;
	int Temp = 0;
	bool Negative = false;

	if (a[0] == '-')
	{
		if (b[0] != '-')
		{
			Negative = true;
		}
		a.erase(a.begin());
	}
	if (b[0] == '-')
	{
		if (a[0] != '-')
		{
			Negative = true;
		}
		b.erase(b.begin());
	}

	for (int i = 0; i < b.size(); i++)
	{
		for (int j = a.size() - 1; j > -1; j--)
		{
			Temp = Temp + (a[j] - '0') * (b[b.size() - i - 1] - '0');
			TempStr.push_back((Temp % 10) + '0');
			Temp = Temp / 10;
		}
		if (Temp != 0) //Nếu vẫn còn Temp.
		{
			TempStr.push_back(Temp + '0');
			Temp = 0;
		}

		//Đảo chuỗi.
		reverse(TempStr.begin(), TempStr.end());

		//Thêm các số 0 cần thiết vào sau.
		for (int j = 0; j < i; j++)
		{
			TempStr.push_back('0');
		}

		//Cộng Result và Temp.
		Result = AddNumberString(Result, TempStr);
		TempStr.clear(); //Dọn chuỗi tạm để dùng cho các bước tiếp theo.
	}

	if (Negative == true)
	{
		Result.insert(Result.begin(), '-');
	}

	return Result;
}

//Hàm cộng hai chuỗi số.
string AddNumberString(string a, string b)
{
	if (a[0] == '-' && b[0] != '-')
	{
		a.erase(a.begin());
		a = SubNumberString(b, a);
	}
	else
	{
		if (b[0] == '-' && a[0] != '-')
		{
			b.erase(b.begin());
			a = SubNumberString(a, b);
		}
		else
		{
			int Temp = 0;
			bool Negative = false;

			if (a[0] == '-' && b[0] == '-')
			{
				Negative = true;
			}

			//Thêm các số 0 vào đầu mỗi chuỗi.
			if (a.size() > b.size())
			{
				while (a.size() != b.size())
				{
					b.insert(b.begin(), '0');
				}
			}
			else
			{
				while (a.size() != b.size())
				{
					a.insert(a.begin(), '0');
				}
			}

			//Tiến hành cộng.
			for (int i = a.size() - 1; i > -1; i--)
			{
				Temp = Temp + a[i] + b[i] - 2 * '0';
				a[i] = (Temp % 10) + '0';
				Temp = Temp / 10;
			}

			//Nếu vẫn còn dư (Temp != 0)
			if (Temp != 0)
			{
				a.insert(a.begin(), Temp + '0');
				Temp = 0;
			}
		}
	}

	//Xóa các số 0 thừa.
	while (a[0] == '0')
	{
		a.erase(a.begin());
	}

	return a;
}

//Hàm trừ hai chuỗi số dương.
string SubNumberString(string a, string b)
{
	int Temp = 0;
	bool Negative = false;

	//Thêm các số 0 vào đầu mỗi chuỗi.
	if (a.size() > b.size())
	{
		while (a.size() != b.size())
		{
			b.insert(b.begin(), '0');
		}
	}
	else
	{
		while (a.size() != b.size())
		{
			a.insert(a.begin(), '0');
		}
	}

	//Kiểm tra a hay b lớn hơn và chuyển a thành số lớn hơn.
	if (strcmp(a.c_str(), b.c_str()) < 0)
	{
		string TempStr = a;
		a = b;
		b = TempStr;
		Negative = true; //a < b => a - b < 0
	}

	//Thực hiện phép trừ.
	for (int i = a.size() - 1; i > -1; i--)
	{
		if ((a[i] - Temp) < b[i])
		{
			a[i] = (10 + a[i] - b[i] - Temp) + '0';
			Temp = 1;
		}
		else
		{
			a[i] = a[i] - b[i] - Temp + '0';
			Temp = 0;
		}
	}

	//Xóa các số 0 thừa.
	while (a[0] == '0')
	{
		a.erase(a.begin());
	}

	//Nếu vẫn còn dư (Temp != 0)
	if (Negative == true)
	{
		a.insert(a.begin(), '-');
	}

	return a;
}

//Hàm chuyển từ QInt sang chuỗi string hệ 10
string QInttoDecString(const QInt &x)
{
	bool* Binary = DecToBin(x);
	string Decimal = "0";
	if (Binary[0] == 1) Decimal = SubNumberString(Decimal, Power(2, 127));
	for (int i = 1; i < 128; i++)
	{
		if (Binary[i] == 1)
		{
			Decimal = AddNumberString(Decimal, Power(2, 127 - i));
		}
	}
	return Decimal;
}

//Hàm chuyển chuỗi thập lục phân sang nhị phân.
bool* StrHextoBin(string Str)
{
	bool* Binary = new bool[128];
	memset(Binary, 0, 128);
	int index = 127;

	for (int i = Str.size() - 1; i >= 0; i--)
	{
		switch (Str[i])
		{
		case '0':
		{
					break;
		}
		case '1':
		{
					Binary[index] = 1;
					break;
		}
		case '2':
		{
					Binary[index - 1] = 1;
					break;
		}
		case '3':
		{
					Binary[index] = 1;
					Binary[index - 1] = 1;
					break;
		}
		case '4':
		{
					Binary[index - 2] = 1;
					break;
		}
		case '5':
		{
					Binary[index] = 1;
					Binary[index - 2] = 1;
					break;
		}
		case '6':
		{
					Binary[index - 1] = 1;
					Binary[index - 2] = 1;
					break;
		}
		case '7':
		{
					Binary[index] = 1;
					Binary[index - 1] = 1;
					Binary[index - 2] = 1;
					break;
		}
		case '8':
		{
					Binary[index - 3] = 1;
					break;
		}
		case '9':
		{
					Binary[index] = 1;
					Binary[index - 3] = 1;
					break;
		}
		case 'A':
		case 'a':
		{
					Binary[index - 1] = 1;
					Binary[index - 3] = 1;
					break;
		}
		case 'B':
		case 'b':
		{
					Binary[index] = 1;
					Binary[index - 1] = 1;
					Binary[index - 3] = 1;
					break;
		}
		case 'C':
		case 'c':
		{
					Binary[index - 2] = 1;
					Binary[index - 3] = 1;
					break;
		}
		case 'D':
		case 'd':
		{
					Binary[index] = 1;
					Binary[index - 2] = 1;
					Binary[index - 3] = 1;
					break;
		}
		case 'E':
		case 'e':
		{
					Binary[index - 1] = 1;
					Binary[index - 2] = 1;
					Binary[index - 3] = 1;
					break;
		}
		case 'F':
		case 'f':
		{
					Binary[index] = 1;
					Binary[index - 1] = 1;
					Binary[index - 2] = 1;
					Binary[index - 3] = 1;
					break;
		}
		}
		index -= 4;
	}

	return Binary;
}
