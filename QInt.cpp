/*
* Lớp số nguyên lớn được xây dụng cới dữ liệu được lưu theo Little Endian.
* Có thể dễ dàng mở rộng chương trình với số nguyên lớn 32, 64, 128 bytes,...
* Dễ dàng trong việc bảo trì và nâng cấp chương trình.
*/

#include "QInt.h"
#include "Convert.h"

//---------------------------------NHÓM HÀM TẠO, HỦY---------------------------------
//Hàm khởi tạo mặc định (tạo số QInt với kích thước lưu trữ 16 byte).
QInt::QInt()
{
}

//Hàm khỏi tạo bằng cách truyền vào bit nhị phân.
QInt::QInt(bool* Binary)
{
	memset(this->Data, 0, _DEFAULT_SIZE);
	for (size_t i = 0; i < _DEFAULT_SIZE * CHAR_BIT; i++)
	{
		if (Binary[i] == '1')
		{
			this->Data[_POSITION] = this->Data[_POSITION] | (1 << _SHIFT_OFFSET);
		}
	}
}

//Hàm hủy dữ liệu của kiểu QInt (Không có xử lý gì đặc biệt).
QInt::~QInt()
{
}

//---------------------------------NHÓM HÀM GET, SET---------------------------------
//Hàm lấy data .
unsigned int* QInt::getData()
{
	return this->Data;
}

//---------------------------------NHÓM TOÁN TỬ VÀ PHÉP TOÁN SỐ HỌC---------------------------------
//Toán tử +.
QInt QInt::operator + (QInt x) const
{
	QInt Result = *this;
	QInt Carry;

	while (!(x.isEqualZero()))
	{
		//Carry là những bit cùng là 1 giữa 2 số.
		Carry = Result & x;

		//Tổng các bit khác nhau của hai số (0 + 1 = 1).
		Result = Result ^ x;

		//Đưa các bit cùng là 1 bên 1 bit để tiếp tục phép cộng cần thực hiện.
		x = Carry << 1;
	}

	return Result;
}

//Toán tử -.
QInt QInt::operator - (QInt x) const
{
	return (*this + Convert::QInttoTwosComplement(x));
}

//Toán tử *.
QInt QInt::operator * (QInt x) const
{
	QInt Result;
	QInt Temp = *this;
	QInt One(Convert::StrDectoBin("1")); //Số 1
	bool Negative = false; //Flag số âm

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

	//Nhân hai số dương.
	while (!(x.isNegative() || x.isEqualZero()))
	{
		if (((x & One) - One).isEqualZero()) //Nếu QInt lẻ thì cộng kết quả với Temp.
		{
			/*
			Giải thích: Vì sau mỗi lân ta đều chia QInt cho 2 nên QInt sẽ có 2 trạng thái chẫn và lẻ.
			Nếu QInt lẻ thì chia 2 sẽ dư 1 nên ta phải cộng Temp vào kết quả.
			VD: 6*3 = (6*2) + 6*1
			*/
			Result = Result + Temp;
		}

		Temp = Temp << 1; //Nhân Temp cho 2;
		x = x >> 1; //Chia QInt cho 2.
	}

	//Nếu như 2 số trái dấu.
	if (Negative == true)
	{
		Result = ~Result + One; //Chuyển Result về dạng bù 2.
	}

	return Result;
}

//Toán tử /.
QInt QInt::operator / (QInt x) const
{
	QInt Result;

	if (this->isEqualZero() || x.isEqualZero())
	{
		return Result;
	}
	else
	{
		QInt One(Convert::StrDectoBin("1"));

		if ((x - One).isEqualZero())
		{
			Result = *this;
		}
		else
		{
			QInt Temp = *this;
			int k = _DEFAULT_SIZE/sizeof(int) * _BIT_UINT32_;
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

			while (k > 0)
			{
				Result = Result << 1;
				Result.Data[0] = Result.Data[0] | ((Temp.Data[_DEFAULT_SIZE/sizeof(int) - 1] & (1 << (_BIT_UINT32_ - 1))) >> (_BIT_UINT32_ - 1));
				Temp = Temp << 1;

				Result = Result - x;
				if (Result.isNegative())
				{
					Result = Result + x;
				}
				else
				{
					Temp.Data[0] = Temp.Data[0] | 1;
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

//Toán tử gán =.
QInt& QInt::operator = (const QInt &x)
{
		for (int i = 0; i < _DEFAULT_SIZE/sizeof(int); i++)
		{
			this->Data[i] = x.Data[i];
		}

	return *this;
}

//---------------------------------NHÓM TOÁN TỬ VÀ PHÉP TOÁN THAO TÁC TRÊN BIT---------------------------------
//Phép toán dịch phải (Shift Right).
QInt QInt::operator >> (const int &ShiftOffset)
{
	QInt Result;
	int SignBit = 1 << 31 & this->Data[0];
	Result.Data[0] = this->Data[0] >> ShiftOffset;

	for (int i = 1; i < _DEFAULT_SIZE / sizeof(int); i++)
	{
		//Đưa các bit tràn ra ngoài vào phần block 4 bytes thấp hơn.
		for (int j = 0; j < ShiftOffset; j++)
		{
			Result.Data[i - 1] = Result.Data[i - 1] | (((1 << j) & this->Data[i]) << (_BIT_UINT32_ - ShiftOffset + j));
		}

		Result.Data[i] = this->Data[i] >> ShiftOffset; //Tiến hành dịch bit.
	}

	Result.Data[_DEFAULT_SIZE / sizeof(int) - 1] = Result.Data[_DEFAULT_SIZE / sizeof(int) - 1] | SignBit; //Giữ lại bit dấu.

	return Result;
}

//Phép toán dịch trái (Shift Left).
QInt QInt::operator << (const int &ShiftOffset)
{
	QInt Result;
	int SignBit = (1 << (_BIT_UINT32_ - 1)) & this->Data[_DEFAULT_SIZE / sizeof(int) - 1];
	Result.Data[_DEFAULT_SIZE / sizeof(int) - 1] = this->Data[_DEFAULT_SIZE / sizeof(int) - 1] << ShiftOffset;
	//Result.Data[Result.Data.size() - 1] = Result.Data[Result.Data.size() - 1] | SignBit; //Giữ lại bit dấu.

	for (int i = _DEFAULT_SIZE / sizeof(int) - 2; i > -1; i--)
	{
		//Đưa các bit tràn ra ngoài vào phần block 4 bytes cao hơn.
		for (int j = 0; j < ShiftOffset; j++)
		{
			Result.Data[i + 1] = Result.Data[i + 1] | (((1 << (_BIT_UINT32_ - j - 1)) & this->Data[i]) >> (_BIT_UINT32_ - ShiftOffset));
		}

		Result.Data[i] = this->Data[i] << ShiftOffset; //Tiến hành dịch bit.
	}

	return Result;
}

//Phép toán &.
QInt QInt::operator & (const QInt &x) const
{
	QInt Result;

	for (int i = 0; i < _DEFAULT_SIZE / sizeof(int); i++)
	{
		Result.Data[i] = this->Data[i] & x.Data[i];
	}

	return Result;
}

//Phép toán |.
QInt QInt::operator | (const QInt &x) const
{
	QInt Result;

	for (int i = 0; i < _DEFAULT_SIZE / sizeof(int); i++)
	{
		Result.Data[i] = this->Data[i] | x.Data[i];
	}

	return Result;
}

//Phép toán ^.
QInt QInt::operator ^ (const QInt &x) const
{
	QInt Result;

	for (int i = 0; i < _DEFAULT_SIZE / sizeof(int); i++)
	{
		Result.Data[i] = this->Data[i] ^ x.Data[i];
	}

	return Result;
}

//Phép toán ~.
QInt& QInt::operator ~ ()
{
	for (int i = 0; i < _DEFAULT_SIZE / sizeof(int); i++)
	{
		this->Data[i] = ~this->Data[i];
	}

	return *this;
}

//Phép toán xoay trái.
QInt& QInt::RoL()
{
	//Lưu bit bị đẩy ra ngoài.
	_Uint32t TempBit = (1 << (_BIT_UINT32_ - 1)) & this->Data[_DEFAULT_SIZE / sizeof(int) - 1];
	*this = *this << 1; //Dịch trái 1 bit.
	this->Data[0] = this->Data[0] | (TempBit >> (_BIT_UINT32_ - 1)); //Chèn bit vùa bị đẩy ra vào.

	return *this;
}

//Phép toán xoay phải.
QInt& QInt::RoR()
{
	//Lưu bit bị đẩy ra ngoài.
	_Uint32t TempBit = 1 & this->Data[0];
	*this = *this >> 1; //Dịch phải 1 bit.
	this->Data[_DEFAULT_SIZE / sizeof(int) - 1] = this->Data[_DEFAULT_SIZE / sizeof(int) - 1] | (TempBit << (_BIT_UINT32_ - 1)); //Chèn bit vùa bị đẩy ra vào.

	return *this;
}

//---------------------------------NHÓM HÀM KIỂM TRA---------------------------------
//Hàm kiếm tra bằng không.
bool QInt::isEqualZero() const
{
	for (int i = 0; i < _DEFAULT_SIZE / sizeof(int); i++)
	{
		if (this->Data[i] != 0)
		{
			return false;
		}
	}

	return true;
}

//Hàm kiểm tra số âm.
bool QInt::isNegative() const
{
	if ((this->Data[_DEFAULT_SIZE / sizeof(int) - 1] & (1 << (_BIT_UINT32_ - 1))) >> (_BIT_UINT32_ - 1) == 1)
	{
		return true;
	}

	return false;
}