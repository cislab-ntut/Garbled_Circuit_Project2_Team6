#ifndef _CIRCUIT_H
#define _CIRCUIT_H
#include <iostream>
#include <vector>
#include <string>
#include "md5.h"
using namespace std;

//hash<string> mystdhash;

struct Gate {
	int gateID;
	int first_inputID;
	int second_inputID;
	bool isTrue[4] = { false, false, false, false };	//�u�Ȫ�
};

struct Lable {
	string lab[2];	//�u��0�M1lable
};

struct Table {
	string ans[4];			//��X�u��lable��
	size_t ans_hash[4];		//�����J�u��lable�ȩM��X�u��lable�Ȭۥ[��hash��
};

class Circuit
{
public:
	Circuit(string fileName, int input_size, bool if_add)
	{
		INIT(fileName); 
		inputSize = input_size;
		ifAdd = if_add;

		create_lable("input_lable");
		create_lable("gate_lable");
		//label vector output
		/*for (int i = 0; i < lable.size(); i++) {
			cout << lable[i].lab[0] <<" = " << lable[i].lab[1] << endl;
		}
		system("pause");*/
		create_table();
		/*for (int i = 0; i < table.size(); i++) {
			for (int j = 0; j < 4; j++)
				cout << table[i].ans[j] << " = " << table[i].ans_hash[j] << "\t";
			cout << "\n";
		}
		system("pause");*/
	};

	~Circuit() {};

	string calculate(vector<string> values);

	vector<Gate> c_in;
	vector<Gate> c_out;
	vector<Lable> lable;
	vector<Table> table;
	int inputSize;
	bool ifAdd;

private:
	void INIT(string fileName);
	void create_lable(string);
	void create_table();
	void random_char(string &theS);
	bool lable_exist(string theS);
	Table find_truthtable(Gate);
	string decide(string a, string b, Table t);

private:
};

#endif