//
//  othello.hpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#ifndef othello_hpp
#define othello_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>
#include <random>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include "neuralnet.hpp"
#include "book.hpp"
#include "math.hpp"
// #include <amp.h>

using  std::cout;
using  std::cin;
using  std::wcout;
using  std::endl;
using  std::abs;
using  std::max;
using  std::min;

using std::string;
using std::vector;
using std::random_device;
using std::mt19937;
using std::normal_distribution;
using std::ostringstream;

// using Concurrency::accelerator;
// using Concurrency::array;
// using Concurrency::array_view;
// using Concurrency::extent;
// using Concurrency::parallel_for_each;
// using Concurrency::index;
// using Concurrency::parallel_for;

class Choose
{
public:
	Choose();
	~Choose();
	virtual void operator()(int(&determined_move)[2], const bool(&moves)[8][8], const int &move_num);
	
private:
};


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

//�Ֆʂ̃N���X
class Board
{
public:
	Board();
	~Board();
	int square[8][8];
	//std::vector<int> board_v;
	int determined_move[2];
	
	//�Ֆʂ̐��K��
	//void rotate();
	//void simp_rotate();
	//void reflect();
	//void back_reflect();
	//void back_rotate();
	void reverse();
	void rotate();
	void reflect();
	
	//���̎�
	void initialize();
	void calculate_moves();
	void calculate_movesM();
	void calculate_movesG();
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
	
	//�Ֆʂ̓��o��
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
	
	//�]���֐�
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
	//��Ֆʂɂ����鎩�������łĂȂ����
	//even board exmove odd space
	int ebeos;
	//�n�C�p�[��
	//�������łĂȂ����
	int hypereven;
	
	
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
	
	int diskdif63();
	
	void openness(int i, int j, int k);
	void opennessv2(int i, int j, int k);
	void opennessv3(int i, int j, int k);
	void calculate_spots();
	void calculate_opennessf();
	void calculate_openness();
	void calculate_opennessv2();
	void calculate_opennessv3();
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
	vector<double> x267();
	vector<double> xin(int inputs);
	
	void move_board_direction(int i, int j, int direction);
	bool right_move(int i, int j) const;
	bool right_moveM(int i, int j) const;
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

//�����̃N���X
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
	
	void writegame() const;
	void writegame5x(int n, std::string fo, std::string fi, int rad);
	void playto(Board &boa, Choose &ch1, Choose &ch2, int level);
	
	void ABout(Board &boa, int n, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	void ABout8(Board &boa, int n, Neuralnet(&neu)[65], const double(&s)[65], const int(&d)[65]);
	void ABouttest(Board &boa, int n, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65], const int(&d)[65]);
	void ABoutbia(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&s)[65], const int(&d)[65]);
	void ABoutbiatest(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&w)[65], const double(&s)[65], const int(&d)[65]);
	void ABoutbiatestwithbook(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&w)[65], const double(&s)[65], const int(&d)[65], Book &book);
	void ABoutbia8(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&s)[65], const int(&d)[65]);
	void ABoutbia8withbook(Board &boa, int n, Neuralnet(&neu)[65], Neuralnet(&neup)[65], const double(&s)[65], const int(&d)[65], Book &book);
	
	void playgame5x(Choose &ch1, Choose &ch2);
	
	int get_turns() const;
	std::vector<std::vector<int> > get_game_matrix() const;
	inline void reverse(int i);
	inline void reverse5x(int i);
	
private:
	int win;
	int turns;
	std::vector<std::vector<int> > game_matrix;
};

class Data
{
public:
	Data();
	virtual ~Data();
	vector<vector<double> > dteach;
	vector<vector<double> > dtest;
	std::vector<std::vector<int> > data;
	std::vector<std::vector<int> > dataggf;
	void readdata(std::string file);
	void readggf(const std::string file, int limit);
	void adddata(std::vector<std::vector<int> >::iterator it, int end);
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
	std::vector<std::vector<double> > data;
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


double alpha(Board &b1, double x);

double beta(Board &b1, double x);

class Elim
{
public:
	Elim();
	~Elim();
	void operator()(Board &b1, Neuralnet &neu, const double &w);
	
private:
	
};

double FDO(Board &b1);


class Abwd
{
public:
	Abwd();
	~Abwd();
	
	double alphabeta(Board &b1, Neuralnet(&neu)[65], int disks, int depth, double alp, double bet);
	double alphabetaend(Board &b1, Neuralnet(&neu)[65], int disks, double alp, double net);
	
	
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
	void runwithbook(Board &boa, Neuralnet(&neu)[65], int level, double bias, Book &book);
	
	//����
	random_device randomd;
	mt19937 mt;
	double rndp(double max);
private:
	
};

double Alpbet(Board &b1, Neuralnet(&neu)[65], int disks, int depth, double alp, double bet);

double AlpbetE(Board &b1, Neuralnet(&neu)[65], int disks, double alp, double bet);





#endif /* othello_hpp */
