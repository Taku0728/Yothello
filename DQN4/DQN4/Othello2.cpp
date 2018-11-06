#include "stdafx.h" 
#include "othello.h"
#include "maths.h"


Board::Board()
	:disks1(0),
	disks2(0),
	rotation(0),
	reflection(0),
	turn(1),
	turnnot(2),
	turnnum(0),
	move_num(0),
	passes(0),
	end_game(false),
	pre_end_game(false),
	filename("board_output.csv"),
	spots_num(10, 0),
	spots_num2(10, 0),
	spots_moves1(10, 0),
	spots_moves2(10, 0)
{
	Fill(square, 0);
	Fill(moves, false);
	determined_move[0] = 0;
	determined_move[1] = 0;
	square[3][3] = 2;
	square[3][4] = 1;
	square[4][3] = 1;
	square[4][4] = 2;
	board_v.clear();
}

Board::~Board()
{
}

Choose::Choose()
{
}

Choose::~Choose()
{
}

void Choose::operator()(int(&determined_move)[2], const bool(&moves)[8][8], const int &move_num)
{
	if (move_num == 0) {
		return;
	}
	int number(0);
	std::random_device rnd;
	//	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> randnum(1, move_num);
	//	number = randnum(mt);
	number = randnum(rnd);

	int n(0);
	int i(-1);
	int j(-1);
	while ((i < 7) && (n < number)) {
		++i;
		j = -1;
		while ((j < 7) && (n < number)) {
			++j;
			if (moves[i][j]) {
				++n;
			}
		}
	}
	determined_move[0] = i;
	determined_move[1] = j;
}

Policy_choose::Policy_choose()
{
}

Policy_choose::~Policy_choose()
{
}

void Policy_choose::operator()(Neuralnet ne, int(&determined_move)[2], const bool(&moves)[8][8], const int &move_num)
{

}

Split::Split()
{
}

Split::~Split()
{
}

Random_choose::Random_choose()
{
}

Random_choose::~Random_choose()
{
}

inline void Random_choose::operator()(int(&determined_move)[2], const bool(&moves)[8][8], const int &move_num)
{
	if (move_num == 0) {
		return;
	}
	int number(0);
	std::random_device rnd;
	//	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> randnum(1, move_num);
	//	number = randnum(mt);
	number = randnum(rnd);

	int n(0);
	int i(-1);
	int j(-1);
	while ((i < 7) && (n < number)) {
		++i;
		j = -1;
		while ((j < 7) && (n < number)) {
			++j;
			if (moves[i][j]) {
				++n;
			}
		}
	}
	determined_move[0] = i;
	determined_move[1] = j;
}

//盤面に代入
inline void Board::set(const int(&squ)[8][8])
{
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			square[i][j] = squ[i][j];
		}
	}
}

//盤面を回転正規化
void Board::rotate()
{
	//４分盤面それぞれの回転正規評価
	std::vector<int> val(4, 0);
	for (int i = 0;i < 4;++i) {
		for (int j = 0;j < 4;++j) {
			val[0] += int(pow(square[i][j], 4) * pow(2, (i * 8 + j)));
			val[1] += int(pow(square[j][7 - i], 4) * pow(2, j * 8 + i));
			val[2] += int(pow(square[7 - i][7 - j], 4) * pow(2, i * 8 + j));
			val[3] += int(pow(square[7 - j][i], 4) * pow(2, j * 8 + i));
		}
	}

	//回転数を決定
	int squtemp[8][8];
	Fill(squtemp, 0);
	switch (std::distance(val.begin(), std::max_element(val.begin(), val.end()))) {
	case 0:
		rotation = 0;
		break;
	case 1:
		rotation = 1;
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[7 - j][i];
			}
		}
		set(squtemp);
		break;
	case 2:
		rotation = 2;
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[7 - i][7 - j];
			}
		}
		set(squtemp);
		break;
	case 3:
		rotation = 3;
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[j][7 - i];
			}
		}
		set(squtemp);
		break;
	}
}

//標準ボードを回転正規化
void Board::simp_rotate()
{
	//４分対角盤面の回転正規評価し、回転数を決定
	int i = 0;
	int j = 0;
	int con = 1;
	while ((i < 4) && (con == 1)) {
		while ((j < 4) && (con == 1)) {
			if (square[i][j] > square[7 - i][7 - j]) {
				rotation = 0;
				con = 0;
			}
			else if (square[i][j] < square[7 - i][7 - j]) {
				rotation = 1;
				con = 0;
			}
			++j;
		}
		++i;
		j = 0;
	}
	if (con == 1) {
		rotation = 0;
	}

	//盤面を回転

	if (rotation == 1) {
		int squtemp[8][8];
		Fill(squtemp, 0);
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[7 - i][7 - j];
			}
		}
		set(squtemp);
	}
};

//盤面を反転正規化
void Board::reflect()
{
	//対角２分盤面により反転数を決定
	int i = 0;
	int j = 0;
	int con = 1;
	while ((i + j < 8) && (con == 1)) {
		while ((i + j < 8) && (con == 1)) {
			if (square[i][j] - square[7 - j][7 - i] > 0) {
				reflection = 0;
				con = 0;
			}
			else if (square[i][j] - square[7 - j][7 - i] < 0) {
				reflection = 1;
				con = 0;
			}
			++j;
		}
		++i;
	}
	if (con == 1) {
		reflection = 0;
	}

	//盤面を反転
	if (reflection == 1) {
		int squtemp[8][8];
		Fill(squtemp, 0);
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[7 - j][7 - i];
			}
		}
		set(squtemp);
	}

};

//盤面反転を元に戻す
void Board::back_reflect()
{
	if (reflection == 1) {
		int squtemp[8][8];
		Fill(squtemp, 0);
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[7 - j][7 - i];
			}
		}
		set(squtemp);
	}
	reflection = 0;
}

//盤面回転を元に戻す
void Board::back_rotate()
{
	int squtemp[8][8];
	Fill(squtemp, 0);
	switch (rotation) {
	case 0:
		break;
	case 1:
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[j][7 - i];
			}
		}
		set(squtemp);
		break;
	case 2:
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[7 - i][7 - j];
			}
		}
		set(squtemp);
		break;
	case 3:
		for (int i = 0;i < 8;++i) {
			for (int j = 0;j < 8;++j) {
				squtemp[i][j] = square[7 - j][i];
			}
		}
		set(squtemp);
		break;
	}
	rotation = 0;
}

//初期盤面作成
void Board::initialize()
{
	rotation = 0;
	reflection = 0;
	turn = 1;
	turnnot = 2;
	turnnum = 0;
	move_num = 0;
	passes = 0;
	end_game = false;
	Fill(square, 0);
	Fill(moves, false);
	determined_move[0] = 0;
	determined_move[1] = 0;
	square[3][3] = 2;
	square[3][4] = 1;
	square[4][3] = 1;
	square[4][4] = 2;
}

//すべての手を計算
void Board::calculate_moves()
{
	Fill(moves, 0);
	move_num = 0;
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (right_move(i, j)) {
				moves[i][j] = true;
				move_num += 1;
			}
		}
	}
	determined_move[0] = -1;
	determined_move[1] = -1;
}

void Board::calculate_moves2()
{
	Fill(moves2, 0);
	move_num2 = 0;
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (right_move2(i, j)) {
				moves2[i][j] = true;
				move_num2 += 1;
			}
		}
	}
}
//手の正しさを判定
bool Board::right_move(int i, int j) const {
	if (square[i][j] != 0) {
		return false;
	}
	for (int direction = 1;direction < 9;++direction) {
		if (right_direction(i, j, direction) == true) {
			return true;
		}
	}
	return false;
}

bool Board::right_move2(int i, int j) const {
	if (square[i][j] != 0) {
		return false;
	}
	for (int direction = 1;direction < 9;++direction) {
		if (right_direction2(i, j, direction) == true) {
			return true;
		}
	}
	return false;
}

//ある方向について手の正しさを判定
inline bool Board::right_direction(int i, int j, int direction) const
{
	int di(0);
	int dj(0);
	switch (direction) {
	case 1:
		di = -1;
		dj = 0;
		break;
	case 2:
		di = -1;
		dj = 1;
		break;
	case 3:
		di = 0;
		dj = 1;
		break;
	case 4:
		di = 1;
		dj = 1;
		break;
	case 5:
		di = 1;
		dj = 0;
		break;
	case 6:
		di = 1;
		dj = -1;
		break;
	case 7:
		di = 0;
		dj = -1;
		break;
	case 8:
		di = -1;
		dj = -1;
		break;
	}

	int ni(i + di);
	int nj(j + dj);
	if ((ni < 0) || (nj < 0) || (ni > 7) || (nj > 7) || (square[ni][nj] != turnnot)) {
		return false;
	}
	ni += di;
	nj += dj;
	bool right(false);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (square[ni][nj] == 0) {
			return false;
		}
		else if (square[ni][nj] == turn) {
			return true;
		}
		ni += di;
		nj += dj;
	}
	return false;
}

inline bool Board::right_direction2(int i, int j, int direction) const
{
	int di(0);
	int dj(0);
	switch (direction) {
	case 1:
		di = -1;
		dj = 0;
		break;
	case 2:
		di = -1;
		dj = 1;
		break;
	case 3:
		di = 0;
		dj = 1;
		break;
	case 4:
		di = 1;
		dj = 1;
		break;
	case 5:
		di = 1;
		dj = 0;
		break;
	case 6:
		di = 1;
		dj = -1;
		break;
	case 7:
		di = 0;
		dj = -1;
		break;
	case 8:
		di = -1;
		dj = -1;
		break;
	}

	int ni(i + di);
	int nj(j + dj);
	if ((ni < 0) || (nj < 0) || (ni > 7) || (nj > 7) || (square[ni][nj] != turn)) {
		return false;
	}
	ni += di;
	nj += dj;
	bool right(false);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (square[ni][nj] == 0) {
			return false;
		}
		else if (square[ni][nj] == turnnot) {
			return true;
		}
		ni += di;
		nj += dj;
	}
	return false;
}

//盤面ベクトルをファイルから入力
void Board::read_board_v()
{
	//ファイルの読み込み
	std::ifstream ifs("Readt.txt");
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}

	//csvファイルを1行ずつ読み込む
	std::string str;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);

		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			board_v.emplace_back(std::stoi(token));
		}
	}
	set_board_v(board_v);
}

//盤面を１次ベクトル出力
inline void Board::write_board_9() const
{
	std::vector<int> boa(9, 0);
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			boa[i] += int(square[i][j] * pow(2, j * 2));
		}
	}
	std::ofstream ofs("Test.txt");
	for (int i = 0;i < 8;++i) {
		std::cout << boa[i] << ",";
	}
	std::cout << turnnum << ",";
	std::cout << turn << ",";
	std::cout << std::endl;
}

inline void Board::write_board_v() const
{
	std::ofstream writing_file;
	writing_file.open("Test.txt", std::ios::app);
	for (int i = 0;i < 8;++i) {
		writing_file << board_v[i] << ",";
	}
}

//盤面を２次元視覚出力
void Board::write_board_full() const
{
	std::string filename = "BOARD.txt";
	std::ofstream writing_file;
	//	writing_file.open(filename, std::ios::app);
	writing_file.open(filename, std::ios::out);
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			writing_file << square[i][j] << ",";
		}
		writing_file << std::endl;
	}
	writing_file << "Turns: " << turnnum << std::endl;
	writing_file << "Turn: " << turn << std::endl;
	writing_file << "Last move: " << determined_move[0] << "," << determined_move[1] << std::endl;
	writing_file << std::endl;
}

inline void Board::write_moves() const
{
	std::string filename = "Test.txt";
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			writing_file << moves[i][j] << ",";
		}
		writing_file << std::endl;
	}
	writing_file << "Turns: " << turnnum << std::endl;
	writing_file << "Turn: " << turn << std::endl;
	writing_file << "Wait: " << turnnot << std::endl;
	writing_file << "Right moves: " << move_num << std::endl;
	writing_file << "Right direction(2,3,5): " << right_direction(2, 3, 5) << std::endl;
	writing_file << std::endl;
}

//次の手をランダムに選ぶ
void Board::random_move()
{
	if (move_num == 0) {
		return;
	}
	int number(0);
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> randnum(0, move_num);
	number = randnum(mt);

	int n(0);
	int i(-1);
	int j(-1);
	while ((i < 7) && (n < number)) {
		++i;
		j = -1;
		while ((j < 7) && (n < number)) {
			++j;
			if (moves[i][j]) {
				++n;
			}
		}
	}
	determined_move[0] = i;
	determined_move[1] = j;
}

//次の盤面に変更
void Board::move_board(Choose &ch)
{
	ch(determined_move, moves, move_num);
	if (end_game == true) {
		return;
	}
	if (move_num == 0) {
		if (pre_end_game == true) {
			--turnnum;
			--passes;
			end_game = true;
			return;
		}
		else {
			++passes;
			pre_end_game = true;
		}
	}
	else {
		pre_end_game = false;
		int i(determined_move[0]);
		int	j(determined_move[1]);
		square[i][j] = turn;
		for (int direction(1); direction < 9;++direction) {
			move_board_direction(i, j, direction);
		}
	}
	int temp(turn);
	turn = turnnot;
	turnnot = temp;
	++turnnum;
}

void Board::move_board(const int i, const int j)
{
	square[i][j] = turn;
	for (int direction(1); direction < 9;++direction) {
		move_board_direction(i, j, direction);
	}
	int temp(turn);
	turn = turnnot;
	turnnot = temp;
	++turnnum;
}

vector<int> Board::move_board_v(const vector<int> &v, const int j, const int k, const int turn1)
{
	//if (j < 0 || k < 0 || turn1 < 1 || j>7 || k>7 || turn1>2) {
	//	return;
	//}
	for (int n(0);n < 8;++n) {
		for (int m(0); m < 8;++m) {
			square[n][m] = v[8 * n + m];
		}
	}
	turn = turn1;
	if (turn == 1) {
		turnnot = 2;
	}
	else {
		turnnot = 1;
	}
	pre_end_game = false;
	square[j][k] = turn1;
	for (int direction(1); direction < 9;++direction) {
		move_board_direction(j, k, direction);
	}
	vector<int> tempv(64, 0);
	for (int n(0);n < 8;++n) {
		for (int m(0); m < 8;++m) {
			tempv[8 * n + m] = square[n][m];
		}
	}
	return tempv;
}

//ある方向について次の盤面に変更
inline void Board::move_board_direction(int i, int j, int direction)
{
	int di(0);
	int dj(0);
	switch (direction) {
	case 1:
		di = -1;
		dj = 0;
		break;
	case 2:
		di = -1;
		dj = 1;
		break;
	case 3:
		di = 0;
		dj = 1;
		break;
	case 4:
		di = 1;
		dj = 1;
		break;
	case 5:
		di = 1;
		dj = 0;
		break;
	case 6:
		di = 1;
		dj = -1;
		break;
	case 7:
		di = 0;
		dj = -1;
		break;
	case 8:
		di = -1;
		dj = -1;
		break;
	}

	int ni(i + di);
	int nj(j + dj);

	if ((ni < 0) || (nj < 0) || (ni > 7) || (nj > 7) || (square[ni][nj] != turnnot)) {
		return;
	}
	ni += di;
	nj += dj;
	bool con(true);
	bool right(false);
	int number(0);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8) && con) {
		if (square[ni][nj] == 0) {
			con = false;
		}
		else if (square[ni][nj] == turn) {
			con = false;
			right = true;
		}
		++number;
		ni += di;
		nj += dj;
	}
	if (right) {
		for (int m(1);m <= number;++m) {
			square[i + m*di][j + m*dj] = turn;
		}
	}
}

inline bool Board::get_end_game() const {
	return end_game;
}

//評価関数計算
inline void Board::calculate_disks()
{
	disks1 = 0;
	disks2 = 0;
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (square[i][j] == 1) {
				++disks1;
			}
			else if (square[i][j] == 2) {
				++disks2;
			}
		}
	}
}

void Board::calculate_spots() {
	spots1.resize(10, 0);
	spots2.resize(10, 0);
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	for (int n(0);n < 4;++n) {
		for (int m(0); m < 4; ++m) {
			switch (square[i][j + m * dj])
			{
			case 1:
				++spots1[m];
				break;
			case 2:
				++spots2[m];
				break;
			}
		}
		for (int m(1); m < 4; ++m) {
			switch (square[i + m * di][j])
			{
			case 1:
				++spots1[m];
				break;
			case 2:
				++spots2[m];
				break;
			}
		}
		for (int m(1); m < 4; ++m) {
			switch (square[i + di][j + m * dj])
			{
			case 1:
				++spots1[3 + m];
				break;
			case 2:
				++spots2[3 + m];
				break;
			}
		}
		for (int m(2); m < 4; ++m) {
			switch (square[i + m * di][j + dj])
			{
			case 1:
				++spots1[3 + m];
				break;
			case 2:
				++spots2[3 + m];
				break;
			}
		}
		for (int m(2); m < 4; ++m) {
			switch (square[i + 2 * di][j + m * dj])
			{
			case 1:
				++spots1[5 + m];
				break;
			case 2:
				++spots2[5 + m];
				break;
			}
		}
		switch (square[i + 3 * di][j + 2 * dj])
		{
		case 1:
			++spots1[8];
			break;
		case 2:
			++spots2[8];
			break;
		}
		switch (square[i + 3 * di][j + 3 * dj])
		{
		case 1:
			++spots1[9];
			break;
		case 2:
			++spots2[9];
			break;
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}

}

void Board::calculate_openness()
{
	spots_openness1.resize(9, 0);
	spots_openness2.resize(9, 0);
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	for (int n(0);n < 4;++n) {
		for (int m(0); m < 4; ++m) {
			if (square[i][j + m * dj] == 0) {
				openness(i, j + m, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (square[i + m * di][j] == 0) {
				openness(i + m, j, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (square[i + di][j + m * dj] == 0) {
				openness(i + 1, j + m, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (square[i + m * di][j + dj] == 0) {
				openness(i + m, j + 1, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (square[i + 2 * di][j + m * dj] == 0) {
				openness(i + 2, j + m, 5 + m);
			}
		}
		if (square[i + 3 * di][j + 2 * dj] == 0) {
			openness(i + 3, j + 2, 8);
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_spots_moves()
{
	calculate_moves();
	calculate_moves2();
	spots_moves1.resize(9, 0);
	spots_moves2.resize(9, 0);
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	for (int n(0);n < 4;++n) {
		for (int m(0); m < 4; ++m) {
			if (moves[i][j + m * dj])
			{
				++spots_moves1[m];
			}
			if (moves2[i][j + m * dj])
			{
				++spots_moves2[m];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (moves[i + m * di][j])
			{
				++spots_moves1[m];
			}
			if (moves2[i + m * di][j])
			{
				++spots_moves2[m];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (moves[i + di][j + m * dj])
			{
				++spots_moves1[3 + m];
			}
			if (moves2[i + di][j + m * dj])
			{
				++spots_moves2[3 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (moves[i + m * di][j + dj])
			{
				++spots_moves1[3 + m];
			}
			if (moves2[i + m * di][j + dj])
			{
				++spots_moves2[3 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (moves[i + 2 * di][j + m * dj])
			{
				++spots_moves1[5 + m];
			}
			if (moves2[i + 2 * di][j + m * dj])
			{
				++spots_moves2[5 + m];
			}
		}
		if (moves[i + 3 * di][j + 2 * dj])
		{
			++spots_moves1[8];
		}
		if (moves2[i + 3 * di][j + 2 * dj])
		{
			++spots_moves2[8];
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_spots_fixed()
{
	spots_fixed1.resize(9, 0);
	spots_fixed2.resize(9, 0);
	calculate_fixed();
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	for (int n(0);n < 4;++n) {
		for (int m(1); m < 4; ++m) {
			if (fixed1_m[i][j + m * dj])
			{
				++spots_fixed1[m - 1];
			}
			if (fixed2_m[i][j + m * dj])
			{
				++spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (fixed1_m[i + m * di][j])
			{
				++spots_fixed1[m - 1];
			}
			if (fixed2_m[i + m * di][j])
			{
				++spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (fixed1_m[i + di][j + m * dj])
			{
				++spots_fixed1[2 + m];
			}
			if (fixed2_m[i + di][j + m * dj])
			{
				++spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (fixed1_m[i + m * di][j + dj])
			{
				++spots_fixed1[2 + m];
			}
			if (fixed2_m[i + m * di][j + dj])
			{
				++spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (fixed1_m[i + 2 * di][j + m * dj])
			{
				++spots_fixed1[4 + m];
			}
			if (fixed2_m[i + 2 * di][j + m * dj])
			{
				++spots_fixed2[4 + m];
			}
		}
		if (fixed1_m[i + 3 * di][j + 2 * dj])
		{
			++spots_fixed1[7];
		}
		if (fixed2_m[i + 3 * di][j + 2 * dj])
		{
			++spots_fixed2[7];
		}
		if (fixed1_m[i + 3 * di][j + 3 * dj])
		{
			++spots_fixed1[8];
		}
		if (fixed2_m[i + 3 * di][j + 3 * dj])
		{
			++spots_fixed2[8];
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_fixed()
{
	fixed1 = 0;
	fixed2 = 0;
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			fixed1_m[i][j] = false;
			fixed2_m[i][j] = false;
		}
	}

	int i(0);
	int j(0);
	int n(0);
	while (n < 4) {
		while (j < 7 - n) {
			fixed_change(i, j);
			++j;
		}
		while (i < 7 - n) {
			fixed_change(i, j);
			++i;
		}
		while (j > n) {
			fixed_change(i, j);
			--j;
		}
		while (i > n) {
			fixed_change(i, j);
			--i;
		}
		while (i < 7 - n) {
			fixed_change(i, j);
			++i;
		}
		while (j < 7 - n) {
			fixed_change(i, j);
			++j;
		}
		while (i > n) {
			fixed_change(i, j);
			--i;
		}
		while (j > n) {
			fixed_change(i, j);
			--j;
		}
		++n;
		i = n;
		j = n;
	}
}

void Board::fixed_change(int i, int j) {
	if (fixed(i, j)) {
		switch (square[i][j])
		{
		case 1:
			fixed1_m[i][j] = true;
			++fixed1;
			break;
		case 2:
			fixed2_m[i][j] = true;
			++fixed2;
			break;
		default:
			break;
		}
	}
}

bool Board::fixed(int i, int j) const
{
	if (square[i][j] == 0) {
		return false;
	}
	bool b[8];
	for (int n(1);n < 9;++n) {
		b[n - 1] = fixed_direction(i, j, n);
	}
	bool b2[4];
	for (int l(0);l < 4;++l) {
		b2[l] = fixed_line(i, j, l);
	}
	if ((b[0] || b[4] || b2[0]) && (b[1] || b[5] || b2[1]) && (b[2] || b[6] || b2[2]) && (b[3] || b[7]) || b2[3]) {
		return true;
	}
	else {
		return false;
	}
}

bool Board::fixed_line(int i, int j, int l) const
{
	int di(0);
	int dj(0);
	switch (l) {
	case 0:
		di = -1;
		dj = 0;
		break;
	case 1:
		di = -1;
		dj = 1;
		break;
	case 2:
		di = 0;
		dj = 1;
		break;
	case 3:
		di = 1;
		dj = 1;
		break;
	}
	int ni(i + di);
	int nj(j + dj);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (square[ni][nj] == 0) {
			return false;
		}
		ni += di;
		nj += dj;
	}
	ni = i - di;
	nj = j - dj;
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (square[ni][nj] == 0) {
			return false;
		}
		ni -= di;
		nj -= dj;
	}
	return true;
}

bool Board::fixed_direction(int i, int j, int n) const
{
	int color(square[i][j]);
	int di(0);
	int dj(0);
	switch (n) {
	case 1:
		di = -1;
		dj = 0;
		break;
	case 2:
		di = -1;
		dj = 1;
		break;
	case 3:
		di = 0;
		dj = 1;
		break;
	case 4:
		di = 1;
		dj = 1;
		break;
	case 5:
		di = 1;
		dj = 0;
		break;
	case 6:
		di = 1;
		dj = -1;
		break;
	case 7:
		di = 0;
		dj = -1;
		break;
	case 8:
		di = -1;
		dj = -1;
		break;
	}

	int ni(i + di);
	int nj(j + dj);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		switch (color)
		{
		case 1:
			if (!fixed1_m[ni][nj]) {
				return false;
			}
			break;
		case 2:
			if (!fixed2_m[ni][nj]) {
				return false;
			}
			break;
		default:
			break;
		}
		ni += di;
		nj += dj;
	}
	return true;
}

void Board::openness(int i, int j, int k) {
	int i1;
	int i2;
	int j1;
	int j2;
	if (i - 1 < 0) {
		i1 = 0;
	}
	else {
		i1 = i - 1;
	}
	if (j - 1 < 0) {
		j1 = 0;
	}
	else {
		j1 = j - 1;
	}
	if (i + 1 > 7) {
		i2 = 7;
	}
	else {
		i2 = i + 1;
	}
	if (j + 1 > 7) {
		j2 = 7;
	}
	else {
		j2 = j + 1;
	}
	for (int n(i1);n <= i2;++n) {
		for (int m(j1);m < j2;++m) {
			if (square[n][m] == 1) {
				++spots_openness1[k];
			}
			else if (square[n][m] == 2) {
				++spots_openness2[k];
			}
		}
	}
}

inline int Board::get_turn() const
{
	return turn;
}

inline int Board::get_turn_num() const
{
	return turnnum;
}

inline int Board::get_move_num() const
{
	return move_num;
}

int Board::get_win()
{
	calculate_disks();
	if (disks1 > disks2) {
		return 1;
	}
	else if (disks1 < disks2) {
		return 2;
	}
	else {
		return 0;
	}
}
//盤面を渡す
std::vector<int> Board::get_board_v8() const
{
	std::vector<int> boa(10, 0);
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			boa[i] += int(square[i][j] * pow(2, j * 2));
		}
	}
	boa[8] = turn;
	boa[9] = turnnum;
	return boa;
}

std::vector<int> Board::get_board_v() const
{
	std::vector<int> boa(66, 0);
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			boa[8 * i + j] = square[i][j];
		}
	}
	boa[64] = turn;
	boa[65] = turnnum;
	return boa;
}

void Board::playerturnboard()
{
	if (turn == 2) {
		for (int i(0);i < 8;++i) {
			for (int j(0);j < 8;++j) {
				switch (square[i][j]) {
				case 1:
					square[i][j] = 2;
					break;
				case 2:
					square[i][j] = 1;
					break;
				}
			}
		}
	}
	turn = 1;
}

//盤面を入力
void Board::set_board_v(std::vector<int> &v)
{
	for (int i = 0;i < 8;++i) {
		for (int j = 7;j >= 0;--j) {
			square[j][i] = int(v[i] / pow(2, j * 2));
			v[i] -= int(square[j][i] * pow(2, j * 2));
		}
	}
	turn = v[8];
	turnnum = v[9];
	move_num = v[10];
	determined_move[0] = v[11];
	determined_move[1] = v[12];
	win = v[13];
}

Game::Game()
	:win(0),
	turns(0),
	elim_err(false),
	choices(1),
	elim_choices(1)
{
	std::vector<std::vector<int>> game_matrix(1, std::vector<int>(1, -1));
}

Game::~Game()
{
}

inline void Game::reverse(int i)
{
	for (int j(0);j < 65;++j) {
		switch (game_matrix[i][j]) {
		case 1:
			game_matrix[i][j] = 2;
			break;
		case 2:
			game_matrix[i][j] = 1;
			break;
		}
	}
	switch (game_matrix[i][69]) {
	case 1:
		game_matrix[i][69] = 2;
		break;
	case 2:
		game_matrix[i][69] = 1;
		break;
	}
}

inline void Game::reverse5x(int i)
{
	vector<int> v(7, 0);
	if (game_matrix[i][0] == 2) {
		v[0] = 1;
		v[1] = game_matrix[i][1];
		v[2] = game_matrix[i][3];
		v[3] = game_matrix[i][2];
		v[4] = game_matrix[i][5];
		v[5] = game_matrix[i][4];
		switch (game_matrix[i][6]) {
		case 1:
			v[6] = 2;
		case 2:
			v[6] = 1;
		}
		game_matrix[i] = v;
	}
}

void Game::playgame(Choose &ch1, Choose &ch2)
{
	Board boa;
	int i(0);
	boa.calculate_moves();
	game_matrix.reserve(80);
	std::vector<int> tempv(boa.get_board_v());
	while ((i < 80) && (!boa.get_end_game())) {
		++i;
		boa.calculate_moves();
		if (boa.get_turn() == 1) {
			boa.move_board(ch1);
		}
		else {
			boa.move_board(ch2);
		}
		tempv.emplace_back(boa.get_move_num());
		tempv.emplace_back(boa.determined_move[0]);
		tempv.emplace_back(boa.determined_move[1]);
		game_matrix.emplace_back(tempv);
		//game_matrix[i].shrink_to_fit();
		tempv = boa.get_board_v();
	}
	win = boa.get_win();
	turns = boa.get_turn_num();
	for (int i = 0;i < boa.get_turn_num() + 1;++i) {
		game_matrix[i].emplace_back(win);
	}
	game_matrix.shrink_to_fit();
}

void Game::playto(Board &boa, Choose &ch1, Choose &ch2, int level)
{
	int i(0);
	boa.calculate_moves();
	boa.calculate_disks();
	std::vector<int> tempv(boa.get_board_v());
	while (((boa.disks1 + boa.disks2) < level) && (!boa.get_end_game())) {
		++i;
		boa.calculate_moves();
		if (boa.get_turn() == 1) {
			boa.move_board(ch1);
		}
		else {
			boa.move_board(ch2);
		}
	}
}

//void Game::playout(Board &boa, int n)
//{
//	Reverse rev;
//	Random_choose rdc;
//	disks = 0;
//	playoutb.reserve(70);
//	boa.calculate_disks();
//	while ((!boa.get_end_game()) && (boa.disks1 + boa.disks2) < n) {
//		boa.calculate_moves();
//		boa.move_board(rdc);
//		boa.calculate_disks();
//	}
//	int turn(boa.get_turn());
//	vector<int> v(boa.get_board_v());
//	boa.calculate_disks();
//	disks = boa.disks1 + boa.disks2;
//	vector<double> x;
//	for (int i(0);i < v.size();++i) {
//		x[i] = double(v[i]);
//	}
//	playoutb = x;
//	if (disks == 64) {
//		playoutb.emplace_back(boa.get_win());
//	}
//	else {
//		Winout wo;
//		playoutb.emplace_back(wo(boa));
//	}
//	if (turn == 2) {
//		rev(playoutb);
//	}
//	playoutb.shrink_to_fit();
//}

void Game::playoutvalue(Board &boa, int n)
{
	Reverse2 rev;
	Random_choose rdc;
	disks = 0;
	playoutb.reserve(70);
	boa.calculate_disks();
	while ((!boa.get_end_game()) && (boa.disks1 + boa.disks2) < n) {
		boa.calculate_moves();
		boa.move_board(rdc);
		boa.calculate_disks();
	}
	int turn(boa.get_turn());
	vector<int> v(boa.get_board_v());
	disks = boa.disks1 + boa.disks2;
	if (turn == 2) {
		rev(v);
	}
	vector<double> x(v.size());
	for (unsigned int i(0);i < v.size();++i) {
		x[i] = double(v[i]);
	}
	playoutb = x;
	if (disks == 64) {
		if (turn == 2) {
			playoutb.emplace_back(boa.disks2 - boa.disks1);
		}
		else {
			playoutb.emplace_back(boa.disks1 - boa.disks2);
		}
	}
	else {
		Difout dif;
		playoutb.emplace_back(dif(boa));
	}
	playoutb.shrink_to_fit();
}

void Game::playoutvalue74(Board &boa, int n)
{
	Reverse2 rev;
	Random_choose rdc;
	disks = 0;
	playoutb.reserve(80);
	boa.calculate_disks();
	while ((!boa.get_end_game()) && (boa.disks1 + boa.disks2) < n) {
		boa.calculate_moves();
		boa.move_board(rdc);
		boa.calculate_disks();
	}
	disks = boa.disks1 + boa.disks2;
	if (disks != n) {
		return;
	}
	int turn(boa.get_turn());
	playoutb = boa.x74();
	if (disks == 64) {
		if (turn == 2) {
			playoutb.emplace_back(boa.disks2 - boa.disks1);
		}
		else {
			playoutb.emplace_back(boa.disks1 - boa.disks2);
		}
	}
	else {
		Difout dif;
		playoutb.emplace_back(dif(boa));
	}
	playoutb.shrink_to_fit();
}

void Game::playoutvalue74test(Board &boa, int n, Neuralnet &neu, const double w)
{
	Reverse2 rev;
	Random_choose rdc;
	disks = 0;
	playoutb.reserve(80);
	boa.calculate_disks();
	while ((!boa.get_end_game()) && (boa.disks1 + boa.disks2) < n) {
		boa.calculate_moves();
		boa.move_board(rdc);
		boa.calculate_disks();
	}
	disks = boa.disks1 + boa.disks2;
	if (disks != n) {
		return;
	}
	int turn(boa.get_turn());
	playoutb = boa.x74();
	if (disks == 64) {
		if (turn == 2) {
			playoutb.emplace_back(boa.disks2 - boa.disks1);
		}
		else {
			playoutb.emplace_back(boa.disks1 - boa.disks2);
		}
	}
	else {
		Difout dif;
		double ma(dif(boa));
		max_abs = abs(ma);
		playoutb.emplace_back(ma);
		if (boa.move_num > 1) {
			choices = boa.move_num;
			Board boa2(boa);
			Elim elim;
			elim(boa2, neu, w);
			//cout << dif.maxi << "," << dif.maxj << "," << dif.maxn << "," << boa.move_num << "," << dif.max << endl;
			if (!boa2.moves[dif.maxi][dif.maxj]) {
				elim_err = true;
			}
			elim_choices = boa2.move_num;
		}
	}
	playoutb.shrink_to_fit();
}

void Game::playoutvalue74wd(Board &boa, int n, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65])
{
	Reverse2 rev;
	Random_choose rdc;
	disks = 0;
	playoutb.reserve(80);
	boa.calculate_disks();
	while ((!boa.get_end_game()) && (boa.disks1 + boa.disks2) < n) {
		boa.calculate_moves();
		boa.move_board(rdc);
		boa.calculate_disks();
	}
	disks = boa.disks1 + boa.disks2;
	if (disks != n) {
		return;
	}
	int turn(boa.get_turn());
	playoutb = boa.x74();
	if (disks == 64) {
		if (turn == 2) {
			playoutb.emplace_back(boa.disks2 - boa.disks1);
		}
		else {
			playoutb.emplace_back(boa.disks1 - boa.disks2);
		}
	}
	else {
		Difoute dife;
		if (25 < disks && disks < 45) {
			dife.dn = 3;
		}
		else {
			dife.dn = 4;
		}
		dife.start_level = n;
		playoutb.emplace_back(dife(boa, neu, w, s));
	}
	playoutb.shrink_to_fit();
}

void Game::playoutvalue74wdtest(Board &boa, int n, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65])
{
	Reverse2 rev;
	Random_choose rdc;
	disks = 0;
	playoutb.reserve(80);
	boa.calculate_disks();
	while ((!boa.get_end_game()) && (boa.disks1 + boa.disks2) < n) {
		boa.calculate_moves();
		boa.move_board(rdc);
		boa.calculate_disks();
	}
	disks = boa.disks1 + boa.disks2;
	if (disks != n) {
		return;
	}
	int turn(boa.get_turn());
	playoutb = boa.x74();
	if (disks == 64) {
		if (turn == 2) {
			playoutb.emplace_back(boa.disks2 - boa.disks1);
		}
		else {
			playoutb.emplace_back(boa.disks1 - boa.disks2);
		}
	}
	else {
		choices = boa.move_num;
		Difoute dife;
		dife.elim_depth = 0;
		if (25 < disks && disks < 45) {
			dife.dn = 3;
		}
		else {
			dife.dn = 4;
		}
		dife.start_level = n;
		double ma(dife(boa, neu, w, s));
		max_abs = abs(ma);
		playoutb.emplace_back(ma);
		if (boa.move_num > 1) {
			Board boa2(boa);
			Elim elim;
			elim(boa2, neu[disks + 1], w[disks + 1]);
			//cout << dif.maxi << "," << dif.maxj << "," << dif.maxn << "," << boa.move_num << "," << dif.max << endl;
			if (!boa2.moves[dife.maxi][dife.maxj]) {
				elim_err = true;
			}
			elim_choices = boa2.move_num;
		}
	}
	playoutb.shrink_to_fit();
}


//5変数評価関数
void Game::playgame5x(Choose &ch1, Choose &ch2)
{
	Board boa;
	int i(0);
	boa.calculate_moves();
	boa.calculate_moves2();
	boa.calculate_disks();
	std::vector<int> tempv(6, 0);
	while ((i < 80) && (!boa.get_end_game())) {
		tempv[0] = boa.turn;
		tempv[1] = boa.turnnum;
		tempv[2] = boa.disks1;
		tempv[3] = boa.disks2;
		tempv[4] = boa.move_num;
		tempv[5] = boa.move_num2;
		game_matrix.emplace_back(tempv);
		++i;
		boa.calculate_moves();
		boa.calculate_moves2();
		boa.calculate_disks();
		if (boa.get_turn() == 1) {
			boa.move_board(ch1);
		}
		else {
			boa.move_board(ch2);
		}
		//game_matrix[i].shrink_to_fit();
	}
	win = boa.get_win();
	turns = boa.get_turn_num();
	for (int i = 0;i < boa.get_turn_num() + 1;++i) {
		game_matrix[i].emplace_back(win);
	}
	game_matrix.shrink_to_fit();
}

vector<double> Board::x74()
{
	calculate_spots();
	calculate_spots_moves();
	calculate_spots_fixed();
	calculate_openness();
	vector<int> v;
	if (turn == 1) {
		v = spots1;
		copy(spots2.begin(), spots2.end(), back_inserter(v));
		copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		v = spots2;
		copy(spots1.begin(), spots1.end(), back_inserter(v));
		copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	vector<double> x(v.size());
	for (unsigned int i(0);i < v.size();++i) {
		x[i] = double(v[i]);
	}
	return x;
}

void Game::writegame() const
{
	std::string filename;
	int win_turn;
	switch (win) {
	case 1:
		filename = "Games1wintest.csv";
		win_turn = 1;
		break;
	case 2:
		filename = "Games2wintest.csv";
		win_turn = 2;
		break;
	case 0:
		filename = "Games0win.txt";
		win_turn = 0;
		return;
		break;
	}
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::app);
	//writing_file.open(filename, std::ios::out);
	for (int i = 0;i < (turns + 1);++i) {
		if (game_matrix[i][64] == win_turn) {
			for (int j = 0;j < (game_matrix[i].end() - game_matrix[i].begin());++j) {
				writing_file << game_matrix[i][j] << ",";
			}
			writing_file << std::endl;
		}
	}
	return;

	//filename = "Readt.txt";
	//std::ofstream writing_file2;
	////	writing_file.open(filename, std::ios::app);
	//writing_file2.open(filename, std::ios::out);
	//for (int j = 0;j < (game_matrix[turns + 1].end() - game_matrix[turns + 1].begin());++j) {
	//	writing_file2 << game_matrix[turns + 1][j] << ",";
	//}
	//writing_file2 << win << std::endl;
}

void Game::writegame2(int n, std::string fo, std::string fi, int rad)
{
	std::string filename(fo + "\\" + fi + "_" + std::to_string(n) + ".csv");
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::app);
	//writing_file.open(filename, std::ios::out);
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> randnum(0, rad);
	for (int i = 2;i < (turns + 1);++i) {
		if (randnum(mt) < 1) {
			if (game_matrix[i][64] == 2) {
				reverse(i);
			}
			switch (game_matrix[i][69]) {
			case 1:
				game_matrix[i][69] = 1;
				break;
			case 2:
				game_matrix[i][69] = -1;
				break;
			}
			for (int j = 0;j < 66;++j) {
				writing_file << game_matrix[i][j] << ",";
			}
			writing_file << game_matrix[i][69] << "," << std::endl;
		}
	}
	return;

	//filename = "Readt.txt";
	//std::ofstream writing_file2;
	////	writing_file.open(filename, std::ios::app);
	//writing_file2.open(filename, std::ios::out);
	//for (int j = 0;j < (game_matrix[turns + 1].end() - game_matrix[turns + 1].begin());++j) {
	//	writing_file2 << game_matrix[turns + 1][j] << ",";
	//}
	//writing_file2 << win << std::endl;
}

void Game::writegame5x(int n, std::string fo, std::string fi, int rad)
{
	std::string filename(fo + "\\" + fi + "_" + std::to_string(n) + ".csv");
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::app);
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> randnum(0, rad);
	//writing_file.open(filename, std::ios::out);
	for (int i = 2;i < (turns + 1);++i) {
		if (randnum(mt) < 1) {
			reverse5x(i);
			switch (game_matrix[i][6]) {
			case 1:
				game_matrix[i][6] = 1;
				break;
			case 2:
				game_matrix[i][6] = -1;
				break;
			}
			for (int j = 0;j < 7;++j) {
				writing_file << game_matrix[i][j] << ",";
			}
			writing_file << std::endl;
		}

	}
	return;

	//filename = "Readt.txt";
	//std::ofstream writing_file2;
	////	writing_file.open(filename, std::ios::app);
	//writing_file2.open(filename, std::ios::out);
	//for (int j = 0;j < (game_matrix[turns + 1].end() - game_matrix[turns + 1].begin());++j) {
	//	writing_file2 << game_matrix[turns + 1][j] << ",";
	//}
	//writing_file2 << win << std::endl;
}

inline std::vector<std::vector<int>> Game::get_game_matrix() const
{
	return game_matrix;
}

inline int Game::get_turns() const
{
	return turns;
}

Data::Data()
	:rows(0)
{

	std::vector<std::vector<int>> data(0, std::vector<int>(0, 0));
}

Data::~Data()
{
}

inline void Data::reverse128(int i)
{
	for (int j(0);j < 129;++j) {
		switch (dataggf[i][j]) {
		case 1:
			dataggf[i][j] = 2;
			break;
		case 2:
			dataggf[i][j] = 1;
			break;
		}
	}
	switch (dataggf[i][130]) {
	case 1:
		dataggf[i][130] = 2;
		break;
	case 2:
		dataggf[i][69] = 1;
		break;
	}
}

void Data::readdata(std::string file)
{
	//ファイルの読み込み
	std::ifstream ifs(file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	data.clear();
	data.reserve(10000);
	//csvファイルを1行ずつ読み込む
	std::string str;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		std::vector<int> tempv(0, 0);
		tempv.reserve(20);
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			tempv.emplace_back(std::stoi(token));
		}
		data.emplace_back(tempv);
	}
	data.shrink_to_fit();
}

//readggf
//void Data::readggf(const std::string file, int limit)
//{
//	char start[] = "BO[8";
//	//ファイルの読み込み
//	std::ifstream ifs(file);
//	if (!ifs) {
//		std::cout << "入力エラー";
//		return;
//	}
//	dataggf.clear();
//	dataggf.reserve(100000);
//	//ggfファイルを;)区切りで読み込む
//	std::string str;
//	Translate tr;
//	int i(0);
//	int j(0);
//	int k(0);
//	int dn(0);
//	bool done(false);
//	int turn(1);
//	int turnnum(0);
//	int win;
//	vector<int> v(64, 0);
//	vector<int> v2(64, 0);
//	Board bo;
//	while (getline(ifs, str, ';')) {
//		if (str[0] == 'G') {
//			//board初期値
//			i = 0;
//			j = 3;
//			const char * p = strstr(str.c_str(), start);
//			if (!p) {
//				break;
//			}
//			while (i < 64) {
//				switch (*(p + i + j)) {
//				case '-':
//					v[i] = 0;
//					++i;
//					break;
//				case 'O':
//					v[i] = 2;
//					++i;
//					break;
//				case '*':
//					v[i] = 1;
//					++i;
//					break;
//				default:
//					++j;
//					break;
//				}
//			}
//			i += j;
//			j = 0;
//			k = 0;
//			//動きを翻訳
//			while (i < int(str.size() - 1)) {
//				switch (*(p + i)) {
//					//黒の手
//				case 'B':
//					if (*(p + i + 1) == '[') {
//						turn = 1;
//						done = true;
//					}
//					break;
//				case 'W':
//					if (*(p + i + 1) == '[') {
//						turn = 2;
//						done = true;
//					}
//					break;
//				}
//				//移動処理
//				if (done) {
//					//pass
//					if (*(p + i + 2) == 'p') {
//						j = -1;
//						k = -1;
//						v2 = v;
//					}
//					else {
//						j = tr(p + i + 2);
//						k = atoi(p + i + 3) - 1;
//						if (j < 0 || k < 0 || turn < 1 || j>7 || k>7 || turn>2) {
//							done = false;
//							break;
//						}
//						v2 = bo.move_board_v(v, j, k, turn);
//					}
//					//指しの配列
//					if (done) {
//						for (int l = 0;l < 64;++l) {
//							v.emplace_back(0);
//						}
//						if (j >= 0) {
//							v[64 + 8 * j + k] = 1;
//						}
//						//cout << j << "," << k << "," << turn << "," << turnnum << "," << i << "," << str.size() << "," << dataggf.size() << "," << endl;
//						v.emplace_back(turn);
//						v.emplace_back(turnnum);
//						dataggf.emplace_back(v);
//						v = v2;
//						++turnnum;
//						++dn;
//						done = false;
//					}
//				}
//				++i;
//			}
//			win = bo.get_win_v(v);
//			for (int l = dn - turnnum;l < dn;++l) {
//				dataggf[l].emplace_back(win);
//			}
//			turnnum = 0;
//		}
//		if (dn > limit) {
//			break;
//		}
//	}
//	data.shrink_to_fit();
//}

inline void Data::adddata(std::vector<std::vector<int>>::iterator it, int end)
{
	for (int i = 1;i <= end;++i) {
		data.emplace_back(it[i]);
		++rows;
	}
}

void Data::writedata(std::ofstream &writing_file)
{
	//std::string filename = "Games.txt";
	//std::ofstream writing_file;
	//writing_file.open(filename, std::ios::app);
	//	writing_file.open(filename, std::ios::out);
	//for (int i = 1;i <= rows;++i) {
	//	for (int j = 0;j <= (data[i].end() - data[i].begin());++j) {
	//		writing_file << data[i][j] << std::endl;
	//	}
	//}
	int s = data.size();
	for (int i = 0;i < s;i++) {
		int ss = data[i].size();
		for (int j = 0;j < ss;++j) {
			writing_file << data[i][j] << ",";
		}
		writing_file << std::endl;
	}
	//filename = "Readt.txt";
	//std::ofstream writing_file2;
	////	writing_file.open(filename, std::ios::app);
	//writing_file2.open(filename, std::ios::out);
	//for (int j = 0;j < (game_matrix[turns + 1].end() - game_matrix[turns + 1].begin());++j) {
	//	writing_file2 << game_matrix[turns + 1][j] << ",";
	//}
	//writing_file2 << win << std::endl;
}

void Data::writeggf(const std::string f)
{
	std::string filename = f;
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//for (int i = 1;i <= rows;++i) {
	//	for (int j = 0;j <= (data[i].end() - data[i].begin());++j) {
	//		writing_file << data[i][j] << std::endl;
	//	}
	//}
	int s = dataggf.size() - 20;
	for (int i = 0;i < s;i++) {
		int ss = dataggf[i].size();
		if (dataggf[i][128] == 2) {
			reverse128(i);
		}
		for (int j = 0;j < ss;++j) {
			switch (dataggf[i][130]) {
			case 1:
				dataggf[i][130] = 1;
				break;
			case 2:
				dataggf[i][130] = -1;
				break;
			}
			writing_file << dataggf[i][j] << ",";
		}
		writing_file << std::endl;
	}
	//filename = "Readt.txt";
	//std::ofstream writing_file2;
	////	writing_file.open(filename, std::ios::app);
	//writing_file2.open(filename, std::ios::out);
	//for (int j = 0;j < (game_matrix[turns + 1].end() - game_matrix[turns + 1].begin());++j) {
	//	writing_file2 << game_matrix[turns + 1][j] << ",";
	//}
	//writing_file2 << win << std::endl;
}

void Data::writeggfwin(const std::string f)
{
	std::string filename = f;
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//for (int i = 1;i <= rows;++i) {
	//	for (int j = 0;j <= (data[i].end() - data[i].begin());++j) {
	//		writing_file << data[i][j] << std::endl;
	//	}
	//}
	int s = dataggf.size() - 20;
	for (int i = 0;i < s;++i) {
		int ss = dataggf[i].size();
		if (dataggf[i][128] == 2 && dataggf[i][130] == 2) {
			reverse128(i);
			dataggf[i][130] = 1;
			for (int j = 0;j < ss;++j) {
				writing_file << dataggf[i][j] << ",";
			}
			writing_file << std::endl;
		}
		if (dataggf[i][128] == 1 && dataggf[i][130] == 1) {
			dataggf[i][130] = 1;
			for (int j = 0;j < ss;++j) {
				writing_file << dataggf[i][j] << ",";
			}
			writing_file << std::endl;
		}
	}
	//filename = "Readt.txt";
	//std::ofstream writing_file2;
	////	writing_file.open(filename, std::ios::app);
	//writing_file2.open(filename, std::ios::out);
	//for (int j = 0;j < (game_matrix[turns + 1].end() - game_matrix[turns + 1].begin());++j) {
	//	writing_file2 << game_matrix[turns + 1][j] << ",";
	//}
	//writing_file2 << win << std::endl;
}

void Data::writeggfwinrnd(const std::string f, int randomize)
{
	std::string filename = f;
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	std::random_device rnd;
	std::mt19937 mt(rnd());
	//for (int i = 1;i <= rows;++i) {
	//	for (int j = 0;j <= (data[i].end() - data[i].begin());++j) {
	//		writing_file << data[i][j] << std::endl;
	//	}
	//}
	int s = dataggf.size();
	cout << s;
	for (int i = 0;i < s;++i) {
		std::uniform_real_distribution<> randnum(0, randomize);
		if (randnum(mt) < 1) {
			int ss = dataggf[i].size();
			if (dataggf[i][128] == 2 && dataggf[i][130] == 2) {
				reverse128(i);
				dataggf[i][130] = 1;
				for (int j = 0;j < ss;++j) {
					writing_file << dataggf[i][j] << ",";
				}
				writing_file << std::endl;
			}
			else if (dataggf[i][128] == 1 && dataggf[i][130] == 1) {
				dataggf[i][130] = 1;
				for (int j = 0;j < ss;++j) {
					writing_file << dataggf[i][j] << ",";
				}
				writing_file << std::endl;
			}
		}
	}
	//filename = "Readt.txt";
	//std::ofstream writing_file2;
	////	writing_file.open(filename, std::ios::app);
	//writing_file2.open(filename, std::ios::out);
	//for (int j = 0;j < (game_matrix[turns + 1].end() - game_matrix[turns + 1].begin());++j) {
	//	writing_file2 << game_matrix[turns + 1][j] << ",";
	//}
	//writing_file2 << win << std::endl;
}

void DataB::readdata(std::string file)
{
	//ファイルの読み込み
	std::ifstream ifs(file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	data.clear();
	data.reserve(10000);
	//csvファイルを1行ずつ読み込む
	std::string str;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		std::vector<double> tempv(0, 0);
		tempv.reserve(20);
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			tempv.emplace_back(std::stod(token));
		}
		data.emplace_back(tempv);
	}
	data.shrink_to_fit();

}

Winout::Winout()
{
}

Winout::~Winout()
{
}

int Winout::operator()(Board &b1)
{
	Winout wo;
	b1.calculate_moves();
	b1.calculate_disks();
	if (b1.move_num == 0) {
		b1.calculate_moves2();
		if (b1.move_num2 == 0) {
			return b1.get_win();
		}
		Board b2(b1);
		b2.turn = b1.turnnot;
		b2.turnnot = b1.turn;
		return wo(b2);
	}
	vector<int> v(b1.move_num, 0);
	int n(0);
	for (int i(0);i < 64;++i) {
		for (int j(0);j < 64;++j) {
			if (b1.moves[i][j]) {
				Board b2(b1);
				b2.move_board(i, j);
				v[n] = wo(b2);
				if (v[n] == b1.turn) {
					return b1.turn;
				}
				++n;
			}
		}
	}
	for (int i(0);i < b1.move_num;++i) {
		if (v[i] == 0) {
			return 0;
		}
	}
	return b1.turnnot;
}

Difout::Difout()
{
}

Difout::~Difout()
{
}

double Difout::operator()(Board &b1)
{
	Difout di;
	maxn = 0;
	b1.calculate_moves();
	b1.calculate_disks();
	if (b1.move_num == 0) {
		b1.calculate_moves2();
		if (b1.move_num2 == 0) {
			if (b1.turn == 1) {
				return (b1.disks1 - b1.disks2);
			}
			else {
				return (b1.disks2 - b1.disks1);
			}
		}
		Board b2(b1);
		b2.turn = b1.turnnot;
		b2.turnnot = b1.turn;
		++b2.turnnum;
		return -di(b2);
	}
	vector<int> v(b1.move_num, 0);
	int n(0);
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (b1.moves[i][j]) {
				Board b2(b1);
				b2.move_board(i, j);
				v[n] = -di(b2);
				++n;
			}
		}
	}
	maxn = distance(v.begin(), max_element(v.begin(), v.end()));
	n = 0;
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (b1.moves[i][j]) {
				if (n == maxn) {
					maxi = i;
					maxj = j;
				}
				++n;
			}
		}
	}
	max = v[maxn];
	return max;
}

Difoute::Difoute()
	:elim_depth(1)
{
}

Difoute::~Difoute()
{
}

double Difoute::operator()(Board &b1, Neuralnet(&neu)[65], const double(&w)[65], const double(&s)[65])
{
	maxn = 0;
	b1.calculate_moves();
	b1.calculate_disks();
	int disks = b1.disks1 + b1.disks2;
	if (b1.move_num == 0) {
		b1.calculate_moves2();
		if (b1.move_num2 == 0) {
			if (b1.turn == 1) {
				return (b1.disks1 - b1.disks2);
			}
			else {
				return (b1.disks2 - b1.disks1);
			}
		}
		Board b2(b1);
		b2.turn = b1.turnnot;
		b2.turnnot = b1.turn;
		++b2.turnnum;
		if (disks >= 62) {
			Difout di;
			return -di(b2);
		}
		Difoute die;
		die.dn = dn;
		return -die(b2, neu, w, s);
	}

	if (dn == 0) {
		int inputs(neu[disks].nodes[0]);
		vector<double> x(inputs, 0);
		x = b1.x74();
		return s[disks] * neu[disks].Forward(x)[0];
	}

	if (b1.move_num > 1 && disks <= start_level + elim_depth - 1 && disks < 56) {
		if (disks <= 25 || 45 <= disks) {
			Elim elim;
			elim(b1, neu[disks + 1], w[disks + 1]);
		}
	}

	vector<double> v(b1.move_num, 0);
	int n(0);
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (b1.moves[i][j]) {
				Board b2(b1);
				b2.move_board(i, j);
				Difoute die;
				die.dn = dn - 1;
				v[n] = -die(b2, neu, w, s);
				++n;
			}
		}
	}
	maxn = distance(v.begin(), max_element(v.begin(), v.end()));
	n = 0;
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (b1.moves[i][j]) {
				if (n == maxn) {
					maxi = i;
					maxj = j;
				}
				++n;
			}
		}
	}
	max = v[maxn];
	return max;
}

Elim::Elim()
{
}

Elim::~Elim()
{
}

void Elim::operator()(Board &b1, Neuralnet &neu, const double w)
{
	b1.calculate_moves();
	if (b1.move_num <= 1) {
		return;
	}
	int inputs(neu.nodes[0]);
	vector<double> y(b1.move_num, 0);
	vector<double> x(inputs, 0);
	int n(0);
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (b1.moves[i][j]) {
				Board b2(b1);
				b2.move_board(i, j);
				x = b2.x74();
				y[n] = -neu.Forward(x)[0];
				++n;
			}
		}
	}
	n = 0;
	double max(*max_element(y.begin(), y.end()));
	for (int i(0);i < 8;++i) {
		for (int j(0);j < 8;++j) {
			if (b1.moves[i][j]) {
				if (y[n] < (max - w)) {
					b1.moves[i][j] = false;
					--b1.move_num;
				}
				++n;
			}
		}
	}
}
