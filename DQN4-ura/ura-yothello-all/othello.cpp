//
//  othello.cpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#include "othello.hpp"

template <typename TYPE>
void extend(vector<double> &v, const vector<TYPE> &x) {
	for (auto i : x) {
		v.push_back(double(i));
	}
}

template <typename TYPE, size_t SIZE>
void extend(vector<double> &v, const TYPE (&x)[SIZE]) {
	for (int i = 0; i != SIZE; ++i) {
		v.push_back(double(x[i]));
	}
}

template <typename TYPE, size_t SIZE>
void extend(vector<double> &v, const TYPE (&x)[SIZE], const double weight) {
	for (int i = 0; i != SIZE; ++i) {
		v.push_back(double(x[i]) * weight);
	}
}

Board to_board(const string s) {
	Board b;
	string black("");
	string white("");
	for (int i(0); i < 16; ++i) {
		black += s[i];
	}
	for (int i(16); i < 32; ++i) {
		white += s[i];
	}
	vector< vector<int> > matrix_b(hex_to_matrix(black));
	vector< vector<int> > matrix_w(hex_to_matrix(white));
	vector< vector<int> > square_init(matrix_to_square(matrix_b, matrix_w));
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			b.square[i][j] = square_init[i][j];
		}
	}
	int turn = s[32] - '0';
	b.turn = turn;
	b.turnnot = turn == 1 ? 2 : 1;
	return b;
}
Board::Board()
:end_game(false),
pre_end_game(false)
{
}

Board::~Board()
{
}

void Board::rotate()
{
	int squtemp[8][8];
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			squtemp[i][j] = square[7 - j][i];
		}
	}
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			square[i][j] = squtemp[i][j];
		}
	}
}

void Board::reflect()
{
	int squtemp[8][8];
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			squtemp[i][j] = square[7 - j][7 - i];
		}
	}
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			square[i][j] = squtemp[i][j];
		}
	}
}

void Board::reverse()
{
	int tem = turn;
	turn = turnnot;
	turnnot = tem;
	
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (square[i][j] == 1) {
				square[i][j] = 2;
			}
			else if (square[i][j] == 2) {
				square[i][j] = 1;
			}
		}
	}
}

//�����Ֆʍ쐬
void Board::initialize()
{
	//rotation = 0;
	//reflection = 0;
	turn = 1;
	turnnot = 2;
	turnnum = 0;
	move_num = 0;
	pre_end_game = false;
	end_game = false;
	int(*p_square)[8](square);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_square[i][j] = 0;
		}
	}
	p_square[3][3] = 2;
	p_square[3][4] = 1;
	p_square[4][3] = 1;
	p_square[4][4] = 2;
}

int Board::diskdif63()
{
	const int *p_turn(&turn);
	const int *p_turnnot(&turnnot);
	const int(*p_square)[8](square);
	int pi(0);
	int pj(0);
	int d1(0);
	int d2(0);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (p_square[i][j] == 1) {
				++d1;
			}
			else if (p_square[i][j] == 2) {
				++d2;
			}
			else {
				pi = i;
				pj = j;
			}
		}
	}
	int dd(0);
	if (*p_turn == 1) {
		dd = d1 - d2;
	}
	else {
		dd = d2 - d1;
	}
	
	//���֤���E
	int flips(0);
	for (int direction = 1; direction < 9; ++direction) {
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
		int ni(pi + di);
		int nj(pj + dj);
		if ((ni >= 0) && (nj >= 0) && (ni <= 7) && (nj <= 7) && (p_square[ni][nj] == *p_turnnot)) {
			int serie(1);
			ni += di;
			nj += dj;
			while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
				if (p_square[ni][nj] == 0) {
					break;
				}
				else if (p_square[ni][nj] == *p_turn) {
					flips += serie;
					break;
				}
				++serie;
				ni += di;
				nj += dj;
			}
		}
	}
	if (flips > 0) {
		return (dd + flips*2 + 1);
	}
	
	//���֤ʤ�
	for (int direction = 1; direction < 9; ++direction) {
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
		int ni(pi + di);
		int nj(pj + dj);
		if ((ni >= 0) && (nj >= 0) && (ni <= 7) && (nj <= 7) && (p_square[ni][nj] == *p_turn)) {
			int serie(1);
			ni += di;
			nj += dj;
			while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
				if (p_square[ni][nj] == 0) {
					break;
				}
				else if (p_square[ni][nj] == *p_turnnot) {
					flips += serie;
				}
				++serie;
				ni += di;
				nj += dj;
			}
		}
	}
	if (flips > 0) {
		return (dd - flips * 2 - 1);
	}
	else {
		return dd;
	}
	
}

void Board::calculate_moves()
{
	bool(*p_moves)[8](moves);
	int *p_move_num(&move_num);
	const int *p_turn(&turn);
	const int *p_turnnot(&turnnot);
	*p_move_num = 0;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_moves[i][j] = false;
		}
	}
	const int(*p_square)[8](square);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (p_square[i][j] == 0) {
				bool end(false);
				for (int direction = 1; direction < 9; ++direction) {
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
					if ((ni >= 0) && (nj >= 0) && (ni <= 7) && (nj <= 7) && (p_square[ni][nj] == *p_turnnot)) {
						ni += di;
						nj += dj;
						while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
							if (p_square[ni][nj] == 0) {
								break;
							}
							else if (p_square[ni][nj] == *p_turn) {
								p_moves[i][j] = true;
								++*p_move_num;
								end = true;
								break;
							}
							ni += di;
							nj += dj;
						}
					}
					if (end) {
						break;
					}
				}
			}
		}
	}
	
}

void Board::calculate_moves2()
{
	bool(*p_moves)[8](moves2);
	int *p_move_num(&move_num2);
	*p_move_num = 0;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_moves[i][j] = false;
		}
	}
	const int(*p_square)[8](square);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (p_square[i][j] == 0) {
				if (right_move2(i, j)) {
					p_moves[i][j] = true;
					*p_move_num += 1;
				}
			}
		}
	}
}

bool Board::is_end_game() {
	calculate_moves();
	calculate_moves2();
	return move_num + move_num2 == 0;
}
bool Board::right_move(int i, int j) const {
	const int(*p_square)[8](square);
	const int *p_turn(&turn);
	const int *p_turnnot(&turnnot);
	for (int direction = 1; direction < 9; ++direction) {
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
		if ((ni >= 0) && (nj >= 0) && (ni <= 7) && (nj <= 7) && (p_square[ni][nj] == *p_turnnot)) {
			ni += di;
			nj += dj;
			while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
				if (p_square[ni][nj] == 0) {
					break;
				}
				else if (p_square[ni][nj] == *p_turn) {
					return true;
				}
				ni += di;
				nj += dj;
			}
		}
	}
	return false;
}



bool Board::right_move2(int i, int j) const {
	const int(*p_square)[8](square);
	const int *p_turn(&turn);
	const int *p_turnnot(&turnnot);
	for (int direction = 1; direction < 9; ++direction) {
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
		if ((ni >= 0) && (nj >= 0) && (ni <= 7) && (nj <= 7) && (p_square[ni][nj] == *p_turn)) {
			ni += di;
			nj += dj;
			while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
				if (p_square[ni][nj] == 0) {
					break;
				}
				if (p_square[ni][nj] == *p_turnnot) {
					return true;
				}
				ni += di;
				nj += dj;
			}
		}
	}
	return false;
}

//��������ɂ��Ď�̐������𔻒�
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
	const int(*p_square)[8](square);
	int ni(i + di);
	int nj(j + dj);
	if ((ni < 0) || (nj < 0) || (ni > 7) || (nj > 7) || (p_square[ni][nj] != turnnot)) {
		return false;
	}
	
	ni += di;
	nj += dj;
	bool right(false);
	const int *p_turn(&turn);
	
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (p_square[ni][nj] == 0) {
			return false;
		}
		else if (p_square[ni][nj] == *p_turn) {
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
	const int(*p_square)[8](square);
	int ni(i + di);
	int nj(j + dj);
	if ((ni < 0) || (nj < 0) || (ni > 7) || (nj > 7) || (p_square[ni][nj] != turn)) {
		return false;
	}
	ni += di;
	nj += dj;
	const int *p_turnnot(&turnnot);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (p_square[ni][nj] == 0) {
			return false;
		}
		else if (p_square[ni][nj] == *p_turnnot) {
			return true;
		}
		ni += di;
		nj += dj;
	}
	return false;
}


inline void Board::write_moves() const
{
	std::string filename = "Test.txt";
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
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

//���̎����_���ɑI��
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

inline void Board::move_board(const int i, const int j)
{	
	if (i == -1 || j == -1) {
		pass();
		return;
	}
	int(*p_square)[8](square);
	int *p_turn(&turn);
	int *p_turnnot(&turnnot);
	p_square[i][j] = *p_turn;
	for (int direction(1); direction < 9; ++direction) {
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
		
		if ((ni >= 0) && (nj >= 0) && (ni <= 7) && (nj <= 7) && (p_square[ni][nj] == *p_turnnot)) {
			ni += di;
			nj += dj;
			bool con(true);
			bool right(false);
			int number(0);
			while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8) && con) {
				if (p_square[ni][nj] == 0) {
					con = false;
				}
				else if (p_square[ni][nj] == *p_turn) {
					con = false;
					right = true;
				}
				++number;
				ni += di;
				nj += dj;
			}
			if (right) {
				for (int m(1); m <= number; ++m) {
					p_square[i + m * di][j + m * dj] = *p_turn;
				}
			}
		}
	}
	int temp(*p_turn);
	*p_turn = *p_turnnot;
	*p_turnnot = temp;
	++turnnum;
}

inline void Board::pass() {
	swap(turn, turnnot);
	++turnnum;
}

//��������ɂ��Ď��̔ՖʂɕύX
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
	
	int(*p_square)[8](square);
	int *p_turn(&turn);
	if ((ni < 0) || (nj < 0) || (ni > 7) || (nj > 7) || (p_square[ni][nj] != turnnot)) {
		return;
	}
	ni += di;
	nj += dj;
	bool con(true);
	bool right(false);
	int number(0);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8) && con) {
		if (p_square[ni][nj] == 0) {
			con = false;
		}
		else if (p_square[ni][nj] == *p_turn) {
			con = false;
			right = true;
		}
		++number;
		ni += di;
		nj += dj;
	}
	if (right) {
		for (int m(1); m <= number; ++m) {
			p_square[i + m * di][j + m * dj] = *p_turn;
		}
	}
}

inline bool Board::get_end_game() const {
	return end_game;
}

//�]���֐��v�Z
void Board::calculate_disks()
{
	int *p_disks1(&disks1);
	int *p_disks2(&disks2);
	int(*p_square)[8](square);
	*p_disks1 = 0;
	*p_disks2 = 0;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (p_square[i][j] == 1) {
				++*p_disks1;
			}
			else if (p_square[i][j] == 2) {
				++*p_disks2;
			}
		}
	}
}

void Board::calculate_spots() {
	int(*p_spots1)(spots1);
	int(*p_spots2)(spots2);
	for (int i(0); i < 10; ++i) {
		p_spots1[i] = 0;
		p_spots2[i] = 0;
	}
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	int(*p_square)[8](square);
	
	for (int n(0); n < 4; ++n) {
		for (int m(0); m < 4; ++m) {
			switch (p_square[i][j + m * dj])
			{
				case 1:
					++p_spots1[m];
					break;
				case 2:
					++p_spots2[m];
					break;
			}
		}
		for (int m(1); m < 4; ++m) {
			switch (p_square[i + m * di][j])
			{
				case 1:
					++p_spots1[m];
					break;
				case 2:
					++p_spots2[m];
					break;
			}
		}
		for (int m(1); m < 4; ++m) {
			switch (p_square[i + di][j + m * dj])
			{
				case 1:
					++p_spots1[3 + m];
					break;
				case 2:
					++p_spots2[3 + m];
					break;
			}
		}
		for (int m(2); m < 4; ++m) {
			switch (p_square[i + m * di][j + dj])
			{
				case 1:
					++p_spots1[3 + m];
					break;
				case 2:
					++p_spots2[3 + m];
					break;
			}
		}
		for (int m(2); m < 4; ++m) {
			switch (p_square[i + 2 * di][j + m * dj])
			{
				case 1:
					++p_spots1[5 + m];
					break;
				case 2:
					++p_spots2[5 + m];
					break;
			}
		}
		switch (p_square[i + 3 * di][j + 2 * dj])
		{
			case 1:
				++p_spots1[8];
				break;
			case 2:
				++p_spots2[8];
				break;
		}
		switch (p_square[i + 3 * di][j + 3 * dj])
		{
			case 1:
				++p_spots1[9];
				break;
			case 2:
				++p_spots2[9];
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

void Board::calculate_opennessf()
{
	int *p_spots_openness1(spots_openness1);
	int *p_spots_openness2(spots_openness2);
	int(*p_openness1)[8](openness1);
	int(*p_openness2)[8](openness2);
	for (int i(0); i < 9; ++i) {
		p_spots_openness1[i] = 0;
		p_spots_openness2[i] = 0;
	}
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_openness1[i][j] = 0;
			p_openness2[i][j] = 0;
		}
	}
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	const int(*p_square)[8](square);
	for (int n(0); n < 4; ++n) {
		for (int m(0); m < 4; ++m) {
			if (p_square[i][j + m * dj] == 0) {
				openness(i, j + m, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + m * di][j] == 0) {
				openness(i + m, j, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + di][j + m * dj] == 0) {
				openness(i + 1, j + m, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + m * di][j + dj] == 0) {
				openness(i + m, j + 1, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + 2 * di][j + m * dj] == 0) {
				openness(i + 2, j + m, 5 + m);
			}
		}
		if (p_square[i + 3 * di][j + 2 * dj] == 0) {
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

void Board::calculate_openness()
{
	int *p_spots_openness1(spots_openness1);
	int *p_spots_openness2(spots_openness2);
	int(*p_openness1)[8](openness1);
	int(*p_openness2)[8](openness2);
	for (int i(0); i < 9; ++i) {
		p_spots_openness1[i] = 0;
		p_spots_openness2[i] = 0;
	}
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_openness1[i][j] = 0;
			p_openness2[i][j] = 0;
		}
	}
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	const int(*p_square)[8](square);
	for (int n(0); n < 4; ++n) {
		for (int m(0); m < 4; ++m) {
			if (p_square[i][j + m * dj] == 0) {
				openness(i, j + m * dj, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + m * di][j] == 0) {
				openness(i + m * di, j, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + di][j + m * dj] == 0) {
				openness(i + di, j + m * dj, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + m * di][j + dj] == 0) {
				openness(i + m * di, j + dj, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + 2 * di][j + m * dj] == 0) {
				openness(i + 2 * di, j + m * dj, 5 + m);
			}
		}
		if (p_square[i + 3 * di][j + 2 * dj] == 0) {
			openness(i + 3 * di, j + 2 * dj, 8);
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_opennessv2()
{
	int *p_spots_openness1(spots_openness1);
	int *p_spots_openness2(spots_openness2);
	int(*p_openness1)[8](openness1);
	int(*p_openness2)[8](openness2);
	for (int i(0); i < 9; ++i) {
		p_spots_openness1[i] = 0;
		p_spots_openness2[i] = 0;
	}
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_openness1[i][j] = 0;
			p_openness2[i][j] = 0;
		}
	}
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	const int(*p_square)[8](square);
	for (int n(0); n < 4; ++n) {
		for (int m(0); m < 4; ++m) {
			if (p_square[i][j + m * dj] == 0) {
				opennessv2(i, j + m * dj, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + m * di][j] == 0) {
				opennessv2(i + m * di, j, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + di][j + m * dj] == 0) {
				opennessv2(i + di, j + m * dj, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + m * di][j + dj] == 0) {
				opennessv2(i + m * di, j + dj, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + 2 * di][j + m * dj] == 0) {
				opennessv2(i + 2 * di, j + m * dj, 5 + m);
			}
		}
		if (p_square[i + 3 * di][j + 2 * dj] == 0) {
			opennessv2(i + 3 * di, j + 2 * dj, 8);
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_opennessv3()
{
	int *p_spots_openness1(spots_openness1);
	int *p_spots_openness2(spots_openness2);
	int(*p_openness1)[8](openness1);
	int(*p_openness2)[8](openness2);
	for (int i(0); i < 9; ++i) {
		p_spots_openness1[i] = 0;
		p_spots_openness2[i] = 0;
	}
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_openness1[i][j] = 0;
			p_openness2[i][j] = 0;
		}
	}
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	const int(*p_square)[8](square);
	for (int n(0); n < 4; ++n) {
		for (int m(0); m < 4; ++m) {
			if (p_square[i][j + m * dj] == 0) {
				opennessv3(i, j + m * dj, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + m * di][j] == 0) {
				opennessv3(i + m * di, j, m);
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_square[i + di][j + m * dj] == 0) {
				opennessv3(i + di, j + m * dj, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + m * di][j + dj] == 0) {
				opennessv3(i + m * di, j + dj, 3 + m);
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_square[i + 2 * di][j + m * dj] == 0) {
				opennessv3(i + 2 * di, j + m * dj, 5 + m);
			}
		}
		if (p_square[i + 3 * di][j + 2 * dj] == 0) {
			opennessv3(i + 3 * di, j + 2 * dj, 8);
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
	int *p_spots_moves1(spots_moves1);
	int *p_spots_moves2(spots_moves2);
	calculate_moves();
	calculate_moves2();
	for (int i(0); i < 9; ++i) {
		p_spots_moves1[i] = 0;
		p_spots_moves2[i] = 0;
	}
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	bool(*p_moves)[8](moves);
	bool(*p_moves2)[8](moves2);
	for (int n(0); n < 4; ++n) {
		for (int m(0); m < 4; ++m) {
			if (p_moves[i][j + m * dj])
			{
				++p_spots_moves1[m];
			}
			if (p_moves2[i][j + m * dj])
			{
				++p_spots_moves2[m];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_moves[i + m * di][j])
			{
				++p_spots_moves1[m];
			}
			if (p_moves2[i + m * di][j])
			{
				++p_spots_moves2[m];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_moves[i + di][j + m * dj])
			{
				++p_spots_moves1[3 + m];
			}
			if (p_moves2[i + di][j + m * dj])
			{
				++p_spots_moves2[3 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_moves[i + m * di][j + dj])
			{
				++p_spots_moves1[3 + m];
			}
			if (p_moves2[i + m * di][j + dj])
			{
				++p_spots_moves2[3 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_moves[i + 2 * di][j + m * dj])
			{
				++p_spots_moves1[5 + m];
			}
			if (p_moves2[i + 2 * di][j + m * dj])
			{
				++p_spots_moves2[5 + m];
			}
		}
		if (p_moves[i + 3 * di][j + 2 * dj])
		{
			++p_spots_moves1[8];
		}
		if (p_moves2[i + 3 * di][j + 2 * dj])
		{
			++p_spots_moves2[8];
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_exmoves()
{
	bool(*p_moves)[8](moves);
	bool(*p_moves2)[8](moves2);
	bool(*p_exmoves1)[8](exmoves1);
	bool(*p_exmoves2)[8](exmoves2);
	bool(*p_bimoves)[8](bimoves);
	
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_bimoves[i][j] = false;
			p_exmoves1[i][j] = false;
			p_exmoves2[i][j] = false;
			
		}
	}
	
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (p_moves[i][j]) {
				if (p_moves2[i][j]) {
					p_bimoves[i][j] = true;
				}
				else {
					p_exmoves1[i][j] = true;
				}
			}
			else if (p_moves2[i][j]) {
				p_exmoves2[i][j] = true;
			}
		}
	}
}

void Board::calculate_spots_exmoves()
{
	int *p_spots_moves1(spots_moves1);
	int *p_spots_moves2(spots_moves2);
	int *p_spots_exmoves1(spots_exmoves1);
	int *p_spots_exmoves2(spots_exmoves2);
	int *p_spots_bimoves(spots_bimoves);
	calculate_moves();
	calculate_moves2();
	calculate_exmoves();
	for (int i(0); i < 9; ++i) {
		p_spots_moves1[i] = 0;
		p_spots_moves2[i] = 0;
		p_spots_exmoves1[i] = 0;
		p_spots_exmoves2[i] = 0;
		p_spots_bimoves[i] = 0;
	}
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	bool(*p_moves)[8](moves);
	bool(*p_moves2)[8](moves2);
	bool(*p_exmoves1)[8](exmoves1);
	bool(*p_exmoves2)[8](exmoves2);
	bool(*p_bimoves)[8](bimoves);
	
	for (int n(0); n < 4; ++n) {
		for (int m(0); m < 4; ++m) {
			if (p_moves[i][j + m * dj])
			{
				++p_spots_moves1[m];
			}
			if (p_moves2[i][j + m * dj])
			{
				++p_spots_moves2[m];
			}
			if (p_exmoves1[i][j + m * dj])
			{
				++p_spots_exmoves1[m];
			}
			if (p_exmoves2[i][j + m * dj])
			{
				++p_spots_exmoves2[m];
			}
			if (p_bimoves[i][j + m * dj])
			{
				++p_spots_bimoves[m];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_moves[i + m * di][j])
			{
				++p_spots_moves1[m];
			}
			if (p_moves2[i + m * di][j])
			{
				++p_spots_moves2[m];
			}
			if (p_exmoves1[i + m * di][j])
			{
				++p_spots_exmoves1[m];
			}
			if (p_exmoves2[i + m * di][j])
			{
				++p_spots_exmoves2[m];
			}
			if (p_bimoves[i + m * di][j])
			{
				++p_spots_bimoves[m];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_moves[i + di][j + m * dj])
			{
				++p_spots_moves1[3 + m];
			}
			if (p_moves2[i + di][j + m * dj])
			{
				++p_spots_moves2[3 + m];
			}
			if (p_exmoves1[i + di][j + m * dj])
			{
				++p_spots_exmoves1[3 + m];
			}
			if (p_exmoves2[i + di][j + m * dj])
			{
				++p_spots_exmoves2[3 + m];
			}
			if (p_bimoves[i + di][j + m * dj])
			{
				++p_spots_bimoves[3 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_moves[i + m * di][j + dj])
			{
				++p_spots_moves1[3 + m];
			}
			if (p_moves2[i + m * di][j + dj])
			{
				++p_spots_moves2[3 + m];
			}
			if (p_exmoves1[i + m * di][j + dj])
			{
				++p_spots_exmoves1[3 + m];
			}
			if (p_exmoves2[i + m * di][j + dj])
			{
				++p_spots_exmoves2[3 + m];
			}
			if (p_bimoves[i + m * di][j + dj])
			{
				++p_spots_bimoves[3 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_moves[i + 2 * di][j + m * dj])
			{
				++p_spots_moves1[5 + m];
			}
			if (p_moves2[i + 2 * di][j + m * dj])
			{
				++p_spots_moves2[5 + m];
			}
			if (p_exmoves1[i + 2 * di][j + m * dj])
			{
				++p_spots_exmoves1[5 + m];
			}
			if (p_exmoves2[i + 2 * di][j + m * dj])
			{
				++p_spots_exmoves2[5 + m];
			}
			if (p_bimoves[i + 2 * di][j + m * dj])
			{
				++p_spots_bimoves[5 + m];
			}
		}
		if (p_moves[i + 3 * di][j + 2 * dj])
		{
			++p_spots_moves1[8];
		}
		if (p_moves2[i + 3 * di][j + 2 * dj])
		{
			++p_spots_moves2[8];
		}
		if (p_exmoves1[i + 3 * di][j + 2 * dj])
		{
			++p_spots_exmoves1[8];
		}
		if (p_exmoves2[i + 3 * di][j + 2 * dj])
		{
			++p_spots_exmoves2[8];
		}
		if (p_bimoves[i + 3 * di][j + 2 * dj])
		{
			++p_spots_bimoves[8];
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
	int *p_spots_fixed1(spots_fixed1);
	int *p_spots_fixed2(spots_fixed2);
	for (int i(0); i < 9; ++i) {
		p_spots_fixed1[i] = 0;
		p_spots_fixed2[i] = 0;
	}
	const bool(*p_fixed1_m)[8](fixed1_m);
	const bool(*p_fixed2_m)[8](fixed2_m);
	
	calculate_fixed();
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	if (fixed1 + fixed2 == 0) {
		return;
	}
	for (int n(0); n < 4; ++n) {
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i][j + m * dj])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i][j + m * dj])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i + m * di][j])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + di][j + m * dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + di][j + m * dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j + dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + m * di][j + dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed1[4 + m];
			}
			if (p_fixed2_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed2[4 + m];
			}
		}
		if (p_fixed1_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed1[7];
		}
		if (p_fixed2_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed2[7];
		}
		if (p_fixed1_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed1[8];
		}
		if (p_fixed2_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed2[8];
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_spots_fixedv2()
{
	int *p_spots_fixed1(spots_fixed1);
	int *p_spots_fixed2(spots_fixed2);
	for (int i(0); i < 9; ++i) {
		p_spots_fixed1[i] = 0;
		p_spots_fixed2[i] = 0;
	}
	const bool(*p_fixed1_m)[8](fixed1_m);
	const bool(*p_fixed2_m)[8](fixed2_m);
	
	calculate_fixedv2();
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	if (fixed1 + fixed2 == 0) {
		return;
	}
	for (int n(0); n < 4; ++n) {
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i][j + m * dj])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i][j + m * dj])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i + m * di][j])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + di][j + m * dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + di][j + m * dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j + dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + m * di][j + dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed1[4 + m];
			}
			if (p_fixed2_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed2[4 + m];
			}
		}
		if (p_fixed1_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed1[7];
		}
		if (p_fixed2_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed2[7];
		}
		if (p_fixed1_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed1[8];
		}
		if (p_fixed2_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed2[8];
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_spots_fixedv3()
{
	int *p_spots_fixed1(spots_fixed1);
	int *p_spots_fixed2(spots_fixed2);
	for (int i(0); i < 9; ++i) {
		p_spots_fixed1[i] = 0;
		p_spots_fixed2[i] = 0;
	}
	const bool(*p_fixed1_m)[8](fixed1_m);
	const bool(*p_fixed2_m)[8](fixed2_m);
	
	calculate_fixedv3();
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	if (fixed1 + fixed2 == 0) {
		return;
	}
	for (int n(0); n < 4; ++n) {
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i][j + m * dj])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i][j + m * dj])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i + m * di][j])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + di][j + m * dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + di][j + m * dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j + dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + m * di][j + dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed1[4 + m];
			}
			if (p_fixed2_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed2[4 + m];
			}
		}
		if (p_fixed1_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed1[7];
		}
		if (p_fixed2_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed2[7];
		}
		if (p_fixed1_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed1[8];
		}
		if (p_fixed2_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed2[8];
		}
		t = i;
		i = j;
		j = 7 - t;
		t = di;
		di = dj;
		dj = -t;
	}
}

void Board::calculate_spots_fixedv4()
{
	int *p_spots_fixed1(spots_fixed1);
	int *p_spots_fixed2(spots_fixed2);
	for (int i(0); i < 9; ++i) {
		p_spots_fixed1[i] = 0;
		p_spots_fixed2[i] = 0;
	}
	const bool(*p_fixed1_m)[8](fixed1_m);
	const bool(*p_fixed2_m)[8](fixed2_m);
	
	calculate_fixedv4();
	int i(0);
	int j(0);
	int di(1);
	int dj(1);
	int t;
	if (fixed1 + fixed2 == 0) {
		return;
	}
	for (int n(0); n < 4; ++n) {
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i][j + m * dj])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i][j + m * dj])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j])
			{
				++p_spots_fixed1[m - 1];
			}
			if (p_fixed2_m[i + m * di][j])
			{
				++p_spots_fixed2[m - 1];
			}
		}
		for (int m(1); m < 4; ++m) {
			if (p_fixed1_m[i + di][j + m * dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + di][j + m * dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + m * di][j + dj])
			{
				++p_spots_fixed1[2 + m];
			}
			if (p_fixed2_m[i + m * di][j + dj])
			{
				++p_spots_fixed2[2 + m];
			}
		}
		for (int m(2); m < 4; ++m) {
			if (p_fixed1_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed1[4 + m];
			}
			if (p_fixed2_m[i + 2 * di][j + m * dj])
			{
				++p_spots_fixed2[4 + m];
			}
		}
		if (p_fixed1_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed1[7];
		}
		if (p_fixed2_m[i + 3 * di][j + 2 * dj])
		{
			++p_spots_fixed2[7];
		}
		if (p_fixed1_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed1[8];
		}
		if (p_fixed2_m[i + 3 * di][j + 3 * dj])
		{
			++p_spots_fixed2[8];
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
	bool(*p_fixed1_m)[8](fixed1_m);
	bool(*p_fixed2_m)[8](fixed2_m);
	int(*p_square)[8](square);
	int *p_fixed1(&fixed1);
	int *p_fixed2(&fixed2);
	*p_fixed1 = 0;
	*p_fixed2 = 0;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_fixed1_m[i][j] = false;
			p_fixed2_m[i][j] = false;
		}
	}
	
	//���𔻒f
	switch (p_square[0][0]) {
		case 1:
			++*p_fixed1;
			p_fixed1_m[0][0] = true;
			break;
		case 2:
			++*p_fixed2;
			p_fixed2_m[0][0] = true;
			break;
	}
	switch (p_square[0][7]) {
		case 1:
			++*p_fixed1;
			p_fixed1_m[0][7] = true;
			break;
		case 2:
			++*p_fixed2;
			p_fixed2_m[0][7] = true;
			break;
	}
	switch (p_square[7][0]) {
		case 1:
			++*p_fixed1;
			p_fixed1_m[7][0] = true;
			break;
		case 2:
			++*p_fixed2;
			p_fixed2_m[7][0] = true;
			break;
	}
	switch (p_square[7][7]) {
		case 1:
			++*p_fixed1;
			p_fixed1_m[7][7] = true;
			break;
		case 2:
			++*p_fixed2;
			p_fixed2_m[7][7] = true;
			break;
	}
	if (*p_fixed1 + *p_fixed2 == 0) {
		return;
	}
	
	bool rowfilled[8];
	bool columnfilled[8];
	//�ӂ𔻒f
	bool filled(true);
	for (int i(0); i < 8; ++i) {
		if (p_square[i][0] == 0) {
			filled = false;
		}
	}
	if (filled) {
		columnfilled[0] = true;
		for (int i(1); i < 7; ++i) {
			switch (p_square[i][0]) {
				case 1:
					++*p_fixed1;
					p_fixed1_m[i][0] = true;
					break;
				case 2:
					++*p_fixed2;
					p_fixed2_m[i][0] = true;
					break;
			}
		}
	}
	filled = true;
	for (int j(0); j < 8; ++j) {
		if (p_square[0][j] == 0) {
			filled = false;
		}
	}
	if (filled) {
		rowfilled[0] = true;
		for (int j(1); j < 7; ++j) {
			switch (p_square[0][j]) {
				case 1:
					++*p_fixed1;
					p_fixed1_m[0][j] = true;
					break;
				case 2:
					++*p_fixed2;
					p_fixed2_m[0][j] = true;
					break;
			}
		}
	}
	filled = true;
	for (int i(0); i < 8; ++i) {
		if (p_square[i][7] == 0) {
			filled = false;
		}
	}
	if (filled) {
		columnfilled[7] = true;
		for (int i(1); i < 7; ++i) {
			switch (p_square[i][7]) {
				case 1:
					++*p_fixed1;
					p_fixed1_m[i][7] = true;
					break;
				case 2:
					++*p_fixed2;
					p_fixed2_m[i][7] = true;
					break;
			}
		}
	}
	filled = true;
	for (int j(0); j < 8; ++j) {
		if (p_square[7][j] == 0) {
			filled = false;
		}
	}
	if (filled) {
		rowfilled[7] = true;
		for (int j(1); j < 7; ++j) {
			switch (p_square[7][j]) {
				case 1:
					++*p_fixed1;
					p_fixed1_m[7][j] = true;
					break;
				case 2:
					++*p_fixed2;
					p_fixed2_m[7][j] = true;
					break;
			}
		}
	}
	
	//�e���C���̖��܂�𔻒f
	for (int cn(1); cn < 7; ++cn) {
		filled = true;
		for (int i(0); i < 8; ++i) {
			if (p_square[i][cn] == 0) {
				filled = false;
			}
		}
		if (filled) {
			columnfilled[cn] = true;
		}
	}
	for (int rn(1); rn < 7; ++rn) {
		filled = true;
		for (int j(0); j < 8; ++j) {
			if (p_square[rn][j] == 0) {
				filled = false;
			}
		}
		if (filled) {
			rowfilled[rn] = true;
		}
	}
	
	int i(0);
	int j(0);
	int n(0);
	
	//�������f��
	//�v���C���[�P
	++j;
	while (j < 7 - n && (p_fixed1_m[i][j - 1] || rowfilled[i])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		++j;
	}
	++i;
	while (i < 7 - n && (p_fixed1_m[i - 1][j] || columnfilled[j])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		++i;
	}
	--j;
	while (j > n && (p_fixed1_m[i][j + 1] || rowfilled[i])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		--j;
	}
	--i;
	while (i > n && (p_fixed1_m[i + 1][j] || columnfilled[j])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		--i;
	}
	++i;
	while (i < 7 - n && (p_fixed1_m[i - 1][j] || columnfilled[j])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		++i;
	}
	++j;
	while (j < 7 - n && (p_fixed1_m[i][j - 1] || rowfilled[i])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		++j;
	}
	--i;
	while (i > n && (p_fixed1_m[i + 1][j] || columnfilled[j])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		--i;
	}
	--j;
	while (j > n && (p_fixed1_m[i][j + 1] || rowfilled[i])) {
		if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
			if (fixed(i, j)) {
				p_fixed1_m[i][j] = true;
				++*p_fixed1;
			}
		}
		--j;
	}
	
	//�v���C���[�Q
	++j;
	while (j < 7 - n && (p_fixed2_m[i][j - 1] || rowfilled[i])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		++j;
	}
	++i;
	while (i < 7 - n && (p_fixed2_m[i - 1][j] || columnfilled[j])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		++i;
	}
	--j;
	while (j > n && (p_fixed2_m[i][j + 1] || rowfilled[i])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		--j;
	}
	--i;
	while (i > n && (p_fixed2_m[i + 1][j] || columnfilled[j])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		--i;
	}
	++i;
	while (i < 7 - n && (p_fixed2_m[i - 1][j] || columnfilled[j])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		++i;
	}
	++j;
	while (j < 7 - n && (p_fixed2_m[i][j - 1] || rowfilled[i])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		++j;
	}
	--i;
	while (i > n && (p_fixed2_m[i + 1][j] || columnfilled[j])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		--i;
	}
	--j;
	while (j > n && (p_fixed2_m[i][j + 1] || rowfilled[i])) {
		if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
			if (fixed(i, j)) {
				p_fixed2_m[i][j] = true;
				++*p_fixed2;
			}
		}
		--j;
	}
	
	++n;
	i = n;
	j = n;
	
	calculate_disks();
	
	//�������f�o���N
	while (n < 4) {
		//�v���C���[�P
		while (j < 7 - n && (p_fixed1_m[i][j - 1] || rowfilled[i])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			++j;
		}
		while (i < 7 - n && (p_fixed1_m[i - 1][j] || columnfilled[j])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			++i;
		}
		while (j > n && (p_fixed1_m[i][j + 1] || rowfilled[i])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			--j;
		}
		while (i > n && (p_fixed1_m[i + 1][j] || columnfilled[j])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			--i;
		}
		while (i < 7 - n && (p_fixed1_m[i - 1][j] || columnfilled[j])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			++i;
		}
		while (j < 7 - n && (p_fixed1_m[i][j - 1] || rowfilled[i])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			++j;
		}
		while (i > n && (p_fixed1_m[i + 1][j] || columnfilled[j])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			--i;
		}
		while (j > n && (p_fixed1_m[i][j + 1] || rowfilled[i])) {
			if (!p_fixed1_m[i][j] && p_square[i][j] == 1) {
				if (fixed(i, j)) {
					p_fixed1_m[i][j] = true;
					++*p_fixed1;
				}
			}
			--j;
		}
		
		//�v���C���[�Q
		while (j < 7 - n && (p_fixed2_m[i][j - 1] || rowfilled[i])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			++j;
		}
		while (i < 7 - n && (p_fixed2_m[i - 1][j] || columnfilled[j])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			++i;
		}
		while (j > n && (p_fixed2_m[i][j + 1] || rowfilled[i])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			--j;
		}
		while (i > n && (p_fixed2_m[i + 1][j] || columnfilled[j])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			--i;
		}
		while (i < 7 - n && (p_fixed2_m[i - 1][j] || columnfilled[j])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			++i;
		}
		while (j < 7 - n && (p_fixed2_m[i][j - 1] || rowfilled[i])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			++j;
		}
		while (i > n && (p_fixed2_m[i + 1][j] || columnfilled[j])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			--i;
		}
		while (j > n && (p_fixed2_m[i][j + 1] || rowfilled[i])) {
			if (!p_fixed2_m[i][j] && p_square[i][j] == 2) {
				if (fixed(i, j)) {
					p_fixed2_m[i][j] = true;
					++*p_fixed2;
				}
			}
			--j;
		}
		
		++n;
		i = n;
		j = n;
	}
}

void Board::calculate_fixedv2()
{
	bool(*p_fixed1_m)[8](fixed1_m);
	bool(*p_fixed2_m)[8](fixed2_m);
	int(*p_square)[8](square);
	int *p_fixed1(&fixed1);
	int *p_fixed2(&fixed2);
	*p_fixed1 = 0;
	*p_fixed2 = 0;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_fixed1_m[i][j] = false;
			p_fixed2_m[i][j] = false;
		}
	}
	
	//���𔻒f
	int i(0);
	int j(0);
	for (int k(0); k < 4; ++k) {
		switch (p_square[i][j]) {
			case 1:
				++*p_fixed1;
				p_fixed1_m[i][j] = true;
				break;
			case 2:
				++*p_fixed2;
				p_fixed2_m[i][j] = true;
				break;
		}
		int temp = i;
		i = j;
		j = 7 - temp;
	}
	if (*p_fixed1 + *p_fixed2 == 0) {
		return;
	}
	
	bool framefilled[4];
	
	//�ӂ𔻒f
	i = 0;
	j = 0;
	int di(0);
	int dj(1);
	for (int n(0); n < 4; ++n) {
		bool filled(true);
		for (int k(0); k < 8; ++k) {
			if (p_square[i + k * di][j + k * dj] == 0) {
				filled = false;
			}
		}
		if (filled) {
			framefilled[n] = true;
			for (int k(1); k < 7; ++k) {
				switch (p_square[i + k * di][j + k * dj]) {
					case 1:
						++*p_fixed1;
						p_fixed1_m[i + k * di][j + k * dj] = true;
						break;
					case 2:
						++*p_fixed2;
						p_fixed2_m[i + k * di][j + k * dj] = true;
						break;
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	
	
	i = 0;
	j = 0;
	di = 0;
	dj = 1;
	
	//�������f��
	for (int m(0); m < 4; ++m) {
		int mother(p_square[i][j]);
		if (mother != 0) {
			for (int k(1); k < 7; ++k) {
				if (p_square[i + k * di][j + k * dj] == mother) {
					if (mother == 1) {
						p_fixed1_m[i + k * di][j + k * dj] = true;
						++*p_fixed1;
					}
					else if (mother == 2) {
						p_fixed2_m[i + k * di][j + k * dj] = true;
						++*p_fixed2;
					}
				}
				else {
					break;
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 0;
	j = 0;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		int mother(p_square[i][j]);
		if (mother != 0) {
			for (int k(1); k < 7; ++k) {
				if (p_square[i + k * di][j + k * dj] == mother) {
					if (mother == 1) {
						p_fixed1_m[i + k * di][j + k * dj] = true;
						++*p_fixed1;
					}
					else if (mother == 2) {
						p_fixed2_m[i + k * di][j + k * dj] = true;
						++*p_fixed2;
					}
				}
				else {
					break;
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	
	//for (int n(0); n < 2; ++n) {
	//	for (int m(0); m < 4; ++m) {
	//		for (int k(1); k < 7; ++k) {
	//			int look = p_square[i + k*di][j + k*dj];
	//			if (look == 1) {
	//				if (p_fixed1_m[i + (k - 1)*di][j + (k - 1)*dj]) {
	//					p_fixed1_m[i + k*di][j + k*dj] = true;
	//					++*p_fixed1;
	//				}
	//			}
	//			else if (look == 2) {
	//				if (p_fixed2_m[i + (k - 1)*di][j + (k - 1)*dj]) {
	//					p_fixed2_m[i + k*di][j + k*dj] = true;
	//					++*p_fixed1;
	//				}
	//			}
	//			else {
	//				break;
	//			}
	//		}
	//		int temp(i);
	//		i = j;
	//		j = 7 - temp;
	//		temp = di;
	//		di = dj;
	//		dj = temp;
	//	}
	//	i = 0;
	//	j = 0;
	//	di = -1;
	//	dj = 0;
	//}
	
	
	//i = 0;
	//j = 0;
	//int di(0);
	//int dj(1);
	//int m(1);
	//for (int n(0); n < 4; ++n) {
	//	for (int k(m); k < (8 - m);++k) {
	//		int look(p_square[n][m]);
	//		if (look == 1) {
	//			if (!fixed1_m[n][m]) {
	//				bool r(fixedv2(n, m));
	//				if (r) {
	//					p_fixed1_m[n][m];
	//					++*p_fixed1;
	//				}
	//			}
	//		}
	//		else if (look == 2) {
	//			if (!fixed2_m[n][m]) {
	//				bool r(fixedv2(n, m));
	//				if (r) {
	//					p_fixed2_m[n][m];
	//					++*p_fixed2;
	//				}
	//			}
	//		}
	//	}
	//}
	
	
	//�������f�o���N
	i = 1;
	j = 1;
	int pi = 0;
	int pj = 0;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (square[pi][pj] != 0) {
			for (int k(0); k < 6; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 1;
	j = 1;
	di = 1;
	dj = 0;
	pi = 0;
	pj = 0;
	for (int m(0); m < 4; ++m) {
		if (square[pi][pj] != 0) {
			for (int k(0); k < 6; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	i = 2;
	j = 2;
	pi = 1;
	pj = 1;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 4; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 2;
	j = 2;
	pi = 1;
	pj = 1;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 4; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	i = 3;
	j = 3;
	pi = 2;
	pj = 2;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 2; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 3;
	j = 3;
	pi = 2;
	pj = 2;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 2; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv2(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
}

void Board::calculate_fixedv3()
{
	bool(*p_fixed1_m)[8](fixed1_m);
	bool(*p_fixed2_m)[8](fixed2_m);
	int(*p_square)[8](square);
	int *p_fixed1(&fixed1);
	int *p_fixed2(&fixed2);
	*p_fixed1 = 0;
	*p_fixed2 = 0;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_fixed1_m[i][j] = false;
			p_fixed2_m[i][j] = false;
		}
	}
	
	//���𔻒f
	int i(0);
	int j(0);
	for (int k(0); k < 4; ++k) {
		switch (p_square[i][j]) {
			case 1:
				++*p_fixed1;
				p_fixed1_m[i][j] = true;
				break;
			case 2:
				++*p_fixed2;
				p_fixed2_m[i][j] = true;
				break;
		}
		int temp = i;
		i = j;
		j = 7 - temp;
	}
	if (*p_fixed1 + *p_fixed2 == 0) {
		return;
	}
	
	bool framefilled[4];
	
	//�ӂ𔻒f
	i = 0;
	j = 0;
	int di(0);
	int dj(1);
	for (int n(0); n < 4; ++n) {
		bool filled(true);
		for (int k(0); k < 8; ++k) {
			if (p_square[i + k * di][j + k * dj] == 0) {
				filled = false;
			}
		}
		if (filled) {
			framefilled[n] = true;
			for (int k(1); k < 7; ++k) {
				switch (p_square[i + k * di][j + k * dj]) {
					case 1:
						++*p_fixed1;
						p_fixed1_m[i + k * di][j + k * dj] = true;
						break;
					case 2:
						++*p_fixed2;
						p_fixed2_m[i + k * di][j + k * dj] = true;
						break;
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	
	

	i = 0;
	j = 0;
	di = 0;
	dj = 1;
	
	//�������f��
	for (int m(0); m < 4; ++m) {
		int mother(p_square[i][j]);
		if (mother != 0) {
			for (int k(1); k < 7; ++k) {
				if (p_square[i + k * di][j + k * dj] == mother) {
					if (mother == 1) {
						p_fixed1_m[i + k * di][j + k * dj] = true;
						++*p_fixed1;
					}
					else if (mother == 2) {
						p_fixed2_m[i + k * di][j + k * dj] = true;
						++*p_fixed2;
					}
				}
				else {
					break;
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 0;
	j = 0;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		int mother(p_square[i][j]);
		if (mother != 0) {
			for (int k(1); k < 7; ++k) {
				if (p_square[i + k * di][j + k * dj] == mother) {
					if (mother == 1) {
						p_fixed1_m[i + k * di][j + k * dj] = true;
						++*p_fixed1;
					}
					else if (mother == 2) {
						p_fixed2_m[i + k * di][j + k * dj] = true;
						++*p_fixed2;
					}
				}
				else {
					break;
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	
	//for (int n(0); n < 2; ++n) {
	//	for (int m(0); m < 4; ++m) {
	//		for (int k(1); k < 7; ++k) {
	//			int look = p_square[i + k*di][j + k*dj];
	//			if (look == 1) {
	//				if (p_fixed1_m[i + (k - 1)*di][j + (k - 1)*dj]) {
	//					p_fixed1_m[i + k*di][j + k*dj] = true;
	//					++*p_fixed1;
	//				}
	//			}
	//			else if (look == 2) {
	//				if (p_fixed2_m[i + (k - 1)*di][j + (k - 1)*dj]) {
	//					p_fixed2_m[i + k*di][j + k*dj] = true;
	//					++*p_fixed1;
	//				}
	//			}
	//			else {
	//				break;
	//			}
	//		}
	//		int temp(i);
	//		i = j;
	//		j = 7 - temp;
	//		temp = di;
	//		di = dj;
	//		dj = temp;
	//	}
	//	i = 0;
	//	j = 0;
	//	di = -1;
	//	dj = 0;
	//}
	
	
	//i = 0;
	//j = 0;
	//int di(0);
	//int dj(1);
	//int m(1);
	//for (int n(0); n < 4; ++n) {
	//	for (int k(m); k < (8 - m);++k) {
	//		int look(p_square[n][m]);
	//		if (look == 1) {
	//			if (!fixed1_m[n][m]) {
	//				bool r(fixedv2(n, m));
	//				if (r) {
	//					p_fixed1_m[n][m];
	//					++*p_fixed1;
	//				}
	//			}
	//		}
	//		else if (look == 2) {
	//			if (!fixed2_m[n][m]) {
	//				bool r(fixedv2(n, m));
	//				if (r) {
	//					p_fixed2_m[n][m];
	//					++*p_fixed2;
	//				}
	//			}
	//		}
	//	}
	//}
	
	
	//�������f�o���N
	i = 1;
	j = 1;
	int pi = 0;
	int pj = 0;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (square[pi][pj] != 0) {
			for (int k(0); k < 6; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 1;
	j = 1;
	di = 1;
	dj = 0;
	pi = 0;
	pj = 0;
	for (int m(0); m < 4; ++m) {
		if (square[pi][pj] != 0) {
			for (int k(0); k < 6; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	i = 2;
	j = 2;
	pi = 1;
	pj = 1;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 4; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 2;
	j = 2;
	pi = 1;
	pj = 1;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 4; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	i = 3;
	j = 3;
	pi = 2;
	pj = 2;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 2; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 3;
	j = 3;
	pi = 2;
	pj = 2;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 2; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv3(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
}

void Board::calculate_fixedv4()
{
	bool(*p_fixed1_m)[8](fixed1_m);
	bool(*p_fixed2_m)[8](fixed2_m);
	int(*p_square)[8](square);
	int *p_fixed1(&fixed1);
	int *p_fixed2(&fixed2);
	*p_fixed1 = 0;
	*p_fixed2 = 0;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_fixed1_m[i][j] = false;
			p_fixed2_m[i][j] = false;
		}
	}
	
	//���𔻒f
	int i(0);
	int j(0);
	for (int k(0); k < 4; ++k) {
		switch (p_square[i][j]) {
			case 1:
				++*p_fixed1;
				p_fixed1_m[i][j] = true;
				break;
			case 2:
				++*p_fixed2;
				p_fixed2_m[i][j] = true;
				break;
		}
		int temp = i;
		i = j;
		j = 7 - temp;
	}
	if (*p_fixed1 + *p_fixed2 == 0) {
		return;
	}
	
	bool(*p_directionfixed)[8][4] = directionfixed;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			for (int k(0); k < 4; ++k) {
				p_directionfixed[i][j][k] = true;
			}
		}
	}
	//p_directionfixed[0][7][2] = false;
	//p_directionfixed[1][7][2] = false;
	//p_directionfixed[0][6][2] = false;
	//p_directionfixed[7][0][2] = false;
	//p_directionfixed[7][1][2] = false;
	//p_directionfixed[6][1][2] = false;
	//p_directionfixed[0][0][3] = false;
	//p_directionfixed[1][0][3] = false;
	//p_directionfixed[0][1][3] = false;
	//p_directionfixed[7][7][3] = false;
	//p_directionfixed[7][6][3] = false;
	//p_directionfixed[6][7][3] = false;
	
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (p_square[i][j] == 0) {
				if (p_directionfixed[i][j][0]) {
					for (int k(0); k < 8; ++k) {
						p_directionfixed[k][j][0] = false;
					}
				}
				if (p_directionfixed[i][j][1]) {
					for (int k(0); k < 8; ++k) {
						p_directionfixed[i][k][1] = false;
					}
				}
				if (p_directionfixed[i][j][2]) {
					int si(0);
					int sj(0);
					int endk(8);
					if (i >= j) {
						si = i - j;
						endk = 8 - si;
					}
					else {
						sj = j - i;
						endk = 8 - sj;
					}
					for (int k(0); k < endk; ++k) {
						p_directionfixed[si + k][sj + k][2] = false;
					}
				}
				if (p_directionfixed[i][j][3]) {
					int si(0);
					int sj(0);
					int endk(8);
					if (i + j >= 7) {
						si = i + j - 7;
						endk = 8 - si;
					}
					else {
						sj = 7 - (i + j);
						endk = 8 - sj;
					}
					for (int k(0); k < endk; ++k) {
						p_directionfixed[si + k][sj - k][3] = false;
					}
				}
			}
		}
	}
	
	bool framefilled[4];
	
	//�ӂ𔻒f
	i = 0;
	j = 0;
	int di(0);
	int dj(1);
	
	for (int n(0); n < 4; ++n) {
		int dr(0);
		if (di == 0) {
			dr = 1;
		}
		if (p_directionfixed[i][j][dr]) {
			framefilled[n] = true;
			for (int k(1); k < 7; ++k) {
				switch (p_square[i + k * di][j + k * dj]) {
					case 1:
						++*p_fixed1;
						p_fixed1_m[i + k * di][j + k * dj] = true;
						break;
					case 2:
						++*p_fixed2;
						p_fixed2_m[i + k * di][j + k * dj] = true;
						break;
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	
	i = 0;
	j = 0;
	di = 0;
	dj = 1;
	//�������f��
	for (int m(0); m < 4; ++m) {
		int mother(p_square[i][j]);
		int dr(0);
		if (di == 0) {
			dr = 1;
		}
		if (mother != 0 && !p_directionfixed[i][j][dr]) {
			for (int k(1); k < 7; ++k) {
				if (p_square[i + k * di][j + k * dj] == mother) {
					if (mother == 1) {
						p_fixed1_m[i + k * di][j + k * dj] = true;
						++*p_fixed1;
					}
					else if (mother == 2) {
						p_fixed2_m[i + k * di][j + k * dj] = true;
						++*p_fixed2;
					}
				}
				else {
					break;
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 0;
	j = 0;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		int mother(p_square[i][j]);
		int dr(0);
		if (di == 0) {
			dr = 1;
		}
		if (mother != 0 && !p_directionfixed[i][j][dr]) {
			for (int k(1); k < 7; ++k) {
				if (p_square[i + k * di][j + k * dj] == mother) {
					if (mother == 1) {
						p_fixed1_m[i + k * di][j + k * dj] = true;
						++*p_fixed1;
					}
					else if (mother == 2) {
						p_fixed2_m[i + k * di][j + k * dj] = true;
						++*p_fixed2;
					}
				}
				else {
					break;
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	
	//for (int n(0); n < 2; ++n) {
	//	for (int m(0); m < 4; ++m) {
	//		for (int k(1); k < 7; ++k) {
	//			int look = p_square[i + k*di][j + k*dj];
	//			if (look == 1) {
	//				if (p_fixed1_m[i + (k - 1)*di][j + (k - 1)*dj]) {
	//					p_fixed1_m[i + k*di][j + k*dj] = true;
	//					++*p_fixed1;
	//				}
	//			}
	//			else if (look == 2) {
	//				if (p_fixed2_m[i + (k - 1)*di][j + (k - 1)*dj]) {
	//					p_fixed2_m[i + k*di][j + k*dj] = true;
	//					++*p_fixed1;
	//				}
	//			}
	//			else {
	//				break;
	//			}
	//		}
	//		int temp(i);
	//		i = j;
	//		j = 7 - temp;
	//		temp = di;
	//		di = dj;
	//		dj = temp;
	//	}
	//	i = 0;
	//	j = 0;
	//	di = -1;
	//	dj = 0;
	//}
	
	
	//i = 0;
	//j = 0;
	//int di(0);
	//int dj(1);
	//int m(1);
	//for (int n(0); n < 4; ++n) {
	//	for (int k(m); k < (8 - m);++k) {
	//		int look(p_square[n][m]);
	//		if (look == 1) {
	//			if (!fixed1_m[n][m]) {
	//				bool r(fixedv2(n, m));
	//				if (r) {
	//					p_fixed1_m[n][m];
	//					++*p_fixed1;
	//				}
	//			}
	//		}
	//		else if (look == 2) {
	//			if (!fixed2_m[n][m]) {
	//				bool r(fixedv2(n, m));
	//				if (r) {
	//					p_fixed2_m[n][m];
	//					++*p_fixed2;
	//				}
	//			}
	//		}
	//	}
	//}
	
	
	//�������f�o���N
	i = 1;
	j = 1;
	int pi = 0;
	int pj = 0;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (square[pi][pj] != 0) {
			for (int k(0); k < 6; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 1;
	j = 1;
	di = 1;
	dj = 0;
	pi = 0;
	pj = 0;
	for (int m(0); m < 4; ++m) {
		if (square[pi][pj] != 0) {
			for (int k(0); k < 6; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	i = 2;
	j = 2;
	pi = 1;
	pj = 1;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 4; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 2;
	j = 2;
	pi = 1;
	pj = 1;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 4; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
	
	i = 3;
	j = 3;
	pi = 2;
	pj = 2;
	di = 0;
	dj = 1;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 2; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(i);
		i = j;
		j = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	i = 3;
	j = 3;
	pi = 2;
	pj = 2;
	di = 1;
	dj = 0;
	for (int m(0); m < 4; ++m) {
		if (p_fixed1_m[pi][pj] || p_fixed2_m[pi][pj]) {
			for (int k(0); k < 2; ++k) {
				int look(p_square[i + k * di][j + k * dj]);
				if (look == 1) {
					if (!fixed1_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed1_m[i + k * di][j + k * dj] = true;
							++*p_fixed1;
						}
					}
				}
				else if (look == 2) {
					if (!fixed2_m[i + k * di][j + k * dj]) {
						bool r(fixedv4(i + k * di, j + k * dj));
						if (r) {
							p_fixed2_m[i + k * di][j + k * dj] = true;
							++*p_fixed2;
						}
					}
				}
			}
		}
		int temp(j);
		j = i;
		i = 7 - temp;
		temp = pi;
		pi = pj;
		pj = 7 - temp;
		temp = dj;
		dj = di;
		di = -temp;
	}
}

void Board::sweep_fixed(const int i, const int j, const int di, const int dj)
{
	int(*p_square)[8](square);
	int si(i - di);
	int sj(j - dj);
	int mother(p_square[si][sj]);
	bool(*p_fixed_m)[8](fixed1_m);
	int *p_fixed(&fixed1);
	int prei;
	int prej;
	int endk(6);
	int k(0);
	bool fin(false);
	if (mother == 0) {
		return;
	}
	if (mother == 2) {
		p_fixed_m = fixed2_m;
		p_fixed = &fixed2;
	}
	if (p_square[si][sj] == mother) {
		if (i < 4) {
			prei = -1;
		}
		else {
			prei = 1;
		}
		if (j < 4) {
			prej = -1;
		}
		else {
			prej = 1;
		}
		if (i == 2 || i == 5) {
			endk = 4;
		}
		fin = true;
		si = i;
		sj = j;
		if (p_square[si][sj] == mother) {
			si += prei;
			if (p_square[si][sj] == mother) {
				si -= prei;
				sj += prej;
				if (p_square[si][sj] == mother) {
					si += prei;
					if (p_square[si][sj] == mother) {
						si += (2 * di);
						sj += (2 * dj);
						if (p_square[si][sj] == mother) {
							++*p_fixed;
							p_fixed_m[i][j] = true;
							fin = false;
						}
					}
				}
			}
		}
		while (k < endk && !fin) {
			fin = true;
			si = (i + k * di);
			sj = (j + k * dj);
			if (p_square[si][sj] == mother) {
				si += (prei + 2 * di);
				sj += (prej + 2 * dj);
				if (p_square[si][sj] == mother) {
					++*p_fixed;
					p_fixed_m[i][j] = true;
					fin = false;
				}
			}
			++k;
		}
	}
	//	int(*p_square)[8](square);
	//	int si(i - di);
	//	int sj(j - dj);
	//	int mother(p_square[si][sj]);
	//	bool(*p_fixed_m)[8](fixed1_m);
	//	int *p_fixed(&fixed1);
	//	int prei;
	//	int prej;
	//	int endk(6);
	//	int k(0);
	//	bool fin(false);
	//	if (mother == 0) {
	//		return;
	//	}
	//	else if (mother == 2) {
	//		p_fixed_m = fixed2_m;
	//		p_fixed = &fixed2;
	//	}
	//	if (p_fixed_m[si][sj]) {
	//		if (i < 4) {
	//			prei = -1;
	//		}
	//		else {
	//			prei = 1;
	//		}
	//		if (j < 4) {
	//			prej = -1;
	//		}
	//		else {
	//			prej = 1;
	//		}
	//		if (i == 2 || i == 5) {
	//			endk = 4;
	//		}
	//		while (k < endk && !fin) {
	//			fin = true;
	//			si = (i + k*di);
	//			sj = (j + k*dj);
	//			if (p_square[si][sj] == mother) {
	//				si += prei;
	//				if (p_fixed_m[si][sj]) {
	//					si -= prei;
	//					sj += prej;
	//					if (p_fixed_m[si][sj]) {
	//						si += prei;
	//						if (p_fixed_m[si][sj]) {
	//							si += (2 * di);
	//							sj += (2 * dj);
	//							if (p_fixed_m[si][sj]) {
	//								++*p_fixed;
	//								p_fixed_m[i][j] = true;
	//								fin = false;
	//							}
	//						}
	//					}
	//				}
	//			}
	//			++k;
	//		}
	//	}
	
	//if (mother == 1 && p_fixed1_m[si][sj]) {
	//	int k(0);
	//	bool con(true);
	//	while (k < end && con) {
	//		si = i + k*di;
	//		sj = j + k*dj;
	//		if (p_square[si][sj] == mother) {
	//			si = i + prei;
	//			if (p_fixed1_m[si][sj]) {
	//				si = i + k*di;
	//				sj = i + k*di + prej;
	//				if (p_fixed1_m[si][sj]) {
	//					si = i + prei;
	//					if (p_fixed1_m[si][sj]) {
	//						si += (2 * di);
	//						sj += (2 * dj);
	//						if (p_fixed1_m[si][sj]) {
	//							p_fixed1_m[i][j] = true;
	//						}
	//					}
	//				}
	//			}
	//		}
	//		else {
	//			con = false;
	//		}
	//		++k;
	//	}
	//}
	//else if (mother == 2 && p_fixed2_m[si][sj]) {
	//	int k(0);
	//	bool con(true);
	//	while (k < end && con) {
	//		si = i + k*di;
	//		sj = j + k*dj;
	//		if (p_square[si][sj] == mother) {
	//			si = i + prei;
	//			if (p_fixed2_m[si][sj]) {
	//				si = i;
	//				sj = i + prej;
	//				if (p_fixed2_m[si][sj]) {
	//					si = i + prei;
	//					if (p_fixed2_m[si][sj]) {
	//						si += (2 * di);
	//						sj += (2 * dj);
	//						if (p_fixed2_m[si][sj]) {
	//							p_fixed2_m[i][j] = true;
	//						}
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			con = false;
	//		}
	//		++k;
	//	}
	//}
}



bool Board::fixed(int i, int j) const
{
	if (square[i][j] == 0) {
		return false;
	}
	if (!(fixed_direction(i, j, 1) || fixed_direction(i, j, 5) || fixed_line(i, j, 0))) {
		return false;
	}
	if (!(fixed_direction(i, j, 2) || fixed_direction(i, j, 6) || fixed_line(i, j, 1))) {
		return false;
	}
	if (!(fixed_direction(i, j, 3) || fixed_direction(i, j, 7) || fixed_line(i, j, 2))) {
		return false;
	}
	if (!(fixed_direction(i, j, 4) || fixed_direction(i, j, 8) || fixed_line(i, j, 3))) {
		return false;
	}
	return true;
	
	//for (int n(1);n < 9;++n) {
	//	b[n - 1] = fixed_direction(i, j, n);
	//}
	//for (int l(0);l < 4;++l) {
	//	b2[l] = fixed_line(i, j, l);
	//}
	
	//if ((b[0] || b[4] || b2[0]) && (b[1] || b[5] || b2[1]) && (b[2] || b[6] || b2[2]) && (b[3] || b[7]) || b2[3]) {
	//	return true;
	//}
	//else {
	//	return false;
	//}
}

bool Board::fixedv2(int i, int j) const
{
	if (!fixed_direction(i, j, 1)) {
		if (!fixed_direction(i, j, 5)) {
			if (!fixed_line(i, j, 0)) {
				return false;
			}
		}
	}
	if (!fixed_direction(i, j, 2)) {
		if (!fixed_direction(i, j, 6)) {
			if (!fixed_line(i, j, 1)) {
				return false;
			}
		}
	}
	if (!fixed_direction(i, j, 3)) {
		if (!fixed_direction(i, j, 7)) {
			if (!fixed_line(i, j, 2)) {
				return false;
			}
		}
	}
	if (!fixed_direction(i, j, 4)) {
		if (!fixed_direction(i, j, 8)) {
			if (!fixed_line(i, j, 3)) {
				return false;
			}
		}
	}
	return true;
	
	//if (!(fixed_direction(i, j, 1) || fixed_direction(i, j, 5) || fixed_line(i, j, 0))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 2) || fixed_direction(i, j, 6) || fixed_line(i, j, 1))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 3) || fixed_direction(i, j, 7) || fixed_line(i, j, 2))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 4) || fixed_direction(i, j, 8) || fixed_line(i, j, 3))) {
	//	return false;
	//}
	//return true;
	
	//for (int n(1);n < 9;++n) {
	//	b[n - 1] = fixed_direction(i, j, n);
	//}
	//for (int l(0);l < 4;++l) {
	//	b2[l] = fixed_line(i, j, l);
	//}
	
	//if ((b[0] || b[4] || b2[0]) && (b[1] || b[5] || b2[1]) && (b[2] || b[6] || b2[2]) && (b[3] || b[7]) || b2[3]) {
	//	return true;
	//}
	//else {
	//	return false;
	//}
}

bool Board::fixedv3(int i, int j) const
{
	if (!fixed_direction(i, j, 1)) {
		if (!fixed_direction(i, j, 5)) {
			return false;
		}
	}
	if (!fixed_direction(i, j, 2)) {
		if (!fixed_direction(i, j, 6)) {
			return false;
		}
	}
	if (!fixed_direction(i, j, 3)) {
		if (!fixed_direction(i, j, 7)) {
			return false;
		}
	}
	if (!fixed_direction(i, j, 4)) {
		if (!fixed_direction(i, j, 8)) {
			return false;
		}
	}
	return true;
	
	//if (!(fixed_direction(i, j, 1) || fixed_direction(i, j, 5) || fixed_line(i, j, 0))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 2) || fixed_direction(i, j, 6) || fixed_line(i, j, 1))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 3) || fixed_direction(i, j, 7) || fixed_line(i, j, 2))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 4) || fixed_direction(i, j, 8) || fixed_line(i, j, 3))) {
	//	return false;
	//}
	//return true;
	
	//for (int n(1);n < 9;++n) {
	//	b[n - 1] = fixed_direction(i, j, n);
	//}
	//for (int l(0);l < 4;++l) {
	//	b2[l] = fixed_line(i, j, l);
	//}
	
	//if ((b[0] || b[4] || b2[0]) && (b[1] || b[5] || b2[1]) && (b[2] || b[6] || b2[2]) && (b[3] || b[7]) || b2[3]) {
	//	return true;
	//}
	//else {
	//	return false;
	//}
}

bool Board::fixedv4(int i, int j)
{
	bool(*p_directionfixed)[8][4] = directionfixed;
	if (!fixed_direction(i, j, 1)) {
		if (!fixed_direction(i, j, 5)) {
			if (!p_directionfixed[i][j][0]) {
				return false;
			}
		}
	}
	if (!fixed_direction(i, j, 2)) {
		if (!fixed_direction(i, j, 6)) {
			if (!p_directionfixed[i][j][3]) {
				return false;
			}
		}
	}
	if (!fixed_direction(i, j, 3)) {
		if (!fixed_direction(i, j, 7)) {
			if (!p_directionfixed[i][j][1]) {
				return false;
			}
		}
	}
	if (!fixed_direction(i, j, 4)) {
		if (!fixed_direction(i, j, 8)) {
			if (!p_directionfixed[i][j][2]) {
				return false;
			}
		}
	}
	return true;
	
	//if (!(fixed_direction(i, j, 1) || fixed_direction(i, j, 5) || fixed_line(i, j, 0))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 2) || fixed_direction(i, j, 6) || fixed_line(i, j, 1))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 3) || fixed_direction(i, j, 7) || fixed_line(i, j, 2))) {
	//	return false;
	//}
	//if (!(fixed_direction(i, j, 4) || fixed_direction(i, j, 8) || fixed_line(i, j, 3))) {
	//	return false;
	//}
	//return true;
	
	//for (int n(1);n < 9;++n) {
	//	b[n - 1] = fixed_direction(i, j, n);
	//}
	//for (int l(0);l < 4;++l) {
	//	b2[l] = fixed_line(i, j, l);
	//}
	
	//if ((b[0] || b[4] || b2[0]) && (b[1] || b[5] || b2[1]) && (b[2] || b[6] || b2[2]) && (b[3] || b[7]) || b2[3]) {
	//	return true;
	//}
	//else {
	//	return false;
	//}
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
	const int(*p_square)[8](square);
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (p_square[ni][nj] == 0) {
			return false;
		}
		ni += di;
		nj += dj;
	}
	ni = i - di;
	nj = j - dj;
	while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		if (p_square[ni][nj] == 0) {
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
	const bool(*p_fixed1_m)[8](fixed1_m);
	const bool(*p_fixed2_m)[8](fixed2_m);
	int ni(i + di);
	int nj(j + dj);
	if ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
		switch (color)
		{
			case 1:
				if (!p_fixed1_m[ni][nj]) {
					return false;
				}
				break;
			case 2:
				if (!p_fixed2_m[ni][nj]) {
					return false;
				}
				break;
		}
		ni += di;
		nj += dj;
	}
	return true;
	//int color(square[i][j]);
	//int di(0);
	//int dj(0);
	//switch (n) {
	//case 1:
	//	di = -1;
	//	dj = 0;
	//	break;
	//case 2:
	//	di = -1;
	//	dj = 1;
	//	break;
	//case 3:
	//	di = 0;
	//	dj = 1;
	//	break;
	//case 4:
	//	di = 1;
	//	dj = 1;
	//	break;
	//case 5:
	//	di = 1;
	//	dj = 0;
	//	break;
	//case 6:
	//	di = 1;
	//	dj = -1;
	//	break;
	//case 7:
	//	di = 0;
	//	dj = -1;
	//	break;
	//case 8:
	//	di = -1;
	//	dj = -1;
	//	break;
	//}
	//const bool(*p_fixed1_m)[8](fixed1_m);
	//const bool(*p_fixed2_m)[8](fixed2_m);
	//int ni(i + di);
	//int nj(j + dj);
	//while ((ni >= 0) && (nj >= 0) && (ni < 8) && (nj < 8)) {
	//	switch (color)
	//	{
	//	case 1:
	//		if (!p_fixed1_m[ni][nj]) {
	//			return false;
	//		}
	//		break;
	//	case 2:
	//		if (!p_fixed2_m[ni][nj]) {
	//			return false;
	//		}
	//		break;
	//	}
	//	ni += di;
	//	nj += dj;
	//}
	//return true;
}

void Board::calculate_mowiv2()
{
	int *p_mountians1(&mountains1);
	int *p_mountians2(&mountains2);
	//�ԈႢ
	int *p_wings1false(&wings1false);
	int *p_wings2false(&wings2false);
	int *p_wings1(&wings1);
	int *p_wings2(&wings2);
	int(*p_square)[8](square);
	int *p_sides1(sides1);
	int *p_sides2(sides2);
	*p_mountians1 = 0;
	*p_mountians2 = 0;
	*p_wings1 = 0;
	*p_wings2 = 0;
	*p_wings1false = 0;
	*p_wings2false = 0;
	bool corneropen[4] = { 0, 0, 0, 0 };
	bool bycorneropen[4] = { 0, 0, 0, 0 };
	if (p_square[7][0] == 0) {
		corneropen[0] = true;
	}
	if (p_square[0][0] == 0) {
		corneropen[1] = true;
	}
	if (p_square[0][7] == 0) {
		corneropen[2] = true;
	}
	if (p_square[7][7] == 0) {
		corneropen[3] = true;
	}
	if (p_square[6][0] == 0 || p_square[1][0] == 0) {
		bycorneropen[0] = true;
	}
	if (p_square[0][1] == 0 || p_square[0][6] == 0) {
		bycorneropen[1] = true;
	}
	if (p_square[1][7] == 0 || p_square[6][7] == 0) {
		bycorneropen[2] = true;
	}
	if (p_square[7][6] == 0 || p_square[7][1] == 0) {
		bycorneropen[3] = true;
	}
	
	for (int i(0); i < 4; ++i) {
		p_sides1[i] = 0;
		p_sides2[i] = 0;
	}
	for (int i(1); i < 7; ++i) {
		switch (p_square[i][0]) {
			case 1:
				++p_sides1[0];
				break;
			case 2:
				++p_sides2[0];
				break;
		}
	}
	for (int j(1); j < 7; ++j) {
		switch (p_square[0][j]) {
			case 1:
				++p_sides1[1];
				break;
			case 2:
				++p_sides2[1];
				break;
		}
	}
	for (int i(1); i < 7; ++i) {
		switch (p_square[i][7]) {
			case 1:
				++p_sides1[2];
				break;
			case 2:
				++p_sides2[2];
				break;
		}
	}
	for (int j(1); j < 7; ++j) {
		switch (p_square[7][j]) {
			case 1:
				++p_sides1[3];
				break;
			case 2:
				++p_sides2[3];
				break;
		}
	}
	//�ԈႢ
	for (int i(0); i < 4; ++i) {
		if (corneropen[i] && corneropen[i + 1]) {
			if (p_sides1[i] == 6) {
				++*p_mountians1;
			}
			if (p_sides1[i] == 5) {
				++*p_wings1false;
			}
			if (p_sides2[i] == 6) {
				++*p_mountians2;
			}
			if (p_sides2[i] == 5) {
				++*p_wings2false;
			}
		}
	}
	
	for (int i(0); i < 4; ++i) {
		int next_i = i + 1 == 4 ? 0 : i + 1;
		if (corneropen[i] && corneropen[next_i]) {
			//if (p_sides1[i] == 6) {
			//	++*p_mountians1;
			//}
			if (p_sides1[i] == 5 && bycorneropen[i]) {
				++*p_wings1;
			}
			//if (p_sides2[i] == 6) {
			//	++*p_mountians2;
			//}
			if (p_sides2[i] == 5 && bycorneropen[i]) {
				++*p_wings2;
			}
		}
	}
}

void Board::calculate_mowi()
{
	int *p_mountians1(&mountains1);
	int *p_mountians2(&mountains2);
	//�ԈႢ
	int *p_wings1false(&wings1false);
	int *p_wings2false(&wings2false);
	int *p_wings1(&wings1);
	int *p_wings2(&wings2);
	int(*p_square)[8](square);
	int *p_sides1(sides1);
	int *p_sides2(sides2);
	*p_mountians1 = 0;
	*p_mountians2 = 0;
	*p_wings1 = 0;
	*p_wings2 = 0;
	*p_wings1false = 0;
	*p_wings2false = 0;
	bool corneropen[4] = { 0, 0, 0, 0 };
	bool bycorneropen[4] = { 0, 0, 0, 0 };
	if (p_square[7][0] == 0) {
		corneropen[0] = true;
	}
	if (p_square[0][0] == 0) {
		corneropen[1] = true;
	}
	if (p_square[0][7] == 0) {
		corneropen[2] = true;
	}
	if (p_square[7][7] == 0) {
		corneropen[3] = true;
	}
	if (p_square[6][0] == 0 || p_square[1][0] == 0) {
		bycorneropen[0] = true;
	}
	if (p_square[0][1] == 0 || p_square[0][6] == 0) {
		bycorneropen[1] = true;
	}
	if (p_square[1][7] == 0 || p_square[6][7] == 0) {
		bycorneropen[2] = true;
	}
	if (p_square[7][6] == 0 || p_square[7][1] == 0) {
		bycorneropen[3] = true;
	}
	
	for (int i(0); i < 4; ++i) {
		p_sides1[i] = 0;
		p_sides2[i] = 0;
	}
	for (int i(1); i < 7; ++i) {
		switch (p_square[i][0]) {
			case 1:
				++p_sides1[0];
				break;
			case 2:
				++p_sides2[0];
				break;
		}
	}
	for (int j(1); j < 7; ++j) {
		switch (p_square[0][j]) {
			case 1:
				++p_sides1[1];
				break;
			case 2:
				++p_sides2[1];
				break;
		}
	}
	for (int i(1); i < 7; ++i) {
		switch (p_square[i][7]) {
			case 1:
				++p_sides1[2];
				break;
			case 2:
				++p_sides2[2];
				break;
		}
	}
	for (int j(1); j < 7; ++j) {
		switch (p_square[7][j]) {
			case 1:
				++p_sides1[3];
				break;
			case 2:
				++p_sides2[3];
				break;
		}
	}
	//�ԈႢ
	for (int i(0); i < 4; ++i) {
		if (corneropen[i] && corneropen[i + 1]) {
			if (p_sides1[i] == 6) {
				++*p_mountians1;
			}
			if (p_sides1[i] == 5) {
				++*p_wings1false;
			}
			if (p_sides2[i] == 6) {
				++*p_mountians2;
			}
			if (p_sides2[i] == 5) {
				++*p_wings2false;
			}
		}
	}
	
	for (int i(0); i < 4; ++i) {
		if (corneropen[i] && corneropen[i + 1]) {
			//if (p_sides1[i] == 6) {
			//	++*p_mountians1;
			//}
			if (p_sides1[i] == 5 && bycorneropen) {
				++*p_wings1;
			}
			//if (p_sides2[i] == 6) {
			//	++*p_mountians2;
			//}
			if (p_sides2[i] == 5 && bycorneropen) {
				++*p_wings2;
			}
		}
	}
}

void Board::calculate_mwb4()
{
	int *p_mountain(mountain);
	int *p_wing(wing);
	int *p_block(block);
	int(*p_square)[8](square);
	int *p_sides1(sides1);
	int *p_sides2(sides2);
	
	bool corneropen[4] = { 0, 0, 0, 0 };
	int bycorneropen[4] = { 0, 0, 0, 0 };
	if (p_square[7][0] == 0) {
		corneropen[0] = true;
	}
	if (p_square[0][0] == 0) {
		corneropen[1] = true;
	}
	if (p_square[0][7] == 0) {
		corneropen[2] = true;
	}
	if (p_square[7][7] == 0) {
		corneropen[3] = true;
	}
	
	if (p_square[6][0] == 0) {
		++bycorneropen[0];
	}
	if (p_square[1][0] == 0) {
		++bycorneropen[0];
	}
	if (p_square[0][1] == 0) {
		++bycorneropen[1];
	}
	if (p_square[0][6] == 0) {
		++bycorneropen[1];
	}
	if (p_square[1][7] == 0) {
		++bycorneropen[2];
	}
	if (p_square[6][7] == 0) {
		++bycorneropen[2];
	}
	if (p_square[7][6] == 0) {
		++bycorneropen[3];
	}
	if (p_square[7][1] == 0) {
		++bycorneropen[3];
	}
	
	
	for (int i(0); i < 4; ++i) {
		p_sides1[i] = 0;
		p_sides2[i] = 0;
	}
	for (int i(1); i < 7; ++i) {
		switch (p_square[i][0]) {
			case 1:
				++p_sides1[0];
				break;
			case 2:
				++p_sides2[0];
				break;
		}
	}
	for (int j(1); j < 7; ++j) {
		switch (p_square[0][j]) {
			case 1:
				++p_sides1[1];
				break;
			case 2:
				++p_sides2[1];
				break;
		}
	}
	for (int i(1); i < 7; ++i) {
		switch (p_square[i][7]) {
			case 1:
				++p_sides1[2];
				break;
			case 2:
				++p_sides2[2];
				break;
		}
	}
	for (int j(1); j < 7; ++j) {
		switch (p_square[7][j]) {
			case 1:
				++p_sides1[3];
				break;
			case 2:
				++p_sides2[3];
				break;
		}
	}
	
	for (int i(0); i < 3; ++i) {
		if (corneropen[i] && corneropen[i + 1]) {
			if (p_sides1[i] == 6) {
				p_mountain[i] = 1;
				p_wing[i] = 0;
				p_block[i] = 0;
			}
			else if (p_sides2[i] == 6) {
				p_mountain[i] = -1;
				p_wing[i] = 0;
				p_block[i] = 0;
			}
			else {
				p_mountain[i] = 0;
				if (bycorneropen[i] == 1) {
					p_block[i] = 0;
					if (p_sides1[i] == 5) {
						p_wing[i] = 1;
					}
					else if (p_sides2[i] == 5) {
						p_wing[i] = -1;
					}
					else {
						p_wing[i] = 0;
					}
				}
				else {
					p_wing[i] = 0;
					if (bycorneropen[i] == 2) {
						if (p_sides1[i] == 4) {
							p_block[i] = 1;
						}
						else if (p_sides2[i] == 4) {
							p_block[i] = -1;
						}
						else {
							p_block[i] = 0;
						}
					}
					else {
						p_block[i] = 0;
					}
				}
			}
		}
		else {
			p_mountain[i] = 0;
			p_wing[i] = 0;
			p_block[i] = 0;
		}
	}
	
	int i = 3;
	if (corneropen[i] && corneropen[0]) {
		if (p_sides1[i] == 6) {
			p_mountain[i] = 1;
			p_wing[i] = 0;
			p_block[i] = 0;
		}
		else if (p_sides2[i] == 6) {
			p_mountain[i] = -1;
			p_wing[i] = 0;
			p_block[i] = 0;
		}
		else {
			p_mountain[i] = 0;
			if (bycorneropen[i] == 1) {
				p_block[i] = 0;
				if (p_sides1[i] == 5) {
					p_wing[i] = 1;
				}
				else if (p_sides2[i] == 5) {
					p_wing[i] = -1;
				}
				else {
					p_wing[i] = 0;
				}
			}
			else {
				p_wing[i] = 0;
				if (bycorneropen[i] == 2) {
					if (p_sides1[i] == 4) {
						p_block[i] = 1;
					}
					else if (p_sides2[i] == 4) {
						p_block[i] = -1;
					}
					else {
						p_block[i] = 0;
					}
				}
				else {
					p_block[i] = 0;
				}
			}
		}
	}
	else {
		p_mountain[i] = 0;
		p_wing[i] = 0;
		p_block[i] = 0;
	}
	
}


void Board::calculate_patternsv2()
{
	int *p_mountain(mountain);
	int *p_wing8(wing8);
	int *p_block(block);
	int(*p_bomb)(bomb);
	int(*p_square)[8](square);
	int *p_sides1(sides1);
	int *p_sides2(sides2);
	int *p_safec(safec);
	int *p_turn(&turn);
	int *p_doublec(doublec);
	int *p_turnnot(&turnnot);
	bool(*p_moves)[8](moves);
	bool(*p_moves2)[8](moves2);
	int(*p_stoner)(stoner);
	
	
	for (int i(0); i < 4; ++i) {
		p_sides1[i] = 0;
		p_sides2[i] = 0;
		p_mountain[i] = 0;
		p_block[i] = 0;
		p_bomb[i] = 0;
	}
	for (int i(0); i < 8; ++i) {
		p_wing8[i] = 0;
		p_safec[i] = 0;
		p_stoner[i] = 0;
		p_doublec[i] = 0;
	}
	
	int frame[4][8];
	int mid[4][8];
	bool framemoves[4][8];
	bool framemoves2[4][8];
	int di(0);
	int dj(1);
	int starti(0);
	int startj(0);
	for (int i(0); i < 4; ++i) {
		for (int j(0); j < 8; ++j) {
			frame[i][j] = p_square[starti + j * di][startj + j * dj];
			framemoves[i][j] = p_moves[starti + j * di][startj + j * dj];
			framemoves2[i][j] = p_moves2[starti + j * di][startj + j * dj];
		}
		int temp = starti;
		starti = startj;
		startj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	di = 0;
	dj = 1;
	starti = 1;
	startj = 0;
	for (int i(0); i < 4; ++i) {
		for (int j(0); j < 8; ++j) {
			mid[i][j] = p_square[starti + j * di][startj + j * dj];
		}
		int temp = starti;
		starti = startj;
		startj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	
	int color1;
	int color2;
	int value;
	bool(*p_fmoves)[8](framemoves);
	bool(*p_fmoves2)[8](framemoves2);
	int itr(0);
	
	//�Ӑ�
	for (int i(0); i < 4; ++i) {
		for (int j(2); j < 6; ++j) {
			if (frame[i][j] == 1) {
				++p_sides1[i];
			}
			else if (frame[i][j] == 2) {
				++p_sides2[i];
			}
		}
	}
	
	//�R�A�E�C���O�A�u���b�N�A���e
	for (int i(0); i < 4; ++i) {
		if (frame[i][0] == 0 && frame[i][7] == 0) {
			if (frame[i][2] != 0) {
				if (frame[i][2] == 1) {
					color1 = 1;
					color2 = 2;
					value = 1;
				}
				else {
					color1 = 2;
					color2 = 1;
					value = -1;
				}
				if (frame[i][5] == color1) {
					if (frame[i][3] == color2 && frame[i][4] == color2 && frame[i][1] == 0 && frame[i][6] == 0) {
						p_bomb[i] = value;
					}
					
					bool serie(true);
					int j(3);
					while (j < 6 && serie) {
						if (frame[i][j] != color1) {
							serie = false;
						}
						++j;
					}
					if (serie) {
						if (frame[i][1] == color1) {
							if (frame[i][6] == color1) {
								p_mountain[i] = value;
							}
							else if (frame[i][6] == 0) {
								p_wing8[2 * i] = value;
							}
						}
						else if (frame[i][1] == 0) {
							if (frame[i][6] == color1) {
								p_wing8[2 * i + 1] = value;
							}
							else if (frame[i][6] == 0) {
								block[i] = value;
							}
						}
					}
				}
			}
		}
	}
	
	//�ӂ̖h�q
	for (int i(0); i < 4; ++i) {
		if (mountain[i] == 0) {
			int start(0);
			int d(0);
			if (frame[i][0] == 0 && frame[i][1] != 0) {
				start = 1;
				d = 1;
				itr = 0;
				if (frame[i][start] == *p_turn) {
					color1 = *p_turn;
					color2 = *p_turnnot;
					p_fmoves = framemoves
					;
					p_fmoves2 = framemoves2;
					value = 1;
				}
				else {
					color1 = *p_turnnot;
					color2 = *p_turn;
					p_fmoves = framemoves2;
					p_fmoves2 = framemoves;
					value = -1;
				}
				int j(start + d);
				int series(1);
				while (frame[i][j] == color1 && j < 8 && j >= 0) {
					++series;
					j += d;
				}
				if (series != 7) {
					if (frame[i][start + series * d] == 0 && !p_fmoves2[i][start + series * d]) {
						p_safec[2 * i + itr] = value;
					}
					else {
						if (frame[i][start + series * d] != 0 && color2 == *p_turn) {
							p_safec[2 * i + itr] = 5;
						}
						else {
							bool cont(true);
							int look(start + series * d + d);
							while (cont && look >= 0 && look < 8) {
								if (frame[i][look] != color2) {
									if (frame[i][look] == color1) {
										p_safec[2 * i + itr] = -5 * value;
									}
									else if (frame[i][look] == 0) {
										p_safec[2 * i + itr] = value;
									}
									cont = false;
								}
								look += d;
							}
							if (cont) {
								p_safec[2 * i + itr] = -5 * value;
							}
						}
					}
				}
			}
			if (frame[i][7] == 0 && frame[i][6] != 0) {
				start = 6;
				d = -1;
				itr = 1;
				if (frame[i][start] == *p_turn) {
					color1 = *p_turn;
					color2 = *p_turnnot;
					p_fmoves = framemoves;
					p_fmoves2 = framemoves2;
					value = 1;
				}
				else {
					color1 = *p_turnnot;
					color2 = *p_turn;
					p_fmoves = framemoves;
					p_fmoves2 = framemoves2;
					value = -1;
				}
				int j(start + d);
				int series(1);
				while (frame[i][j] == color1 && j < 8 && j >= 0) {
					++series;
					j += d;
				}
				if (series != 7) {
					if (frame[i][start + series * d] == 0 && !p_fmoves2[i][start + series * d]) {
						p_safec[2 * i + itr] = value;
					}
					else {
						if (frame[i][start + series * d] != 0 && color2 == *p_turn) {
							p_safec[2 * i + itr] = 5;
						}
						else {
							bool cont(true);
							int look(start + series * d + d);
							while (cont && look >= 0 && look < 8) {
								if (frame[i][look] != color2) {
									if (frame[i][look] == color1) {
										p_safec[2 * i + itr] = -5 * value;
									}
									else if (frame[i][look] == 0) {
										p_safec[2 * i + itr] = value;
									}
									cont = false;
								}
								look += d;
							}
							if (cont) {
								p_safec[2 * i + itr] = -5 * value;
							}
						}
					}
				}
			}
		}
	}
	
	//�o��C�ł�
	for (int i(0); i < 4; ++i) {
		if ((frame[i][0] == 0 || frame[i][7] == 0) && (p_sides1[i] + p_sides2[i]) == 3) {
			bool critical(false);
			if (frame[i][0] != 0 || frame[i][7] != 0) {
				critical = true;
			}
			if (frame[i][0] != frame[i][1] && frame[i][7] != frame[i][6]) {
				if (frame[i][1] != 0 && frame[i][6] != 0 && frame[i][1] != frame[i][6]) {
					if (frame[i][1] == *p_turn) {
						color1 = *p_turn;
						color2 = *p_turnnot;
						itr = 0;
					}
					else {
						color1 = *p_turnnot;
						color2 = *p_turn;
						itr = 1;
					}
					bool stage1(false);
					bool con(true);
					int j(2);
					int serie(1);
					while (j < 6 && con) {
						if (!stage1) {
							if (frame[i][j] == 0) {
								stage1 = true;
							}
							else if (frame[i][j] == color2) {
								con = false;
							}
							else {
								++serie;
							}
						}
						else {
							if (frame[i][j] != color2) {
								con = false;
							}
						}
						++j;
					}
					if (con) {
						if (critical) {
							if (framemoves[i][serie + 1]) {
								p_doublec[2 * i + itr] = 1;
							}
							else if (framemoves2[i][serie + 1]) {
								p_doublec[2 * i + itr] = -1;
							}
						}
						else {
							if (framemoves[i][serie + 1]) {
								p_doublec[2 * i + itr] += 1;
							}
							if (framemoves2[i][serie + 1]) {
								p_doublec[2 * i + itr] -= 1;
							}
						}
					}
				}
			}
		}
	}
	
	//�X�g�i�[
	for (int i(0); i < 4; ++i) {
		if (frame[i][0] == 0 && frame[i][7] == 0) {
			int start(2);
			int d(1);
			int virt(1);
			int virt2(-1);
			if (i == 3) {
				virt = -3;
			}
			if (i == 0) {
				virt2 = 3;
			}
			bool con(false);
			//���O
			if (frame[i][1] == *p_turn && frame[i][2] == *p_turn && frame[i][3] == *p_turn && frame[i][6] == 0 && mid[i + virt][1] == *p_turnnot) {
				color1 = *p_turn;
				color2 = *p_turnnot;
				int series(4);
				value = -1;
				itr = 0;
				if (frame[i][4] == color1 && frame[i][5] == color2) {
					con = true;
				}
				else if (frame[i][4] == color2 && frame[i][5] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
				
			}
			else if (frame[i][6] == *p_turn && frame[i][5] == *p_turn && frame[i][4] == *p_turn && frame[i][1] == 0 && mid[i + virt2][6] == *p_turnnot) {
				start = 5;
				d = -1;
				virt = -1;
				if (i == 0) {
					virt = 3;
				}
				itr = 1;
				color1 = *p_turn;
				color2 = *p_turnnot;
				int series(4);
				value = -1;
				if (frame[i][3] == color1 && frame[i][2] == color2) {
					con = true;
				}
				else if (frame[i][3] == color2 && frame[i][2] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
			}
			//���O��O
			else if (frame[i][1] == *p_turnnot && frame[i][2] == *p_turnnot && frame[i][3] == *p_turnnot && frame[i][5] == 0 && frame[i][6] == 0 && mid[i + virt][1] == *p_turn) {
				color1 = *p_turnnot;
				color2 = *p_turn;
				int series(4);
				value = 1;
				itr = 0;
				if (frame[i][4] == color1 && framemoves[i][5] == true) {
					con = true;
				}
				else if (framemoves[i][4] == true && frame[i][5] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
			}
			else if (frame[i][6] == *p_turnnot && frame[i][5] == *p_turnnot && frame[i][4] == *p_turnnot && frame[i][2] == 0 && frame[i][1] == 0 && mid[i + virt2][6] == *p_turn) {
				start = 5;
				d = -1;
				virt = -1;
				if (i == 0) {
					virt = 3;
				}
				itr = 1;
				color1 = *p_turnnot;
				color2 = *p_turn;
				int series(4);
				value = 1;
				itr = 0;
				if (frame[i][3] == color1 && framemoves[i][2] == true) {
					con = true;
				}
				else if (framemoves[i][3] == true && frame[i][2] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
			}
		}
	}
	
}


void Board::calculate_patterns()
{
	int *p_mountain(mountain);
	int *p_wing8(wing8);
	int *p_block(block);
	int(*p_bomb)(bomb);
	int(*p_square)[8](square);
	int *p_sides1(sides1);
	int *p_sides2(sides2);
	int *p_safec(safec);
	int *p_turn(&turn);
	int *p_doublec(doublec);
	int *p_turnnot(&turnnot);
	bool(*p_moves)[8](moves);
	bool(*p_moves2)[8](moves2);
	int(*p_stoner)(stoner);
	
	
	for (int i(0); i < 4; ++i) {
		p_sides1[i] = 0;
		p_sides2[i] = 0;
		p_mountain[i] = 0;
		p_block[i] = 0;
		p_bomb[i] = 0;
	}
	for (int i(0); i < 8; ++i) {
		p_wing8[i] = 0;
		p_safec[i] = 0;
		p_stoner[i] = 0;
		p_doublec[i] = 0;
	}
	
	int frame[4][8];
	int mid[4][8];
	bool framemoves[4][8];
	bool framemoves2[4][8];
	int di(0);
	int dj(1);
	int starti(0);
	int startj(0);
	for (int i(0); i < 4; ++i) {
		for (int j(0); j < 8; ++j) {
			frame[i][j] = p_square[starti + j * di][startj + j * dj];
			framemoves[i][j] = p_moves[starti + j * di][startj + j * dj];
			framemoves2[i][j] = p_moves2[starti + j * di][startj + j * dj];
		}
		int temp = starti;
		starti = startj;
		startj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	di = 0;
	dj = 1;
	starti = 1;
	startj = 0;
	for (int i(0); i < 4; ++i) {
		for (int j(0); j < 8; ++j) {
			mid[i][j] = p_square[starti + j * di][startj + j * dj];
		}
		int temp = starti;
		starti = startj;
		startj = 7 - temp;
		temp = di;
		di = dj;
		dj = -temp;
	}
	
	int color1;
	int color2;
	int value;
	bool(*p_fmoves)[8](framemoves);
	bool(*p_fmoves2)[8](framemoves2);
	int itr(0);
	
	//�Ӑ�
	for (int i(0); i < 4; ++i) {
		for (int j(2); j < 6; ++j) {
			if (frame[i][j] == 1) {
				++p_sides1[i];
			}
			else if (frame[i][j] == 2) {
				++p_sides2[i];
			}
		}
	}
	
	//�R�A�E�C���O�A�u���b�N�A���e
	for (int i(0); i < 4; ++i) {
		if (frame[i][0] == 0 && frame[i][7] == 0) {
			if (frame[i][2] != 0) {
				if (frame[i][2] == 1) {
					color1 = 1;
					color2 = 2;
					value = 1;
				}
				else {
					color1 = 2;
					color2 = 1;
					value = -1;
				}
				if (frame[i][5] == color1) {
					if (frame[i][3] == color2 && frame[i][4] == color2 && frame[i][1] == 0 && frame[i][6] == 0) {
						p_bomb[i] = value;
					}
					
					bool serie(true);
					int j(3);
					while (j < 6 && serie) {
						if (frame[i][j] != color1) {
							serie = false;
						}
						++j;
					}
					if (serie) {
						if (frame[i][1] == color1) {
							if (frame[i][6] == color1) {
								p_mountain[i] = value;
							}
							else if (frame[i][6] == 0) {
								p_wing8[2 * i] = value;
							}
						}
						else if (frame[i][1] == 0) {
							if (frame[i][6] == color1) {
								p_wing8[2 * i + 1] = value;
							}
							else if (frame[i][6] == 0) {
								block[i] = value;
							}
						}
					}
				}
			}
		}
	}
	
	//�ӂ̖h�q
	for (int i(0); i < 4; ++i) {
		if (mountain[i] == 0) {
			int start(0);
			int d(0);
			if (frame[i][0] == 0 && frame[i][1] != 0) {
				start = 1;
				d = 1;
				itr = 0;
				if (frame[i][start] == *p_turn) {
					color1 = *p_turn;
					color2 = *p_turnnot;
					p_fmoves = framemoves
					;
					p_fmoves2 = framemoves2;
					value = 1;
				}
				else {
					color1 = *p_turnnot;
					color2 = *p_turn;
					p_fmoves = framemoves2;
					p_fmoves2 = framemoves;
					value = -1;
				}
				int j(start + d);
				int series(1);
				while (frame[i][j] == color1 && j < 8 && j >= 0) {
					++series;
					j += d;
				}
				if (series != 7) {
					if (frame[i][start + series * d] == 0 && !p_fmoves2[i][start + series * d]) {
						p_safec[2 * i + itr] = value;
					}
					else {
						if (frame[i][start + series * d] != 0 && color2 == *p_turn) {
							p_safec[2 * i + itr] = 5;
						}
						else {
							bool cont(true);
							int look(start + series * d + d);
							while (cont && look >= 0 && look < 8) {
								if (frame[i][look] != color2) {
									if (frame[i][look] == color1) {
										p_safec[2 * i + itr] = -5 * value;
									}
									else if (frame[i][look] == 0) {
										p_safec[2 * i + itr] = value;
									}
									cont = false;
								}
								look += d;
							}
							if (cont) {
								p_safec[2 * i + itr] = -5 * value;
							}
						}
					}
				}
			}
			if (frame[i][7] == 0 && frame[i][6] != 0) {
				start = 6;
				d = -1;
				itr = 1;
				if (frame[i][start] == *p_turn) {
					color1 = *p_turn;
					color2 = *p_turnnot;
					p_fmoves = framemoves;
					p_fmoves2 = framemoves2;
					value = 1;
				}
				else {
					color1 = *p_turnnot;
					color2 = *p_turn;
					(*p_fmoves)[0] = framemoves;
					(*p_fmoves2)[0] = framemoves2;
					value = -1;
				}
				int j(start + d);
				int series(1);
				while (frame[i][j] == color1 && j < 8 && j >= 0) {
					++series;
					j += d;
				}
				if (series != 7) {
					if (frame[i][start + series * d] == 0 && !p_fmoves2[i][start + series * d]) {
						p_safec[2 * i + itr] = value;
					}
					else {
						if (frame[i][start + series * d] != 0 && color2 == *p_turn) {
							p_safec[2 * i + itr] = 5;
						}
						else {
							bool cont(true);
							int look(start + series * d + d);
							while (cont && look >= 0 && look < 8) {
								if (frame[i][look] != color2) {
									if (frame[i][look] == color1) {
										p_safec[2 * i + itr] = -5 * value;
									}
									else if (frame[i][look] == 0) {
										p_safec[2 * i + itr] = value;
									}
									cont = false;
								}
								look += d;
							}
							if (cont) {
								p_safec[2 * i + itr] = -5 * value;
							}
						}
					}
				}
			}
		}
	}
	
	//�o��C�ł�
	for (int i(0); i < 4; ++i) {
		if (frame[i][0] == 0 || frame[i][7] == 0 && (p_sides1[i] + p_sides2[i]) == 3) {
			bool critical(false);
			if (frame[i][0] != 0 || frame[i][7] != 0) {
				critical = true;
			}
			if (frame[i][0] != frame[i][1] && frame[i][7] != frame[i][6]) {
				if (frame[i][1] != 0 && frame[i][6] != 0 && frame[i][1] != frame[i][6]) {
					if (frame[i][1] == *p_turn) {
						color1 = *p_turn;
						color2 = *p_turnnot;
						itr = 0;
					}
					else {
						color1 = *p_turnnot;
						color2 = *p_turn;
						itr = 1;
					}
					bool stage1(false);
					bool con(true);
					int j(2);
					int serie(1);
					while (j < 6 && con) {
						if (!stage1) {
							if (frame[i][j] == 0) {
								stage1 = true;
							}
							else if (frame[i][j] == color2) {
								con = false;
							}
							else {
								++serie;
							}
						}
						else {
							if (frame[i][j] != color2) {
								con = false;
							}
						}
						++j;
					}
					if (con) {
						if (critical) {
							if (framemoves[i][serie + 1]) {
								p_doublec[2 * i + itr] = 1;
							}
							else if (framemoves2[i][serie + 1]) {
								p_doublec[2 * i + itr] = -1;
							}
						}
						else {
							if (framemoves[i][serie + 1]) {
								p_doublec[2 * i + itr] += 1;
							}
							if (framemoves2[i][serie + 1]) {
								p_doublec[2 * i + itr] -= 1;
							}
						}
					}
				}
			}
		}
	}
	
	//�X�g�i�[
	for (int i(0); i < 4; ++i) {
		if (frame[i][0] == 0 && frame[i][7] == 0) {
			int start(2);
			int d(1);
			int virt(1);
			int virt2(-1);
			if (i == 3) {
				virt = -3;
			}
			if (i == 0) {
				virt2 = 3;
			}
			bool con(false);
			//���O
			if (frame[i][1] == *p_turn && frame[i][2] == *p_turn && frame[i][3] == *p_turn && frame[i][6] == 0 && mid[i + virt][1] == *p_turnnot) {
				color1 = *p_turn;
				color2 = *p_turnnot;
				int series(4);
				value = -1;
				itr = 0;
				if (frame[i][4] == color1 && frame[i][5] == color2) {
					con = true;
				}
				else if (frame[i][4] == color2 && frame[i][5] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
				
			}
			else if (frame[i][6] == *p_turn && frame[i][5] == *p_turn && frame[i][4] == *p_turn && frame[i][1] == 0 && mid[i + virt2][6] == *p_turnnot) {
				start = 5;
				d = -1;
				virt = -1;
				if (i == 0) {
					virt = 3;
				}
				itr = 1;
				color1 = *p_turn;
				color2 = *p_turnnot;
				int series(4);
				value = -1;
				if (frame[i][3] == color1 && frame[i][2] == color2) {
					con = true;
				}
				else if (frame[i][3] == color2 && frame[i][2] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
			}
			//���O��O
			else if (frame[i][1] == *p_turnnot && frame[i][2] == *p_turnnot && frame[i][3] == *p_turnnot && frame[i][5] == 0 && frame[i][6] == 0 && mid[i + virt][1] == *p_turn) {
				color1 = *p_turnnot;
				color2 = *p_turn;
				int series(4);
				value = 1;
				itr = 0;
				if (frame[i][4] == color1 && framemoves[i][5] == true) {
					con = true;
				}
				else if (framemoves[i][4] == true && frame[i][5] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
			}
			else if (frame[i][6] == *p_turnnot && frame[i][5] == *p_turnnot && frame[i][4] == *p_turnnot && frame[i][2] == 0 && frame[i][1] == 0 && mid[i + virt2][6] == *p_turn) {
				start = 5;
				d = -1;
				virt = -1;
				if (i == 0) {
					virt = 3;
				}
				itr = 1;
				color1 = *p_turnnot;
				color2 = *p_turn;
				int series(4);
				value = 1;
				itr = 0;
				if (frame[i][3] == color1 && framemoves[i][2] == true) {
					con = true;
				}
				else if (framemoves[i][3] == true && frame[i][2] == 0) {
					con = true;
					series = 3;
				}
				if (con) {
					int j(start);
					p_stoner[2 * i + itr] = value;
					if (series == 4) {
						int con2(true);
						while (con2 && j >= 0 && j <= 7) {
							if (mid[i + virt][j] == 0) {
								con2 = false;
								p_stoner[2 * i + itr] = 0;
							}
							else if (mid[i + virt][j] == color1) {
								con2 = false;
							}
							j += d;
						}
						if (con2) {
							p_stoner[2 * i + itr] = 0;
						}
					}
					else {
						if (frame[i + virt][start] != color1) {
							p_stoner[2 * i + itr] = 0;
						}
					}
				}
			}
		}
	}
	
}

void Board::calculate_corners()
{
	int(*p_square)[8](square);
	int(*p_corner)(corner);
	int(*p_bycorner)(bycorner);
	int(*p_xcorner)(xcorner);
	
	if (turn == 1) {
		switch (p_square[0][0]) {
			case 1:
				p_corner[0] = 1;
				break;
			case 2:
				p_corner[0] = -1;
				break;
			default:
				p_corner[0] = 0;
				break;
		}
		switch (p_square[0][7]) {
			case 1:
				p_corner[1] = 1;
				break;
			case 2:
				p_corner[1] = -1;
				break;
			default:
				p_corner[1] = 0;
				break;
		}
		switch (p_square[7][7]) {
			case 1:
				p_corner[2] = 1;
				break;
			case 2:
				p_corner[2] = -1;
				break;
			default:
				p_corner[2] = 0;
				break;
		}
		switch (p_square[7][0]) {
			case 1:
				p_corner[3] = 1;
				break;
			case 2:
				p_corner[3] = -1;
				break;
			default:
				p_corner[3] = 0;
				break;
		}
		
		switch (p_square[1][1]) {
			case 1:
				p_xcorner[0] = 1;
				break;
			case 2:
				p_xcorner[0] = -1;
				break;
			default:
				p_xcorner[0] = 0;
				break;
		}
		switch (p_square[1][6]) {
			case 1:
				p_xcorner[1] = 1;
				break;
			case 2:
				p_xcorner[1] = -1;
				break;
			default:
				p_xcorner[1] = 0;
				break;
		}
		switch (p_square[6][6]) {
			case 1:
				p_xcorner[2] = 1;
				break;
			case 2:
				p_xcorner[2] = -1;
				break;
			default:
				p_xcorner[2] = 0;
				break;
		}
		switch (p_square[6][1]) {
			case 1:
				p_xcorner[3] = 1;
				break;
			case 2:
				p_xcorner[3] = -1;
				break;
			default:
				p_xcorner[3] = 0;
				break;
		}
		
		switch (p_square[1][0]) {
			case 1:
				p_bycorner[0] = 1;
				break;
			case 2:
				p_bycorner[0] = -1;
				break;
			default:
				p_bycorner[0] = 0;
				break;
		}
		switch (p_square[0][1]) {
			case 1:
				p_bycorner[1] = 1;
				break;
			case 2:
				p_bycorner[1] = -1;
				break;
			default:
				p_bycorner[1] = 0;
				break;
		}
		switch (p_square[0][6]) {
			case 1:
				p_bycorner[2] = 1;
				break;
			case 2:
				p_bycorner[2] = -1;
				break;
			default:
				p_bycorner[2] = 0;
				break;
		}
		switch (p_square[1][7]) {
			case 1:
				p_bycorner[3] = 1;
				break;
			case 2:
				p_bycorner[3] = -1;
				break;
			default:
				p_bycorner[3] = 0;
				break;
		}
		switch (p_square[6][7]) {
			case 1:
				p_bycorner[4] = 1;
				break;
			case 2:
				p_bycorner[4] = -1;
				break;
			default:
				p_bycorner[4] = 0;
				break;
		}
		switch (p_square[7][6]) {
			case 1:
				p_bycorner[5] = 1;
				break;
			case 2:
				p_bycorner[5] = -1;
				break;
			default:
				p_bycorner[5] = 0;
				break;
		}
		switch (p_square[7][1]) {
			case 1:
				p_bycorner[6] = 1;
				break;
			case 2:
				p_bycorner[6] = -1;
				break;
			default:
				p_bycorner[6] = 0;
				break;
		}
		switch (p_square[0][6]) {
			case 1:
				p_bycorner[7] = 1;
				break;
			case 2:
				p_bycorner[7] = -1;
				break;
			default:
				p_bycorner[7] = 0;
				break;
		}
	}
	else {
		switch (p_square[0][0]) {
			case 2:
				p_corner[0] = 1;
				break;
			case 1:
				p_corner[0] = -1;
				break;
			default:
				p_corner[0] = 0;
				break;
		}
		switch (p_square[0][7]) {
			case 2:
				p_corner[1] = 1;
				break;
			case 1:
				p_corner[1] = -1;
				break;
			default:
				p_corner[1] = 0;
				break;
		}
		switch (p_square[7][7]) {
			case 2:
				p_corner[2] = 1;
				break;
			case 1:
				p_corner[2] = -1;
				break;
			default:
				p_corner[2] = 0;
				break;
		}
		switch (p_square[7][0]) {
			case 2:
				p_corner[3] = 1;
				break;
			case 1:
				p_corner[3] = -1;
				break;
			default:
				p_corner[3] = 0;
				break;
		}
		
		switch (p_square[1][1]) {
			case 2:
				p_xcorner[0] = 1;
				break;
			case 1:
				p_xcorner[0] = -1;
				break;
			default:
				p_xcorner[0] = 0;
				break;
		}
		switch (p_square[1][6]) {
			case 2:
				p_xcorner[1] = 1;
				break;
			case 1:
				p_xcorner[1] = -1;
				break;
			default:
				p_xcorner[1] = 0;
				break;
		}
		switch (p_square[6][6]) {
			case 2:
				p_xcorner[2] = 1;
				break;
			case 1:
				p_xcorner[2] = -1;
				break;
			default:
				p_xcorner[2] = 0;
				break;
		}
		switch (p_square[6][1]) {
			case 2:
				p_xcorner[3] = 1;
				break;
			case 1:
				p_xcorner[3] = -1;
				break;
			default:
				p_xcorner[3] = 0;
				break;
		}
		
		switch (p_square[1][0]) {
			case 2:
				p_bycorner[0] = 1;
				break;
			case 1:
				p_bycorner[0] = -1;
				break;
			default:
				p_bycorner[0] = 0;
				break;
		}
		switch (p_square[0][1]) {
			case 2:
				p_bycorner[1] = 1;
				break;
			case 1:
				p_bycorner[1] = -1;
				break;
			default:
				p_bycorner[1] = 0;
				break;
		}
		switch (p_square[0][6]) {
			case 2:
				p_bycorner[2] = 1;
				break;
			case 1:
				p_bycorner[2] = -1;
				break;
			default:
				p_bycorner[2] = 0;
				break;
		}
		switch (p_square[1][7]) {
			case 2:
				p_bycorner[3] = 1;
				break;
			case 1:
				p_bycorner[3] = -1;
				break;
			default:
				p_bycorner[3] = 0;
				break;
		}
		switch (p_square[6][7]) {
			case 2:
				p_bycorner[4] = 1;
				break;
			case 1:
				p_bycorner[4] = -1;
				break;
			default:
				p_bycorner[4] = 0;
				break;
		}
		switch (p_square[7][6]) {
			case 2:
				p_bycorner[5] = 1;
				break;
			case 1:
				p_bycorner[5] = -1;
				break;
			default:
				p_bycorner[5] = 0;
				break;
		}
		switch (p_square[7][1]) {
			case 2:
				p_bycorner[6] = 1;
				break;
			case 1:
				p_bycorner[6] = -1;
				break;
			default:
				p_bycorner[6] = 0;
				break;
		}
		switch (p_square[0][6]) {
			case 2:
				p_bycorner[7] = 1;
				break;
			case 1:
				p_bycorner[7] = -1;
				break;
			default:
				p_bycorner[7] = 0;
				break;
		}
	}
	
}

void Board::calculate_framemoves()
{
	const int(*p_square)[8](square);
	const bool(*p_move)[8](moves);
	const bool(*p_move2)[8](moves2);
	int(*p_cornermove)(cornermove);
	int(*p_cornermove2)(cornermove2);
	int(*p_bycornermove)(bycornermove);
	int(*p_bycornermove2)(bycornermove2);
	int(*p_xcornermove)(xcornermove);
	int(*p_xcornermove2)(xcornermove2);
	int(*p_sidemoves)(sidemoves);
	int(*p_sidemoves2)(sidemoves2);
	
	//���̉��x1
	if (p_move[0][0]) {
		p_cornermove[0] = 1;
	}
	else {
		p_cornermove[0] = 0;
	}
	if (p_move[0][7]) {
		p_cornermove[1] = 1;
	}
	else {
		p_cornermove[1] = 0;
	}
	if (p_move[7][7]) {
		p_cornermove[2] = 1;
	}
	else {
		p_cornermove[2] = 0;
	}
	if (p_move[7][0]) {
		p_cornermove[3] = 1;
	}
	else {
		p_cornermove[3] = 0;
	}
	
	//���̉��x2
	if (p_move2[0][0]) {
		p_cornermove2[0] = 1;
	}
	else {
		p_cornermove2[0] = 0;
	}
	if (p_move[0][7]) {
		p_cornermove2[1] = 1;
	}
	else {
		p_cornermove2[1] = 0;
	}
	if (p_move2[7][7]) {
		p_cornermove2[2] = 1;
	}
	else {
		p_cornermove2[2] = 0;
	}
	if (p_move2[7][0]) {
		p_cornermove2[3] = 1;
	}
	else {
		p_cornermove2[3] = 0;
	}
	
	//���ׂ̉��x1
	if (p_move[1][0]) {
		p_bycornermove[0] = 1;
	}
	else {
		p_bycornermove[0] = 0;
	}
	if (p_move[0][1]) {
		p_bycornermove[1] = 1;
	}
	else {
		p_bycornermove[1] = 0;
	}
	if (p_move[0][6]) {
		p_bycornermove[2] = 1;
	}
	else {
		p_bycornermove[2] = 0;
	}
	if (p_move[1][7]) {
		p_bycornermove[3] = 1;
	}
	else {
		p_bycornermove[3] = 0;
	}
	if (p_move[6][7]) {
		p_bycornermove[4] = 1;
	}
	else {
		p_bycornermove[4] = 0;
	}
	if (p_move[7][6]) {
		p_bycornermove[5] = 1;
	}
	else {
		p_bycornermove[5] = 0;
	}
	if (p_move[7][1]) {
		p_bycornermove[6] = 1;
	}
	else {
		p_bycornermove[6] = 0;
	}
	if (p_move[6][0]) {
		p_bycornermove[7] = 1;
	}
	else {
		p_bycornermove[7] = 0;
	}
	
	//���ׂ̉��x2
	if (p_move2[1][0]) {
		p_bycornermove2[0] = 1;
	}
	else {
		p_bycornermove2[0] = 0;
	}
	if (p_move2[0][1]) {
		p_bycornermove2[1] = 1;
	}
	else {
		p_bycornermove2[1] = 0;
	}
	if (p_move2[0][6]) {
		p_bycornermove2[2] = 1;
	}
	else {
		p_bycornermove2[2] = 0;
	}
	if (p_move2[1][7]) {
		p_bycornermove2[3] = 1;
	}
	else {
		p_bycornermove2[3] = 0;
	}
	if (p_move2[6][7]) {
		p_bycornermove2[4] = 1;
	}
	else {
		p_bycornermove2[4] = 0;
	}
	if (p_move2[7][6]) {
		p_bycornermove2[5] = 1;
	}
	else {
		p_bycornermove2[5] = 0;
	}
	if (p_move2[7][1]) {
		p_bycornermove2[6] = 1;
	}
	else {
		p_bycornermove2[6] = 0;
	}
	if (p_move2[6][0]) {
		p_bycornermove2[7] = 1;
	}
	else {
		p_bycornermove2[7] = 0;
	}
	
	//X�̉��x1
	if (p_move[1][1]) {
		p_xcornermove[0] = 1;
	}
	else {
		p_xcornermove[0] = 0;
	}
	if (p_move[1][6]) {
		p_xcornermove[1] = 1;
	}
	else {
		p_xcornermove[1] = 0;
	}
	if (p_move[6][6]) {
		p_xcornermove[2] = 1;
	}
	else {
		p_xcornermove[2] = 0;
	}
	if (p_move[6][1]) {
		p_xcornermove[3] = 1;
	}
	else {
		p_xcornermove[3] = 0;
	}
	
	//X�̉��x2
	if (p_move2[1][1]) {
		p_xcornermove2[0] = 1;
	}
	else {
		p_xcornermove2[0] = 0;
	}
	if (p_move2[1][6]) {
		p_xcornermove2[1] = 1;
	}
	else {
		p_xcornermove2[1] = 0;
	}
	if (p_move2[6][6]) {
		p_xcornermove2[2] = 1;
	}
	else {
		p_xcornermove2[2] = 0;
	}
	if (p_move2[6][1]) {
		p_xcornermove2[3] = 1;
	}
	else {
		p_xcornermove2[3] = 0;
	}
	
	//�ӂ̉��x������
	for (int i(0); i < 4; ++i) {
		p_sidemoves[i] = 0;
		p_sidemoves2[i] = 0;
	}
	
	//�ӂ̉��x1
	for (int i(2); i < 6; ++i) {
		if (moves[0][i]) {
			++p_sidemoves[0];
		}
	}
	for (int i(2); i < 6; ++i) {
		if (moves[i][7]) {
			++p_sidemoves[1];
		}
	}
	for (int i(2); i < 6; ++i) {
		if (moves[7][i]) {
			++p_sidemoves[2];
		}
	}
	for (int i(2); i < 6; ++i) {
		if (moves[i][0]) {
			++p_sidemoves[3];
		}
	}
	
	//�ӂ̉��x2
	for (int i(2); i < 6; ++i) {
		if (moves2[0][i]) {
			++p_sidemoves2[0];
		}
	}
	for (int i(2); i < 6; ++i) {
		if (moves2[i][7]) {
			++p_sidemoves2[1];
		}
	}
	for (int i(2); i < 6; ++i) {
		if (moves2[7][i]) {
			++p_sidemoves2[2];
		}
	}
	for (int i(2); i < 6; ++i) {
		if (moves2[i][0]) {
			++p_sidemoves2[3];
		}
	}
}


void Board::calculate_lines()
{
	int(*p_whiteline)(whiteline);
	int(*p_blackline)(blackline);
	int(*p_midsides1)(midsides1);
	int(*p_midsides2)(midsides2);
	int(*p_square)[8](square);
	int(*p_turn)(&turn);
	int(*p_turnnot)(&turnnot);
	
	for (int i(0); i < 4; ++i) {
		p_midsides1[i] = 0;
		p_midsides2[i] = 0;
	}
	
	for (int i(2); i < 6; ++i) {
		switch (p_square[i][i]) {
			case 1:
				p_whiteline[i - 2] = 1;
				break;
			case 2:
				p_whiteline[i - 2] = -1;
				break;
			default:
				p_whiteline[i - 2] = 0;
				break;
		}
		switch (p_square[7 - i][i]) {
			case 1:
				p_blackline[i - 2] = 1;
				break;
			case 2:
				p_blackline[i - 2] = -1;
				break;
			default:
				p_blackline[i - 2] = 0;
				break;
		}
		switch (p_square[i][1]) {
			case 1:
				++p_midsides1[0];
				break;
			case 2:
				++p_midsides2[0];
				break;
		}
		switch (p_square[1][i]) {
			case 1:
				++p_midsides1[1];
				break;
			case 2:
				++p_midsides2[1];
				break;
		}
		switch (p_square[i][6]) {
			case 1:
				++p_midsides1[2];
				break;
			case 2:
				++p_midsides2[2];
				break;
		}
		switch (p_square[6][i]) {
			case 1:
				++p_midsides1[3];
				break;
			case 2:
				++p_midsides2[3];
				break;
		}
	}
}

void Board::calculate_frameopenness()
{
	const int(*p_square)[8](square);
	const int(*p_openness1)[8](openness1);
	const int(*p_openness2)[8](openness2);
	int(*p_corneropenness1)(corneropenness1);
	int(*p_corneropenness2)(corneropenness2);
	int(*p_bycorneropenness1)(bycorneropenness1);
	int(*p_bycorneropenness2)(bycorneropenness2);
	int(*p_xcorneropenness1)(xcorneropenness1);
	int(*p_xcorneropenness2)(xcorneropenness2);
	int(*p_sideopenness1)(sideopenness1);
	int(*p_sideopenness2)(sideopenness2);
	
	//���̊J���x
	p_corneropenness1[0] = p_openness1[0][0];
	p_corneropenness1[1] = p_openness1[0][7];
	p_corneropenness1[2] = p_openness1[7][7];
	p_corneropenness1[3] = p_openness1[7][0];
	
	p_corneropenness2[0] = p_openness2[0][0];
	p_corneropenness2[1] = p_openness2[0][7];
	p_corneropenness2[2] = p_openness2[7][7];
	p_corneropenness2[3] = p_openness2[7][0];
	
	
	//���ׂ̊J���x
	p_bycorneropenness1[0] = p_openness1[1][0];
	p_bycorneropenness1[1] = p_openness1[0][1];
	p_bycorneropenness1[2] = p_openness1[0][6];
	p_bycorneropenness1[3] = p_openness1[1][7];
	p_bycorneropenness1[4] = p_openness1[6][7];
	p_bycorneropenness1[5] = p_openness1[7][6];
	p_bycorneropenness1[6] = p_openness1[7][1];
	p_bycorneropenness1[7] = p_openness1[6][0];
	
	p_bycorneropenness2[0] = p_openness2[1][0];
	p_bycorneropenness2[1] = p_openness2[0][1];
	p_bycorneropenness2[2] = p_openness2[0][6];
	p_bycorneropenness2[3] = p_openness2[1][7];
	p_bycorneropenness2[4] = p_openness2[6][7];
	p_bycorneropenness2[5] = p_openness2[7][6];
	p_bycorneropenness2[6] = p_openness2[7][1];
	p_bycorneropenness2[7] = p_openness2[6][0];
	
	//X�̊J���x
	p_xcorneropenness1[0] = p_openness1[1][1];
	p_xcorneropenness1[1] = p_openness1[1][6];
	p_xcorneropenness1[2] = p_openness1[6][6];
	p_xcorneropenness1[3] = p_openness1[6][1];
	
	p_xcorneropenness2[0] = p_openness2[1][1];
	p_xcorneropenness2[1] = p_openness2[1][6];
	p_xcorneropenness2[2] = p_openness2[6][6];
	p_xcorneropenness2[3] = p_openness2[6][1];
	
	//�ӂ̊J���x
	for (int i(0); i < 4; ++i) {
		p_sideopenness1[i] = 0;
		p_sideopenness2[i] = 0;
	}
	for (int i(2); i < 6; ++i) {
		p_sideopenness1[0] += p_openness1[0][i];
		p_sideopenness2[0] += p_openness2[0][i];
		p_sideopenness1[1] += p_openness1[i][7];
		p_sideopenness2[1] += p_openness2[i][7];
		p_sideopenness1[2] += p_openness1[7][i];
		p_sideopenness2[2] += p_openness2[7][i];
		p_sideopenness1[3] += p_openness1[i][0];
		p_sideopenness2[3] += p_openness2[i][0];
	}
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
	const int(*p_square)[8](square);
	int *p_spots_openness1(spots_openness1);
	int *p_spots_openness2(spots_openness2);
	int(*p_openness1)[8](openness1);
	int(*p_openness2)[8](openness2);
	for (int n(i1); n <= i2; ++n) {
		for (int m(j1); m <= j2; ++m) {
			if (p_square[n][m] == 1) {
				++p_spots_openness1[k];
				++p_openness1[i][j];
			}
			else if (p_square[n][m] == 2) {
				++p_spots_openness2[k];
				++p_openness2[i][j];
			}
		}
	}
}

void Board::opennessv2(int i, int j, int k) {
	int i1;
	int i2;
	int j1;
	int j2;
	if (i - 1 < 0) {
		i1 = 0;
	}
	else if (i - 2 < 0) {
		i1 = 1;
	}
	else {
		i1 = i - 1;
	}
	if (j - 1 < 0) {
		j1 = 0;
	}
	else if (j - 2 < 0) {
		j1 = 1;
	}
	else {
		j1 = j - 1;
	}
	if (i + 1 > 7) {
		i2 = 7;
	}
	else if (i + 2 > 7) {
		i2 = 6;
	}
	else {
		i2 = i + 1;
	}
	if (j + 1 > 7) {
		j2 = 7;
	}
	else if (j + 2 > 7) {
		j2 = 6;
	}
	else {
		j2 = j + 1;
	}
	const int(*p_square)[8](square);
	int *p_spots_openness1(spots_openness1);
	int *p_spots_openness2(spots_openness2);
	int(*p_openness1)[8](openness1);
	int(*p_openness2)[8](openness2);
	for (int n(i1); n <= i2; ++n) {
		for (int m(j1); m <= j2; ++m) {
			if (p_square[n][m] == 1) {
				++p_spots_openness1[k];
				++p_openness1[i][j];
			}
			else if (p_square[n][m] == 2) {
				++p_spots_openness2[k];
				++p_openness2[i][j];
			}
		}
	}
}

void Board::opennessv3(int i, int j, int k) {
	int i1;
	int i2;
	int j1;
	int j2;
	if (i == 1) {
		i1 = 1;
	}
	else if (i == 0) {
		i1 = 0;
	}
	else {
		i1 = i - 1;
	}
	if (j == 1) {
		j1 = 1;
	}
	else if (j == 0) {
		j1 = 0;
	}
	else {
		j1 = j - 1;
	}
	if (i == 7) {
		i2 = 7;
	}
	else if (i == 6) {
		i2 = 6;
	}
	else {
		i2 = i + 1;
	}
	if (j == 7) {
		j2 = 7;
	}
	else if (j == 6) {
		j2 = 6;
	}
	else {
		j2 = j + 1;
	}
	const int(*p_square)[8](square);
	int *p_spots_openness1(spots_openness1);
	int *p_spots_openness2(spots_openness2);
	int(*p_openness1)[8](openness1);
	int(*p_openness2)[8](openness2);
	for (int n(i1); n <= i2; ++n) {
		for (int m(j1); m <= j2; ++m) {
			if (p_square[n][m] == 1) {
				++p_spots_openness1[k];
				++p_openness1[i][j];
			}
			else if (p_square[n][m] == 2) {
				++p_spots_openness2[k];
				++p_openness2[i][j];
			}
		}
	}
}


void Board::calculate_spaces() {
	
	int(*p_square)[8](square);
	int(*p_space)(space);
	int(*p_group)[8](group);
	int(*p_spaces)(&spaces);
	int(*p_ebeos)(&ebeos);
	int(*p_hypereven)(&hypereven);
	bool(*p_space_move)(space_move);
	bool(*p_space_move2)(space_move2);
	bool(*p_moves)[8](moves);
	bool(*p_moves2)[8](moves2);
	
	for (int i(0); i < 20; ++i) {
		p_space[i] = 0;
		space_move[i] = false;
		space_move2[i] = false;
	}
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			p_group[i][j] = 0;
		}
	}
	*p_spaces = 0;
	*p_ebeos = 0;
	*p_hypereven = 0;
	
	if (disks1 + disks2 < 40) {
		return;
	}
	if ((disks1 + disks2) % 2 != 0) {
		return;
	}
	
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (p_square[i][j] == 0) {
				if (p_group[i][j] == 0) {
					++*p_spaces;
					int k = *p_spaces;
					group[i][j] = k;
					++p_space[k - 1];
					if (p_moves[i][j]) {
						space_move[*p_spaces] = true;
					}
					if (p_moves2[i][j]) {
						space_move2[*p_spaces] = true;
					}
					spread(i, j, k);
				}
				
				
			}
		}
	}
	
	for (int i(0); i < *p_spaces; ++i) {
		if (space_move[i] && !space_move2[i] && p_space[i] % 2 != 0) {
			*p_ebeos = 1;
			break;
		}
	}
	
	for (int i(0); i < *p_spaces; ++i) {
		if (!space_move[i] && space_move2[i] && p_space[i] % 2 == 0) {
			*p_hypereven = 1;
			break;
		}
	}
	
}

void Board::spread(int i, int j, int k)
{
	int(*p_square)[8](square);
	int(*p_space)(space);
	int(*p_group)[8](group);
	bool(*p_space_move)(space_move);
	bool(*p_space_move2)(space_move2);
	bool(*p_moves)[8](moves);
	bool(*p_moves2)[8](moves2);
	int starti(i - 1);
	int endi(i + 1);
	int startj(j - 1);
	int endj(j + 1);
	if (i - 1 < 0) {
		starti = 0;
	}
	if (i + 1 > 7) {
		endi = 7;
	}
	if (j - 1 < 0) {
		startj = 0;
	}
	if (j + 1 > 7) {
		endj = 7;
	}
	int si(starti);
	for (int si(starti); si <= endi; ++si) {
		for (int sj(startj); sj <= endj; ++sj) {
			if (p_square[si][sj] == 0) {
				if (p_group[si][sj] == 0) {
					p_group[si][sj] = k;
					++p_space[k - 1];
					if (p_moves[si][sj]) {
						space_move[k] = true;
					}
					if (p_moves2[si][sj]) {
						space_move2[k] = true;
					}
					spread(si, sj, k);
				}
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


void Board::playerturnboard()
{
	if (turn == 2) {
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
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

vector<double> Board::x74()
{
	calculate_spots();
	calculate_spots_moves();
	calculate_spots_fixed();
	calculate_openness();
	vector<double> v(74);
	int n(0);
	if (turn == 1) {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		//v = spots1;
		//copy(spots2.begin(), spots2.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		//v = spots2;
		//copy(spots1.begin(), spots1.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	
	//vector<double> x(v.size());
	//for (unsigned int i(0);i < v.size();++i) {
	//	x[i] = double(v[i]);
	//}
	return v;
}

vector<double> Board::x78()
{
	calculate_spots();
	calculate_spots_moves();
	calculate_spots_fixed();
	calculate_opennessf();
	calculate_mowi();
	vector<double> v(78);
	int n(0);
	if (turn == 1) {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		v[n] = (double)mountains1;
		++n;
		v[n] = (double)mountains2;
		++n;
		v[n] = (double)wings1;
		++n;
		v[n] = (double)wings2;
		++n;
		//v = spots1;
		//copy(spots2.begin(), spots2.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		v[n] = (double)mountains2;
		++n;
		v[n] = (double)mountains1;
		++n;
		v[n] = (double)wings2;
		++n;
		v[n] = (double)wings1;
		++n;
		//v = spots2;
		//copy(spots1.begin(), spots1.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	
	//vector<double> x(v.size());
	//for (unsigned int i(0);i < v.size();++i) {
	//	x[i] = double(v[i]);
	//}
	return v;
}

vector<double> Board::x102false()
{
	calculate_spots();
	calculate_spots_moves();
	calculate_spots_fixed();
	calculate_opennessf();
	calculate_mowi();
	calculate_corners();
	vector<double> v(102);
	int n(0);
	if (turn == 1) {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		v[n] = (double)mountains1;
		++n;
		v[n] = (double)mountains2;
		++n;
		v[n] = (double)wings1false;
		++n;
		v[n] = (double)wings2false;
		++n;
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides2[i];
			++n;
		}
		//v = spots1;
		//copy(spots2.begin(), spots2.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		v[n] = (double)mountains2;
		++n;
		v[n] = (double)mountains1;
		++n;
		v[n] = (double)wings2false;
		++n;
		v[n] = (double)wings1false;
		++n;
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides1[i];
			++n;
		}
		//v = spots2;
		//copy(spots1.begin(), spots1.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)corner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)xcorner[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)bycorner[i];
		++n;
	}
	
	//vector<double> x(v.size());
	//for (unsigned int i(0);i < v.size();++i) {
	//	x[i] = double(v[i]);
	//}
	return v;
}

vector<double> Board::x102()
{
	calculate_spots();
	calculate_spots_moves();
	calculate_spots_fixed();
	calculate_opennessf();
	calculate_mowi();
	calculate_corners();
	vector<double> v(102);
	int n(0);
	if (turn == 1) {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		v[n] = (double)mountains1;
		++n;
		v[n] = (double)mountains2;
		++n;
		v[n] = (double)wings1;
		++n;
		v[n] = (double)wings2;
		++n;
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides2[i];
			++n;
		}
		//v = spots1;
		//copy(spots2.begin(), spots2.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots2[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)spots1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)spots_openness1[i];
			++n;
		}
		v[n] = (double)mountains2;
		++n;
		v[n] = (double)mountains1;
		++n;
		v[n] = (double)wings2;
		++n;
		v[n] = (double)wings1;
		++n;
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)sides1[i];
			++n;
		}
		//v = spots2;
		//copy(spots1.begin(), spots1.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)corner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)xcorner[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)bycorner[i];
		++n;
	}
	
	//vector<double> x(v.size());
	//for (unsigned int i(0);i < v.size();++i) {
	//	x[i] = double(v[i]);
	//}
	return v;
}

vector<double> Board::x206()
{
	calculate_spots();
	calculate_spots_moves();
	calculate_spots_fixed();
	calculate_openness();
	calculate_mwb4();
	calculate_lines();
	calculate_frameopenness();
	calculate_corners();
	calculate_framemoves();
	vector<double> v(206);
	
	int(*p_spots1)(spots1);
	int(*p_spots2)(spots2);
	int(*p_spots_moves1)(spots_moves1);
	int(*p_spots_moves2)(spots_moves2);
	int(*p_spots_fixed1)(spots_fixed1);
	int(*p_spots_fixed2)(spots_fixed2);
	int(*p_spots_openness1)(spots_openness1);
	int(*p_spots_openness2)(spots_openness2);
	int(*p_mountain)(mountain);
	int(*p_wing)(wing);
	int(*p_block)(block);
	int(*p_sides1)(sides1);
	int(*p_sides2)(sides1);
	int(*p_whiteline)(whiteline);
	int(*p_blackline)(blackline);
	int(*p_midsides1)(midsides1);
	int(*p_midsides2)(midsides2);
	
	int(*p_corneropenness1)(corneropenness1);
	int(*p_corneropenness2)(corneropenness2);
	int(*p_bycorneropenness1)(bycorneropenness1);
	int(*p_bycorneropenness2)(bycorneropenness2);
	int(*p_xcorneropenness1)(xcorneropenness1);
	int(*p_xcorneropenness2)(xcorneropenness2);
	int(*p_sideopenness1)(sideopenness1);
	int(*p_sideopenness2)(sideopenness2);
	
	
	int(*p_corner)(corner);
	int(*p_bycorner)(bycorner);
	int(*p_xcorner)(xcorner);
	
	
	int(*p_cornermove)(cornermove);
	int(*p_bycornermove)(bycornermove);
	int(*p_xcornermove)(xcornermove);
	int(*p_cornermove2)(cornermove2);
	int(*p_bycornermove2)(bycornermove2);
	int(*p_xcornermove2)(xcornermove2);
	int(*p_sidemoves)(sidemoves);
	int(*p_sidemoves2)(sidemoves2);
	
	int n(0);
	if (turn == 1) {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots1[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_mountain[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_wing[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_block[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_whiteline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_blackline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness2[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness1[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness2[i];
			++n;
		}
		//v = spots1;
		//copy(spots2.begin(), spots2.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots2[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_moves1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_moves2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_mountain[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_wing[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_block[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_whiteline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_blackline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness1[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness2[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness1[i];
			++n;
		}
		//v = spots2;
		//copy(spots1.begin(), spots1.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_corner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcorner[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycorner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_cornermove[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_cornermove2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcornermove[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcornermove2[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycornermove[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycornermove2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_sidemoves[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_sidemoves2[i];
		++n;
	}
	//vector<double> x(v.size());
	//for (unsigned int i(0);i < v.size();++i) {
	//	x[i] = double(v[i]);
	//}
	return v;
}

vector<double> Board::x266()
{
	calculate_spots();
	calculate_spots_exmoves();
	calculate_spots_fixedv2();
	calculate_opennessv2();
	calculate_patterns();
	calculate_lines();
	calculate_frameopenness();
	calculate_corners();
	calculate_framemoves();
	calculate_spaces();
	vector<double> v(266);
	
	int(*p_spots1)(spots1);
	int(*p_spots2)(spots2);
	int(*p_spots_moves1)(spots_moves1);
	int(*p_spots_moves2)(spots_moves2);
	int *p_spots_exmoves1(spots_exmoves1);
	int *p_spots_exmoves2(spots_exmoves2);
	int *p_spots_bimoves(spots_bimoves);
	int(*p_spots_fixed1)(spots_fixed1);
	int(*p_spots_fixed2)(spots_fixed2);
	int(*p_spots_openness1)(spots_openness1);
	int(*p_spots_openness2)(spots_openness2);
	int(*p_mountain)(mountain);
	int(*p_wing8)(wing8);
	int(*p_block)(block);
	int(*p_bomb)(bomb);
	int(*p_safec)(safec);
	int(*p_doublec)(doublec);
	int(*p_stoner)(stoner);
	int(*p_sides1)(sides1);
	int(*p_sides2)(sides1);
	int(*p_whiteline)(whiteline);
	int(*p_blackline)(blackline);
	int(*p_midsides1)(midsides1);
	int(*p_midsides2)(midsides2);
	
	int(*p_corneropenness1)(corneropenness1);
	int(*p_corneropenness2)(corneropenness2);
	int(*p_bycorneropenness1)(bycorneropenness1);
	int(*p_bycorneropenness2)(bycorneropenness2);
	int(*p_xcorneropenness1)(xcorneropenness1);
	int(*p_xcorneropenness2)(xcorneropenness2);
	int(*p_sideopenness1)(sideopenness1);
	int(*p_sideopenness2)(sideopenness2);
	
	
	int(*p_corner)(corner);
	int(*p_bycorner)(bycorner);
	int(*p_xcorner)(xcorner);
	
	
	int(*p_cornermove)(cornermove);
	int(*p_bycornermove)(bycornermove);
	int(*p_xcornermove)(xcornermove);
	int(*p_cornermove2)(cornermove2);
	int(*p_bycornermove2)(bycornermove2);
	int(*p_xcornermove2)(xcornermove2);
	int(*p_sidemoves)(sidemoves);
	int(*p_sidemoves2)(sidemoves2);
	
	int n(0);
	if (turn == 1) {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots1[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_mountain[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_wing8[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_block[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_whiteline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_blackline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness2[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness1[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_bomb[i];
			++n;
		}
		//v = spots1;
		//copy(spots2.begin(), spots2.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots2[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_mountain[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)-p_wing8[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_block[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_whiteline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_blackline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness1[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness2[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_bomb[i];
			++n;
		}
		//v = spots2;
		//copy(spots1.begin(), spots1.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_moves1[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_moves2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_corner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcorner[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycorner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_cornermove[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_cornermove2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcornermove[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcornermove2[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycornermove[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycornermove2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_sidemoves[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_sidemoves2[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_exmoves1[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_exmoves2[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_bimoves[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_safec[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_doublec[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_stoner[i];
		++n;
	}
	v[n] = (double)ebeos;
	++n;
	//vector<double> x(v.size());
	//for (unsigned int i(0);i < v.size();++i) {
	//	x[i] = double(v[i]);
	//}
	return v;
}

vector<double> Board::x267()
{
	calculate_spots();
	calculate_spots_exmoves();
	calculate_spots_fixedv2();
	calculate_opennessv3();
	calculate_patterns();
	calculate_lines();
	calculate_frameopenness();
	calculate_corners();
	calculate_framemoves();
	calculate_spaces();
	vector<double> v(267);
	
	int(*p_spots1)(spots1);
	int(*p_spots2)(spots2);
	int(*p_spots_moves1)(spots_moves1);
	int(*p_spots_moves2)(spots_moves2);
	int *p_spots_exmoves1(spots_exmoves1);
	int *p_spots_exmoves2(spots_exmoves2);
	int *p_spots_bimoves(spots_bimoves);
	int(*p_spots_fixed1)(spots_fixed1);
	int(*p_spots_fixed2)(spots_fixed2);
	int(*p_spots_openness1)(spots_openness1);
	int(*p_spots_openness2)(spots_openness2);
	int(*p_mountain)(mountain);
	int(*p_wing8)(wing8);
	int(*p_block)(block);
	int(*p_bomb)(bomb);
	int(*p_safec)(safec);
	int(*p_doublec)(doublec);
	int(*p_stoner)(stoner);
	int(*p_sides1)(sides1);
	int(*p_sides2)(sides1);
	int(*p_whiteline)(whiteline);
	int(*p_blackline)(blackline);
	int(*p_midsides1)(midsides1);
	int(*p_midsides2)(midsides2);
	
	int(*p_corneropenness1)(corneropenness1);
	int(*p_corneropenness2)(corneropenness2);
	int(*p_bycorneropenness1)(bycorneropenness1);
	int(*p_bycorneropenness2)(bycorneropenness2);
	int(*p_xcorneropenness1)(xcorneropenness1);
	int(*p_xcorneropenness2)(xcorneropenness2);
	int(*p_sideopenness1)(sideopenness1);
	int(*p_sideopenness2)(sideopenness2);
	
	
	int(*p_corner)(corner);
	int(*p_bycorner)(bycorner);
	int(*p_xcorner)(xcorner);
	
	
	int(*p_cornermove)(cornermove);
	int(*p_bycornermove)(bycornermove);
	int(*p_xcornermove)(xcornermove);
	int(*p_cornermove2)(cornermove2);
	int(*p_bycornermove2)(bycornermove2);
	int(*p_xcornermove2)(xcornermove2);
	int(*p_sidemoves)(sidemoves);
	int(*p_sidemoves2)(sidemoves2);
	
	int n(0);
	if (turn == 1) {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots1[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_mountain[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_wing8[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_block[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_whiteline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_blackline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness2[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness1[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_bomb[i];
			++n;
		}
		//v = spots1;
		//copy(spots2.begin(), spots2.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
	}
	else {
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots2[i];
			++n;
		}
		for (int i(0); i < 10; ++i) {
			v[n] = (double)p_spots1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_fixed1[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness2[i];
			++n;
		}
		for (int i(0); i < 9; ++i) {
			v[n] = (double)p_spots_openness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_mountain[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)-p_wing8[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_block[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_whiteline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_blackline[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_midsides1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_corneropenness1[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness2[i];
			++n;
		}
		for (int i(0); i < 8; ++i) {
			v[n] = (double)p_bycorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_xcorneropenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness2[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)p_sideopenness1[i];
			++n;
		}
		for (int i(0); i < 4; ++i) {
			v[n] = (double)-p_bomb[i];
			++n;
		}
		//v = spots2;
		//copy(spots1.begin(), spots1.end(), back_inserter(v));
		//copy(spots_moves1.begin(), spots_moves1.end(), back_inserter(v));
		//copy(spots_moves2.begin(), spots_moves2.end(), back_inserter(v));
		//copy(spots_fixed2.begin(), spots_fixed2.end(), back_inserter(v));
		//copy(spots_fixed1.begin(), spots_fixed1.end(), back_inserter(v));
		//copy(spots_openness2.begin(), spots_openness2.end(), back_inserter(v));
		//copy(spots_openness1.begin(), spots_openness1.end(), back_inserter(v));
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_moves1[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_moves2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_corner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcorner[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycorner[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_cornermove[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_cornermove2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcornermove[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_xcornermove2[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycornermove[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_bycornermove2[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_sidemoves[i];
		++n;
	}
	for (int i(0); i < 4; ++i) {
		v[n] = (double)p_sidemoves2[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_exmoves1[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_exmoves2[i];
		++n;
	}
	for (int i(0); i < 9; ++i) {
		v[n] = (double)p_spots_bimoves[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_safec[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_doublec[i];
		++n;
	}
	for (int i(0); i < 8; ++i) {
		v[n] = (double)p_stoner[i];
		++n;
	}
	v[n] = (double)ebeos;
	++n;
	v[n] = (double)hypereven;
	++n;
	//vector<double> x(v.size());
	//for (unsigned int i(0);i < v.size();++i) {
	//	x[i] = double(v[i]);
	//}
	return v;
}

vector<double> Board::x271()
{
	calculate_disks();
	calculate_spots();
	calculate_spots_exmoves();
	calculate_spots_fixedv2();
	calculate_opennessv3();
	calculate_patternsv2();
	calculate_lines();
	calculate_frameopenness();
	calculate_corners();
	calculate_framemoves();
	calculate_spaces();
	vector<double> v;
	
	int n(0);
	v.emplace_back(double(disks1 + disks2));
	v.emplace_back(double(64 - (disks1 + disks2)));
	v.emplace_back(double((disks1 + disks2) % 2));
	v.emplace_back(double(turn));
	double weight = turn == 1 ? 1 : -1;
	if (turn == 1) {
		extend(v, spots1);
		extend(v, spots2);
		extend(v, spots_fixed1);
		extend(v, spots_fixed2);
		extend(v, spots_openness1);
		extend(v, spots_openness2);
		extend(v, sides1);
		extend(v, sides2);
		extend(v, midsides1);
		extend(v, midsides2);
		extend(v,corneropenness1);
		extend(v,corneropenness2);
		extend(v,bycorneropenness1);
		extend(v,bycorneropenness2);
		extend(v,xcorneropenness1);
		extend(v,xcorneropenness2);
		extend(v,sideopenness1);
		extend(v,sideopenness2);
	}
	else {
		extend(v, spots2);
		extend(v, spots1);
		extend(v, spots_fixed2);
		extend(v, spots_fixed1);
		extend(v, spots_openness2);
		extend(v, spots_openness1);
		extend(v, sides2);
		extend(v, sides1);
		extend(v, midsides2);
		extend(v, midsides1);
		extend(v,corneropenness2);
		extend(v,corneropenness1);
		extend(v,bycorneropenness2);
		extend(v,bycorneropenness1);
		extend(v,xcorneropenness2);
		extend(v,xcorneropenness1);
		extend(v,sideopenness2);
		extend(v,sideopenness1);
	}
	extend(v, mountain, weight);
	extend(v, wing8, weight);
	extend(v, block, weight);
	extend(v, whiteline, weight);
	extend(v, blackline, weight);
	extend(v, bomb, weight);
	extend(v, spots_moves1);
	extend(v, spots_moves2);
	extend(v, corner);
	extend(v, xcorner);
	extend(v, bycorner);
	extend(v, cornermove);
	extend(v, cornermove2);
	extend(v, xcornermove);
	extend(v, xcornermove2);
	extend(v, bycornermove);
	extend(v, bycornermove2);
	extend(v, sidemoves);
	extend(v, sidemoves2);
	extend(v, spots_exmoves1);
	extend(v, spots_exmoves2);
	extend(v, spots_bimoves);
	extend(v, safec);
	extend(v, doublec);
	extend(v, stoner);
	v.emplace_back(double(ebeos));
	v.emplace_back(double(hypereven));
	return v;
}

vector<double> Board::xin(int inputs) {
	if (inputs == 271) {
		return x271();
	}
	if (inputs == 267) {
		return x267();
	}
	if (inputs == 266) {
		return x266();
	}
	if (inputs == 206) {
		return x206();
	}
	if (inputs == 102) {
		return x102();
	}
	if (inputs == 78) {
		return x78();
	}
	return x271();
}

void Board::print() {
	cout << endl << "    ";
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
	}
	cout << endl << endl;
	for (int i(0); i < 8; ++i) {
		cout << i << "   ";
		for (int j(0); j < 8; ++j) {
			if (square[i][j] == 0) {
				cout << "- ";
			}
			else if (square[i][j] == 1) {
				cout << "X ";
			}
			else {
				cout << "O ";
			}
		}
		cout << "  " << i << endl;
	}
	cout << endl << "    ";
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
	}
	cout << endl << endl;
}

string to_text(Board board, bool unique) {
	string t("");
	if (unique) {
		vector<string> s(0);
		for (int i(0); i < 4; ++i) {
			vector< vector<int> > b(8, vector<int> (8, 0));
			for (int i(0); i < 8; ++i) {
				for (int j(0); j < 8; ++j) {
					if (board.square[i][j] == 1) {
						b[i][j] = 1;
					}
				}
			}
			vector< vector<int> > w(8, vector<int> (8, 0));
			for (int i(0); i < 8; ++i) {
				for (int j(0); j < 8; ++j) {
					if (board.square[i][j] == 2) {
						w[i][j] = 1;
					}
				}
			}
			s.emplace_back(matrix_to_hex(b) + matrix_to_hex(w) + to_string(board.turn));
			board.rotate();
		}
		board.reflect();
		for (int i(0); i < 4; ++i) {
			vector< vector<int> > b(8, vector<int> (8, 0));
			for (int i(0); i < 8; ++i) {
				for (int j(0); j < 8; ++j) {
					if (board.square[i][j] == 1) {
						b[i][j] = 1;
					}
				}
			}
			vector< vector<int> > w(8, vector<int> (8, 0));
			for (int i(0); i < 8; ++i) {
				for (int j(0); j < 8; ++j) {
					if (board.square[i][j] == 2) {
						w[i][j] = 1;
					}
				}
			}
			s.emplace_back(matrix_to_hex(b) + matrix_to_hex(w) + to_string(board.turn));
			board.rotate();
		}
		sort(begin(s), end(s));
		t = s[0];
	}
	else {
		vector< vector<int> > b(8, vector<int> (8, 0));
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				if (board.square[i][j] == 1) {
					b[i][j] = 1;
				}
			}
		}
		vector< vector<int> > w(8, vector<int> (8, 0));
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				if (board.square[i][j] == 2) {
					w[i][j] = 1;
				}
			}
		}
		t = matrix_to_hex(b) + matrix_to_hex(w) + to_string(board.turn);
	}
	return t;
}

pair<bool, pair<double, double> > board_in_book(const Board &b, const map<string, pair<double, double> > &m)
{
	string s(to_text(b, true));
	if (m.count(s) == 0) {
		return pair<bool, pair<double, double> > (false, pair<double, double>(0, 0));
	}
	else {
		return pair<bool, pair<double, double> > (true, pair<double, double>(m.at(s).first, m.at(s).second));
	}
}

double Alpbet(Board &b1, Neuralnet(&neu)[65], int disks, int depth, double alp, double bet)
{
	int inputs(neu[disks].nodes[0]);
	//���B
	if (depth == 0) {
		b1.calculate_moves();
		int moves(b1.move_num);
		if (moves == 0) {
			b1.calculate_moves2();
			if (b1.move_num2 == 0) {
				b1.calculate_disks();
				if (b1.turn == 1) {
					return (double)(b1.disks1 - b1.disks2);
				}
				else {
					return (double)(b1.disks2 - b1.disks1);
				}
			}
		}
		vector<double> x;
		x = b1.xin(inputs);
		//if (inputs == 266) {
		//	x = b1.x266();
		//}
		//else if (inputs == 206) {
		//	x = b1.x206();
		//}
		//else if (inputs == 102) {
		//	x = b1.x102();
		//}
		//double z(-neu[disks + 1].ForwardG(x)[0]);
		return neu[disks].Forward(x)[0];
	}
	
	//�\�[�g���Ȃ�
	else if (depth < 4 || disks > 54) {
		
		//b1.calculate_moves();
		int moves(0);
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				if (b1.square[i][j] == 0) {
					bool r(b1.right_move(i, j));
					//bool r(b1.moves[i][j]);
					if (r) {
						++moves;
						Board b2(b1);
						b2.move_board(i, j);
						double tempalp(-Alpbet(b2, neu, disks + 1, depth - 1, -bet, -alp));
						alp = max(alp, tempalp);
						if (alp >= bet) {
							return alp;
						}
					}
				}
			}
		}
		if (moves > 0) {
			return alp;
		}
		else {
			b1.calculate_moves2();
			if (b1.move_num2 == 0) {
				b1.calculate_disks();
				if (b1.turn == 1) {
					return (double)(b1.disks1 - b1.disks2);
				}
				else {
					return (double)(b1.disks2 - b1.disks1);
				}
			}
			else {
				Board b2(b1);
				b2.turn = b1.turnnot;
				b2.turnnot = b1.turn;
				++b2.turnnum;
				double tempalp(-Alpbet(b2, neu, disks, depth - 1, -bet, -alp));
				return max(alp, tempalp);
			}
		}
	}
	
	else {
		b1.calculate_moves();
		int moves(b1.move_num);
		int temp_moves(moves);
		int eliminate_n(0);
		
		if (moves == 0) {
			b1.calculate_moves2();
			if (b1.move_num2 == 0) {
				b1.calculate_disks();
				if (b1.turn == 1) {
					return (double)(b1.disks1 - b1.disks2);
				}
				else {
					return (double)(b1.disks2 - b1.disks1);
				}
			}
			else {
				Board b2(b1);
				b2.turn = b1.turnnot;
				b2.turnnot = b1.turn;
				++b2.turnnum;
				//return -betawd(b2, -x);
				double tempalp(-Alpbet(b2, neu, disks, depth - 1, -bet, -alp));
				return max(alp, tempalp);
			}
		}
		
		else {
			vector<vector<double> > v(moves);
			int n(0);
			for (int i(0); i < 8; ++i) {
				for (int j(0); j < 8; ++j) {
					if (b1.moves[i][j]) {
						v[n].resize(3);
						v[n][0] = 0;
						v[n][1] = i;
						v[n][2] = j;
						++n;
					}
				}
			}
			
			//�\�[�g����
			if (moves != 1) {
				int mini = 2*int((depth - 8)/2);
				for (int n(0); n < moves; ++n) {
					Board b2(b1);
					b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
					double y(0);
					if (mini > 0) {
						y = -Alpbet(b2, neu, disks + 1, mini, -999, 999);
					}
					else {
						vector<double> x;
						x = b2.xin(inputs);
						y = -neu[disks + 1].Forward(x)[0];
					}
					//vector<double> x;
					//if (inputs == 266) {
					//	x = b2.x266();
					//}
					//else if (inputs == 206) {
					//	x = b2.x206();
					//}
					//else if (inputs == 102) {
					//	x = b2.x102();
					//}
					////double z(-neu[disks + 1].ForwardG(x)[0]);
					//double y(-neu[disks + 1].Forward(x)[0]);
					v[n][0] = y;
				}
				sort(v.begin(), v.end());
				std::reverse(v.begin(), v.end());
				for (int i(0); i < moves; ++i) {
					if (v[i][0] < v[0][0] - 10 - mini) {
						++eliminate_n;
					}
				}
				temp_moves = moves - eliminate_n;
			}
			
			
			//���̃m�[�h��
			for (int n(0); n < temp_moves; ++n) {
				Board b2(b1);
				b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
				double tempalp(-Alpbet(b2, neu, disks + 1, depth - 1, -bet, -alp));
				alp = max(alp, tempalp);
				if (alp >= bet) {
					return alp;
				}
			}
			return alp;
		}
	}
	
}

double AlpbetE(Board &b1, Neuralnet(&neu)[65], int disks, double alp, double bet)
{
	int inputs(neu[disks].nodes[0]);
	
	//���B
	if (disks == 63) {
		return b1.diskdif63();
	}
	
	//�\�[�g���Ȃ�
	else if (disks > 54) {
		//b1.calculate_moves();
		int moves(0);
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				if (b1.square[i][j] == 0) {
					bool r(b1.right_move(i, j));
					//bool r(b1.moves[i][j]);
					if (r) {
						++moves;
						Board b2(b1);
						b2.move_board(i, j);
						double tempalp(-AlpbetE(b2, neu, disks + 1, -bet, -alp));
						alp = max(alp, tempalp);
						if (alp >= bet) {
							return alp;
						}
					}
				}
			}
		}
		if (moves > 0) {
			return alp;
		}
		else {
			b1.calculate_moves2();
			if (b1.move_num2 == 0) {
				b1.calculate_disks();
				if (b1.turn == 1) {
					return (double)(b1.disks1 - b1.disks2);
				}
				else {
					return (double)(b1.disks2 - b1.disks1);
				}
			}
			else {
				Board b2(b1);
				b2.turn = b1.turnnot;
				b2.turnnot = b1.turn;
				++b2.turnnum;
				//return -betawd(b2, -x);
				double tempalp(-AlpbetE(b2, neu, disks, -bet, -alp));
				return max(alp, tempalp);
			}
		}
	}
	
	else {
		b1.calculate_moves();
		int moves(b1.move_num);
		int temp_moves(moves);
		int eliminate_n(0);
		
		if (moves == 0) {
			b1.calculate_moves2();
			if (b1.move_num2 == 0) {
				b1.calculate_disks();
				if (b1.turn == 1) {
					return (double)(b1.disks1 - b1.disks2);
				}
				else {
					return (double)(b1.disks2 - b1.disks1);
				}
			}
			else {
				Board b2(b1);
				b2.turn = b1.turnnot;
				b2.turnnot = b1.turn;
				++b2.turnnum;
				//return -betawd(b2, -x);
				double tempalp(-AlpbetE(b2, neu, disks, -bet, -alp));
				return max(alp, tempalp);
			}
		}
		
		else {
			vector<vector<double> > v(moves);
			int n(0);
			for (int i(0); i < 8; ++i) {
				for (int j(0); j < 8; ++j) {
					if (b1.moves[i][j]) {
						v[n].resize(3);
						v[n][0] = 0;
						v[n][1] = i;
						v[n][2] = j;
						++n;
					}
				}
			}
			
			//�\�[�g����
			if (moves != 1) {
				int mini = 2 * int((50 - disks) / 2);
				for (int n(0); n < moves; ++n) {
					Board b2(b1);
					b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
					double y(0);
					if (mini > 0) {
						y = -Alpbet(b2, neu, disks + 1, mini, -999, 999);
					}
					else {
						vector<double> x;
						x = b2.xin(inputs);
						y = -neu[disks + 1].Forward(x)[0];
					}
					v[n][0] = y;
				}
				sort(v.begin(), v.end());
				std::reverse(v.begin(), v.end());
				for (int i(0); i < moves; ++i) {
					if (v[i][0] < v[0][0] - 15) {
						++eliminate_n;
					}
				}
				temp_moves = moves - eliminate_n;
			}
			
			
			//���̃m�[�h��
			for (int n(0); n < temp_moves; ++n) {
				Board b2(b1);
				b2.move_board(int(v[n][1] + 0.5), int(v[n][2] + 0.5));
				double tempalp(-AlpbetE(b2, neu, disks + 1, -bet, -alp));
				alp = max(alp, tempalp);
				if (alp >= bet) {
					return alp;
				}
			}
			return alp;
		}
	}
}
vector<Board> next_boards(const Board &b) {
	vector<Board> next_b(0);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (b.square[i][j] == 0) {
				if (b.right_move(i, j)) {
					Board b2(b);
					b2.move_board(i, j);
					next_b.emplace_back(b2);
				}
			}
		}
	}
	return next_b;
}

vector<Board> next_boards(const Board &b, const vector<pair<int, int> > &moves) {
	vector<Board> next_b(0);
	for (auto &m : moves) {
		Board b2(b);
		b2.move_board(m.first, m.second);
		next_b.emplace_back(b2);
	}
	return next_b;
}

vector<pair<int, int> > next_moves(const Board &b) {
	vector<pair<int, int> > next_m(0);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (b.square[i][j] == 0) {
				if (b.right_move(i, j)) {
					Board b2(b);
					b2.move_board(i, j);
					next_m.emplace_back(pair<int, int>(i, j));
				}
			}
		}
	}
	return next_m;
}

pair<int,int> moved_location(const Board &b0, const Board &b1) {
	pair<int, int> ij(-1, -1);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (b0.square[i][j] == 0 && b1.square[i][j] != 0) {
				ij.first = i;
				ij.second = j;
				return ij;
			}
		}
	}
	return ij;
}

void sort_boards(vector<Board> &boards, const Neuralnet &neuralnet, const int disks, const int depth, const double cut_ratio) {
	vector<pair<int,double> > next_b_r(0);
	double sum_ratio(0);
	for (int i = 0; i != boards.size(); ++i) {
		double value = -AB_value(boards[i], neuralnet, disks, depth, -99999, 99999, cut_ratio);
		double ratio = pow(2, value / 5.);
		sum_ratio += ratio;
		next_b_r.emplace_back(pair<int, double>(i, ratio));
	}
	sort(begin(next_b_r), end(next_b_r), 
		[](pair<int, double> &l, pair<int, double> &r){ return l.second > r.second; });
	vector<Board> boards_ordered(0);
	double tem_ratio(0);
	for (int i = 0; i < next_b_r.size(); ++i) {
		boards_ordered.emplace_back(boards[next_b_r[i].first]);
		tem_ratio += next_b_r[i].second;
		if (tem_ratio > sum_ratio * (1. - cut_ratio)) {
			break;
		}
	}
	boards = boards_ordered;
}

double AB_value(map<string, pair<double, double> > &book,
				Board &b,
				const Neuralnet &neuralnet,
				const int disks,
				const int depth,
				double alp,
				double bet,
				const double cut_ratio)
{
	double lower(-99999), upper(99999), value(-99999);
	auto info = board_in_book(b, book);
	if (info.first) {
		lower = info.second.first;
		if (bet < lower) return lower;
		upper = info.second.second;
		if (alp > upper) return upper;
		alp = max(alp, lower);
		bet = min(bet, upper);
	}
	int inputs(neuralnet.nodes[0]);
	if (disks == 64) {
		b.calculate_disks();
		return -(b.turn == 1 ? 1. : -1.) * (double)(b.disks1 - b.disks2);
	}
	if (depth == 0) {
		vector<double> x(b.xin(inputs));
		return neuralnet.Forward(x)[0];
	}
	vector<Board> next_bs = next_boards(b);
	int move_number(next_bs.size());
	if (move_number == 0) {
		Board next_b(b);
		next_b.pass();
		next_b.calculate_moves();
		if (next_b.move_num == 0) {
			b.calculate_disks();
			return -(b.turn == 1 ? 1. : -1.) * (double)(b.disks1 - b.disks2);
		}
		//return -betawd(b2, -x);
		double value = -AB_value(book, next_b, neuralnet, disks, depth - 1, -bet, -alp, cut_ratio);
		return max(alp, value);
	}
	if (4 <= depth && disks <= 54 && move_number > 1) {
		int sub_depth = min(max(0, (depth - 8)/2*2), max(0, int((52 - depth)/2)*2));
		sort_boards(next_bs, neuralnet, disks + 1, sub_depth, cut_ratio);
	}
	vector<pair<int, double> > next_b_v(0);
	for (auto &next_b : next_bs) {
		double t_value = -AB_value(book, next_b, neuralnet, disks + 1, depth - 1, -bet, -alp, cut_ratio);
		value = max(value, t_value);
		if (bet <= value) {
			record_book(b, book, lower, value);
			return value;
		}
		if (alp < value) {
			alp = value;			
		}
	}
	record_book(b, book, value, value);
	return value;
}

double AB_value(Board &b,
				const Neuralnet &neuralnet,
				const int disks,
				const int depth,
				double alp,
				double bet,
				const double cut_ratio,
				vector<pair<int, int> > &route)
{
	double lower(-99999), upper(99999), value(-99999);
	int inputs(neuralnet.nodes[0]);
	if (disks == 64) {
		b.calculate_disks();
		return -(b.turn == 1 ? 1. : -1.) * (double)(b.disks1 - b.disks2);
	}
	if (depth == 0) {
		vector<double> x(b.xin(inputs));
		return neuralnet.Forward(x)[0];
	}
	vector<Board> next_bs = next_boards(b);
	int move_number(next_bs.size());
	if (move_number == 0) {
		Board next_b(b);
		next_b.pass();
		next_b.calculate_moves();
		if (next_b.move_num == 0) {
			b.calculate_disks();
			return -(b.turn == 1 ? 1. : -1.) * (double)(b.disks1 - b.disks2);
		}
		//return -betawd(b2, -x);
		route.emplace_back(pair<int, int>(-1,-1));
		double value = -AB_value(next_b, neuralnet, disks, depth - 1, -bet, -alp, cut_ratio, route);
		return max(alp, value);
	}
	if (4 <= depth && disks <= 54 && move_number > 1) {
		int sub_depth = min(max(0, (depth - 8)/2*2), max(0, int((52 - depth)/2)*2));
		sort_boards(next_bs, neuralnet, disks + 1, sub_depth, cut_ratio);
	}
	vector<pair<int, double> > next_b_v(0);
	vector<pair<int, int> > b_route(route);
	for (auto &next_b : next_bs) {
		vector<pair<int, int> > n_route(route);
		n_route.emplace_back(moved_location(b ,next_b));
		double t_value = -AB_value(next_b, neuralnet, disks + 1, depth - 1, -bet, -alp, cut_ratio, n_route);
		value = max(value, t_value);
		if (bet <= value) {
			b_route = n_route;
			return value;
		}
		if (alp < value) {
			b_route = n_route;
			alp = value;			
		}
	}
	route = b_route;
	return value;
}

double AB_value(Board &b,
				const Neuralnet &neuralnet,
				const int disks,
				const int depth,
				double alp,
				double bet,
				const double cut_ratio)
{
	double value(-99999);
	int inputs(neuralnet.nodes[0]);
	if (disks == 64) {
		b.calculate_disks();
		return -(b.turn == 1 ? 1. : -1.) * (double)(b.disks1 - b.disks2);
	}
	if (depth == 0) {
		vector<double> x(b.xin(inputs));
		return neuralnet.Forward(x)[0];
	}
	vector<Board> next_bs = next_boards(b);
	int move_number(next_bs.size());
	if (move_number == 0) {
		Board next_b(b);
		next_b.pass();
		next_b.calculate_moves();
		if (next_b.move_num == 0) {
			b.calculate_disks();
			return -(b.turn == 1 ? 1. : -1.) * (double)(b.disks1 - b.disks2);
		}
		//return -betawd(b2, -x);
		double value = -AB_value(next_b, neuralnet, disks, depth - 1, -bet, -alp, cut_ratio);
		return max(alp, value);
	}
	if (4 <= depth && disks <= 54 && move_number > 1) {
		int sub_depth = min(max(0, (depth - 8)/2*2), max(0, int((52 - depth)/2)*2));
		sort_boards(next_bs, neuralnet, disks + 1, sub_depth, cut_ratio);
	}
	vector<pair<int, double> > next_b_v(0);
	for (auto &next_b : next_bs) {
		double t_value = -AB_value(next_b, neuralnet, disks + 1, depth - 1, -bet, -alp, cut_ratio);
		value = max(value, t_value);
		if (bet <= value) {
			return value;
		}
		if (alp < value) {
			alp = value;
		}
	}
	return value;
}

double AB_move(map<string, pair<double, double> > book,
				Board &b,
				const Neuralnet &neuralnet,
				const int disks,
				const int depth,
				const double random,
				const double cut_ratio)
{
	int inputs(neuralnet.nodes[0]);
	vector<Board> next_bs = next_boards(b);
	int move_number(next_bs.size());
	if (move_number == 0) {
		b.pass();
		return -999;
	}
	if (4 <= depth && disks <= 54 && next_bs.size() > 1 && disks != 4) {
		int sub_depth = min(max(0, (depth - 8)/2*2), max(0, (50 - depth)/2*2));
		sort_boards(next_bs, neuralnet, disks + 1, sub_depth, cut_ratio);
	}
	vector<pair<int, double> > next_b_v(0);
	if (random >= 100 || depth == 0 || disks == 4) {
		random_device rnd;
		std::uniform_real_distribution<> randnum(0, next_bs.size());
		int point = randnum(rnd);
		b = next_bs[point];
		return -999;
	}
	else if (random <= 0) {
		double alp(-99999), bet(99999);
		for (auto &next_b : next_bs) {
			auto info = board_in_book(b, book);
			double value(0);
			if (info.first) {
				value = -info.second.first;
			}
			else {
				value = -AB_value(next_b, neuralnet, disks + 1, depth - 1, -bet, -alp, cut_ratio);
			}
			if (value > alp) {
				b = next_b;
				alp = value;
			}
		}
		return alp;
	}
	else {
		vector<double> values(0);
		vector<double> rates(0);
		double sum_rate(0);
		for (auto &next_b : next_bs) {
			auto info = board_in_book(next_b, book);
			double value(0);
			if (info.first) {
				value = -info.second.first;
			}
			else {
				value = -AB_value(next_b, neuralnet, disks + 1, depth - 1, -99999, 99999, cut_ratio);
			}
			values.emplace_back(value);
			double rate = pow(2, value / random);
			rates.emplace_back(rate);
			sum_rate += rate;
		}
		std::random_device rnd;
		// std::mt19937 mt(rnd());
		std::uniform_real_distribution<> randnum(0, sum_rate);
		//	number = randnum(mt);
		double point(randnum(rnd));
		double tem(0);
		for (int i = 0; i != rates.size(); ++i) {
			tem += rates[i];
			if (tem >= point) {
				b = next_bs[i];
				return values[i];
			}
		}
	}
	return -999;
}

vector<Board> random_play_boards(const int number,
								map<string, pair<double, double> > &book,
								const Neuralnet &neuralnet,
								const int bottom_disks,
								const int top_disks,
								const int search_depth,
								const double random,
								const double cut_ratio)
{
	int games(0);
	vector<Board> b_vec(0);
	while (games < number) {
		++games;
		// cout << "1" << endl;
		Board b;
		b.initialize();
		bool end = false;
		int disks = 4;
		while (!end) {
			double v = AB_move(book, b, neuralnet, disks, search_depth, random, cut_ratio);
			b.calculate_disks();
			disks = b.disks1 + b.disks2;
			if (disks == bottom_disks || b.is_end_game()) {
				end = true;
			}
		}
		if (disks == bottom_disks) {
			end = false;
			b_vec.emplace_back(b);
			while (!end) {
				vector<Board> n_boards = next_boards(b);
				// auto perm = random_permutation(n_boards.size());
				// for (int i(0); i < 1 && i < perm.size(); ++i) {
				// 	b_vec.emplace_back(n_boards[perm[i]]);
				// }
				for (auto n_b : n_boards) {
					b_vec.emplace_back(n_b);
				}
				double v = AB_move(book, b, neuralnet, disks, search_depth, random, cut_ratio);
				b.calculate_disks();
				disks = b.disks1 + b.disks2;
				if (disks == top_disks || b.is_end_game()) {
					end = true;
				}
			}
		}
	}
	return b_vec;
}

vector<Board> random_play_boards(const int number,
					 			map<string, pair<double, double> > &book,
								const Neuralnet &neuralnet,
								const int search_depth, const double random,
								const double cut_ratio)
{
	int boards(0);
	vector<Board> b_vec(0);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> urd(8, 55);
	while (boards < number) {
		int end_disks = urd(mt);
		// cout << "1" << endl;
		Board b;
		b.initialize();
		bool end = false;
		int disks = 4;
		while (!end) {
			double v = AB_move(book, b, neuralnet, disks, search_depth, random, cut_ratio);
			b.calculate_disks();
			disks = b.disks1 + b.disks2;
			if (disks == end_disks || b.is_end_game()) {
				end = true;
			}
		}
		if (disks == end_disks) {
			++boards;
			b_vec.emplace_back(b);
		}
	}
	return b_vec;
}

void record_book(const Board &b, map<string, pair<double, double> > &m, const double l, const double r) {
	m[to_text(b, true)] = pair<double, double> (l, r);
}
