// OPT.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h" 
#include "othello.h"
#include "maths.h"
#include "Learning.h"
#include "Tree.h"


int main()
{
	std::ifstream ifs("settings.txt");
	if (!ifs) {
		std::cout << "入力エラー";
	}
	//初期判断
	std::string str;
	getline(ifs, str);
	string folder2(str + "\\");
	getline(ifs, str);
	string folder1(str + "\\");
	int wins1(0);
	int wins2(0);
	int windisks1(0);
	int windisks2(0);
	double ew = 18.0;
	for (int tries(0); tries < 1;++tries) {
		wins1 = 0;
		wins2 = 0;
		//（先攻１後攻２
		for (int num(0);num < 1; ++num) {
			Play p1;
			Play p2;
			p1.input();
			p2.player = 2;
			p2.opponent = 1;
			p1.read(folder1);
			p2.read(folder2);
			p1.elim_width = ew;
			p2.elim_width = ew;
			while (!p1.endgame && !p2.endgame) {
				for (int i(0); i < 8; ++i) {
					for (int j(0); j < 8; ++j) {
						cout << p1.b.square[i][j] << ",";
					}
					cout << endl;
				}
				cout << endl;
				p1.runabwithbook();
				cout << p1.level << " " << folder1 << " " << p1.player << " " << p1.bestval << " " << p1.time_consumed << endl << endl;
				p1.record();
				//for (int i(0); i < 8;++i) {
				//	for (int j(0); j < 8;++j) {
				//		file << p1.b.square[i][j] << ",";
				//	}
				//	file << endl;
				//}
				p2.b = p1.b;
				for (int i(0); i < 8; ++i) {
					for (int j(0); j < 8; ++j) {
						cout << p2.b.square[i][j] << ",";
					}
					cout << endl;
				}
				cout << endl;
				p2.runabwithbook();
				cout << p2.level << " " << folder2 << " " << p2.player << " " << p2.bestval << " " << p2.time_consumed << endl << endl;
				p2.record();
				//p2.record();
				//for (int i(0); i < 8;++i) {
				//	for (int j(0); j < 8;++j) {
				//		file << p2.b.square[i][j] << ",";
				//	}
				//	file << endl;
				//}
				p1.b = p2.b;
			}
			p1.b.calculate_disks();
			if (p1.b.disks1 > p1.b.disks2) {
				cout << 1 << " win" << endl << endl;
				++wins1;
				windisks1 += p1.b.disks1 - p1.b.disks2;
			}
			else {
				cout << 2 << " win" << endl << endl;
				++wins2;
				windisks2 += p1.b.disks2 - p1.b.disks1;
			}
		}
		//（先攻２後攻１）
		for (int num(0);num < 1; ++num) {
			Play p1;
			Play p2;
			p1.input();
			p2.player = 2;
			p2.opponent = 1;
			p1.read(folder2);
			p2.read(folder1);
			p1.elim_width = ew;
			p2.elim_width = ew;
			while (!p1.endgame && !p2.endgame) {
				for (int i(0); i < 8; ++i) {
					for (int j(0); j < 8; ++j) {
						cout << p1.b.square[i][j] << ",";
					}
					cout << endl;
				}
				cout << endl;
				p1.runabwithbook();
				cout << p1.level << " " << folder2 << " " << p1.player << " " << p1.bestval << " " << p1.time_consumed << endl << endl;
				p1.record();
				//p1.record();
				//for (int i(0); i < 8;++i) {
				//	for (int j(0); j < 8;++j) {
				//		file << p1.b.square[i][j] << ",";
				//	}
				//	file << endl;
				//}
				p2.b = p1.b;
				for (int i(0); i < 8; ++i) {
					for (int j(0); j < 8; ++j) {
						cout << p2.b.square[i][j] << ",";
					}
					cout << endl;
				}
				cout << endl;
				p2.runabwithbook();
				cout << p2.level << " " << folder1 << " " << p2.player << " " << p2.bestval << " " << p2.time_consumed << endl << endl;
				p2.record();
				//p2.record();
				//for (int i(0); i < 8;++i) {
				//	for (int j(0); j < 8;++j) {
				//		file << p2.b.square[i][j] << ",";
				//	}
				//	file << endl;
				//}
				p1.b = p2.b;
			}
			p1.b.calculate_disks();
			if (p1.b.disks1 > p1.b.disks2) {
				cout << 1 << " win" << endl << endl;
				++wins2;
				windisks2 += p1.b.disks1 - p1.b.disks2;
			}
			else {
				cout << 2 << " win" << endl << endl;
				++wins1;
				windisks1 += p1.b.disks2 - p1.b.disks1;
			}
		}
		std::ofstream file;
		file.open("result.txt", std::ios::app);
		file << folder1 << "," << wins1 << "," << windisks1 << "," << endl;
		file << folder2 << "," << wins2 << "," << windisks2 << "," << endl << endl;
		cout << folder1 << "," << wins1 << "," << windisks1 << "," << endl;
		cout << folder2 << "," << wins2 << "," << windisks2 << "," << endl << endl;
	}

	/*
	std::ofstream test_file;
	test_file.open("PLAY\\test.txt", std::ios::out);
	test_file << wins1 << endl;
	test_file << wins2 << endl;*/

}

