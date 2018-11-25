
#include "neuralnet.hpp"
#include "math.hpp"
#include "othello.hpp"

int main() {
	map<string, string> paras = map_read_file("settings");
	int search_depth = stoi(paras["search_depth"]);
	int perfect_search = stoi(paras["perfect_search"]);
	string neuralnet_folder = paras["neuralnet_folder0"];
	bool perfect_random = stoi(paras["perfect_random"]);
	double cut_ratio = stod(paras["cut_ratio0"]);

	Neuralnet neuralnet;
	neuralnet.load(neuralnet_folder + "/bw");
	auto book = map_read_book(neuralnet_folder + "/book");
	double error0(0), error1(0), time0(0), time1(0);
	double random = stod(paras["random"]);
	int win0 = 0;
	int win1 = 0;

	int player_turn = 1;
	cout << "- YOTHELLO -" << endl;
	cout << "(-1, -1): PASS" << endl;
	cout << "(-2, -2): UNDO" << endl << endl;;
	cout << "PLAYER TURN (1 or 2) : ";
	cin >> player_turn;

	bool end(false);
	vector<Board> vb;
	Board b;
	b.initialize();
	int weight = player_turn == 1 ? -1 : 1;
	int disks = 4;
	double value0(0), value1(0);
	int number(0);
	while (!end) {
		b.calculate_disks();
		disks = b.disks1 + b.disks2;
		vb.push_back(b);
		b.print();
		Timer timer;
		if (b.turn == player_turn) {
			int i, j;
			while (1) {
				cout << "INPUT MOVE : ";
				cin >> i >> j;
				if (i == -2) {
					cout << "UNDO?(y/n) : ";
					string s;
					cin >> s;
					if (s == "y") {
						while (1) {
							vb.pop_back();
							b = vb[vb.size() - 1];
							if (b.turn == player_turn) {
								break;
							}
						}
						b.print();
					}
					continue;
				}
				else if (i < 0 || 7 < i || j < 0 || 7 < j) {
					b.calculate_moves();
					if (b.move_num == 0) {
						break;
					}
				}
				else {
					if (b.square[i][j] == 0 && b.right_move(i, j)) {
						break;
					}
				}
				cout << "INVALID MOVE" << endl;
			}
			b.move_board(i, j);
			auto t = timer.count();
			time1 += t;
		}
		else {
			int depth = disks + perfect_search >= 64 ? 99 : search_depth;
			double r = (perfect_random || depth < 99) ? random : 0;
			double cut_ratio_r = (disks + perfect_search <= 64) ? cut_ratio : 0;
			double v = AB_move(book, b, neuralnet, disks, depth, r, cut_ratio_r);
			auto t = timer.count();
			value0 += v;
			time0 += t;
			++number;
			cout << "VALUE: " << v << endl;
			cout << "TIME: " << t / 1000 << endl;
			cout << "DISKS: " << disks / 1000 << endl;
			b.calculate_disks();
			disks = b.disks1 + b.disks2;
			if (disks == 64 || b.is_end_game()) {
				end = true;
			}
		}
	}
	b.print();
	win0 += ((b.disks1 - b.disks2) * weight) > 0;
	win1 += ((b.disks1 - b.disks2) * -weight) > 0;
	cout << (((b.disks1 - b.disks2) * weight) > 0 ? "YOTHELLO" : "PLAYER")
		<< " WIN " << b.disks1 - b.disks2 << endl << endl;
}
