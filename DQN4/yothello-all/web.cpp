
#include "neuralnet.hpp"
#include "math.hpp"
#include "othello.hpp"
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
	string neuralnet_folder = paras["neuralnet_folder"];
	bool perfect_random = stoi(paras["perfect_random"]);
	double cut_ratio = stod(paras["cut_ratio"]);
	double random = stod(paras["random"]);
	

	Neuralnet neuralnet;
	neuralnet.load(neuralnet_folder + "/bw");
	auto book = map_read_book(neuralnet_folder + "/book");
	Board b;
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			b.square[i][j] = square_init[i][j];
		}
	}
	b.turn = turn;
	b.turnnot = turn == 1 ? 2 : 1;
	b.calculate_disks();
	int disks = b.disks1 + b.disks2;
	int depth = disks + perfect_search >= 64 ? 99 : search_depth;
	double r = (perfect_random || depth < 99) ? random : 0;
	double cut_ratio_r = (disks + perfect_search <= 64) ? cut_ratio : 0;
	Board b2(b);
	double v = AB_move(book, b2, neuralnet, disks, depth, r, cut_ratio_r);
	pair<int,int> ij(moved_location(b, b2));
	int out = ij.first < 0 ? -1 : ij.first * 8 + ij.second;
	cout << out << endl;

	// auto_play();
	return 0;
}
