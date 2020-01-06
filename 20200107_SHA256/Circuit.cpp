#include"Circuit.h"
#include<fstream>
#include<algorithm> 
#include <stdio.h>
#include <time.h>

hash<string> h;

vector<Gate> parse(string s)
{
	vector<Gate> c_temp;

	string temp;
	for (int i = 0; i < s.size(); i++)
	{
		Gate tempG;
		int count = 0;
		while (i < s.size())
		{
			if (s[i] == '[')
				count++;
			else if (s[i] == ']' || s[i] == ',')
				;
			else if (count == 1)
			{
				temp = "";
				while (s[i] != ',')
					temp += s[i++];
				tempG.gateID = stoi(temp);
			}
			else if (count == 2)
			{
				temp = "";
				while (s[i] != ',')
					temp += s[i++];
				tempG.first_inputID = stoi(temp);
				i++;
				temp = "";
				while (s[i] != ']')
					temp += s[i++];
				tempG.second_inputID = stoi(temp);
			}
			else if (count == 3)
			{
				while (s[i] != ']')
				{
					int index = s[i++] - 48;
					if (index >= 0)
						tempG.isTrue[index] = true;
				}
				c_temp.push_back(tempG);
				break;
			}
			i++;
		}
	}

	return c_temp;
}
	
void Circuit::INIT(string fileName) {
	string s_in, s_out;

	ifstream inFile(fileName);
	inFile >> s_in;
	inFile >> s_out;
	inFile.close();

	c_in = parse(s_in);
	//cout << c_in.size() << endl;
	c_out = parse(s_out);

	/*
	for (auto c : c_in)
	{
		cout << "----------------------------------------------------------\n";
		cout << c.gateID << endl;
		cout << c.first_inputID << endl;
		cout << c.second_inputID << endl;
		for (int i = 0; i < 4; i++)
			cout << c.isTrue[i] << endl;
	}
	*/
}

void Circuit::create_lable(string labletype)
{
	int LABELSIZE = 16;

	int range;

	if (labletype == "input_lable")
		range = (inputSize * 32) + ifAdd;
	else if (labletype == "gate_lable")
		range = c_in.size() + c_out.size();

	for (int i = 0; i < range; i++) {  //for (int i = 0; i < (inputSize * 32); i++)
		string zero = "";
		string one = "";
		do {
			for (int i = 0; i < LABELSIZE; i++) {
				random_char(zero);
			}
		} while (lable_exist(zero));
		do {
			for (int i = 0; i < LABELSIZE; i++) {
				random_char(one);
			}
		} while (lable_exist(one));

		Lable theResult;
		theResult.lab[0] = zero;
		theResult.lab[1] = one;
		lable.push_back(theResult);
	}
}

void Circuit::random_char(string &theS)
{
	int flag = rand() % 3;
	switch (flag)
	{
	case 0:
		theS += char('A' + rand() % 26);
		break;
	case 1:
		theS += char('a' + rand() % 26);
		break;
	case 2:
		theS += to_string(rand() % 10);
		break;
	default:
		theS += 'x';
		break;
	}
}

bool Circuit::lable_exist(string theS) {
	for (int i = 0; i < lable.size(); i++) {
		if (lable[i].lab[0] == theS)
			return true;
		if (lable[i].lab[1] == theS)
			return true;
	}
	return false;
}

void Circuit::create_table()
{
	for (int i = 0; i < c_in.size(); i++) {
		Table temp;
		temp = find_truthtable(c_in[i]);
		table.push_back(temp);
	}

	for (int i = 0; i < c_out.size(); i++) {
		Table temp;
		temp = find_truthtable(c_out[i]);
		table.push_back(temp);
	}

}

Table Circuit::find_truthtable(Gate c_gate)
{
	/*cout << lable[c_gate.first_inputID].lab[0] << " " << lable[c_gate.first_inputID].lab[1]
		<< " - " << lable[c_gate.second_inputID].lab[0] << " " << lable[c_gate.second_inputID].lab[1] << endl;*/

	// 建立gate的input
	vector<string>md5_input;
	string temp;
	temp = md5(lable[c_gate.first_inputID].lab[0]) + md5(lable[c_gate.second_inputID].lab[0]);
	md5_input.push_back(temp);
	temp = md5(lable[c_gate.first_inputID].lab[0]) + md5(lable[c_gate.second_inputID].lab[1]);
	md5_input.push_back(temp);
	temp = md5(lable[c_gate.first_inputID].lab[1]) + md5(lable[c_gate.second_inputID].lab[0]);
	md5_input.push_back(temp);
	temp = md5(lable[c_gate.first_inputID].lab[1]) + md5(lable[c_gate.second_inputID].lab[1]);
	md5_input.push_back(temp);

	// 建立gate的table
	Table t;
	int lableID;
	lableID = c_gate.gateID + (inputSize * 32) + ifAdd;
	for (int i = 0; i < 4; i++) {
		size_t hashtemp;
		if (c_gate.isTrue[i]) {
			t.ans[i] = lable[lableID].lab[1];
			temp = md5_input[i] + lable[lableID].lab[1];
			//temp做hash
			hashtemp = h(temp);
		}
		else {
			t.ans[i] = lable[lableID].lab[0];
			temp = md5_input[i] + lable[lableID].lab[0];
			//temp做hash
			hashtemp = h(temp);
		}
		t.ans_hash[i] = hashtemp;
	}
	//打亂table t 
	//int seed = (unsigned)time(NULL);
	//srand(seed);
	//int arr_rd[4] = { -1,-1,-1,-1 }; // 紀錄隨機存放的位置
	//for (int i = 0; i < 4; i++) {
	//	int rd = rand() % 4;
	//	bool isexist = false;
	//	for (int j = 0; j < 4; j++) {
	//		if (arr_rd[j] == rd) {
	//			isexist = true;
	//			i -= 1;
	//			break;
	//		}
	//	}
	//	if (!isexist)
	//		arr_rd[i] = rd;

	//}

	//Table t_rd;//打亂後的table
	//for (int i = 0; i < 4; i++) {
	//	int index = arr_rd[i];
	//	t_rd.ans[index] = t_rd.ans[i];
	//	t_rd.ans_hash[index] = t_rd.ans_hash[i];
	//}
	return t;
}

string Circuit::decide(string a, string b, Table t)
{
	size_t hashAns;
	string temp = md5(a) + md5(b);
	for (int i = 0; i < 4; i++)
	{
		hashAns = h(temp + t.ans[i]);
		if (hashAns == t.ans_hash[i])
			return t.ans[i];
	}
	//system("pause");
	return t.ans[rand() % 4];
}

string Circuit::calculate(vector<string> values)
{
	//輸入的值加上gate輸出的值
	int size = values.size();
	int newSize = size + c_in.size() + c_out.size();
	values.resize(newSize);
	
	int i = 0;
	//完成 IN
	for (int j = 0; j < c_in.size(); i++, j++) {
		string a = values[c_in[j].first_inputID]; //找出上面的值
		string b = values[c_in[j].second_inputID]; //找出下面的值
		Table t = table[i]; //算出輸出值

		//放回去values
		values[size + c_in[j].gateID] = decide(a, b, t);
	}

	//完成 OUT
	string answer = "";
	for (int j = 0; j < c_out.size(); i++, j++) {
		string a = values[c_out[j].first_inputID]; //找出上面的值
		string b = values[c_out[j].second_inputID]; //找出下面的值
		Table t = table[i]; //算出輸出值

		//放回去values
		string tempAns = decide(a, b, t);
		values[size + c_out[j].gateID] = tempAns;
		if (tempAns == lable[size + c_out[j].gateID].lab[0])
			answer += "0";
		else
			answer += "1";
	}
	return answer;
}