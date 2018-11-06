
#include "stdafx.h" 
#include "othello.h"
#include "maths.h"
#include "Learning.h"
#include "Tree.h"
#include "Book.h"

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
	//テスト用
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

//すべての手にバイアスをかけてプレイアウト
//bool Node::playoutbias(Board &b1, Neuralnet(&neu)[65])
//{
//	int l(b1.disks1 + b1.disks2);
//	//終盤判断
//	int movenum(b1.move_num);
//	if (movenum == 0) {
//		b1.calculate_moves2();
//		if (b1.move_num2 == 0) {
//			if (b1.turn == 1) {
//				if (b1.disks1 - b1.disks2 >= 0) {
//					return true;
//				}
//				else {
//					return false;
//				}
//			}
//			else {
//				if (b1.disks2 - b1.disks1 >= 0) {
//					return true;
//				}
//				else {
//					return false;
//				}
//			}
//		}
//		Board b2(b1);
//		b2.turn = b1.turnnot;
//		b2.turnnot = b1.turn;
//		++b2.turnnum;
//		b2.calculate_moves();
//		b2.calculate_disks();
//		return !playoutbias(b2, neu);
//	}
//	bool(*p_moves)[8](b1.moves);
//
//	//到達判断
//	if (l == level + depth - 1) {
//		//一手のみ
//		if (movenum == 1) {
//			for (int i(0); i < 8; ++i) {
//				for (int j(0);j < 8;++j) {
//					if (p_moves[i][j]) {
//						Board b2(b1);
//						b2.move_board(i, j);
//						double va(-neu[l].Forward(b1.x102())[0]);
//						if (va >= 0) {
//							return true;
//						}
//						else {
//							return false;
//						}
//					}
//				}
//			}
//		}
//
//		//二手以上
//		int n(0);
//		vector<double> v0(movenum);
//		vector<double> v(movenum);
//		double sumv(0);
//		for (int i(0); i < 8; ++i) {
//			for (int j(0);j < 8;++j) {
//				if (b1.moves[i][j]) {
//					Board b2(b1);
//					b2.move_board(i, j);
//					v0[n] = -neu[l + 1].Forward(b2.x102())[0];
//					v[n] = pow(base, v0[n]);
//					sumv += v[n];
//					++n;
//				}
//			}
//		}
//		double markv(0);
//		double rndpoint(rndp(sumv));
//		for (int i(0); i < movenum; ++i) {
//			markv += v[i];
//			if (markv >= rndpoint) {
//				if (v0[i] >= 0) {
//					return true;
//				}
//				else {
//					return false;
//				}
//			}
//		}
//	}
//
//	//1手のみ
//	if (movenum == 1) {
//		for (int i(0); i < 8; ++i) {
//			for (int j(0);j < 8;++j) {
//				if (p_moves[i][j]) {
//					Board b2(b1);
//					b2.move_board(i, j);
//					b2.calculate_moves();
//					b2.calculate_disks();
//					return !playoutbias(b2, neu);
//				}
//			}
//		}
//	}
//
//	//2手以上
//	int n(0);
//	vector<double> v(movenum);
//	vector<int> mi(movenum);
//	vector<int> mj(movenum);
//	double sumv(0);
//	for (int i(0); i < 8; ++i) {
//		for (int j(0);j < 8;++j) {
//			if (b1.moves[i][j]) {
//				mi[n] = i;
//				mj[n] = j;
//				Board b2(b1);
//				b2.move_board(i, j);
//				v[n] = pow(base, -neu[l + 1].Forward(b2.x102())[0]);
//				sumv += v[n];
//				++n;
//			}
//		}
//	}
//	double markv(0);
//	double rndpoint(rndp(sumv));
//	for (int i(0); i < movenum; ++i) {
//		markv += v[i];
//		if (markv >= rndpoint) {
//			Board b2(b1);
//			b2.move_board(mi[i], mj[i]);
//			b2.calculate_moves();
//			b2.calculate_disks();
//			return !playoutbias(b2, neu);
//		}
//	}
//	return false;
//}

//ある深さまでプレイアウト
//bool Node::playoutrand(Board &b1, Neuralnet(&neu)[65])
//{
//	int l(b1.disks1 + b1.disks2);
//	//終盤判断
//	int movenum(b1.move_num);
//	if (movenum == 0) {
//		b1.calculate_moves2();
//		if (b1.move_num2 == 0) {
//			if (b1.turn == 1) {
//				if (b1.disks1 - b1.disks2 > 0) {
//					return true;
//				}
//				else {
//					return false;
//				}
//			}
//			else {
//				if (b1.disks2 - b1.disks1 > 0) {
//					return true;
//				}
//				else {
//					return false;
//				}
//			}
//		}
//		Board b2(b1);
//		b2.turn = b1.turnnot;
//		b2.turnnot = b1.turn;
//		++b2.turnnum;
//		b2.calculate_moves();
//		b2.calculate_disks();
//		return !playoutrand(b2, neu);
//	}
//
//	//到達判断
//	if (l == level + depth) {
//		double va(neu[l].Forward(b1.x102())[0]);
//		if (va > 0) {
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//
//	double markv(0);
//	double rndpoint(rndp(movenum));
//	for (int i(0); i < 8; ++i) {
//		for (int j(0); j < 8; ++j) {
//			if (b1.moves[i][j]) {
//				markv += 1;
//				if (markv >= rndpoint) {
//					Board b2(b1);
//					b2.move_board(i, j);
//					b2.calculate_moves();
//					b2.calculate_disks();
//					return !playoutrand(b2, neu);
//				}
//			}
//		}
//	}
//	return false;
//}

//終盤までプレイアウトする
//bool Node::playendrand(Board &b1)
//{
//	int l(b1.disks1 + b1.disks2);
//	//終盤判断
//	int movenum(b1.move_num);
//	if (movenum == 0) {
//		b1.calculate_moves2();
//		if (b1.move_num2 == 0) {
//			if (b1.turn == 1) {
//				if (b1.disks1 - b1.disks2 > 0) {
//					return true;
//				}
//				else {
//					return false;
//				}
//			}
//			else {
//				if (b1.disks2 - b1.disks1 > 0) {
//					return true;
//				}
//				else {
//					return false;
//				}
//			}
//		}
//		Board b2(b1);
//		b2.turn = b1.turnnot;
//		b2.turnnot = b1.turn;
//		++b2.turnnum;
//		b2.calculate_moves();
//		b2.calculate_disks();
//		return !playendrand(b2);
//	}
//
//	double markv(0);
//	double rndpoint(rndp(movenum));
//	for (int i(0); i < 8; ++i) {
//		for (int j(0); j < 8; ++j) {
//			if (b1.moves[i][j]) {
//				markv += 1;
//				if (markv >= rndpoint) {
//					Board b2(b1);
//					b2.move_board(i, j);
//					b2.calculate_moves();
//					b2.calculate_disks();
//					return !playendrand(b2);
//				}
//			}
//		}
//	}
//	return false;
//}

inline double Node::rndp(double max)
{
	std::uniform_real_distribution<> randnum(0, max);
	return randnum(mt);
}

//void Node::search(Neuralnet(&neu)[65])
//{
//	//終端の場合
//	if (end) {
//		if (disksdif > 0) {
//			++wins;
//			wintrans = true;
//		}
//		else {
//			wintrans = false;
//		}
//		++visits;
//		return;
//	}
//	//枝の場合（進む）
//	if (!leaf) {
//		calQ();
//		(*pchildnode[maxQn]).search(neu);
//		if (!(*pchildnode[maxQn]).wintrans) {
//			++wins;
//			wintrans = true;
//		}
//		else {
//			wintrans = false;
//		}
//		++passes;
//		++visits;
//		return;
//	}
//
//	//リーフの場合（プレイアウト）
//	bool winorlose;
//	switch (policy_number) {
//	case 1:
//		winorlose = playoutbias(b, neu);
//		break;
//	case 2:
//		winorlose = playoutrand(b, neu);
//		break;
//	case 3:
//		winorlose = playendrand(b);
//		break;
//	}
//
//	if (winorlose) {
//		++wins;
//		wintrans = true;
//	}
//	else {
//		wintrans = false;
//	}
//	++visits;
//	if (visits > exlimit) {
//		expand();
//	}
//	return;
//}

//void RouteNode::searchbest(Neuralnet(&neu)[65])
//{
//	clock_t start(clock());
//	clock_t passing(clock());
//	expand();
//
//	//if (level > 52) {
//	//	vector<double> v(mvs);
//	//	vector<int> vi(mvs);
//	//	vector<int> vj(mvs);
//	//	int n(0);
//	//	for (int i(0);i < 8;++i) {
//	//		for (int j(0);j < 8;++j) {
//	//			if (b.moves[i][j]) {
//	//				Board b2(b);
//	//				b2.move_board(i, j);
//	//				v[n] = -FDO(b2);
//	//				vi[n] = i;
//	//				vj[n] = j;
//	//				++n;
//	//			}
//	//		}
//	//	}
//	//	bestn = distance(v.begin(), max_element(v.begin(), v.end()));	
//	//	besti = vi[bestn];
//	//	bestj = vj[bestn];
//	//	bestvalue = v[bestn];
//	//	return;
//	//}
//
//	if (level > 47) {
//		Abwd abwd;
//		bestvalue = abwd.net(b, neu);
//		besti = abwd.maxi;
//		bestj = abwd.maxj;
//		return;
//	}
//
//	while (passing - start < time) {
//		calQ();
//		(*pchildnode[maxQn]).search(neu);
//		if (!(*pchildnode[maxQn]).wintrans) {
//			++wins;
//		}
//		++passes;
//		++visits;
//		passing = clock();
//	}
//	vector<int> v(mvs, 0);
//	for (int i(0); i < mvs; ++i) {
//		v[i] = (*pchildnode[i]).visits;
//	}
//	bestn = distance(v.begin(), max_element(v.begin(), v.end()));
//	besti = (*pchildnode[bestn]).lastmovei;
//	bestj = (*pchildnode[bestn]).lastmovej;
//}

Play::Play()
	:play_folder("PLAY\\"),
	start(false),
	endgame(false),
	dep(12),
	perf(43)
{
	inputmove_file = play_folder + "inputmove.txt";
	outputmove_file = play_folder + "outputmove.txt";
	board_file = play_folder + "PlayingBoard.txt";
	record_file = "record.txt";
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
		std::cout << "入力エラー";
		return;
	}
	//初期判断
	std::string str;
	getline(ifs, str);
	if (stoi(str) == 0) {
		b.initialize();
		start = true;
	}
	else {
		start = false;
	}

	//先手後手
	getline(ifs, str);
	player = stoi(str);
	if (player == 1) {
		opponent = 2;
	}
	else {
		opponent = 1;
	}

	//盤面を読む
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
		std::cout << "入力エラー";
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
		std::cout << "入力エラー";
		return;
	}
	//初期判断
	std::string str;
	getline(ifs, str);
	if (stoi(str) == 0) {
		b.initialize();
		start = true;
	}
	else {
		start = false;
	}

	//先手後手
	getline(ifs, str);
	player = stoi(str);
	if (player == 1) {
		opponent = 2;
	}
	else {
		opponent = 1;
	}

	//盤面を読む
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
		neu[i].load(folder + "bw\\bw" + to_string(i) + ".csv");
	}
}

//void Play::run(string folder)
//{
//	if (player == 2) {
//		b.reverse();
//	}
//	if (start && player == 1) {
//		b.move_board(2, 3);
//		start = false;
//		return;
//	}
//	n0.b = b;
//	n0.init();
//
//	if (n0.end) {
//		endgame = true;
//		choseni = -1;
//		chosenj = -1;
//		if (player == 2) {
//			b.reverse();
//		}
//		start = false;
//		return;
//	}
//	if (n0.mvs == 0) {
//		choseni = -1;
//		chosenj = -1;
//		int temp(b.turn);
//		b.turn = b.turnnot;
//		b.turnnot = temp;
//		if (player == 2) {
//			b.reverse();
//		}
//		start = false;
//		return;
//	}
//
//	n0.searchbest(neu);
//	choseni = n0.besti;
//	chosenj = n0.bestj;
//	b.move_board(choseni, chosenj);
//	if (player == 2) {
//		b.reverse();
//	}
//	start = false;
//}

//void Play::runab0(string folder)
//{
//	clock_t start_time = clock();
//	b.calculate_disks();
//	level = b.disks1 + b.disks2;
//	b.calculate_moves();
//	move_choices = b.move_num;
//	bool(*p_moves)[8](b.moves);
//	if (player == 2) {
//		b.reverse();
//	}
//
//	for (int i(0); i < 65; ++i) {
//		depth[i] = 10;
//	}
//	for (int i(44); i < 65; ++i) {
//		depth[i] = 12;
//	}
//
//	//開始の一手
//	if (start && player == 1) {
//		choseni = 2;
//		chosenj = 3;
//	}
//
//	//ゲーム終了
//	else if (level == 64) {
//		endgame = true;
//		choseni = -1;
//		chosenj = -1;
//	}
//
//	//PASSの場合
//	else if (move_choices == 0) {
//		choseni = -1;
//		chosenj = -1;
//	}
//
//	//一手しかない場合
//	else if (move_choices == 1) {
//		for (int i(0); i < 8; ++i) {
//			for (int j(0);j < 8;++j) {
//				if (p_moves[i][j]) {
//					choseni = i;
//					chosenj = j;
//				}
//			}
//		}
//	}
//
//	//二手以上かつ44より大きい
//	else if (level > 44) {
//		Abwd abwd;
//		if (level > 50) {
//			abwd.elim_width = 99;
//		}
//		if (level > 48) {
//			abwd.elim_width = 99;
//		}
//		else if (level > 46) {
//			abwd.elim_width = 99;
//		}
//		else {
//			abwd.elim_width = 18;
//		}
//		bestval = abwd.end(b, neu);
//		choseni = abwd.maxi;
//		chosenj = abwd.maxj;
//	}
//
//	//二手以上
//	else {
//		Abwd abwd;
//		abwd.elim_width = elim_width;
//		abwd.mvs = move_choices;
//		abwd.disks = level;
//		bestval = abwd.absearch(b, neu, level + depth[level]);
//		choseni = abwd.maxi;
//		chosenj = abwd.maxj;
//	}
//
//	start = false;
//	clock_t end_time = clock();
//	time_consumed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
//
//
//	if (choseni >= 0) {
//		b.move_board(choseni, chosenj);
//	}
//	else {
//		int temp(b.turn);
//		b.turn = b.turnnot;
//		b.turnnot = temp;
//	}
//
//	if (player == 2) {
//		b.reverse();
//	}
//	return;
//}

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
		if (i > perf) {
			depth[i] = 99;
		}
		else if (i < 9 || i > 42) {
			depth[i] = dep + 2;
		}
		else {
			depth[i] = dep;
		}
	}

	//開始の一手
	if (start && player == 1) {
		//choseni = 2;
		//chosenj = 3;
		int dc[2];
		Random_choose rdc;
		rdc(dc, b.moves, b.move_num);
		choseni = dc[0];
		chosenj = dc[1];
	}

	//ゲーム終了
	else if (level == 64) {
		endgame = true;
		choseni = -1;
		chosenj = -1;
	}

	//PASSの場合
	else if (move_choices == 0) {
		bestval = -999;
		choseni = -1;
		chosenj = -1;
	}

	//一手しかない場合
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

	//二手以上
	else {
		vector<vector<double>> v(move_choices);
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

		//ソートする
		if (level < 55) {
			for (int n(0); n < move_choices; ++n) {
				Board b2(b);
				b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
				double y;
				if (neu[level + 1].nodes[0] == 266) {
					y = -neu[level + 1].Forward(b2.x266())[0];
				}
				else if (neu[level + 1].nodes[0] == 206) {
					y = -neu[level + 1].Forward(b2.x206())[0];
				}
				else if (neu[level + 1].nodes[0] == 102) {
					y = -neu[level + 1].Forward(b2.x102())[0];
				}
				v[n][0] = y;
			}
			sort(v.begin(), v.end());
			std::reverse(v.begin(), v.end());
		}

		bestval = -999;
		//下のノードへ
		for (int n(0);n < move_choices;++n) {
			Abwd abwd;
			Board b2(b);
			b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
			double nextval = -abwd.alphabeta(b2, neu, level, depth[level] - 1, -999, -bestval);
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
	book.initialize(level, 25);
	book.neu_folder = neu_folder;
	book.load();

	bool(*p_moves)[8](b.moves);
	if (player == 2) {
		b.reverse();
	}
	for (int i(0); i < 65; ++i) {
		if (i > perf) {
			depth[i] = 99;
		}
		else if (i < 9 || i > 42) {
			depth[i] = dep + 2;
		}
		else {
			depth[i] = dep;
		}
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
	//開始の一手
	if (start && player == 1) {
		int dc[2];
		Random_choose rdc;
		rdc(dc, b.moves, b.move_num);
		choseni = dc[0];
		chosenj = dc[1];
		start = false;
	}

	//ゲーム終了
	else if (level == 64) {
		endgame = true;
		choseni = -1;
		chosenj = -1;
	}

	//PASSの場合
	else if (move_choices == 0) {
		bestval = -999;
		choseni = -1;
		chosenj = -1;
		b.calculate_moves2();
		if (b.move_num2 == 0) {
			endgame = true;
		}
	}

	//一手しかない場合
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

	//二手以上
	else {
		vector<vector<double>> v(move_choices);
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

		//ソートする
		if (level < 55) {
			for (int n(0); n < move_choices; ++n) {
				Board b2(b);
				b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
				double y;
				if (neu[level + 1].nodes[0] == 266) {
					y = -neu[level + 1].Forward(b2.x266())[0];
				}
				else if (neu[level + 1].nodes[0] == 206) {
					y = -neu[level + 1].Forward(b2.x206())[0];
				}
				else if (neu[level + 1].nodes[0] == 102) {
					y = -neu[level + 1].Forward(b2.x102())[0];
				}
				v[n][0] = y;
			}
			std::sort(v.begin(), v.end());
			std::reverse(v.begin(), v.end());
		}

		bestval = -999;
		bool bookall(true);
		//下のノードへ
		for (int n(0);n < move_choices;++n) {
			Abwd abwd;
			Board b2(b);
			abwd.elim_width = elim_width;
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
			else {
				bookall = false;
			}
			if (!bookon) {
				if (level + depth[level] - 1 > 63) {
					nextval = -abwd.alphabetaend(b2, neu, level + 1, -999, -bestval);
				}
				else {
					nextval = -abwd.alphabeta(b2, neu, level + 1, depth[level] - 1, -999, -bestval);
				}
			}
			if (nextval > bestval) {
				bestval = nextval;
				choseni = int(v[n][1] + 0.5);
				chosenj = int(v[n][2] + 0.5);
			}
		}
		if (bookall) {
			double t_line[30];
			int t_maxd;
			int t_ij[2];
			loopbook(book, b, t_line, t_maxd, t_ij, level);
			bestval = t_line[t_maxd];
			choseni = t_ij[0];
			chosenj = t_ij[1];
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

void Play::loopbook(Book &book, Board b, double(&line)[30], int &maxd, int(&bestij)[2], int disks)
{
	int n(0);
	b.calculate_moves();
	double best(-999);
	double v[20][3];
	double a_line[20][30];
	int a_maxd[20];
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (b.moves[i][j]) {
				Board b2(b);
				b2.move_board(i, j);
				int num = book.number(disks + 1, b2.turn, b2.square);
				double up = book.upper(disks + 1, num);
				v[n][0] = -up;
				v[n][1] = i;
				v[n][2] = j;
				a_line[n][1] = -up;
				a_maxd[n] = 1;
				best = max(best, -up);
				++n;
			}
		}
	}

	bool end(false);
	for (int j(0); j < b.move_num; ++j) {
		if (!end && v[j][0] == best) {
			Board b2(b);
			b2.move_board(int(v[j][1] + 0.5), int(v[j][2] + 0.5));
			if (!followbook(book, b2, disks + 1)) {
				line[1] = v[j][0];
				maxd = 1;
				bestij[0] = int(v[j][1] + 0.5);
				bestij[1] = int(v[j][2] + 0.5);
				end = true;
			}
		}
	}

	if (!end) {
		for (int j(0); j < b.move_num; ++j) {
			Board b2(b);
			b2.move_board(int(v[j][1] + 0.5), int(v[j][2] + 0.5));
			if (followbook(book, b2, disks + 1)) {
				double temp_line[30];
				int temp_maxd;
				int temp_ij[2];
				loopbook(book, b2, temp_line, temp_maxd, temp_ij, disks + 1);
				a_maxd[j] = temp_maxd + 1;
				for (int k(1); k <= a_maxd[j]; ++k) {
					a_line[j][k + 1] = -temp_line[k];
				}
			}
		}
	}

	int i(2);
	while (!end) {
		best = -999;
		for (int j(0); j < b.move_num; ++j) {
			if (a_maxd[j] >= i) {
				best = max(best, a_line[j][i]);
			}
		}
		for (int j(0); j < b.move_num; ++j) {
			if (!end && best == a_line[j][i] && a_maxd[j] == i) {
				for (int k(1); k <= i; ++k) {
					line[k] = a_line[j][k];
				}
				maxd = i;
				bestij[0] = int(v[j][1] + 0.5);
				bestij[1] = int(v[j][2] + 0.5);
				end = true;
			}
		}
		++i;
	}
}

bool Play::followbook(Book &book, Board b, int disks)
{
	b.calculate_moves();
	bool bookall(true);
	for (int k(0); k < b.move_num; ++k) {
		for (int i(0);i < 8;++i) {
			for (int j(0);j < 8;++j) {
				if (b.moves[i][j]) {
					Board b2(b);
					b2.move_board(i, j);
					int num = book.number(disks + 1, b2.turn, b2.square);
					if (num < 0) {
						bookall = false;
					}
				}
			}
		}
	}
	return bookall;
}

//void Play::runabw()
//{
//	clock_t start_time = clock();
//	b.calculate_disks();
//	level = b.disks1 + b.disks2;
//	b.calculate_moves();
//	move_choices = b.move_num;
//	bool(*p_moves)[8](b.moves);
//	if (player == 2) {
//		b.reverse();
//	}
//
//	for (int i(0); i < 65; ++i) {
//		depth[i] = 8;
//	}
//	for (int i(42); i < 65; ++i) {
//		depth[i] = 12;
//	}
//	//for (int i(5); i < 7; ++i) {
//	//	depth[i] = 12;
//	//}
//
//	//開始の一手
//	if (start && player == 1) {
//		choseni = 2;
//		chosenj = 3;
//	}
//
//	//ゲーム終了
//	else if (level == 64) {
//		endgame = true;
//		choseni = -1;
//		chosenj = -1;
//	}
//
//	//45より大きい
//	else if (level > 45) {
//		Abwd abwd;
//		if (level > 50) {
//			abwd.elim_width = 999;
//		}
//		if (level > 48) {
//			abwd.elim_width = 999;
//		}
//		else if (level == 48) {
//			abwd.elim_width = 22;
//		}
//		else if (level == 47) {
//			abwd.elim_width = 15;
//		}
//		else if (level == 46) {
//			abwd.elim_width = 10;
//		}
//		//else if (level == 44) {
//		//	abwd.elim_width = 12;
//		//}
//		bestval = abwd.wend(b, neu);
//		choseni = abwd.maxi;
//		chosenj = abwd.maxj;
//	}
//
//	//44以下
//	else {
//		Abwd abwd;
//		abwd.elim_width = elim_width;
//		abwd.mvs = move_choices;
//		abwd.disks = level;
//		bestval = abwd.abws(b, neu, level + depth[level]);
//		choseni = abwd.maxi;
//		chosenj = abwd.maxj;
//	}
//
//	start = false;
//	clock_t end_time = clock();
//	time_consumed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
//
//
//	if (choseni >= 0) {
//		b.move_board(choseni, chosenj);
//	}
//	else {
//		int temp(b.turn);
//		b.turn = b.turnnot;
//		b.turnnot = temp;
//	}
//
//	if (player == 2) {
//		b.reverse();
//	}
//	return;
//}

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
	std::ifstream ifs("settings.txt");
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	//初期判断
	std::string str;
	getline(ifs, str);
	neu_folder = str + "\\";
}

void Play::display()
{
	cout << "  ";
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
	}
	cout << endl;
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
		for (int j(0); j < 8; ++j) {
			switch (b.square[i][j]) {
			case 1:
				cout << "＊";
				break;
			case 2:
				cout << "〇";
				break;
			default:
				cout << "―";
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
}