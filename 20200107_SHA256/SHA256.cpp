#include"SHA256.h"
#include"Circuit.h"
#include<fstream>
#include<algorithm>
#include<vector>
#include<string>

Circuit C_LS0 = Circuit("LSigma_0.txt", 1, 0);
Circuit C_LS1 = Circuit("LSigma_1.txt", 1, 0);
Circuit C_Con = Circuit("conditional.txt", 3, 0);
Circuit C_Maj = Circuit("Majority.txt", 3, 0);
Circuit C_Add = Circuit("adder.txt", 2, 1);

string WW[64] = {};
string KK[64] = {};

//64个32比特字的常数（前64个素数的立方根小数前32位）
const UInt32 K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

//初始化杂凑值（前8个素数的平方根小数前32位）
void SHA256::INIT() {
	MD.H[0] = 0x6a09e667;
	MD.H[1] = 0xbb67ae85;
	MD.H[2] = 0x3c6ef372;
	MD.H[3] = 0xa54ff53a;
	MD.H[4] = 0x510e527f;
	MD.H[5] = 0x9b05688c;
	MD.H[6] = 0x1f83d9ab;
	MD.H[7] = 0x5be0cd19;
}

void opration(A_H &a_h, int It)
{
	//T1_LS1
	vector<string> values;
	for (int i = 0; i < 32 ; i++)
		values.push_back(C_LS1.lable[i].lab[a_h.E[i] - '0']);
	string T1_LS1 = C_LS1.calculate(values);

	//T1_ADD1
	vector<string> tempV1;
	for (int i = 0; i < 32 ; i++)
		tempV1.push_back(C_Add.lable[i].lab[a_h.H[i] - '0']);
	//reverse(tempV1.begin(), tempV1.end());
	vector<string> tempV2;
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Add.lable[i + 32].lab[T1_LS1[i] - '0']);
	//reverse(tempV2.begin(), tempV2.end());

	//values = tempV1 + tempV2 + C_Add.label[64][0];
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.push_back(C_Add.lable[64].lab[0]);//tempV1 + tempV2 + C_Add.label[64][0]
	string T1_ADD1 = C_Add.calculate(tempV1);
	//reverse(T1_ADD1.begin(), T1_ADD1.end());

	//T1_Con
	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Con.lable[i].lab[a_h.E[i] - '0']);
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Con.lable[i + 32].lab[a_h.F[i] - '0']);
	vector<string> tempV3;
	for (int i = 0; i < 32; i++)
		tempV3.push_back(C_Con.lable[i + 64].lab[a_h.G[i] - '0']);
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.insert(tempV1.end(), tempV3.begin(), tempV3.end());//tempV1 + tempV2 + tempV3
	string T1_Con = C_Con.calculate(tempV1);

	//T1_ADD2
	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Add.lable[i].lab[T1_ADD1[i] - '0']);
	//reverse(tempV1.begin(), tempV1.end());
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Add.lable[i + 32].lab[T1_Con[i] - '0']);
	//reverse(tempV2.begin(), tempV2.end());
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.push_back(C_Add.lable[64].lab[0]);//T1_ADD1 + T1_Con + C_Add.label[64][0]
	string T1_ADD2 = C_Add.calculate(tempV1);
	//reverse(T1_ADD2.begin(), T1_ADD2.end());

	//T1_ADD3
	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Add.lable[i].lab[T1_ADD2[i] - '0']);
	//reverse(tempV1.begin(), tempV1.end());
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Add.lable[i + 32].lab[KK[It][i] - '0']);
	//reverse(tempV2.begin(), tempV2.end());
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.push_back(C_Add.lable[64].lab[0]);//T1_ADD2 + KK[It] + C_Add.label[64][0]
	string T1_ADD3 = C_Add.calculate(tempV1);
	//reverse(T1_ADD3.begin(), T1_ADD3.end());

	//T1
	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Add.lable[i].lab[T1_ADD3[i] - '0']);
	//reverse(tempV1.begin(), tempV1.end());
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Add.lable[i + 32].lab[WW[It][i] - '0']);
	//reverse(tempV2.begin(), tempV2.end());
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.push_back(C_Add.lable[64].lab[0]);//T1_ADD3 + WW[It] + C_Add.label[64][0]
	string T1 = C_Add.calculate(tempV1);
	//reverse(T1.begin(), T1.end());

	//T2
	values.clear();
	for (int i = 0; i < 32; i++)
		values.push_back(C_LS0.lable[i].lab[a_h.A[i] - '0']);
	string T2_LS0 = C_LS0.calculate(values);

	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Maj.lable[i].lab[a_h.A[i] - '0']);
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Maj.lable[i + 32].lab[a_h.B[i] - '0']);
	tempV3.clear();
	for (int i = 0; i < 32; i++)
		tempV3.push_back(C_Maj.lable[i + 64].lab[a_h.C[i] - '0']);
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.insert(tempV1.end(), tempV3.begin(), tempV3.end());//tempV1 + tempV2 + tempV3
	string T2_Maj = C_Maj.calculate(tempV1);

	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Add.lable[i].lab[T2_LS0[i] - '0']);
	//reverse(tempV1.begin(), tempV1.end());
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Add.lable[i + 32].lab[T2_Maj[i] - '0']);
	//reverse(tempV2.begin(), tempV2.end());
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.push_back(C_Add.lable[64].lab[0]);//T2_LS0 + T2_Maj + C_Add.label[64][0]
	string T2 = C_Add.calculate(tempV1);
	//reverse(T2.begin(), T2.end());

	//D + T1
	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Add.lable[i].lab[a_h.D[i] - '0']);
	//reverse(tempV1.begin(), tempV1.end());
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Add.lable[i + 32].lab[T1[i] - '0']);
	//reverse(tempV2.begin(), tempV2.end());
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.push_back(C_Add.lable[64].lab[0]);//D + T1 + C_Add.label[64][0]
	string D_T1 = C_Add.calculate(tempV1);
	//reverse(D_T1.begin(), D_T1.end());

	//T1 + T2
	tempV1.clear();
	for (int i = 0; i < 32; i++)
		tempV1.push_back(C_Add.lable[i].lab[T1[i] - '0']);
	//reverse(tempV1.begin(), tempV1.end());
	tempV2.clear();
	for (int i = 0; i < 32; i++)
		tempV2.push_back(C_Add.lable[i + 32].lab[T2[i] - '0']);
	//reverse(tempV2.begin(), tempV2.end());
	tempV1.insert(tempV1.end(), tempV2.begin(), tempV2.end());//tempV1 + tempV2
	tempV1.push_back(C_Add.lable[64].lab[0]);//T1 + T2 + C_Add.label[64][0]
	string T1_T2 = C_Add.calculate(tempV1);
	//reverse(T1_T2.begin(), T1_T2.end());

	a_h.H = a_h.G;
	a_h.G = a_h.F;
	a_h.F = a_h.E;
	a_h.E = D_T1;
	a_h.D = a_h.C;
	a_h.C = a_h.B;
	a_h.B = a_h.A;
	a_h.A = T1_T2;
}

string toBinary(UInt32 n)
{
	string result;

	for (int i = 0; i < 32; i++)
	{
		result.push_back('0' + (n & 1));
		n >>= 1;
	}

	reverse(result.begin(), result.end());
	return result;
}

int toUInt32(string s)
{
	int len = s.length();
	UInt32 n = s[0] - '0';
	for (int i = 1; i < len; i++) {
		n <<= 1;
		if (s[i] == '1')
			n += 1;
	}

	return n;
}

//处理512比特数据，返回信息摘要
Message_Digest SHA256::DEAL(UInt32 M[16]) {
	int i;
	UInt32 T1 = 0, T2 = 0;
	UInt32 W[64] = { 0 };
	//UInt32 A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0, H = 0;

	for (i = 0; i < 16; i++) 
		W[i] = M[i];
	
	for (i = 16; i < 64; i++) 
		W[i] = SSigma_1(W[i - 2]) + W[i - 7] + SSigma_0(W[i - 15]) + W[i - 16];

	for (i = 0; i < 64; i++)
		WW[i] = toBinary(W[i]);
	for (i = 0; i < 64; i++)
		KK[i] = toBinary(K[i]);

	A_H a_h;

	a_h.A = toBinary(MD.H[0]);
	a_h.B = toBinary(MD.H[1]);
	a_h.C = toBinary(MD.H[2]);
	a_h.D = toBinary(MD.H[3]);
	a_h.E = toBinary(MD.H[4]);
	a_h.F = toBinary(MD.H[5]);
	a_h.G = toBinary(MD.H[6]);
	a_h.H = toBinary(MD.H[7]);

	cout << "初始：";
	cout << hex << MD.H[0] << " " << MD.H[1] << " " << MD.H[2] << " " << MD.H[3] << " "
		<< MD.H[4] << " " << MD.H[5] << " " << MD.H[6] << " " << MD.H[7] << endl;

	//fstream outfile;      //宣告fstream物件
	//outfile.open("H.txt", ios::out);
	////開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	//outfile << hex << A << " " << B << " " << C << " " << D << " " << E << " " << F << " " << G << " " << H << endl; //將str寫入檔案
	//outfile.close();       //關閉檔案
	//outfile.open("W.txt", ios::out);
	//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	//for (i = 0; i < 64; i++) {
	//	outfile << hex << W[i] << " ";
	//}
	//outfile.close();       //關閉檔案

	for (i = 0; i < 64; i++) {

		//fstream outfile;      //宣告fstream物件
		//outfile.open("I.txt", ios::out);
		//outfile << i;
		//outfile.close();       //關閉檔案

		//system("main.exe");

		/*
		T1 = H + LSigma_1(E) + Conditional(E, F, G) + K[i] + W[i];
		T2 = LSigma_0(A) + Majority(A, B, C);
		H = G;
		G = F;
		F = E;
		E = D + T1;
		D = C;
		C = B;
		B = A;
		A = T1 + T2;
		//*/
		
		//fstream infile;      //宣告fstream物件
		//infile.open("H.txt", ios::in);        //開啟檔案為輸出狀態
		//infile >> hex >> A >> B >> C >> D >> E >> F >> G >> H;
		//infile.close();       //關閉檔案

		/*vector<string> values;
		for (int i = 0; i < 32; i++)
			values.push_back(C_LS0.lable[i].lab[a_h.E[i] - '0']);
		string T1_LS0 = C_LS0.calculate(values);*/

		//cout << a_h.A << " " << a_h.B << endl;
		opration(a_h, i);
		//cout << a_h.A << " " << a_h.B << endl;
		//system("pause");

		/*cout << a_h.A << " " << a_h.B << " " << a_h.C << " " << a_h.D << " "
			<< a_h.E << " " << a_h.F << " " << a_h.G << " " << a_h.H << endl;*/
	}

	UInt32 A = toUInt32(a_h.A), B = toUInt32(a_h.B), C = toUInt32(a_h.C)
		, D = toUInt32(a_h.D), E = toUInt32(a_h.E), F = toUInt32(a_h.F)
		, G = toUInt32(a_h.G), H = toUInt32(a_h.H);

	MD.H[0] = (MD.H[0] + A) & 0xFFFFFFFF;
	MD.H[1] = (MD.H[1] + B) & 0xFFFFFFFF;
	MD.H[2] = (MD.H[2] + C) & 0xFFFFFFFF;
	MD.H[3] = (MD.H[3] + D) & 0xFFFFFFFF;
	MD.H[4] = (MD.H[4] + E) & 0xFFFFFFFF;
	MD.H[5] = (MD.H[5] + F) & 0xFFFFFFFF;
	MD.H[6] = (MD.H[6] + G) & 0xFFFFFFFF;
	MD.H[7] = (MD.H[7] + H) & 0xFFFFFFFF;

	return MD;
}

//右旋转
UInt32 SHA256::ROTR(UInt32 W, int n) {
	return ((W >> n) & 0xFFFFFFFF) | (W) << (32 - (n));
}

//左旋转
UInt32 SHA256::ROTL(UInt32 W, int n) {
	return ((W << n) & 0xFFFFFFFF) | (W) >> (32 - (n));
}

//右移位
UInt32 SHA256::SHR(UInt32 W, int n) {
	return ((W >> n) & 0xFFFFFFFF);
}