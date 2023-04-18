#include<iostream>
#include<cstring>
#include<map>
using namespace std;

//将输入的16进制明文字符串转化为2进制明文字符串
string Hto2(string m)
{
	map<char, string> hto2;
	hto2['0'] = "0000"; hto2['1'] = "0001"; hto2['2'] = "0010"; hto2['3'] = "0011"; hto2['4'] = "0100"; hto2['5'] = "0101"; hto2['6'] = "0110"; hto2['7'] = "0111";
	hto2['8'] = "1000"; hto2['9'] = "1001"; hto2['A'] = "1010"; hto2['B'] = "1011"; hto2['C'] = "1100"; hto2['D'] = "1101"; hto2['E'] = "1110"; hto2['F'] = "1111";
	hto2['a'] = "1010"; hto2['b'] = "1011"; hto2['c'] = "1100"; hto2['d'] = "1101"; hto2['e'] = "1110"; hto2['f'] = "1111";
	string M = "";
	for (int i = 0; i < m.length(); i++)
		M += hto2[m[i]];
	return M;
}

//将二进制密文转化为十六进制字符串
string btoh(string m)
{
	map<string, string> hto2;
	hto2["0000"] = "0"; hto2["0001"] = "1"; hto2["0010"] = "2"; hto2["0011"] = "3"; hto2["0100"] = "4"; hto2["0101"] = "5"; hto2["0110"] = "6"; hto2["0111"] = "7";
	hto2["1000"] = "8"; hto2["1001"] = "9"; hto2["1010"] = "A"; hto2["1011"] = "B"; hto2["1100"] = "C"; hto2["1101"] = "D"; hto2["1110"] = "E"; hto2["1111"] = "F";
	string M = "";
	for (int i = 0; i < 16; i++)
		M += hto2[m.substr(i * 4, 4)];
	return M;
}

//初始置换IP
string IP(string c)
{
	string temp = c;

	//置换IP表
	int ipTable[64] =
	{
		58,50,42,34,26,18,10,2,
		60,52,44,36,28,20,12,4,
		62,54,46,38,30,22,14,6,
		64,56,48,40,32,24,16,8,
		57,49,41,33,25,17,9,1,
		59,51,43,35,27,19,11,3,
		61,53,45,37,29,21,13,5,
		63,55,47,39,31,23,15,7
	};

	for (int i = 0; i < c.length(); i++)
		temp[i] = c[ipTable[i] - 1];
	return temp;
}

//逆初始置换IP_rev
string IP_rev(string c)
{
	string temp = c;

	//逆初始置换表
	int ipRevTable[64] =
	{
		40,8,48,16,56,24,64,32,
		39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30,
		37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28,
		35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26,
		33,1,41,9,49,17,57,25
	};

	for (int i = 0; i < c.length(); i++)
		temp[i] = c[ipRevTable[i] - 1];
	return temp;
}

//选择扩展运算E（32->48）
string E(string c)
{
	//长度48的字符串
	string temp(48, '0');
	//E表
	int eTable[48] =
	{
		32,1,2,3,4,5,
		4,5,6,7,8,9,
		8,9,10,11,12,13,
		12,13,14,15,16,17,
		16,17,18,19,20,21,
		20,21,22,23,24,25,
		24,25,26,27,28,29,
		28,29,30,31,32,1
	};
	for (int i = 0; i < 48; i++)
		temp[i] = c[eTable[i] - 1];
	return temp;
}

//密钥置换选择PC_1
string PC_1(string c)
{
	string temp(56, '0');
	//PC_1表
	int pc1Table[56] =
	{
		57,49,41,33,25,17,9,1,
		58,50,42,34,26,18,10,2,
		59,51,43,35,27,19,11,3,
		60,52,44,36,63,55,47,39,
		31,23,15,7,62,54,46,38,
		30,22,14,6,61,53,45,37,
		29,21,13,5,28,20,12,4
	};
	for (int i = 0; i < 56; i++)
		temp[i] = c[pc1Table[i] - 1];
	return temp;
}

//密钥左右部分循环移位
void loop(string& l, string& r, int n)
{
	//各轮的左循环移位位数
	int loopTable[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };
	//第n轮位数
	int move = loopTable[n];
	string l_temp = l; string r_temp = r;
	for (int i = 0; i < l.length(); i++)
	{
		l_temp[i] = l[(i + move) % 28];
		r_temp[i] = r[(i + move) % 28];
	}
	l = l_temp;
	r = r_temp;
	return;
}

//密钥置换选择PC_2
string PC_2(string l, string r)
{
	string k = l + r;
	string temp(48, '0');
	//PC_2表
	int pc2Table[48] =
	{
		14,17,11,24,1,5,
		3,28,15,6,21,10,
		23,19,12,4,26,8,
		16,7,27,20,13,2,
		41,52,31,37,47,55,
		30,40,51,45,33,48,
		44,49,39,56,34,53,
		46,42,50,36,29,32
	};
	for (int i = 0; i < 48; i++)
		temp[i] = k[pc2Table[i] - 1];
	return temp;
}

//异或运算
string XOR(string r, string k)
{
	string res(r.length(), '0');
	for (int i = 0; i < r.length(); i++)
	{
		if (r[i] == k[i])
			res[i] = '0';
		else
			res[i] = '1';
	}
	return res;
}

//S盒代换
string S(string c)
{
	//S盒
	int sBox[8][4][16] =
	{
		//S1
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
		//S2
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
		//S3
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
		//S4
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
		//S5
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
		//S6
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
		//S7
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
		//S8
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	};

	//字符串转行号&列号
	map<string, int> stringtoint;
	stringtoint["00"] = 0; stringtoint["01"] = 1; stringtoint["10"] = 2; stringtoint["11"] = 3;
	stringtoint["0000"] = 0; stringtoint["0001"] = 1; stringtoint["0010"] = 2; stringtoint["0011"] = 3; stringtoint["0100"] = 4; stringtoint["0101"] = 5; stringtoint["0110"] = 6;
	stringtoint["0111"] = 7; stringtoint["1000"] = 8; stringtoint["1001"] = 9; stringtoint["1010"] = 10; stringtoint["1011"] = 11; stringtoint["1100"] = 12; stringtoint["1101"] = 13;
	stringtoint["1110"] = 14; stringtoint["1111"] = 15;

	//数字转四位二进制字符串
	map<int, string> inttostring;
	inttostring[0] = "0000"; inttostring[1] = "0001"; inttostring[2] = "0010"; inttostring[3] = "0011"; inttostring[4] = "0100"; inttostring[5] = "0101"; inttostring[6] = "0110";
	inttostring[7] = "0111"; inttostring[8] = "1000"; inttostring[9] = "1001"; inttostring[10] = "1010"; inttostring[11] = "1011"; inttostring[12] = "1100"; inttostring[13] = "1101";
	inttostring[14] = "1110"; inttostring[15] = "1111";

	//等分成6bit的8份
	string g[8];
	for (int i = 0; i < 8; i++)
		g[i] = c.substr(i * 6, 6);
	for (int i = 0; i < 8; i++)
	{//第i个S盒
		string row(2, '0'); row[0] = g[i][0]; row[1] = g[i][5];
		string column(4, '0');
		for (int j = 0; j < 4; j++)
			column[j] = g[i][j + 1];
		int temp = sBox[i][stringtoint[row]][stringtoint[column]];
		string s_temp = inttostring[temp];
		g[i] = s_temp;
	}
	string res = "";
	for (int i = 0; i < 8; i++)
		res += g[i];
	return res;
}

//置换运算P
string P(string c)
{
	string temp = c;
	//P表
	int pTable[32] =
	{
		16,7,20,21,
		29,12,28,17,
		1,15,23,26,
		5,18,31,10,
		2,8,24,14,
		32,27,3,9,
		19,13,30,6,
		22,11,4,25
	};
	for (int i = 0; i < 32; i++)
		temp[i] = c[pTable[i] - 1];
	return temp;
}

int main()
{
	cout << "***********DES加密解密系统***********" << endl;
	while (1)
	{
		L:cout << "请选择你的操作：[1：加密；0：解密]";
		int op; cin >> op;
		if ((op != 1 && op != 0))
		{
			cout << "请输入合法指令！" << endl;
			goto L;
		}
		//输入明文和密钥
		cout << "请输入密钥（16进制）：";
		string k; cin >> k;
		if (k.length() != 16)
		{
			cout << "请输入64位密钥！" << endl;
			goto L;
		}
		if (op == 1)
			cout << "请输入明文（16进制）：";
		else
			cout << "请输入密文（16进制）：";
		string m16; cin >> m16;

		//将输入的16进制明/密文字符串转化为2进制明/密文字符串
		string M = Hto2(m16);

		//明/密文分组
		int n = (M.length() / 64) * 64 == M.length() ? M.length() / 64 : M.length() / 64 + 1;
		string* m = new string[n];
		for (int i = 0; i < n; i++)
			m[i] = M.substr(i * 64, 64);
		//最后一组不足64位，用0补齐
		int last_len = m[n - 1].length();
		for (int i = 0; i < 64 - last_len; i++)
			m[n - 1] += "0";

		string key = Hto2(k);
		//置换选择PC_1，生成56位初始密钥
		key = PC_1(key);

		//密钥分成左右两部分
		string k_L = key.substr(0, 28);
		string k_R = key.substr(28, 28);

		//产生16个子密钥
		string keys[16];
		for (int i = 0; i < 16; i++)
		{
			//密钥左循环移位
			loop(k_L, k_R, i);
			//密钥置换选择PC_2，得到48位轮密钥
			keys[i] = PC_2(k_L, k_R);
		}

		//密/明文空间
		string* C = new string[n];

		//循环对每个分组加密
		for (int i = 0; i < n; i++)
		{
			string c = m[i];

			//初始置换IP
			c = IP(c);

			//明/密文分成左右两部分
			string L = c.substr(0, 32);
			string R = c.substr(32, 32);

			//轮结构（16轮）
			for (int j = 0; j < 16; j++)
			{
				string temp = R;

				//选择扩展运算E
				R = E(R);

				//异或运算
				//根据加解密控制位op选择密钥顺序
				if (op == 1)
					R = XOR(R, keys[j]);
				else
					R = XOR(R, keys[15 - j]);

				//S盒（代换/选择）
				R = S(R);

				//置换P
				R = P(R);

				//异或运算
				R = XOR(L, R);

				L = temp;
			}

			//逆初始置换IP_rev
			c = R + L;
			c = IP_rev(c);
			C[i] = c;
		}

		//输出明/密文
		if (op == 1)
			cout << "密文（16进制）：";
		else
			cout << "明文（16进制）：";
		for (int i = 0; i < n; i++)
		{
			/*
			//雪崩效应检验辅助程序段
			int sum = 0;
			for (int j = 0; j < 64; j++)
			{
				if (j == 3)
				{
					if (C[i][j] == '0')
						sum++;
				}
				else
				{
					if (C[i][j] == '1')
						sum++;
				}
			}*/
			C[i] = btoh(C[i]);
			cout << C[i];
			//<< " 改变的位数：" << sum;
		}
		cout << endl;
	LOOP:cout << "是否继续？[y/n]";
		char e; cin >> e;
		if (e == 'y')
			continue;
		else if (e == 'n')
			break;
		else
		{
			cout << "请输入合法指令！" << endl;
			goto LOOP;
		}
	}
	system("pause");
	return 0;
}