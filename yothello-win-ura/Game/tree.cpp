//
//  tree.cpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#include "tree.hpp"

const double Node::c = 1.4;

Node::Node()
:wins(0),
visits(0),
passes(0),
end(false),
leaf(true),
depth(10),
mt(randomd()),
mvs(1),
exlimit(200),
policy_number(1),
//�e�X�g�p
base(1.25)
{
}


Node::~Node()
{
	for (unsigned int i(0); i < pchildnode.size();++i) {
		delete pchildnode[i];
	}
}

RouteNode::RouteNode()
:time(10000)
{
}

RouteNode::~RouteNode()
{
}

void Node::init()
{
	if (b.turn == 2) {
		b.reverse();
	}
	b.calculate_moves();
	b.calculate_disks();
	if (b.move_num == 0) {
		b.calculate_moves2();
		if (b.move_num2 == 0) {
			end = true;
		}
	}
	level = b.disks1 + b.disks2;
	if (b.turn == 1) {
		disksdif = b.disks1 - b.disks2;
	}
	else {
		disksdif = b.disks2 - b.disks1;
	}
	mvs = b.move_num;
	//for (int i(0); i < 8; ++i) {
	//	for (int j(0); j < 8; ++j) {
	//		cout << b.square[i][j] << ",";
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//cout << b.disks1 << "," << b.disks2 << "," << disksdif << "," << mvs << endl;
	//cout << endl;
}

void Node::expand()
{
	if (end) {
		return;
	}
	if (mvs == 0) {
		Board b1(b);
		int t(b1.turn);
		b1.turn = b1.turnnot;
		b1.turnnot = t;
		pchildnode.resize(1);
		pchildnode[0] = new Node;
		(*pchildnode[0]).b = b1;
		(*pchildnode[0]).lastmovei = -1;
		(*pchildnode[0]).lastmovej = -1;
		(*pchildnode[0]).init();
		(*pchildnode[0]).policy_number = policy_number;
		(*pchildnode[0]).depth = depth;
		(*pchildnode[0]).exlimit = exlimit;
	}
	else {
		pchildnode.resize(mvs);
		childQ.resize(mvs);
		int n(0);
		for (int i(0); i < 8;++i) {
			for (int j(0); j < 8;++j) {
				if (b.moves[i][j]) {
					Board b1(b);
					//cout << b1.turn << "," << b1.turnnot << "," << i << "," << j << "," << endl;
					b1.move_board(i, j);
					pchildnode[n] = new Node;
					(*pchildnode[n]).b = b1;
					(*pchildnode[n]).lastmovei = i;
					(*pchildnode[n]).lastmovej = j;
					(*pchildnode[n]).init();
					//cout << (*pchildnode[n]).disksdif << "," << (*pchildnode[n]).mvs << endl;
					(*pchildnode[n]).policy_number = policy_number;
					(*pchildnode[n]).depth = depth;
					(*pchildnode[n]).exlimit = exlimit;
					++n;
				}
			}
		}
	}
	leaf = false;
}

inline void Node::calQ()
{
	for (int i(0); i < b.move_num;++i) {
		childQ[i] = ((double)(*pchildnode[i]).visits - (double)(*pchildnode[i]).wins) / ((double)(*pchildnode[i]).visits + 1.0) + c*sqrt(log(double(passes)) / ((double)(*pchildnode[i]).visits + 1.0));
	}
	maxQn = distance(childQ.begin(), max_element(childQ.begin(), childQ.end()));
}


inline double Node::rndp(double max)
{
	std::uniform_real_distribution<> randnum(0, max);
	return randnum(mt);
}


Play::Play()
:play_folder("PLAY/"),
start(false),
endgame(false),
dep(12),
perf(43)
{
}

Play::~Play()
{
}

void Play::operator()()
{
	load();
	input();
	read(neu_folder);
	time_consumed = 0;
	elim_width = 18.0;
	runab();
	output();
}

void Play::selfplay(string folder)
{
	inputboard();
	read(folder);
	runab();
	output2();
}

void Play::input()
{
	std::ifstream ifs(board_file);
	if (!ifs) {
		std::cout << "CANNOT READ INPUT\n";
		exit(3);
		return;
	}
	//�������f
	std::string str;
	getline(ifs, str);
	if (stoi(str) == 0) {
		b.initialize();
		start = true;
	}
	else {
		start = false;
	}
	
	//�����
	getline(ifs, str);
	player = stoi(str);
	if (player == 1) {
		opponent = 2;
	}
	else {
		opponent = 1;
	}	
	//�Ֆʂ�ǂ�

	if (!start) {
		for (int i(0);i < 8;++i) {
			getline(ifs, str);
			std::string token;
			std::istringstream stream(str);
			for (int j(0);j < 8;++j) {
				getline(stream, token, ',');
				b.square[i][j] = stoi(token);
			}
		}
	}
	
	ifs.close();
	
	if (start&&player == 1) {
		return;
	}
	
	std::ifstream ifs2(inputmove_file);
	if (!ifs2) {
		std::cout << "CANNOT READ INPUT\n";
		exit(4);
		return;
	}
	getline(ifs2, str);
	lastmovei = stoi(str);

	getline(ifs2, str);
	lastmovej = stoi(str);
	
	if (lastmovei < 0) {
		b.turn = player;
		b.turnnot = opponent;
	}
	else {
		b.turn = opponent;
		b.turnnot = player;
		b.move_board(lastmovei, lastmovej);
	}
	ifs2.close();
}

void Play::inputboard()
{
	std::ifstream ifs(board_file);
	if (!ifs) {
		std::cout << "CANNOT LOAD BOARD\n";
		exit(5);
		return;
	}
	//�������f
	std::string str;
	getline(ifs, str);
	if (stoi(str) == 0) {
		b.initialize();
		start = true;
	}
	else {
		start = false;
	}
	
	//�����
	getline(ifs, str);
	player = stoi(str);
	if (player == 1) {
		opponent = 2;
	}
	else {
		opponent = 1;
	}
	//�Ֆʂ�ǂ�
	if (!start) {
		for (int i(0);i < 8;++i) {
			getline(ifs, str);
			std::string token;
			std::istringstream stream(str);
			for (int j(0);j < 8;++j) {
				getline(stream, token, ',');
				b.square[i][j] = stoi(token);
			}
		}
	}
	b.turn = player;
	b.turnnot = opponent;
	ifs.close();
	
}

void Play::read(string folder)
{
	for (int i(6);i < 65;++i) {
		neu[i].load(folder + "bw/bw" + to_string(i) + ".csv");
//		neu[i].loadG(folder + "bw/bw" + to_string(i) + ".csv");
	}
}

void Play::runab()
{
	clock_t start_time = clock();
	b.calculate_disks();
	level = b.disks1 + b.disks2;
	b.calculate_moves();
	move_choices = b.move_num;
	bool(*p_moves)[8](b.moves);
	if (player == 2) {
		b.reverse();
	}
	
	for (int i(0); i < 65; ++i) {
		depth[i] = dep;
	}
	for (int i(42); i < 65; ++i) {
		depth[i] = dep + 2;
	}
	for (int i(5); i < 9; ++i) {
		depth[i] = dep+2;
	}
	for (int i(perf); i < 65; ++i) {
		depth[i] = 99;
	}
	
	//�J�n�̈��
	if (start && player == 1) {
		//choseni = 2;
		//chosenj = 3;
		int dc[2];
		Random_choose rdc;
		rdc(dc, b.moves, b.move_num);
		choseni = dc[0];
		chosenj = dc[1];
	}
	
	//�Q�[���I��
	else if (level == 64) {
		endgame = true;
		choseni = -1;
		chosenj = -1;
	}
	
	//PASS�̏ꍇ
	else if (move_choices == 0) {
		bestval = -999;
		choseni = -1;
		chosenj = -1;
	}
	
	//��肵���Ȃ��ꍇ
	else if (move_choices == 1) {
		for (int i(0); i < 8; ++i) {
			for (int j(0);j < 8;++j) {
				if (p_moves[i][j]) {
					bestval = -999;
					choseni = i;
					chosenj = j;
				}
			}
		}
	}
	
	//���ȏ�
	else {
		vector< vector<double> > v(move_choices);
		int n(0);
		for (int i(0);i < 8;++i) {
			for (int j(0);j < 8;++j) {
				if (b.moves[i][j]) {
					v[n].resize(3);
					v[n][0] = 0;
					v[n][1] = i;
					v[n][2] = j;
					++n;
				}
			}
		}
		
		//�\�[�g����
		if (level < 55) {
			int mini(2*int((depth[level] - 8)/2));
			for (int n(0); n < move_choices; ++n) {
				Board b2(b);
				b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
				double y(0);
				if (mini > 0) {
					y = -Alpbet(b2, neu, level + 1, mini, -999, 999);
				}
				else {
					vector<double> x;
					x = b2.xin(neu[level].nodes[0]);
					y = -neu[level + 1].Forward(x)[0];
				}
				v[n][0] = y;
			}
			std::sort(v.begin(), v.end());
			std::reverse(v.begin(), v.end());
		}
		
		bestval = -999;
		//���̃m�[�h��
		for (int n(0);n < move_choices;++n) {
			Abwd abwd;
			Board b2(b);
			b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
			double nextval = -Alpbet(b2, neu, level, depth[level] - 1, -999, -bestval);
			if (nextval > bestval) {
				bestval = nextval;
				choseni = int(v[n][1] + 0.5);
				chosenj = int(v[n][2] + 0.5);
			}
		}
	}
	
	start = false;
	clock_t end_time = clock();
	time_consumed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	
	if (choseni >= 0) {
		b.move_board(choseni, chosenj);
	}
	else {
		int temp(b.turn);
		b.turn = b.turnnot;
		b.turnnot = temp;
	}
	
	if (player == 2) {
		b.reverse();
	}
	return;
}

void Play::runabwithbook()
{
	clock_t start_time = clock();
	
	b.calculate_disks();
	level = b.disks1 + b.disks2;
	b.calculate_moves();
	move_choices = b.move_num;
	
	Book book;
	book.initialize(level, 12);
	book.neu_folder = neu_folder;
	book.load();
	
	bool(*p_moves)[8](b.moves);
	if (player == 2) {
		b.reverse();
	}
	for (int i(0); i < 65; ++i) {
		depth[i] = dep;
	}
	for (int i(42); i < 65; ++i) {
		depth[i] = dep + 2;
	}
	for (int i(5); i < 9; ++i) {
		depth[i] = dep + 2;
	}
	for (int i(perf); i < 65; ++i) {
		depth[i] = 99;
	}
	
	
	elim_width = 20;
	if (level > 44) {
		elim_width = 99;
	}
	if (level == 44) {
		elim_width = 28;
	}
	if (level == 43) {
		elim_width = 22;
	}
	//�J�n�̈��
	if (start && player == 1) {
		int dc[2];
		Random_choose rdc;
		rdc(dc, b.moves, b.move_num);
		choseni = dc[0];
		chosenj = dc[1];
	}
	
	//�Q�[���I��
	else if (level == 64) {
		endgame = true;
		choseni = -1;
		chosenj = -1;
	}
	
	//PASS�̏ꍇ
	else if (move_choices == 0) {
		bestval = -999;
		choseni = -1;
		chosenj = -1;
		b.calculate_moves2();
		if (b.move_num2 == 0) {
			endgame = true;
		}
	}
	
	//��肵���Ȃ��ꍇ
	else if (move_choices == 1) {
		for (int i(0); i < 8; ++i) {
			for (int j(0);j < 8;++j) {
				if (p_moves[i][j]) {
					bestval = -999;
					choseni = i;
					chosenj = j;
				}
			}
		}
	}
	
	//���ȏ�
	else {
		vector<vector<double> > v(move_choices);
		int n(0);
		for (int i(0);i < 8;++i) {
			for (int j(0);j < 8;++j) {
				if (b.moves[i][j]) {
					v[n].resize(3);
					v[n][0] = 0;
					v[n][1] = i;
					v[n][2] = j;
					++n;
				}
			}
		}
		
		//�\�[�g����
		if (level < 55) {
			int mini(2*int((depth[level] - 8)/2));
			for (int n(0); n < move_choices; ++n) {
				Board b2(b);
				b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
				double y(0);
				if (mini > 0) {
					y = -Alpbet(b2, neu, level + 1, mini, -999, 999);
				}
				else {
					vector<double> x;
					x = b2.xin(neu[level].nodes[0]);
					y = -neu[level + 1].Forward(x)[0];
				}
				v[n][0] = y;
			}
			std::sort(v.begin(), v.end());
			std::reverse(v.begin(), v.end());
		}
		
		bestval = -999;
		//���̃m�[�h��
		for (int n(0);n < move_choices;++n) {
			Board b2(b);
			b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
			double nextval;
			int num = book.number(level + 1, b2.turn, b2.square);
			bool bookon(false);
			if (num >= 0) {
				double up = book.upper(level + 1, num);
				double lo = book.lower(level + 1, num);
				if (up == lo) {
					nextval = -up;
					bookon = true;
				}
				
			}
			if(!bookon) {
				if (level + depth[level] - 1 > 63) {
					nextval = -AlpbetE(b2, neu, level + 1, -999, -bestval);
				}
				else {
					nextval = -Alpbet(b2, neu, level + 1, depth[level] - 1, -999, -bestval);
				}
			}
			if (nextval > bestval) {
				bestval = nextval;
				choseni = int(v[n][1] + 0.5);
				chosenj = int(v[n][2] + 0.5);
			}
		}
	}
	
	start = false;
	clock_t end_time = clock();
	time_consumed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	
	if (choseni >= 0) {
		b.move_board(choseni, chosenj);
	}
	else {
		int temp(b.turn);
		b.turn = b.turnnot;
		b.turnnot = temp;
	}
	
	if (player == 2) {
		b.reverse();
	}
	return;
}


void Play::output()
{
	std::ofstream b_file;
	b_file.open(board_file, std::ios::out);
	b_file << 1 << endl;
	b_file << player << endl;
	for (int i(0); i < 8;++i) {
		for (int j(0); j < 8;++j) {
			b_file << b.square[i][j] << ",";
		}
		b_file << endl;
	}
	b_file.close();
	
	std::ofstream o_file;
	o_file.open(outputmove_file, std::ios::out);
	o_file << choseni << endl;
	o_file << chosenj;
	o_file.close();
}

void Play::output2()
{
	std::ofstream b_file;
	b_file.open(board_file, std::ios::out);
	b_file << 1 << endl;
	b_file << opponent << endl;
	for (int i(0); i < 8;++i) {
		for (int j(0); j < 8;++j) {
			b_file << b.square[i][j] << ",";
		}
		b_file << endl;
	}
	b_file.close();
	
	std::ofstream o_file;
	o_file.open(outputmove_file, std::ios::out);
	o_file << choseni << endl;
	o_file << chosenj;
	o_file.close();
}

void Play::record()
{
	std::ofstream r_file;
	r_file.open(record_file, std::ios::app);
	r_file << level << "," << bestval << "," << choseni << "," << chosenj << "," << time_consumed << endl;
	r_file.close();
	n0.visits = 0;
	n0.wins = 0;
}

void Play::load()
{
	std::ifstream ifs(local_folder + "settings.txt");
	if (!ifs) {
		std::cout << "CANNOT LOAD SETTINGS\n";
		exit(2);
		return;
	}
	//�������f
	std::string str;
	getline(ifs, str);
	neu_folder = local_folder + str + "/";
	getline(ifs, str);
	dep = stoi(str);
	getline(ifs, str);
	perf = stoi(str);
	
	inputmove_file = local_folder + play_folder + "inputmove.txt";
	outputmove_file = local_folder + play_folder + "outputmove.txt";
	board_file = local_folder + play_folder + "PlayingBoard.txt";
	record_file = local_folder + "record.txt";
}

