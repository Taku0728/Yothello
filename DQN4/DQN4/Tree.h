#pragma once

#include "stdafx.h" 
#include "Learning.h" 
#include "maths.h"
#include "othello.h"

class Node
{
public:
	Node();
	virtual ~Node();
	Board b;
	int wins;
	int visits;
	int passes;
	int depth;
	bool leaf;
	bool end;
	int maxQn;
	bool wintrans;

	//ポリシー
	int policy_number;

	//盤面基本情報
	int level;
	int disksdif;
	int mvs;
	int lastmovei;
	int lastmovej;

	static const double c;
	int exlimit;
	double base;
	vector<Node*> pchildnode;
	vector<double> childQ;

	//乱数
	random_device randomd;
	mt19937 mt;
	double rndp(double max);

	void expand();
	void init();
	void calQ();
	void search(Neuralnet(&neu)[65]);
	bool playoutbias(Board &b1, Neuralnet(&neu)[65]);
	bool playoutrand(Board &b1, Neuralnet(&neu)[65]);
	bool playendrand(Board &b1);

private:

};

class RouteNode:public Node
{
public:
	RouteNode();
	~RouteNode();
	void searchbest(Neuralnet(&neu)[65]);

	int plays;
	int besti;
	int bestj;
	int bestn;
	double bestvalue;
	int time;

private:

};

class Play
{
public:
	Play();
	~Play();
	void operator()();
	void selfplay(string folder);
	void input();
	void inputboard();
	void read(string folder);
	void run(string folder);
	void runab0(string folder);
	void runab();
	void runabw();
	void output();
	void output2();
	void record();
	void load();

	string neu_folder;
	string policy_folder;
	const string play_folder;
	string inputmove_file;
	string outputmove_file;
	string board_file;
	string record_file;

	Neuralnet neu[65];
	RouteNode n0;
	int depth[65];

	Board b;
	int move_choices;
	int level;
	int player;
	int opponent;
	bool start;
	int lastmovei;
	int lastmovej;
	int choseni;
	int chosenj;
	bool endgame;
	double bestval;
	double time_consumed;

	int policy_number;
	double elim_width;

private:

};

