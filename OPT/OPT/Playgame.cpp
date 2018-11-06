// Playgame.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h" 
#include "othello.h"
#include "maths.h"
#include "Learning.h"
#include "Tree.h"


int main()
{
	//int d1 = 16;
	//int d2 = 16;
	//int wins1(0);
	//int wins2(0);
	//int exl1(1000);
	//int exl2(2000);

	//for (int num(0);num < 5; ++num) {
	//	Play p1;
	//	Play p2;
	//	p1.input();
	//	p2.player = 2;
	//	p2.opponent = 2;
	//	p1.read("vn_74\\");
	//	p2.read("vn_74\\");
	//	std::ofstream file;
	//	file.open("record.txt", std::ios::out);
	//	p1.n0.depth = d1;
	//	p2.n0.depth = d2;
	//	p1.n0.exlimit = exl1;
	//	p2.n0.exlimit = exl2;
	//	p1.policy_number = 2;
	//	p2.policy_number = 2;
	//	while (!p1.endgame && !p2.endgame) {

	//		for (int i(0); i < 8; ++i) {
	//			for (int j(0); j < 8; ++j) {
	//				cout << p1.b.square[i][j] << ",";
	//			}
	//			cout << endl;
	//		}
	//		cout << endl;

	//		p1.run("vn_74\\");
	//		p1.record();
	//		for (int i(0); i < 8;++i) {
	//			for (int j(0); j < 8;++j) {
	//				file << p1.b.square[i][j] << ",";
	//			}
	//			file << endl;
	//		}
	//		p2.b = p1.b;
	//		p2.n0.b = p1.b;

	//		for (int i(0); i < 8; ++i) {
	//			for (int j(0); j < 8; ++j) {
	//				cout << p2.b.square[i][j] << ",";
	//			}
	//			cout << endl;
	//		}
	//		cout << endl;

	//		p2.run("vn_74\\");
	//		p2.record();
	//		for (int i(0); i < 8;++i) {
	//			for (int j(0); j < 8;++j) {
	//				file << p2.b.square[i][j] << ",";
	//			}
	//			file << endl;
	//		}
	//		p1.b = p2.b;
	//		p1.n0.b = p2.b;
	//	}
	//	p1.n0.b.calculate_disks();
	//	if (p1.n0.b.disks1 > p1.n0.b.disks2) {
	//		++wins1;
	//	}
	//	else {
	//		++wins2;
	//	}
	//}

	//std::ofstream test_file;
	//test_file.open("PALY\\test.txt", std::ios::out);

	//test_file << wins1 << endl;
	//test_file << wins2 << endl;

	Play p3;
	p3.policy_number = 2;
	p3("vn_74\\");
	p3.record();

	//Play p;
	//p.selfplay("vn_74\\");
}

