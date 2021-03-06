#include "QInt.h"
#include "Qfloat.h"
#include <fstream>
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		ofstream Out;
		Out.open("input.txt");
		cout << "NHAP DU LIEU INPUT ( Nhap \"exit\" de ket thuc) "<< endl;
		string s;
		do
		{
			fflush(stdin);
			getline(cin, s);
			if (s != "exit")
			{
				Out << s << endl;
			}
		} while (s != "exit");
		Out.close();
	}
	if (argc == 4)
	{
		ifstream Inp;
		ofstream Out;
		Inp.open(argv[1]);
		Out.open(argv[2]);
		if (strcmp(argv[3], "1") == 0)
		{
			while (!Inp.eof())
			{
				string s;
				getline(Inp, s);
				QInt Result;
				string Final;
				if (s.find('+') < s.length() || (s.find('-') < s.length() && s[s.find('-', 4) + 1] == ' ') || s.find('*') < s.length() || s.find('/') < s.length()) // Nhận biết toán hạng để thực hiện phép toán số học
				{
					//-----------------tách các thành phần p và các toán hạng, toán tử
					string n = s.substr(0, s.find(' '));
					s.erase(0, s.find(' ') + 1);
					string s1 = s.substr(0, s.find(' '));
					s.erase(0, s.find(' ') + 1);
					string Calc = s.substr(0, s.find(' '));
					s.erase(0, s.find(' ') + 1);
					string s2 = s.substr(0, s.length());
					//-----------------
					if (n == "2") //thực hiện tính toán, xử lý ở chế độ Binary 
					{
						QInt a(s1);
						QInt b(s2);
						if (Calc == "+")  Result = a + b;
						if (Calc == "-")  Result = a - b;
						if (Calc == "*")  Result = a * b;
						if (Calc == "/")  Result = a / b;
						//---------------------Xử lí xuất file
						bool*temp=  DecToBin(Result);
						for (int i = 0; i < 128; i++) Final.push_back(temp[i] + '0');
						Final.erase(0, Final.find('1'));
						Out << Final << endl;
					}
					if (n == "10")  // thực hiện tính toán, xử lý ở chế độ Decimal
					{
						bool *temp1 = StrQIntDecToBin(s1);
						bool *temp2 = StrQIntDecToBin(s2);
						QInt a(temp1);
						QInt b(temp2);
						if (Calc == "+")  Result = a + b;
						if (Calc == "-")  Result = a - b;
						if (Calc == "*")  Result = a * b;
						if (Calc == "/")  Result = a / b;
						Final = QInttoDecString(Result);
						Out << Final << endl;
					}
					if (n == "16") // thực hiện tính toán, xử lý ở chế độ Hexa
					{
						bool *temp1 = StrHextoBin(s1);
						bool *temp2 = StrHextoBin(s2);
						QInt a(temp1);
						QInt b(temp2);
						if (Calc == "+")  Result = a + b;
						if (Calc == "-")  Result = a - b;
						if (Calc == "*")  Result = a * b;
						if (Calc == "/")  Result = a / b;
						Final = DecToHex(Result);
						Out << Final << endl;
					}
				}
				else //---------------------Nhận biết yêu cầu chuyển đổi cơ số hay các phép thao tác trên bit
				{
					int space = 0;
					for (int i = 0; i < s.length(); i++)
					if (s[i] == ' ') space = space + 1;
					if (space == 2) // input có 2 khoảng trắng sẽ là phép chuyển đổi cơ số
					{
						string p1 = s.substr(0, s.find(' '));
						s.erase(0, s.find(' ') + 1);
						string p2 = s.substr(0, s.find(' '));
						s.erase(0, s.find(' ') + 1);
						string str = s.substr(0, s.length());
						if (p1 == "2" && (str != "~" || str != "RoL" || str != "RoR"))
						{
							QInt Result(str);
							if (p2 == "10") //thao tác xử lí chuyển đổi cơ số từ 2 -> 10
							{
								Final = QInttoDecString(Result);
								Out << Final << endl;
							}
							if (p2 == "16") // //thao tác xử lí chuyển đổi cơ số từ 2 -> 16
							{
								Final = DecToHex(Result);
								Out << Final << endl;
							}
						}
						if (p1 == "10" && (str != "~" || str != "RoL" || str != "RoR"))
						{
							bool *temp = StrQIntDecToBin(str);
							QInt Result(temp);
							if (p2 == "2") //thao tác xử lí chuyển đổi cơ số từ 10 -> 2
							{
								bool*temp = DecToBin(Result);
								for (int i = 0; i < 128; i++) Final.push_back(temp[i] + '0');
								Final.erase(0, Final.find('1'));
								Out << Final << endl;
							}
							if (p2 == "16") //thao tác xử lí chuyển đổi cơ số từ 10 -> 16
							{
								Final = DecToHex(Result);
								Out << Final << endl;
							}
						}
						if (p1 == "16" && (str != "~" || str != "RoL" || str != "RoR"))
						{
							bool*temp = StrHextoBin(str);
							QInt Result(str);
							if (p2 == "2") //thao tác xử lí chuyển đổi cơ số từ 16 -> 2
							{
								for (int i = 0; i < 128; i++) Final.push_back(temp[i] + '0');
								Final.erase(0, Final.find('1'));
								Out << Final << endl;
							}
							if (p2 == "10") //thao tác xử lí chuyển đổi cơ số từ 16 -> 10
							{
								Final = QInttoDecString(Result);
								Out << Final << endl;
							}
						}
						if (str == "~")
						{
							QInt Result;
							if (p1 == "10") // xử lí bit ở cơ số 10
							{
								bool *temp =StrQIntDecToBin(p2);
								QInt a(temp);
								Result = ~a;
								Final = QInttoDecString(Result);
								Out << Final << endl;
							}
							if (p1 == "2") // xử lí bit ở cơ số 2
							{
								QInt a(p2);
								Result = ~a;
								bool*temp = DecToBin(Result);
								for (int i = 0; i < 128; i++) Final.push_back(temp[i] + '0');
								Final.erase(0, Final.find('1'));
								Out << Final << endl;
							}
							if (p1 == "16")
							{
								bool*temp = StrHextoBin(p2);
								QInt a(temp);
								Result = ~a;
								Final = DecToHex(Result);
								Out << Final << endl;
							}
						}
						if (str == "RoL")
						{
							QInt Result;
							if (p1 == "10") // xử lí bit ở cơ số 10
							{
								bool *temp = StrQIntDecToBin(p2);
								QInt a(temp);
								Result = a.RoL();
								Final = QInttoDecString(Result);
								Out << Final << endl;
							}
							if (p1 == "2") // xử lí bit ở cơ số 2
							{
								QInt a(p2);
								Result = a.RoL();
								bool*temp = DecToBin(Result);
								for (int i = 0; i < 128; i++) Final.push_back(temp[i] + '0');
								Final.erase(0, Final.find('1'));
								Out << Final << endl;
							}
							if (p1 == "16")
							{
								bool*temp = StrHextoBin(p2);
								QInt a(temp);
								Result = a.RoL();
								Final = DecToHex(Result);
								Out << Final << endl;
							}
						}
						if (str == "RoR")
						{
							QInt Result;
							if (p1 == "10") // xử lí bit ở cơ số 10
							{
								bool *temp = StrQIntDecToBin(p2);
								QInt a(temp);
								Result = a.RoR();
								Final = QInttoDecString(Result);
								Out << Final << endl;
							}
							if (p1 == "2") // xử lí bit ở cơ số 2
							{
								QInt a(p2);
								Result = a.RoR();
								bool*temp = DecToBin(Result);
								for (int i = 0; i < 128; i++) Final.push_back(temp[i] + '0');
								Final.erase(0, Final.find('1'));
								Out << Final << endl;
							}
							if (p1 == "16")
							{
								bool*temp = StrHextoBin(p2);
								QInt a(temp);
								Result = a.RoR();
								Final = DecToHex(Result);
								Out << Final << endl;
							}
						}
					}
					if (space == 3) //input có 3 khoảng trắng sẽ là các pháp thao tác trên bit
					{
						string n = s.substr(0, s.find(' '));
						s.erase(0, s.find(' ') + 1);
						string str = s.substr(0, s.find(' '));
						s.erase(0, s.find(' ') + 1);
						string Calc = s.substr(0, s.find(' '));
						s.erase(0, s.find(' ') + 1);
						string len = s.substr(0, s.length());
						if (n == "10") // xử lí bit ở cơ số 10
						{
							bool *temp = StrQIntDecToBin(str);
							QInt a(temp), Result;
							if (Calc == ">>")
							{
								Result = a >> stoi(len, 0, 10);
							}
							if (Calc == "<<")
							{
								Result = a << stoi(len, 0, 10);
							}
							if (Calc == "&" || Calc == "|" || Calc == "^")
							{
								bool *temp1 = StrQIntDecToBin(len);
								QInt b(temp1);
								if (Calc == "&")
								{
									Result = a & b;
								}
								else if (Calc == "|")
								{
									Result = a | b;
								}
								else Result = a^b;
							}
							Final = QInttoDecString(Result);
							Out << Final << endl;

						}
						if (n == "2") // xử lí bit ở cơ số 2
						{
							QInt a(str), Result;
							if (Calc == ">>")
							{
								Result = a >> stoi(len, 0, 10);
							}
							if (Calc == "<<")
							{
								Result = a << stoi(len, 0, 10);
							}
							if (Calc == "&" || Calc == "|" || Calc == "^")
							{
								QInt b(len);
								if (Calc == "&")
								{
									Result = a & b;
								}
								else if (Calc == "|")
								{
									Result = a | b;
								}
								else Result = a^b;
							}
							bool*temp = DecToBin(Result);
							for (int i = 0; i < 128; i++) Final.push_back(temp[i] + '0');
							Final.erase(0, Final.find('1'));
							Out << Final << endl;
						}
						if (n == "16")
						{
							bool*temp = StrHextoBin(str);
							QInt a(temp), Result;
							if (Calc == ">>")
							{
								Result = a >> stoi(len, 0, 10);
							}
							if (Calc == "<<")
							{
								Result = a << stoi(len, 0, 10);
							}
							if (Calc == "&" || Calc == "|" || Calc == "^")
							{
								bool *temp1 = StrHextoBin(len);
								QInt b(temp1);
								if (Calc == "&")
								{
									Result = a & b;
								}
								else if (Calc == "|")
								{
									Result = a | b;
								}
								else Result = a^b;
							}
							Final = DecToHex(Result);
							Out << Final << endl;
						}
					}
				}
			}
		}
		else if (strcmp(argv[3], "2") == 0)
		{
			while (!Inp.eof())
			{
				string s;
				getline(Inp, s);
				string Final;
				string p1 = s.substr(0, s.find(' '));
				s.erase(0, s.find(' ') + 1);
				string p2 = s.substr(0, s.find(' '));
				s.erase(0, s.find(' ') + 1);
				string str = s.substr(0, s.length());
				if (p1 == "2")
				{
					QFloat x(str);
					Final = QFloatToDecStr(x);
					Out << Final << endl;
				}
				if (p1 == "10")
				{
					bool* temp = StrDecToBin(str);
					QFloat Result(temp);
					bool*temp1 = DecToBin(Result);
					for (int i = 0; i < 128; i++) Final.push_back(temp1[i] + '0');
					reverse(Final.begin(), Final.end());
					Final.erase(0, Final.find('1'));
					reverse(Final.begin(), Final.end());
					Out << Final << endl;
				}
			}
		}
		Inp.close();
		Out.close();
	}
	return 0;
}
