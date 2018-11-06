

#pragma once
#include "stdafx.h" 
#include "maths.h"

using namespace std;

class Book
{
public:
	Book();
	~Book();
	string neu_folder;

	//int ReferTable[20][1000][9];
	vector<vector<vector<unsigned long long>>> ReferTable;
	vector<vector<double>> TransTableUpper;
	vector<vector<double>> TransTableLower;
	int Size[40];
	int disks0;
	int sized;

	int number(const int disks,const int turn,const int(&s)[8][8]);
	double upper(const int disks, const  int n);
	double lower(const int disks, const  int n);
	void newdata(const int disks, const  int turn, const  int(&s)[8][8], const double upper, const  double lower);
	void record(const int disks, const  int n, const double upper, const double lower);
	void initialize(const int disks, const int depth);
	void display(const int disks, const  int turn, const int(&s)[8][8], int n);
	void save();
	void load();

	//void v8(const int turn, const  int(&sq)[8][8], unsigned long long(&mat)[2]);
	void v2(const int turn, const  int(&sq)[8][8], unsigned long long(&mat)[2]);

private:

};
