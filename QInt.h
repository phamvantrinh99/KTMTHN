#include <iostream>

using namespace std;

class QInt //Lớp QInt dùng cho số nguyên lớn có dấu.
{
protected:
	//Vùng nhớ lưu trữ cho kiểu QInt.
	unsigned int m_Data[4];
public:
	//---------------------------------NHÓM HÀM TẠO, HỦY---------------------------------
	QInt();
	//Hàm khỏi tạo bằng cách truyền vào chuỗi bit nhị phân.
	QInt(const string&Binary);
	//Hàm hủy dữ liệu của kiểu QInt (Không có xử lý gì đặc biệt).
	~QInt();

	//---------------------------------NHÓM HÀM GET, SET---------------------------------
	//Hàm lấy data .
	unsigned int* getData() const;

	//Toán tử +
	QInt QInt::operator + (QInt x) const;

	//Toán tử gán =.
	QInt& operator = (const QInt&x);

	//---------------------------------NHÓM TOÁN TỬ VÀ PHÉP TOÁN THAO TÁC TRÊN BIT---------------------------------
	//Phép toán dịch phải(Shift Right).
	QInt operator >> (const int&shift);
	//Phép toán dịch trái(Shift Left).
	QInt operator << (const int&shift);
	//Phép toán &.
	QInt operator & (const QInt&) const;
	//Phép toán |.
	QInt operator | (const QInt&) const;
	//Phép toán ^.
	QInt operator ^ (const QInt&) const;
	//Phép toán ~.
	QInt& operator ~ ();
	//Phép toán xoay trái.
	QInt& RoL();
	//Phép toán xoay phải
	QInt& RoR();
	//Hàm chuyển số QInt sang số bù 2.
	QInt QInttoTwosComplement();
	//Hàm kiếm tra bằng không.
	bool isEqualZero() const;
	//Hàm kiểm tra số âm.
	bool isNegative() const;
};
