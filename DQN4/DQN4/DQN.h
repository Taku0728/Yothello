
#pragma once
#include "stdafx.h" 
#include "Learning.h" 
#include "neuralnet.h"
#include "maths.h"
#include "othello.h"


using namespace std;

class Teach
{
public:
	Teach();
	virtual ~Teach();
	virtual void operator()(Learning &le, Neuralnet &neu, Optimizer &op, int i);

private:

};

class Teach8 : public Teach
{
public:
	Teach8();
	~Teach8();
	void operator()(Learning &le, Neuralnet &neu, Optimizer &op, int i);

private:

};

class Teachv8 : public Teach
{
public:
	Teachv8();
	~Teachv8();
	void operator()(Learning &le, Neuralnet &neu, Optimizer &op, int i);

private:

};

class DQN
{
public:
	DQN(string file);
	~DQN();
	void run(Teach te, Optimizer op);
	void DQN::run(const Data &da, Teach te, Optimizer &op);
	vector<double> para;
	vector<int> net;
	Neuralnet neu;
	Learning le;
	Learning_Adam la;

	int opt_time;

private:

	string folder;
	string opt_file;
	string para_file;
	string teach_file;
	string test_file;
	string aia_file;
	string wb_file;
	string policy_file;
};

class TDL
{
public:
	TDL();
	~TDL();
	void run();
	void readjob();
	void read();
	void read_sum();
	void learn(const int level, double as);
	void sum(const int level);
	void addsum(const int level);
	void test(const int level);
	void read_policy();
	void write_test();
	void renewjob();
	void renewcontrol_start();
	void renewcontrol_end();

	string thispc;
	string foldername;
	string foldername2;
	string foldername3;
	string folder1;
	string folder2;
	string folder3;
	string opt_file;
	string opt_file2;
	string bw_file[65];
	string bw_file2[65];
	string bw_file3[65];
	string acc_file[65];
	string acc_file2[65];
	string sigacc_file[65];
	string sigacc_file2[65];
	string summary_file;
	string summary_file2;
	string policy_file;
	string policy_file2;
	string test_file;
	vector<double> para;
	vector<int> net;
	vector<double> para2;
	vector<int> net2;

	bool biaslearning;
	
	bool running_condition;
	int learning_level;
	int startlevel;
	int endlevel;
	int opt_level;
	int opt_level2;
	double acc[65];
	double acc2[65];
	double sigacc[65];
	double sigacc2[65];
	double err[65];
	double err2[65];
	double choices_ave[65];
	double choices_ave2[65];
	double elim_choices_ave[65];
	double elim_choices_ave2[65];
	double max_ave[65];
	double max_ave2[65];
	double max_squ_ave[65];
	double max_squ_ave2[65];
	double w[65];
	double opt_a[65];
	double opt_a2[65];
	int depth[65];
	double shrink[65];
	double bias;

	Optimizer opt;
	Optimizer opt2;
	//Learning lea[65];
	//Learning lea2[65];
	Learning lea;
	Learning lea2;
	Neuralnet neu[65];
	Neuralnet neu2[65];
	//Play—pneuralnet
	Neuralnet neu3[65];
	Teach tea;
	Data data;
	Random_choose chr;

private:
	bool readdone;
	bool readjobdone;

};

class Test
{
public:
	Test();
	~Test();
	void run(string folder,const int level);


private:

};