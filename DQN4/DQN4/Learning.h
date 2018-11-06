#pragma once
#include "stdafx.h"
#include "maths.h"
using namespace std;

class Neuralnet
{
public:
	Neuralnet();
	~Neuralnet();
	int layers;
	vector<int> nodes;

	vector<vector<double>> bia;
	vector<vector<vector<double>>> wei;

	void initialize_bw();
	void initialize_bw_sym();
	void set_network(vector<int> &no);
	vector<double> Forward(const vector<double> &x) const;
	void Backward(vector<vector<double>> &dEdb, vector<vector<vector<double>>> &dEdw, const vector<double> &x, const vector<double> &y) const;

	//ì¸èoóÕ
	void save(string filename) const;
	void load(string filename);

private:
	double activate(double x);

};

class Learning
{
public:
	Learning();
	~Learning();

	int times;
	int batch;
	int tests;
	double a;
	int t;

	vector<int> net;
	vector<vector<double>> SdEdb;
	vector<vector<vector<double>>> SdEdw;
	vector<double> ai_accuracy;
	vector<double> ai_accuracy_sign;

	vector<vector<double>> teach_data_x;
	vector<vector<double>> teach_data_y;
	vector<vector<double>> test_data_x;
	vector<vector<double>> test_data_y;
	vector<vector<double>> teach_data_all;
	vector<vector<double>> test_data_all;

	void read_teach_data(const string file);
	void read_test_data(const string file);
	void separate_data();
	void initial_parameters(const vector<int> &n);
	void set_parameters(const vector<double> &x, const vector<int> &n);
	void set_paras(const vector<double> &x, const vector<int> &n);
	void test_accuracy(Neuralnet &ne);
	void test_accuracy_sign(Neuralnet &ne);
	void write_accuracy(const string file);
	void write_accuracy_sign(const string file);
	double get_accuracy_e() const;
	void clearS();

	void save(string filename) const;
	void load(string filename);

protected:
	int x_begin;
	int x_end;
	int y_begin;
	int y_end;

private:

};

class Learning_Adam : public Learning
{
public:
	Learning_Adam();
	~Learning_Adam();
	vector<vector<double>> mAdamb;
	vector<vector<double>> vAdamb;
	vector<vector<vector<double>>> mAdamw;
	vector<vector<vector<double>>> vAdamw;

	void initial_parameters(const vector<int> &n);
	void set_parameters(const vector<double> &x, const vector<int> &n);
	void teach(Neuralnet &ne);

	void save(string filename) const;
	void load(string filename);

	int tA;

private:
};

class Optimizer
{
public:
	Optimizer();
	Optimizer(double a1);
	virtual ~Optimizer();
	void operator()(Neuralnet &net, Learning &le);
	double a;

protected:

private:
};

class Adam : public Optimizer
{
public:
	Adam();
	~Adam();
	void operator()(Neuralnet &net, Learning_Adam &le);

private:
	double b1;
	double b2;
	double e;
};

class SDG : public Optimizer
{
public:
	SDG();
	SDG(double a1);
	~SDG();
	void operator()(Neuralnet &net, Learning &le);

private:
};