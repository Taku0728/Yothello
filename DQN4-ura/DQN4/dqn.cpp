
#include "dqn.hpp"
#include "neuralnet.hpp"
#include "math.hpp"
#include "othello.hpp"

int main() {
	montecarlo_learn("vn");
	// auto_play();
	// build_book();
	// test_accuracy();
	return 0;
}

void view_book() {
	map<string, string> paras = map_read_file("settings_book");
	Neuralnet neuralnet;
	int search_depth = stoi(paras["search_depth"]);
	string neuralnet_folder = paras["neuralnet_folder0"];
	double cut_ratio = stod(paras["cut_ratio0"]);
	double step_cost = stod(paras["step_cost"]);
	double range = stod(paras["range"]);
	neuralnet.load(neuralnet_folder + "/bw");
	auto book = map_read_book(neuralnet_folder + "/book");
	map<string, pair<double, double> > book_viewed;
	
	Board b;
	b.initialize();
	double tem_range = 0;
	while (tem_range < range) {
		tem_range += 0.1;
		surf_board(b, book, book_viewed, tem_range, step_cost);
	}
}


void surf_board(const Board &b,
					map<string, pair<double, double> > &m,
					map<string, pair<double, double> > &m2,
					const double range,
					const double step_cost)
{
	if (range < 0) {
		return;
	}
	auto nbs = next_boards(b);
	vector<double> v_value(0);
	double max_value = -99999;
	for (auto nb : nbs) {
		double value = 0;
		auto info = board_in_book(nb, m);
		auto info1 = board_in_book(nb, m2);
		if (info.first && !info1.first) {
			cout << "ADDING TURN: " << nb.turn << " VALUE: " << info.second.first << endl;
			value = info.second.first;
			nb.print();
			cin.get();	
			record_book(nb, m2, value, value);
		}
		else {
			value = -99999;
		}
		v_value.emplace_back(-value);
		max_value = max(max_value, -value);
	}
	for (int i(0); i < nbs.size(); ++i) {
		double n_range = range - (max_value - v_value[i]) - step_cost;
		surf_board(nbs[i], m, m2, n_range, step_cost);
	}
}

void explore_board(const Neuralnet neuralnet,
					const Board &b,
					map<string, pair<double, double> > &m,
					const int depth,
					const double cut_ratio,
					const double range,
					const double step_cost)
{
	if (range < 0) {
		return;
	}
	auto nbs = next_boards(b);
	vector<double> v_value(0);
	double max_value = -99999;
	for (auto nb : nbs) {
		double value = 0;
		auto info = board_in_book(nb, m);
		if (!info.first) {
			nb.calculate_disks();
			value = AB_value(nb, neuralnet, nb.disks1 + nb.disks2, depth, -99999, 99999, cut_ratio);
			record_book(nb, m, value, value);
			cout << "ADDING TURN: " << nb.turn << " VALUE: " << value << endl;
			nb.print();
		}
		else {
			value = info.second.first;
		}
		v_value.emplace_back(-value);
		max_value = max(max_value, -value);
	}
	for (int i(0); i < nbs.size(); ++i) {
		double n_range = range - (max_value - v_value[i]) - step_cost;
		explore_board(neuralnet, nbs[i], m, depth, cut_ratio, n_range, step_cost);
	}
}
void auto_play() {
	map<string, string> paras = map_read_file("settings");
	int search_depth0 = stoi(paras["search_depth0"]);
	int search_depth1 = stoi(paras["search_depth1"]);
	int perfect_search0 = stoi(paras["perfect_search0"]);
	int perfect_search1 = stoi(paras["perfect_search1"]);
	int games = stoi(paras["games"]);
	string neuralnet_folder0 = paras["neuralnet_folder0"];
	string neuralnet_folder1 = paras["neuralnet_folder1"];
	bool perfect_random = stoi(paras["perfect_random"]);
	bool show_all = stoi(paras["show_all"]);
	double cut_ratio0 = stod(paras["cut_ratio0"]);
	double cut_ratio1 = stod(paras["cut_ratio1"]);

	Neuralnet neuralnet0, neuralnet1;
	neuralnet0.load(neuralnet_folder0 + "/bw");
	neuralnet1.load(neuralnet_folder1 + "/bw");
	auto book0 = map_read_book(neuralnet_folder0 + "/book");
	auto book1 = map_read_book(neuralnet_folder1 + "/book");
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
			int search_depth = neuralnet0_turn ? search_depth0 : search_depth1;
			int perfect_search = neuralnet0_turn ? perfect_search0 : perfect_search1;
			int depth = disks + perfect_search >= 64 ? 99 : search_depth;
			double c_ratio = neuralnet0_turn ? cut_ratio0 : cut_ratio1;
			Neuralnet neuralnet = neuralnet0_turn ? neuralnet0 : neuralnet1;
			auto book = neuralnet0_turn ? book0 : book1;
			double r = (perfect_random || depth < 99) ? random : 0;
			double v = AB_move(book, b, neuralnet, b.disks1 + b.disks2, depth, r, c_ratio);
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
		error0 += abs(-(b.disks1 - b.disks2) * weight - value0 / number);
		error1 += abs(-(b.disks1 - b.disks2) * weight - value1 / number);
		win0 += (-(b.disks1 - b.disks2) * weight) > 0;
		win1 += (-(b.disks1 - b.disks2) * -weight) > 0;
		cout << (-((b.disks1 - b.disks2) * weight) > 0 ? neuralnet_folder0 : neuralnet_folder1)
			<< (-(b.disks1 - b.disks2) > 0 ? " BLACK" : " WHITE") << " WIN "
			<< b.disks1 - b.disks2 << endl << endl;
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
	neuralnet.load(neuralnet_folder + "/bw");
	auto book = map_read_book(neuralnet_folder + "/book");
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
			ml.games, book, neuralnet, ml.bottom_disks, ml.top_disks, ml.playing_search_depth, ml.random, ml.cut_ratio);
		vector<pair<Board, double> > dataset;
		cout << boards.size() << endl;
		cout << "\tVALUING >> ";
		vector<pair<double, Board> > display(0);
		vector<pair<int, double> > times(64, pair<int, double>(0, 0));
		for (auto b : boards) {
			b.calculate_disks();
			int disks = b.disks1 + b.disks2;
			int depth = disks + ml.perfect_search_depth >= 64 ? 99 : ml.teaching_search_depth;
			depth -= urd(mt) < 0.5 ? 1 : 0;
			depth += disks > 44 ? 1 : 0;
			Timer timer1;
			vector<pair<int, int> > route(0);
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
		neuralnet.save(neuralnet_folder + "/bw");
		
		double sec = timer0.count() / 1000;
		cout << "\tTIME: " << sec << endl;
		ml.save_log(neuralnet_folder + "/log", train_loss, test_loss, sec);

		int index = int(urd(mt) * dataset.size());
		vector<double> x = dataset[index].first.xin(inputs);
		cout << endl << dataset[index].first.turn << " " << dataset[index].second << " " << neuralnet.Forward(x)[0] << endl;
		dataset[index].first.print();
	}
}


void build_book()
{
	map<string, string> paras = map_read_file("settings_book");
	Neuralnet neuralnet;
	int search_depth = stoi(paras["search_depth"]);
	string neuralnet_folder = paras["neuralnet_folder0"];
	double cut_ratio = stod(paras["cut_ratio0"]);
	double step_cost = stod(paras["step_cost"]);
	double range = stod(paras["range"]);
	neuralnet.load(neuralnet_folder + "/bw");
	auto m = map_read_book(neuralnet_folder + "/book");
	

	Board b;
	b.initialize();
	double tem_range = 0;
	while (tem_range < range) {
		tem_range += 0.1;
		explore_board(neuralnet, b, m, search_depth, cut_ratio, tem_range, step_cost);
		map_save_book(neuralnet_folder + "/book", m);
		cout << "SAVED" << endl;
	}
}

void test_accuracy() {
	map<string, string> paras = map_read_file("settings_test");
	Neuralnet neuralnet;
	int number = stoi(paras["number"]);
	int search_depth = stoi(paras["search_depth"]);
	string neuralnet_folder = paras["neuralnet_folder0"];
	double cut_ratio = stod(paras["cut_ratio0"]);
	double random = stod(paras["random"]);
	double step = stod(paras["step"]);
	double max_cut = stod(paras["max_cut"]);

	neuralnet.load(neuralnet_folder + "/bw");
	auto book = map_read_book(neuralnet_folder + "/book");
	vector<Board> vb = random_play_boards(number, book, neuralnet, search_depth, random, cut_ratio);
	vector<double> result(int(max_cut/step),0);
	int count = 0;
	for (auto b : vb) {
		b.calculate_disks();
		int disks = b.disks1 + b.disks2;
		auto nbs = next_boards(b);
		if (nbs.size() == 0) {
			continue;
		}
		double sum_ratio(0);
		double max_con_value = -99999.;
		int max_index = 0;
		vector<pair<int, double> > vr;
		for (int i = 0; i != nbs.size(); ++i) {
			double value = -AB_value(nbs[i], neuralnet, disks + 1, 1, -99999, 99999, 0);
			double con_value = -AB_value(nbs[i], neuralnet, disks + 1, 2, -99999, 99999, 0);
			if (max_con_value < con_value) {
				max_con_value = con_value;
				max_index = i;
			}
			double ratio = pow(2, value / 5.);
			sum_ratio += ratio;
			vr.emplace_back(pair<int, double>(i, ratio));
		}
		sort(begin(vr), end(vr), 
			[](auto &l, auto &r){ return l.second > r.second; });
		
		vector<double> tem_result(int(max_cut/step), 0);
		for (int i(0); i < int(max_cut/step); ++i) {
			double w((i + 1.) * step);
			double tem_ratio(0);
			bool is_contained(false);
			for (int j = 0; j < vr.size(); ++j) {
				if (vr[j].first == max_index) {
					is_contained = true;
				}
				tem_ratio += vr[j].second;
				if (tem_ratio > sum_ratio * (1. - w)) {
					break;
				}
			}
			tem_result[i] += is_contained ? 1. : 0.;
		}
		++count;
		for (int i(0); i < int(max_cut/step); ++i) {
			result[i] += tem_result[i];
		}
	}

	cout << "STEP: " << step << endl;
	for (int i(0); i < int(max_cut/step); ++i) {
		cout << i * step << ": " << result[i] / count << endl;
	}

	std::ofstream writing_file(neuralnet_folder + "/acc.csv", std::ios::out);
	writing_file << "cut_ratio,error1,error2,error3,error4,error5,error6" << endl;
	for (int i(0); i < int(max_cut/step); ++i) {
		writing_file << i * step << "," << std::fixed;
		for (int j(1); j < 9; ++j) {
			writing_file << std::setprecision(4) << pow(result[i] / count, j) << ",";
		}
		writing_file << endl;
	}
    writing_file.close();
}
