#include "dqn.hpp"
#include "neuralnet.hpp"
#include "book.hpp"
#include "math.hpp"
#include "othello.hpp"
#include "tree.hpp"
vector< vector<int> > hex_to_matrix(const string arg) {
	vector< vector<int> > v(8, vector<int>(8, 0));
	unsigned long long black(0);
	unsigned long long white(0);
	int x(0), y(0);
	for (int i(0); i < 16; ++i) {
		int a(arg[i] - '0');
		if (a > 9) {
			a += '0' - 'a' + 10;
		}
		for (int j(0); j < 4; ++j) {
			int d(a % 2);
			v[y][x + 3 - j] = d;
			a /= 2;
		}
		y += x == 4 ? 1 : 0;
 		x = x == 0 ? 4 : 0;
	}
	return v;
}

vector< vector<int> > matrix_to_square(const vector< vector<int> > b, const vector< vector<int> > w) {
	vector< vector<int> > s(8, vector<int> (8, 0));
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (b[i][j]) {
				s[i][j] = 1;
			}
			else if (w[i][j]) {
				s[i][j] = 2;
			}
		}
	}
	return s;
}

int main(int argc, char **argv) {
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

	map<string, string> paras(map_read_file("settings"));
	int search_depth = stoi(paras["search_depth"]);
	int perfect_search = stoi(paras["perfect_search"]);
	string neuralnet_folder = paras["neuralnet_folder0"];
	bool perfect_random = stoi(paras["perfect_random"]);
	bool show_all = stoi(paras["show_all"]);
	double cut_ratio = stod(paras["cut_ratio0"]);
	double random = stod(paras["random"]);


	Neuralnet neuralnet;
	neuralnet.load(neuralnet_folder + "/bw.csv");
	Board b;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			b.square[i][j] = square_init[i][j];
		}
	}
	b.turn = turn;
	Board b2;
	b.calculate_disks();
	int disks = b.disks1 + b.disks2;
	int depth = disks + perfect_search >= 64 ? 99 : search_depth;
	double r = (perfect_random || depth < 99) ? random : 0;
	double v = AB_move(b2, neuralnet, disks, depth, r, cut_ratio);
	pair<int,int> ij(moved_location(b, b2));
	int out = ij.first < 0 ? -1 : ij.first + ij.second;
	cout << out << endl;

	// auto_play();
	return 0;
}
