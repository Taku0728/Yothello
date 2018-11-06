#ifndef INCLUDED_Othello_h_
#define INCLUDED_Othello_h_

#pragma once
#include "stdafx.h"
#include "Learning.h"
using namespace std;

class Choose
{
public:
	Choose();
	~Choose();
	virtual void operator()(int(&determined_move)[2], const bool(&moves)[8][8], const int &move_num);

private:
};


//ランダム指し関数のクラス
class Random_choose :public Choose
{
public:
	Random_choose();
	~Random_choose();
	void operator()(int(&determined_move)[2], const bool(&moves)[8][8], const int &move_num);
private:

};

class Policy_choose
{
public:
	Policy_choose();
	~Policy_choose();
	void operator()(Neuralnet ne, int(&determined_move)[2], const bool(&moves)[8][8], const int &move_num);

private:

};


class Split
{
public:
	Split();
	~Split();
	std::vector < std::string > operator()(std::string &input, char delimiter);

private:

};

//盤面のクラス
class Board
{
public:
	Board();
	~Board();
	int square[8][8];
	//std::vector<int> board_v;
	int determined_move[2];

	//盤面の正規化
	//void rotate();
	//void simp_rotate();
	//void reflect();
	//void back_reflect();
	//void back_rotate();
	void reverse();
	void rotate();
	void reflect();

	//次の手
	void initialize();
	void calculate_moves();
	void calculate_moves2();
	void random_move();
	void move_board(Choose &c);
	void move_board(const int i, const int j);
	//vector<int> move_board_v(const vector<int> &v, const int j, const int k, const int turn1);

	int turn;
	int turnnot;
	int turnnum;
	int disks1;
	int disks2;
	int move_num;
	int move_num2;
	bool moves[8][8];
	bool moves2[8][8];
	bool fixed1_m[8][8];
	bool fixed2_m[8][8];

	//盤面の入出力
	//void set(const int(&squ)[8][8]);
	int get_turn() const;
	int get_turn_num() const;
	int get_move_num() const;
	bool get_end_game() const;
	//std::vector<int> get_board_v() const;
	//std::vector<int> get_board_v8() const;
	//void set_board_v(std::vector<int> &v);
	//void read_board_v();
	//void write_board_9() const;
	//void write_board_v() const;
	//void write_board_65() const;
	//void write_board_full() const;
	void write_moves() const;
	void calculate_disks();
	void playerturnboard();

	int get_win();
	//int get_win_v(vector<int> &v);
	//int winout();

	//std::vector<int> spots_num;
	//std::vector<int> spots_num2;

	//評価関数
	int spots1[10];
	int spots2[10];
	int spots_moves1[9];
	int spots_moves2[9];
	bool exmoves1[8][8];
	bool exmoves2[8][8];
	bool bimoves[8][8];
	int spots_exmoves1[9];
	int spots_exmoves2[9];
	int spots_bimoves[9];
	int openness_num1;
	int openness_num2;
	int spots_openness1[9];
	int openness1[8][8];
	int openness2[8][8];
	int spots_openness2[9];
	int fixed1;
	int fixed2;
	int spots_fixed1[9];
	int spots_fixed2[9];
	int mountains1;
	int mountains2;
	int wings1false;
	int wings2false;
	int wings1;
	int wings2;
	int sides1[4];
	int sides2[4];
	int corner[4];
	int bycorner[8];
	int xcorner[4];

	int mountain[4];
	int wing[4];
	int wing8[8];
	int stoner[8];
	int block[4];
	int bomb[4];
	int doublec[8];
	int safec[8];

	int space[20];
	int group[8][8];
	int spaces;
	//偶数盤面における自分しか打てない奇数空き
	//even board exmove odd space
	int ebeos;


	//int row1[8];
	//int row2[8];
	//int column1[8];
	//int column2[8];
	//int slash1[15];
	//int slash2[15];
	//int bslash1[15];
	//int bslash2[15];

	int cornermove[4];
	int cornermove2[4];
	int bycornermove[8];
	int bycornermove2[8];
	int xcornermove[4];
	int xcornermove2[4];
	int sidemoves[4];
	int sidemoves2[4];

	int corneropenness1[4];
	int corneropenness2[4];
	int bycorneropenness1[8];
	int bycorneropenness2[8];
	int xcorneropenness1[4];
	int xcorneropenness2[4];
	int sideopenness1[4];
	int sideopenness2[4];

	int whiteline[4];
	int blackline[4];
	int midsides1[4];
	int midsides2[4];

	bool directionfixed[8][8][4];
	//int midsidelline;
	//int midsideuline;
	//int midsiderline;
	//int midsidedline;

	void openness(int i, int j, int k);
	void opennessv2(int i, int j, int k);
	void calculate_spots();
	void calculate_opennessf();
	void calculate_openness();
	void calculate_opennessv2();
	void calculate_fixed();
	void calculate_fixedv2();
	void calculate_fixedv3();
	void calculate_fixedv4();
	void sweep_fixed(const int i,const int j,const int di,const int dj);
	//bool simp_fixed1(const int i, const int j);
	//bool simp_fixed2(const int i, const int j);
	//void bulk_fixed();
	void calculate_spots_fixed();
	void calculate_spots_fixedv2();
	void calculate_spots_fixedv3();
	void calculate_spots_fixedv4();
	void calculate_spots_moves();
	void calculate_mowi();
	void calculate_corners();
	void calculate_framemoves();
	void calculate_lines();
	void calculate_mwb4();
	void calculate_frameopenness();
	void calculate_exmoves();
	void calculate_spots_exmoves();
	void calculate_patterns();
	void calculate_spaces();
	void spread(int i, int  j, int k);

	vector<double> x74();
	vector<double> x78();
	vector<double> x102false();
	vector<double> x102();
	vector<double> x206();
	vector<double> x266();

	void move_board_direction(int i, int j, int direction);
	bool right_move(int i, int j) const;
	bool right_move2(int i, int j) const;
	bool right_direction(int i, int j, int d) const;
	bool right_direction2(int i, int j, int d) const;
	bool fixed(int i, int j) const;
	bool fixedv2(int i, int j) const;
	bool fixedv3(int i, int j) const;
	bool fixedv4(int i, int j);
	//void fixed_change(int i, int j);
	bool fixed_line(int i, int j, int l) const;
	bool fixed_direction(int i, int j, int d) const;

	bool space_move[20];
	bool space_move2[20];

private:
	//int rotation;
	//int reflection;
	int passes;
	bool pre_end_game;
	bool end_game;
	int win;
	std::string filename;
};

//試合のクラス
class Game
{
public:
	Game();
	~Game();
	std::string foldern;
	std::string filen;
	int randomize;
	std::vector<double> playoutb;
	std::vector<double> playoutb8[8];

	int disks;
	int disksdif;
	bool elim_err;
	int choices;
	int elim_choices;
	double max_abs;
	double max_squ;

	double bias;

	//void addboard(std::vector<int> &board_v) const;
	void writegame() const;
	//void writegame2(int n, std::string fo, std::string fi, int rad);
	void writegame5x(int n, std::string fo, std::string fi, int rad);
	//void playgame(Choose &ch1, Choose &ch2);
	void playto(Board &boa, Choose &ch1, Choose &ch2, int level);
	//void playout(Board &boa, int n);
	//void playoutvalue(Board &boa, int n);
	//void playoutvalue78(Board &boa, int n);
	//void playoutvalue78test(Board &boa, int n, Neuralnet &neu, const double w);
	//void playoutvalue78wd(Board &boa, int n, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65]);
	//void playoutvalue78wdtest(Board &boa, int n, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65]);
	//void ABout74(Board &boa, int n, Neuralnet&neu, const double w);
	//void ABout74test(Board &boa, int n, Neuralnet &neu, const double w);
	//void ABout78(Board &boa, int n, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	//void ABout78test(Board &boa, int n, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65], const int(&d)[65]);
	//void ABoutbia78(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&s)[65], const int(&d)[65]);
	//void ABoutbia78test(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&w)[65], const double(&s)[65], const int(&d)[65]);

	void ABout(Board &boa, int n, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	void ABout8(Board &boa, int n, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	void ABouttest(Board &boa, int n, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65], const int(&d)[65]);
	void ABoutbia(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&s)[65], const int(&d)[65]);
	void ABoutbiatest(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&w)[65], const double(&s)[65], const int(&d)[65]);
	void ABoutbia8(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&s)[65], const int(&d)[65]);

	void playgame5x(Choose &ch1, Choose &ch2);

	int get_turns() const;
	std::vector<std::vector<int>> get_game_matrix() const;
	inline void reverse(int i);
	inline void reverse5x(int i);

private:
	int win;
	int turns;
	std::vector<std::vector<int>> game_matrix;
	//void win_lose();
	//std::vector<std::vector<int>> all_board_8;
};

class Data
{
public:
	Data();
	virtual ~Data();
	vector<vector<double>> dteach;
	vector<vector<double>> dtest;
	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> dataggf;
	void readdata(std::string file);
	void readggf(const std::string file, int limit);
	void adddata(std::vector<std::vector<int>>::iterator it, int end);
	void writedata(std::ofstream &writing_file);
	void writeggf(const std::string f);
	void writeggfwin(const std::string f);
	void writeggfwinrnd(const std::string f, int randomize);
	void reverse128(int i);

private:
	int rows;
};

class DataB : public Data
{
public:
	std::vector<std::vector<double>> data;
	void readdata(std::string file);

};


class Winout
{
public:
	Winout();
	~Winout();
	int operator()(Board &b);

private:

};

//class Difout
//{
//public:
//	Difout();
//	virtual ~Difout();
//	double operator()(Board &b);
//	int maxi;
//	int maxj;
//	int maxn;
//	double max;
//
//private:
//
//};
//
//class FastDifout
//{
//public:
//	FastDifout();
//	~FastDifout();
//	double operator()(Board &b);
//
//private:
//
//};

double alpha(Board &b1, double x);

double beta(Board &b1, double x);

//class Difoute : public Difout
//{
//public:
//	Difoute();
//	~Difoute();
//	double operator()(Board &b, Neuralnet(&nue)[65], const double(&w)[65], const double(&s)[65]);
//	int dn;
//	int start_level;
//	int elim_depth;
//
//private:
//
//};

class Elim
{
public:
	Elim();
	~Elim();
	void operator()(Board &b1, Neuralnet &neu, const double &w);

private:

};

double FDO(Board &b1);

#endif

class Abwd
{
public:
	Abwd();
	~Abwd();
	//double alphawd(Board &b1, double x);
	//double betawd(Board &b1, double x);
	//double alphawd(Board &b1, double x, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	//double betawd(Board &b1, double x, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	//double run(Board &b1, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	//double alphanet(Board &b1, double x, Neuralnet(&neu)[65]);
	//double betanet(Board &b1, double x, Neuralnet(&neu)[65]);
	//double net(Board &b1, Neuralnet(&neu)[65]);

	//double awe(Board &b1, double x, Neuralnet(&neu)[65], double bv, double w);
	//double bwe(Board &b1, double x, Neuralnet(&neu)[65], double bv, double w);
	//double wend(Board &b1, Neuralnet(&neu)[65]);


	//double alphaend(Board &b1, double x, Neuralnet(&neu)[65]);
	//double betaend(Board &b1, double x, Neuralnet(&neu)[65]);
	//double end(Board &b1, Neuralnet(&neu)[65]);

	//double aws(Board &b1, double x, Neuralnet(&neu)[65], const int levelend, double bv, double w);
	//double bws(Board &b1, double x, Neuralnet(&neu)[65], const int levelend, double bv, double w);
	//double abws(Board &b1, Neuralnet(&neu)[65], const int levelend);

	//double alphasearch(Board &b1, double x, Neuralnet(&neu)[65], const int levelend);
	//double betasearch(Board &b1, double x, Neuralnet(&neu)[65], const int levelend);
	//double absearch(Board &b1, Neuralnet(&neu)[65], const int levelend);

	double alphabeta(Board &b1, Neuralnet(&neu)[65], int disks, int depth, double alp, double bet);


	int level;
	int maxi;
	int maxj;
	int maxn;
	int mvs;
	int disks;
	double max;
	bool elim;

	double shrink;
	double elim_width;

private:

};

class Ab
{
public:
	Ab();
	~Ab();
	double run(Board &b1, Neuralnet &neu);
	int level;
	int maxi;
	int maxj;
	int maxn;
	int mvs;
	double max;
private:

};

class Playbias
{
public:
	Playbias();
	~Playbias();
	void operator()(Board &boa, Neuralnet(&neu)[65], int level, double bias);

	//乱数
	random_device randomd;
	mt19937 mt;
	double rndp(double max);
private:

};


