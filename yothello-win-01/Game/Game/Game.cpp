// OPT.cpp : 僐儞僜乕儖 傾僾儕働乕僔儑儞偺僄儞僩儕 億僀儞僩傪掕媊偟傑偡丅
//

#include "stdafx.h" 
#include "othello.h"
#include "maths.h"
#include "Learning.h"
#include "Tree.h"

int input(int &mode);
void outputb(Play &p1, int mode, bool cal);

int main()
{
	int player;
	int type;
	cout <<
		"--- YOTHELLO ---" << endl <<
		"-1 -> ij : PASS" << endl <<
		"-2 -> ij : REDO" << endl <<
		"-3 -> ij : RANDOM" << endl <<
		"90 -> ij : SETTING" << endl <<
		"99 -> ij : EXIT" << endl <<endl;
	cout << "BOARD TYPE: ";
	cin >> type;
	cout << "YOTHELLO PLAYS: ";
	cin >> player;

	int mode(1);
	cout << "MODE: ";
	cin >> mode;
	cout << endl;

	Board brec;
	double ew = 20.0;
	Play p1;
	p1.load();
	p1.input();
	p1.read(p1.neu_folder);
	p1.elim_width = ew;
	if (type == 2) {
		p1.b.square[3][3] = 1;
		p1.b.square[3][4] = 2;
		p1.b.square[4][3] = 2;
		p1.b.square[4][4] = 1;
	}
	if (player == 2) {
		p1.player = 2;
		p1.opponent = 1;
	}

	outputb(p1, mode, false);

	for (int ever(0); ever < 10000; ++ever) {
		//brec.turn = p1.b.turn;
		//brec.turnnot = p1.b.turnnot;
		//for (int i(0); i < 8;++i) {
		//	for (int j(0); j < 8; ++j) {
		//		brec.square[i][j] = p1.b.square[i][j];
		//	}
		//}

		int stay(true);
		int ini(0);
		int inj(0);
		while (stay) {
			stay = false;
			bool cal(false);
			if (ever != 0 || player != 2) {
				if (mode != 3) {
					cout << endl << "CALCULATING..." << endl << endl;
					p1.runabwithbook();
					cal = true;
				}
				else {
					int ij(input(mode));
					if (mode != 3) {
						stay = true;
					}
					else if (ij == -1) {
						int tem(p1.b.turn);
						p1.b.turn = p1.b.turnnot;
						p1.b.turnnot = tem;
					}
					else if (ij == -2) {
						p1.b = brec;
						stay = true;
					}
					else if (ij == -3) {
						int dc[2];
						Random_choose rdc;
						p1.b.calculate_moves();
						rdc(dc, p1.b.moves, p1.b.move_num);
						int i = dc[0];
						int j = dc[1];
						p1.b.move_board(i, j);
					}
					else {
						int i = int(ij / 10);
						int j = ij % 10;
						p1.b.move_board(i, j);
					}
				}
				outputb(p1, mode, cal);
			}
			p1.record();
		}

		stay = true;
		while (stay) {
			stay = false;

			int ij(input(mode));
			if (ij == -1) {
				int tem(p1.b.turn);
				p1.b.turn = p1.b.turnnot;
				p1.b.turnnot = tem;
				ini = -1;
			}
			else if (ij == -2) {
				p1.b = brec;
				outputb(p1, mode, false);
				stay = true;
			}
			else if (ij == -3) {
				int dc[2];
				Random_choose rdc;
				p1.b.calculate_moves();
				rdc(dc, p1.b.moves, p1.b.move_num);
				ini = dc[0];
				inj = dc[1];
			}
			else {
				ini = int(ij / 10);
				inj = ij % 10;
			}

		}

		brec = p1.b;

		//p2.record();
		//for (int i(0); i < 8;++i) {
		//	for (int j(0); j < 8;++j) {
		//		file << p2.b.square[i][j] << ",";
		//	}
		//	file << endl;
		//}

		if (ini >= 0) {
			p1.b.move_board(ini, inj);
		}
		else {
			int tem(p1.b.turn);
			p1.b.turn = p1.b.turnnot;
			p1.b.turnnot = tem;
		}
		outputb(p1, mode, false);
	}
	/*
	std::ofstream test_file;
	test_file.open("PLAY\\test.txt", std::ios::out);
	test_file << wins1 << endl;
	test_file << wins2 << endl;*/

}

int input(int &mode)
{
	bool stay(true);
	int ij;
	while (stay) {
		stay = false;
		cout << "ij <= ";
		cin >> ij;
		cout << endl;
		int i;
		int j;

		if (ij == 99) {
			cout << "EXITING" << endl;
			exit(1);
		}
		else if (ij == 90) {
			cout << "MODE SETTING" << endl
				<< "1. PLAY WITH YOTHELLO" << endl
				<< "2. PLAY WITH YOTHELLO (NO SCORE)" << endl
				<< "3. PLAY WITHOUT YOTHELLO" << endl
				<< "<= ";
			int m;
			cin >> m;
			mode = m;
			stay = true;
			cout << endl;
		}
		else if (ij != -1 && ij != -2 && ij != -3) {
			i = int(ij / 10);
			j = ij % 10;
			if (i < 0 || i>7 || j < 0 || j>7) {
				cout << "0 <= i <= 7 , 0 <= j <= 7" << endl;
				stay = true;
			}
		}
	}
	return ij;

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
				cout << "―";
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
