
#include "dqn.hpp"
#include "neuralnet.hpp"
#include "book.hpp"
#include "math.hpp"
#include "othello.hpp"
#include "tree.hpp"

int main() {
	montecarlo_learn("vn");
	// auto_play();
	return 0;
}

void auto_play() {
	map<string, string> paras = map_read_file("settings");
	int search_depth = stoi(paras["search_depth"]);
	int perfect_search = stoi(paras["perfect_search"]);
	int games = stoi(paras["games"]);
	string neuralnet_folder0 = paras["neuralnet_folder0"];
	string neuralnet_folder1 = paras["neuralnet_folder1"];
	bool perfect_random = stoi(paras["perfect_random"]);
	bool show_all = stoi(paras["show_all"]);
	double cut_ratio0 = stod(paras["cut_ratio0"]);
	double cut_ratio1 = stod(paras["cut_ratio1"]);

	Neuralnet neuralnet0, neuralnet1;
	neuralnet0.load(neuralnet_folder0 + "/bw.csv");
	neuralnet1.load(neuralnet_folder1 + "/bw.csv");
	double error0(0), error1(0), time0(0), time1(0);
	double random = stod(paras["random"]);
	int win0 = 0;
	int win1 = 0;
	for (int i(0); i < games; ++i) {
		bool end(false);
		Board b;
		b.initialize();
		int weight = i % 2 ? -1 : 1;
		int disks = 4;
		double value0(0), value1(0);
		int number(0);
		while (!end) {
			Timer timer;
			bool neuralnet0_turn = (b.turn == 1) ^ (i % 2);
			int depth = disks + perfect_search >= 64 ? 99 : search_depth;
			double c_ratio = neuralnet0_turn ? cut_ratio0 : cut_ratio1;
			Neuralnet neuralnet = neuralnet0_turn ? neuralnet0 : neuralnet1;
			double r = (perfect_random || depth < 99) ? random : 0;
			double v = AB_move(b, neuralnet, b.disks1 + b.disks2, depth, r, c_ratio);
			auto t = timer.count();
			value0 += (neuralnet0_turn && depth < 99 ? 1 : 0) * v;
			value1 += (neuralnet0_turn && depth < 99 ? 0 : 1) * v;
			time0 += (neuralnet0_turn ? 1 : 0) * t;
			time1 += (neuralnet0_turn ? 0 : 1) * t;
			++number;
			if (show_all) {
				cout << neuralnet_folder0 << ": " << (i % 2 ? "WHITE" : "BLACK") << endl;
				cout << "VALUE: " << v * (neuralnet0_turn ? 1 : -1) << endl;
				cout << "TIME: " << t / 1000 << endl;
				cout << "DISKS: " << disks / 1000 << endl;
				b.print();
			}
			b.calculate_disks();
			disks = b.disks1 + b.disks2;
			if (disks == 64 || b.is_end_game()) {
				end = true;
			}
		}
		if (!show_all) {
			b.print();
		}
		error0 += abs((b.disks1 - b.disks2) * weight - value0 / number);
		error1 += abs((b.disks1 - b.disks2) * weight - value1 / number);
		win0 += ((b.disks1 - b.disks2) * weight) > 0;
		win1 += ((b.disks1 - b.disks2) * -weight) > 0;
		cout << (((b.disks1 - b.disks2) * weight) > 0 ? neuralnet_folder0 : neuralnet_folder1)
			<< " WIN " << b.disks1 - b.disks2 << endl << endl;
	}
	cout << "RATE " << neuralnet_folder0 << " " << win0 * 100. / games << "%" <<
		" : " << neuralnet_folder1 << " " << win1 * 100. / games << "%" << endl;
	cout << "ERROR " << neuralnet_folder0 << " " << error0 / games << 
		" : " << neuralnet_folder1 << " " << error1 / games << endl;
	cout << "TIME " << neuralnet_folder0 << " " << time0 / games << 
		" : " << neuralnet_folder1 << " " << time1 / games << endl;

}


void montecarlo_learn(const string neuralnet_folder) {
	Learning ml;
	ml.load(neuralnet_folder + "/opt.txt");

	Neuralnet neuralnet;
	neuralnet.load(neuralnet_folder + "/bw.csv");
	if (!neuralnet.loaded) {
		neuralnet.set_network(ml.net);
		neuralnet.initialize_bw();
	}
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> urd(0.0, 1.0);
	int inputs(neuralnet.nodes[0]);
	Optimizer opt;
	opt.a = ml.a;
	
	for (int l(0); l < ml.learns; ++l) {
		--ml.bottom_disks;
		ml.bottom_disks = max(7, ml.bottom_disks);
		Timer timer0;
		cout << "LEARNING " << l << "/" << ml.learns << endl;
		cout << "\tPLAYING >> ";
		vector<Board> boards = random_play_boards(
			ml.games, neuralnet, ml.bottom_disks, ml.top_disks, ml.playing_search_depth, ml.random, ml.cut_ratio);
		vector<pair<Board, double> > dataset;
		cout << boards.size() << endl;
		cout << "\tVALUING >> ";
		vector<pair<double, Board> > display(0);
		vector<pair<int, double> > times(64, pair<int, double>(0, 0));
		for (auto b : boards) {
			b.calculate_disks();
			int disks = b.disks1 + b.disks2;
			vector<pair<int,int> > route(0);
			int depth = disks + ml.perfect_search_depth >= 64 ? 99 : ml.teaching_search_depth;
			depth -= urd(mt) < 0.5 ? 1 : 0;
			depth += disks > 44 ? 1 : 0;
			Timer timer1;
			double value = AB_value(b, neuralnet, disks, depth, -99999, 99999, ml.cut_ratio, route);
			
			
			++times[disks].first;
			times[disks].second += timer1.count();
			dataset.emplace_back(pair<Board, double>(b, value));
			double tem_value = value;
			if (depth < 20) {
				continue;
			}
			for (int i(0); i < route.size() && disks + i < ml.top_disks; ++i) {
				tem_value *= -1;
				b.move_board(route[i].first, route[i].second);
				dataset.emplace_back(pair<Board, double>(b, tem_value));
			}
		}
		cout << dataset.size() << endl;
		print_order(times, ml.show_time);

		vector<pair<Board, double> > dataset8;
		cout << "\tEXTENDING >> ";
		for (auto d : dataset) {
			Board b(d.first);
			double value(d.second);
			for (int i = 0; i != 4; ++i) {
				dataset8.emplace_back(pair<Board, double>(b, value));
				b.rotate();
			}
			b.reflect();
			for (int i = 0; i != 4; ++i) {
				dataset8.emplace_back(pair<Board, double>(b, value));
				b.rotate();
			}
		}
		cout << dataset8.size() << endl;
		vector<pair<vector<double>, vector<double> > > train_dataset;
		for (auto d : dataset8) {
			train_dataset.emplace_back(pair<vector<double>, vector<double> >
				(d.first.xin(inputs), vector<double> ({d.second})));
		}
		double test_loss = 0;
		ml.clearS();
		for (auto b : train_dataset) {
			neuralnet.Backward(ml.SdEdb, ml.SdEdw, b.first, b.second);
			double loss = pow(neuralnet.Forward(b.first)[0] - b.second[0], 2);
			test_loss += loss;
		}
		test_loss /= train_dataset.size();
		cout << "\tTEST LOSS: " << test_loss << endl;

		double train_loss = 0;
		for (int epoch(0); epoch < ml.n_epoch; ++epoch) {
			vector<int> perm(random_permutation(train_dataset.size(), mt));
			double sum_loss = 0;
			for (int i(0); i < train_dataset.size() - ml.batch; i += ml.batch) {
				vector<pair<vector<double>, vector<double> > > train_batch;
				for (int j(0); j < ml.batch; ++j) {
					train_batch.emplace_back(train_dataset[perm[i+j]]);
				}
				ml.clearS();
				for (auto b : train_batch) {
					neuralnet.Backward(ml.SdEdb, ml.SdEdw, b.first, b.second);
					double loss = pow(neuralnet.Forward(b.first)[0] - b.second[0], 2);
					sum_loss += loss;
				}
				opt(neuralnet, ml);
			}
			sum_loss /= train_dataset.size();
			train_loss += sum_loss;
			cout << "\t" << "EPOCH " << epoch << "/" << ml.n_epoch << " | " << "LOSS: " << sum_loss << endl;
		}
		train_loss /= ml.n_epoch;
		neuralnet.save(neuralnet_folder + "/bw.csv");
		
		double sec = timer0.count() / 1000;
		cout << "\tTIME: " << sec << endl;
		ml.save_log(neuralnet_folder + "/log.csv", train_loss, test_loss, sec);

		int index = int(urd(mt) * dataset.size());
		vector<double> x = dataset[index].first.xin(inputs);
		cout << dataset[index].first.turn << " " << dataset[index].second << " " << neuralnet.Forward(x)[0] << endl;
		dataset[index].first.print();
	}
}
