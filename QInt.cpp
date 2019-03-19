#include "QInt.h"

QInt::QInt(){};
//Hàm khỏi tạo bằng cách truyền vào chuỗi bit nhị phân.
QInt::QInt(const string&Binary)
{
	memset(this->m_Data, 0, 16);
	for (size_t i = 0; i < Binary.size(); i++)
	{
		if (Binary[i] == '1')
		{
			this->m_Data[3 - (Binary.size() - 1 - i) / 32] = this->m_Data[3 - (Binary.size() - 1 - i) / 32] | ((1 << ((Binary.size() - 1 - i) % 32)));
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
		Result[i] = this->m_Data[i];
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

//Toán tử gán =.
QInt& QInt::operator = (const QInt&x)
{
	for (int i = 0; i < 4; i++)
		this->m_Data[i] = x.m_Data[i];
	return *this;
}

//---------------------------------NHÓM TOÁN TỬ VÀ PHÉP TOÁN THAO TÁC TRÊN BIT---------------------------------
//Phép toán dịch phải(Shift Right).
QInt QInt::operator >> (const int&shift)
{
	QInt Result;
	memset(Result.m_Data, 0, 16);

	unsigned int SignBit = (1 << 31) & this->m_Data[0];
	Result.m_Data[3] = this->m_Data[3] >> shift;
	for (int i = 2; i >= 0; i--)
	{
		//Đưa các bit tràn ra ngoài vào phần block 4 bytes thấp hơn.
		for (int j = 0; j < shift; j++)
		{

			Result.m_Data[i + 1] = Result.m_Data[i + 1] | (((1 << j) & this->m_Data[i]) << (32 - shift));
		}

		Result.m_Data[i] = this->m_Data[i] >> shift; //Tiến hành dịch bit.
	}
	for (int i = 0; i < shift; i++)
		Result.m_Data[0] = Result.m_Data[0] | (SignBit >> i); //Giữ lại bit dấu.

	return Result;
}
//Phép toán dịch trái(Shift Left).
QInt QInt::operator << (const int&shift)
{
	QInt Result;
	memset(Result.m_Data, 0, 16);

	unsigned int SignBit = (1 << 31) & this->m_Data[0];
	Result.m_Data[0] = this->m_Data[0] << shift;
	for (int i = 1; i < 4; i++)
	{
		//Đưa các bit tràn ra ngoài vào phần block 4 bytes cao hơn.
		for (int j = 0; j < shift; j++)
		{
			Result.m_Data[i - 1] = Result.m_Data[i - 1] | (((1 << (31 - j)) & this->m_Data[i]) >> (32 - shift));
		}

		Result.m_Data[i] = this->m_Data[i] << shift; //Tiến hành dịch bit.
	}
	Result.m_Data[0] = Result.m_Data[0] | SignBit; //Giữ lại bit dấu.

	return Result;
}
//Phép toán &.
QInt QInt::operator & (const QInt&x) const
{
	QInt Result;
	memset(Result.m_Data, 0, 16);

	for (int i = 0; i < 3; i++)
	{
		Result.m_Data[i] = this->m_Data[i] & x.m_Data[i];
	}

	return Result;
}
//Phép toán |.
QInt QInt::operator | (const QInt&x) const
{
	QInt Result;
	memset(Result.m_Data, 0, 16);

	for (int i = 0; i < 4; i++)
	{
		Result.m_Data[i] = this->m_Data[i] | x.m_Data[i];
	}

	return Result;
}
//Phép toán ^.
QInt QInt::operator ^ (const QInt&x) const
{
	QInt Result;
	memset(Result.m_Data, 0, 16);

	for (int i = 0; i < 4; i++)
	{
		Result.m_Data[i] = this->m_Data[i] ^ x.m_Data[i];
	}

	return Result;
}
//Phép toán ~.
QInt& QInt::operator ~ ()
{
	for (int i = 0; i < 4; i++)
	{
		this->m_Data[i] = ~this->m_Data[i];
	}

	return *this;
}
//Phép toán xoay trái.
QInt& QInt::RoL()
{
	//Lưu bit bị đẩy ra ngoài.
	unsigned int TempBit = (1 << 31) & this->m_Data[0];
	*this = *this << 1; //Dịch trái 1 bit.
	this->m_Data[3] = this->m_Data[3] | (TempBit >> 31); //Chèn bit vùa bị đẩy ra vào.

	return *this;
}
//Phép toán xoay phải
QInt& QInt::RoR()
{
	int TempBit = 1 & this->m_Data[3];
	*this = *this >> 1; //Dịch phải 1 bit.
	this->m_Data[0] = this->m_Data[0] | (TempBit << 31); //Chèn bit vùa bị đẩy ra vào.

	return *this;
}

//Hàm kiếm tra bằng không.
bool QInt::isEqualZero() const
{
	for (int i = 0; i < 4; i++)
	{
		if (this->m_Data[i] != 0)
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
	if (((this->m_Data[0] & (1 << 31)) >> 31) == 1)
	{
		return true;
	}

	return false;
}
