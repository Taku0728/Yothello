//
//  main.cpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#include <iostream>
#include "tree.hpp"

int input(int &mode);
void outputb(Play &p1, int mode, bool cal);

int main(int argc, char **argv)
{
	// unsigned long long 

	int player;
	int type;
	string black("");
	string white("");
	int turn(0);
	int person(0);
	int movei(0), movej(0);
	for (int i(0); i < 16; ++i) {
		black += argv[1][i];
	}
	for (int i(16); i < 32; ++i) {
		white += argv[1][i];
	}
	vector< vector<int> > matrix_b(hex_to_matrix(black));
	vector< vector<int> > matrix_w(hex_to_matrix(white));
	vector< vector<int> > square_init(matrix_to_square(matrix_b, matrix_w));
	turn = argv[1][32] - '0';
	person = argv[1][33] - '0';
	movei = argv[1][34] - '0';
	movej = argv[1][35] - '0';

	Board b;
	ostringstream ossb;
	ostringstream ossw;
	if (person == 0) {
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				b.square[i][j] = square_init[i][j];
			}
		}
		if (movei != 8) {
			b.turn = turn + 1;
			b.turnnot = !turn + 1;
			b.move_board(movei, movej);
			vector< vector<int> > s(8, vector<int>(8, 0));
			for (int i(0); i < 8; ++i) {
				for (int j(0); j < 8; ++j) {
					s[i][j] = b.square[i][j];
				}
			}
			ossb = matrix_to_hex(square_to_black(s));
			ossw = matrix_to_hex(square_to_white(s));
		}
		else {
			ossb << black;
			ossw << white;
		}
	}
	else {
		string local = argv[0];
		while (local.back() != '/') {
			local.pop_back();
		}
		Play p1;
		p1.local_folder = local;
		p1.load();
		p1.read(p1.neu_folder);
		double ew = 20.0;
		p1.elim_width = ew;
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				p1.b.square[i][j] = square_init[i][j];
			}
		}
		if (turn == 1) {
			p1.player = 2;
			p1.opponent = 1;
		}
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				b.square[i][j] = square_init[i][j];
			}
		}
		b.turn = turn + 1;
		b.turnnot = !turn + 1;
		p1.b = b;
		p1.runabwithbook();
		vector< vector<int> > s(8, vector<int>(8, 0));
		b = p1.b;
		for (int i(0); i < 8; ++i) {
			for (int j(0); j < 8; ++j) {
				s[i][j] = b.square[i][j];
			}
		}
		ossb = matrix_to_hex(square_to_black(s));
		ossw = matrix_to_hex(square_to_white(s));
	}
	b.turn = turn ? 1 : 2;
	b.turnnot = turn ? 2 : 1;
	b.calculate_moves();
	vector< vector<int> > moves(8, vector<int>(8, 0));
	int has_move(0);
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			moves[i][j] = b.moves[i][j];
			if (moves[i][j]) {
				has_move = 1;
			}
		}
	}
	cout << ossb.str() << ossw.str() << (turn ? 0 : 1) << matrix_to_hex(moves).str() << has_move << endl;
}

void outputb(Play &p1, int mode, bool cal)
{
	p1.b.calculate_disks();
	cout << "  ";
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
	}
	cout << endl;
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
		for (int j(0); j < 8; ++j) {
			if (p1.b.square[i][j] == 1) {
				cout << "＊";
			}
			else if (p1.b.square[i][j] == 2) {
				cout << "〇";
			}
			else {
				cout << "ー";
			}
		}
		cout << endl;
	}
	cout << endl;
	
	cout << "BLACK:" << p1.b.disks1 << " ";
	cout << "WHITE:" << p1.b.disks2 << endl;
	if (cal) {
		if (mode == 1 && p1.bestval < 100 && p1.bestval - 100) {
			cout << "VALUE:" << round(p1.bestval) << " TIME:" << p1.time_consumed << endl;
		}
		else if (mode == 2 && p1.time_consumed < 1000 && p1.time_consumed >= 0) {
			cout << "TIME:" << p1.time_consumed << endl;
		}
	}
	cout << endl;
}
