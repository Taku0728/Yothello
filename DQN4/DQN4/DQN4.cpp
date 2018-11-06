
#include "stdafx.h" 
#include "othello.h"
#include "maths.h"
#include "Learning.h"
#include "DQN.h"

static const char *mypc("DESKTOP-O9TNNR9");

DQN::DQN(string folder)
//{time, batch, x_begin, x_end, y_begin, y_end, tests}
	:para(0, 0),
	net(0, 0)
{
	opt_file = folder + "opt.txt";
	std::ifstream ifs(opt_file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	std::string str;
	getline(ifs, str);
	opt_time = stoi(str);
	getline(ifs, str);
	teach_file = folder + str;
	getline(ifs, str);
	test_file = folder + str;
	getline(ifs, str);
	aia_file = folder + str;
	getline(ifs, str);
	wb_file = folder + str;
	getline(ifs, str);
	std::string token;
	std::istringstream stream(str);
	while (getline(stream, token, ',')) {
		para.emplace_back(std::stod(token));
	}
	getline(ifs, str);
	std::istringstream stream2(str);
	while (getline(stream2, token, ',')) {
		net.emplace_back(std::stoi(token));
	}
}

DQN::~DQN()
{
	le.save(para_file);
	neu.save(wb_file);

}

Teach::Teach()
{
}

Teach::~Teach()
{
}

void Teach::operator()(Learning &le, Neuralnet &neu, Optimizer &op, int i)
{
	for (int j = 0;j <= le.batch - 1;++j) {
		neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i + j], le.teach_data_y[i + j]);
	}
	op(neu, le);
}

Teach8::Teach8()
{
}

Teach8::~Teach8()
{
}

void Teach8::operator()(Learning &le, Neuralnet &neu, Optimizer &op, int i)
{
	Rotate_board rtb;
	Reflect_board rfb;
	for (int j(0);j <= le.batch;++j) {
		neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		for (int k(0);k < 3;++k) {
			rtb(le.teach_data_x[i*le.batch + j]);
			rtb(le.teach_data_y[i*le.batch + j]);
			neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		}
		rfb(le.teach_data_x[i*le.batch + j]);
		rfb(le.teach_data_y[i*le.batch + j]);
		neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		for (int k(0);k < 3;++k) {
			rtb(le.teach_data_x[i*le.batch + j]);
			rtb(le.teach_data_y[i*le.batch + j]);
			neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		}
	}
	op(neu, le);
}

Teachv8::Teachv8()
{
}

Teachv8::~Teachv8()
{
}

void Teachv8::operator()(Learning &le, Neuralnet &neu, Optimizer &op, int i)
{
	Rotate_board rtb;
	Reflect_board rfb;
	for (int j(0);j <= le.batch - 1;++j) {
		neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		for (int k(0);k < 3;++k) {
			rtb(le.teach_data_x[i*le.batch + j]);
			neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		}
		rfb(le.teach_data_x[i*le.batch + j]);
		neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		for (int k(0);k < 3;++k) {
			rtb(le.teach_data_x[i*le.batch + j]);
			neu.Backward(le.SdEdb, le.SdEdw, le.teach_data_x[i*le.batch + j], le.teach_data_y[i*le.batch + j]);
		}
	}
	op(neu, le);
}

void DQN::run(Teach te, Optimizer op)
{
	Learning le;
	le.set_parameters(para, net);
	le.read_teach_data(teach_file);
	le.read_test_data(test_file);
	le.separate_data();
	op.a = le.a;

	Rotate_board rtb;
	Reflect_board rfb;

	if (opt_time == 0) {
		neu.set_network(net);
		double e(0);

		while (e == 0) {
			neu.initialize_bw();
			le.test_accuracy(neu);
			te(le, neu, op, 0);
			le.test_accuracy(neu);
			e = le.get_accuracy_e();
		}
	}
	else {
		le.load(para_file);
		neu.load(wb_file);
		op.a = le.a;
	}

	int i(0);
	double percentage(0);
	while (i < le.times) {
		for (int k(0);k < 100;++k) {
			te(le, neu, op, i);
			++i;
		}
		le.test_accuracy(neu);
		if (percentage < i * 100 / le.times) {
			cout << percentage << "%" << endl;
			++percentage;
		}
	}
	le.write_accuracy(aia_file);
	neu.save(wb_file);
}

void DQN::run(const Data &da, Teach te, Optimizer &op)
{
	Learning le;
	le.set_parameters(para, net);
	le.teach_data_all = da.dteach;
	le.test_data_all = da.dtest;
	le.separate_data();
	op.a = le.a;

	Rotate_board rtb;
	Reflect_board rfb;

	if (opt_time == 0) {
		neu.set_network(net);
		double e(0);

		while (e == 0) {
			neu.initialize_bw();
			le.test_accuracy(neu);
			te(le, neu, op, 0);
			le.test_accuracy(neu);
			e = le.get_accuracy_e();
		}
	}
	else {
		le.load(para_file);
		neu.load(wb_file);
		op.a = le.a;
	}

	int i(0);
	double percentage(0);
	while (i < le.times) {
		for (int k(0);k < 100;++k) {
			te(le, neu, op, i);
			++i;
		}
		le.test_accuracy(neu);
		if (percentage < i * 100 / le.times) {
			cout << percentage << "%" << endl;
			++percentage;
		}
	}
	le.write_accuracy(aia_file);
	neu.save(wb_file);
}

//int main()
//{

//	//Board b;
//	//b.read_board_v();
//	//b.write_board_full();
//	clock_t start = clock();
//	string folder("sp64_2\\");
//	
//	DQN dqn1(folder);
//	Teach8 te8;
//	SDG sdg;
//	Adam ad;
//	dqn1.run(te8, sdg);
//
//	clock_t end = clock();
//	std::string filename = folder + "Result.txt";
//	std::ofstream writing_file2;
//	writing_file2.open(filename, std::ios::app);
//	writing_file2 << "file      : " << folder << std::endl;
//	writing_file2 << "Time Cost : " << end - start << std::endl;

//Learning le;
//Data d;
//int n(51);

//for (int i = 43;i <= n;++i) {
//	d.readggf("Othello.e4\\Othello." + to_string(i) + "e4.ggf", 150000);
//	d.writeggfwin("Othello.e4\\Othello" + to_string(i) + ".e4.csv");
//}
//}

TDL::TDL()
	:readdone(false),
	readjobdone(false),
	running_condition(true),
	biaslearning(false),
	bias(1.1)
{
	Fill(elim_choices_ave, 0);
	Fill(choices_ave, 0);
	Fill(err, 0);
}

TDL::~TDL()
{
}

void TDL::run()
{
	thispc = get_pcname();
	renewcontrol_start();
	if (running_condition == false) {
		renewcontrol_end();
		return;
	}

	std::ifstream ifs("control\\jobs4.txt");
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	ifs.close();

	int patch(4);
	readjob();
	for (int l(startlevel);l >= endlevel; --l) {
		learning_level = l;
		renewjob();
		renewcontrol_start();
		if (running_condition == false) {
			renewcontrol_end();
			return;
		}
		cout << "level " << l << " >> ";

		for (int i(0);i < patch;++i) {
			learn(l, pow(0.8, i));
			cout << (i + 1) * 25 << "% >> ";
		}


		if (l < 64) {
			addsum(l);
		}
		cout << endl;
	}
	write_test();
	renewjob();
	renewcontrol_end();
}

void TDL::read()
{
	opt_file = folder1 + "opt.txt";
	opt_file2 = folder2 + "opt.txt";
	summary_file = folder1 + "summary.csv";
	summary_file2 = folder2 + "summary.csv";
	test_file = folder1 + "test.txt";

	std::ifstream ifs(opt_file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	std::string str;
	getline(ifs, str);
	opt_level = stoi(str);
	for (int i(1);i < 65;++i) {
		bw_file[i] = folder1 + "bw\\bw" + to_string(i) + ".csv";
	}
	for (int i(1);i < 65;++i) {
		acc_file[i] = folder1 + "acc\\acc" + to_string(i) + ".csv";
	}
	for (int i(1);i < 65;++i) {
		sigacc_file[i] = folder1 + "sigacc\\sigacc" + to_string(i) + ".csv";
	}
	policy_file = folder1 + "policy.csv";
	getline(ifs, str);
	std::string token;
	std::istringstream stream(str);
	while (getline(stream, token, ',')) {
		para.emplace_back(std::stod(token));
	}
	getline(ifs, str);
	std::istringstream stream2(str);
	while (getline(stream2, token, ',')) {
		net.emplace_back(std::stoi(token));
	}
	ifs.close();

	for (int i(6);i < 65;++i) {
		neu[i].set_network(net);
	}

	for (int i(64);i > 5;--i) {
		std::ifstream ifs(bw_file[i]);
		if (ifs) {
			opt_level = i;
		}
		ifs.close();
		neu[i].load(bw_file[i]);
	}
	//read_sum();


	if (folder1 != folder2) {
		std::ifstream ifs2(opt_file2);
		if (!ifs2) {
			std::cout << "入力エラー";
			return;
		}
		getline(ifs2, str);
		for (int i(1);i < 65;++i) {
			bw_file2[i] = folder2 + "bw\\bw" + to_string(i) + ".csv";
		}
		for (int i(1);i < 65;++i) {
			acc_file2[i] = folder2 + "acc\\acc" + to_string(i) + ".csv";
		}
		for (int i(1);i < 65;++i) {
			sigacc_file2[i] = folder2 + "sigacc\\sigacc" + to_string(i) + ".csv";
		}
		policy_file2 = folder2 + "policy.csv";
		getline(ifs2, str);
		std::istringstream stream3(str);
		while (getline(stream3, token, ',')) {
			para2.emplace_back(std::stod(token));
		}
		getline(ifs2, str);
		std::istringstream stream4(str);
		while (getline(stream4, token, ',')) {
			net2.emplace_back(std::stoi(token));
		}
		ifs2.close();

		for (int i(6);i < 65;++i) {
			neu2[i].set_network(net2);
		}
		opt_level2 = 65;
		for (int i(64);i > 5;--i) {
			std::ifstream ifs2(bw_file2[i]);
			if (ifs2) {
				opt_level2 = i;
			}
			ifs2.close();
			neu2[i].load(bw_file2[i]);
		}
	}

	if (biaslearning) {
		for (int i(1);i < 65;++i) {
			bw_file3[i] = folder3 + "bw\\bw" + to_string(i) + ".csv";
		}
		for (int i(64);i > 5;--i) {
			neu3[i].load(bw_file3[i]);
		}
	}

	//read_sum();

	read_policy();

	readdone = true;
}

void TDL::read_policy()
{
	std::ifstream ifs(policy_file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	std::string str;
	getline(ifs, str);
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		getline(stream, token, ',');
		int lev = stoi(token);
		getline(stream, token, ',');
		opt_a[lev] = stod(token);
		getline(stream, token, ',');
		w[lev] = stod(token);
		getline(stream, token, ',');
		depth[lev] = stoi(token);
		getline(stream, token, ',');
		shrink[lev] = stod(token);
	}
	ifs.close();

	if (folder1 != folder2) {
		std::ifstream ifs2(policy_file2);
		if (!ifs2) {
			std::cout << "入力エラー";
			return;
		}
		getline(ifs2, str);
		while (getline(ifs2, str)) {
			std::string token;
			std::istringstream stream(str);
			getline(stream, token, ',');
			int lev = stoi(token);
			getline(stream, token, ',');
			opt_a2[lev] = stod(token);
		}
		ifs2.close();
	}
}

void TDL::read_sum()
{
	std::ifstream ifs(summary_file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	std::string str;
	getline(ifs, str);

	int l(65);
	while (getline(ifs, str) && (l > opt_level + 2)) {
		string token;
		istringstream stream(str);
		getline(stream, token, ',');
		l = stoi(token);
		getline(stream, token, ',');
		acc[l] = stod(token);
		getline(stream, token, ',');
		sigacc[l] = stod(token);
		getline(stream, token, ',');
		err[l] = stod(token);
		getline(stream, token, ',');
		choices_ave[l] = stod(token);
		getline(stream, token, ',');
		elim_choices_ave[l] = stod(token);
	}
	ifs.close();
}

void TDL::readjob()
{
	std::ifstream ifs("control\\jobs4.txt");
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	string str;
	getline(ifs, str);
	if (thispc != mypc) {
		getline(ifs, str);
	}
	string token;
	istringstream stream(str);
	getline(stream, token, ',');
	foldername = token;
	folder1 = token + "\\";
	getline(stream, token, ',');
	foldername2 = token;
	folder2 = token + "\\";
	getline(stream, token, ',');
	startlevel = stoi(token);
	getline(stream, token, ',');
	endlevel = stoi(token);
	getline(stream, token, ',');
	bias = stod(token);
	if (bias != 0) {
		biaslearning = true;
	}
	if (biaslearning) {
		getline(stream, token, ',');
		foldername3 = token;
		folder3 = token + "\\";
	}

	if (foldername != foldername2) {
		cout << "LEARN " << foldername << "(BASE) & " << foldername2;
	}
	else {
		cout << "LEARN " << foldername << " SELF ";
	}
	if (biaslearning) {
		cout << " (PLAY WITH " << foldername3 << ")";
	}
	cout << endl;
	readjobdone = true;
	ifs.close();
}

void TDL::renewjob()
{
	std::ifstream ifs("control\\jobs4.txt");
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}

	string job1;
	string job2;
	string str;
	getline(ifs, job1);
	getline(ifs, job2);
	ifs.close();

	double temi;
	if (biaslearning) {
		temi = bias;
	}
	else {
		temi = 0;
	}
	ofstream ofs;
	ofs.open("control\\jobs4.txt", std::ios::out);
	if (thispc == mypc) {
		ofs << foldername << "," << foldername2 << "," << learning_level << "," << 6 << "," << temi << ",";
		if (biaslearning) {
			ofs << foldername3 << ",";
		}
		ofs << endl;
		ofs << job2 << endl;
	}
	else {
		ofs << job1 << endl;
		ofs << foldername << "," << foldername2 << "," << learning_level << "," << 6 << "," << temi << ",";
		if (biaslearning) {
			ofs << foldername3 << ",";
		}
		ofs << endl;
	}
	ofs.close();
}

//void TDL::learn(const int level, double as)
//{
//	if (!readjobdone) {
//		readjob();
//	}
//	if (!readdone) {
//		read();
//	}
//
//	lea[level].set_paras(para, net);
//	if (folder1 != folder2) {
//		lea2[level].set_paras(para, net2);
//	}
//
//	//clock_t start = clock();
//
//	int inputs(net[0]);
//	int i(0);
//	int tr(0);
//	int iout(1);
//	int trial(lea[level].times*lea[level].batch);
//	while (i < trial) {
//		Game g;
//		Board b;
//		b.initialize();
//		//g.playto(b, chr, chr, level - 1);
//		//if (!b.get_end_game()) {
//		//}
//
//		//if (level > 54) {
//		//	g.playoutvalue74(b, level);
//		//}
//		//else {
//		//	g.playoutvalue74wd(b, level, neu, w, shrink);
//		//}
//		if (inputs == 74) {
//			if (biaslearning) {
//				g.ABoutbia74(b, level, neu, neu3, shrink, depth);
//			}
//			else {
//				g.ABout74(b, level, neu, shrink, depth);
//			}
//		}
//		else if (inputs == 102) {
//			if (biaslearning) {
//				g.ABoutbia102(b, level, neu, neu3, shrink, depth);
//			}
//			else {
//				g.ABout102(b, level, neu, shrink, depth);
//			}
//		}
//		else {
//			return;
//		}
//
//		//g.playoutvalue(b, level);
//		//g.playout(b,level);
//		++tr;
//		if (g.disks == level) {
//			//vector<double> v(65, 0);
//			lea[level].teach_data_x[i].resize(inputs);
//			if (folder1 != folder2) {
//				lea2[level].teach_data_x[i].resize(inputs);
//			}
//			for (int j(0);j < inputs;++j) {
//				lea[level].teach_data_x[i][j] = g.playoutb[j];
//				if (folder1 != folder2) {
//					lea2[level].teach_data_x[i][j] = g.playoutb[j];
//				}
//			}
//			lea[level].teach_data_y[i].resize(1, g.playoutb[inputs]);
//			if (folder1 != folder2) {
//				lea2[level].teach_data_y[i].resize(1, g.playoutb[inputs]);
//			}
//			//lea.teach_data_y[i][0] = double(g.playoutb[66]);
//			//lea.teach_data_y[i][0] = winvalue(g.playoutb[66]);
//			++i;
//			if (double(i) / double(trial) *10.0 > iout) {
//				cout << iout * 10 << "% >> ";
//				iout += 1;
//			}
//			//cout << i << endl;
//		}
//	}
//
//	i = 0;
//	while (i < lea[level].tests) {
//		Game g;
//		Board b;
//		b.initialize();
//		//if (level > 54) {
//		//	g.playoutvalue74(b, level);
//		//}
//		//else {
//		//	g.playoutvalue74wd(b, level, neu, w, shrink);
//		//}
//		if (inputs == 74) {
//			if (biaslearning) {
//				g.ABoutbia74(b, level, neu, neu3, shrink, depth);
//			}
//			else {
//				g.ABout74(b, level, neu, shrink, depth);
//			}
//		}
//		else if (inputs == 102) {
//			if (biaslearning) {
//				g.ABoutbia102(b, level, neu, neu3, shrink, depth);
//			}
//			else {
//				g.ABout102(b, level, neu, shrink, depth);
//			}
//		}
//		//g.playoutvalue(b, chr, chr, level);
//		//g.playout(b, chr, chr, level);
//		++tr;
//		if (g.disks == level) {
//			//vector<double> v(65, 0);
//			lea[level].test_data_x[i].resize(inputs);
//			if (folder1 != folder2) {
//				lea2[level].test_data_x[i].resize(inputs);
//			}
//			for (int j(0);j < inputs;++j) {
//				lea[level].test_data_x[i][j] = g.playoutb[j];
//				if (folder1 != folder2) {
//					lea2[level].test_data_x[i][j] = g.playoutb[j];
//				}
//			}
//			lea[level].test_data_y[i].resize(1, g.playoutb[inputs]);
//			if (folder1 != folder2) {
//				lea2[level].test_data_y[i].resize(1, g.playoutb[inputs]);
//			}
//			//lea.test_data_y[i][0] = double(g.playoutb[66]);
//			//lea.teach_data_y[i][0] = winvalue(g.playoutb[66]);
//			++i;
//		}
//	}
//
//	//clock_t mid = clock();
//
//	//std::string filename = folder + "testdata.csv";
//	//std::ofstream writing_file;
//	//writing_file.open(filename, std::ios::app);
//	//for (int m(0);m < 50;++m) {
//	//	for (int l(0); l < inputs;++l) {
//	//		writing_file << lea.test_data_x[m][l] << ",";
//	//	}
//	//	writing_file << lea.test_data_y[m][0] << "," << endl;
//	//}
//
//	//Rotate_board rtb;
//	//Reflect_board rfb;
//
//	if (opt_level > 64) {
//		neu[level].set_network(net);
//		double e(0);
//
//		while (e == 0) {
//			lea[level].t = 0;
//			neu[level].initialize_bw();
//			lea[level].test_accuracy(neu[level]);
//			tea(lea[level], neu[level], opt, 0);
//			lea[level].test_accuracy(neu[level]);
//			e = lea[level].get_accuracy_e();
//		}
//		opt_level = level;
//	}
//	opt.a = opt_a[level] * as;
//
//	if (opt_level > level) {
//		int rl(opt_level - 1);
//		while (rl >= level) {
//			neu[rl] = neu[rl + 1];
//			--rl;
//		}
//		opt_level = level;
//	}
//
//
//	i = 0;
//	lea[level].ai_accuracy.resize(0);
//	lea[level].ai_accuracy_sign.resize(0);
//	while (i < lea[level].times*lea[level].batch) {
//		for (int k(0);k < 1000;++k) {
//			tea(lea[level], neu[level], opt, i);
//			++i;
//		}
//		lea[level].test_accuracy(neu[level]);
//		lea[level].test_accuracy_sign(neu[level]);
//	}
//	lea[level].write_accuracy(acc_file[level]);
//	lea[level].write_accuracy_sign(sigacc_file[level]);
//	neu[level].save(bw_file[level]);
//
//
//	if (folder1 != folder2) {
//		//2も学習
//		if (opt_level2 > level) {
//			int rl(opt_level2 - 1);
//			while (rl >= level) {
//				neu2[rl] = neu2[rl + 1];
//				--rl;
//			}
//			opt_level2 = level;
//		}
//		opt2.a = opt_a2[level] * as;
//
//
//		i = 0;
//		lea2[level].ai_accuracy.resize(0);
//		lea2[level].ai_accuracy_sign.resize(0);
//		while (i < lea[level].times*lea[level].batch) {
//			for (int k(0);k < 1000;++k) {
//				tea(lea2[level], neu2[level], opt2, i);
//				++i;
//			}
//			lea2[level].test_accuracy(neu2[level]);
//			lea2[level].test_accuracy_sign(neu2[level]);
//		}
//		lea2[level].write_accuracy(acc_file2[level]);
//		lea2[level].write_accuracy_sign(sigacc_file2[level]);
//		neu2[level].save(bw_file2[level]);
//	}
//
//	//lea.save(para_file);
//	//cout << pl << "%" << endl;
//	//}
//
//	//clock_t end = clock();
//	//std::string filename2 = folder1 + "Result.txt";
//	//std::ofstream writing_file2;
//	//writing_file2.open(filename2, std::ios::app);
//	//writing_file2 << "Level    : " << level << std::endl;
//	//writing_file2 << "TDG time : " << mid - start << std::endl;
//	//writing_file2 << "OPT time : " << end - mid << std::endl;
//	//writing_file2 << std::endl;
//	//writing_file2 << "OPT time : " << end - start << std::endl;
//
//	//writing_file2.close();
//}

void TDL::learn(const int level, double as)
{
	if (!readjobdone) {
		readjob();
	}
	if (!readdone) {
		read();
	}

	lea.set_paras(para, net);
	if (folder1 != folder2) {
		lea2.set_paras(para, net2);
	}

	//clock_t start = clock();


	int i(0);
	int inputs(net[0]);
	int iout(1);
	int trial(lea.times*lea.batch);

	while (i < trial) {
		Game g;
		g.bias = bias;
		Board b;
		b.initialize();
		//g.playto(b, chr, chr, level - 1);
		//if (!b.get_end_game()) {
		//}

		//if (level > 54) {
		//	g.playoutvalue74(b, level);
		//}
		//else {
		//	g.playoutvalue74wd(b, level, neu, w, shrink);
		//}

		if (biaslearning) {
			g.ABoutbia8(b, level, neu, neu3, shrink, depth);
		}
		else {
			g.ABout(b, level, neu, shrink, depth);
		}

		//g.playoutvalue(b, level);
		//g.playout(b,level);
		if (g.disks == level) {
			for (int k(0);k < 8;++k) {
				//vector<double> v(65, 0);
				lea.teach_data_x[i].resize(inputs);
				if (folder1 != folder2) {
					lea2.teach_data_x[i].resize(inputs);
				}
				for (int j(0);j < inputs;++j) {
					lea.teach_data_x[i][j] = g.playoutb8[k][j];
					if (folder1 != folder2) {
						lea2.teach_data_x[i][j] = g.playoutb8[k][j];
					}
				}
				lea.teach_data_y[i].resize(1, g.playoutb8[k][inputs]);
				if (folder1 != folder2) {
					lea2.teach_data_y[i].resize(1, g.playoutb8[k][inputs]);
				}
				++i;
				//lea.teach_data_y[i][0] = double(g.playoutb[66]);
				//lea.teach_data_y[i][0] = winvalue(g.playoutb[66]);
				//cout << i << endl;
			}
			//if (double(i) / double(trial) *10.0 > iout) {
			//cout << iout * 10 << "% >> ";
			//iout += 1;
			//}
		}
	}


	i = 0;
	while (i < lea.tests) {
		Game g;
		g.bias = bias;
		Board b;
		b.initialize();
		//if (level > 54) {
		//	g.playoutvalue74(b, level);
		//}
		//else {
		//	g.playoutvalue74wd(b, level, neu, w, shrink);
		//}

		if (biaslearning) {
			g.ABoutbia(b, level, neu, neu3, shrink, depth);
		}
		else {
			g.ABout(b, level, neu, shrink, depth);
		}

		//g.playoutvalue(b, chr, chr, level);
		//g.playout(b, chr, chr, level);
		if (g.disks == level) {
			//vector<double> v(65, 0);
			lea.test_data_x[i].resize(inputs);
			if (folder1 != folder2) {
				lea2.test_data_x[i].resize(inputs);
			}
			for (int j(0);j < inputs;++j) {
				lea.test_data_x[i][j] = g.playoutb[j];
				if (folder1 != folder2) {
					lea2.test_data_x[i][j] = g.playoutb[j];
				}
			}
			lea.test_data_y[i].resize(1, g.playoutb[inputs]);
			if (folder1 != folder2) {
				lea2.test_data_y[i].resize(1, g.playoutb[inputs]);
			}
			++i;
			//lea.test_data_y[i][0] = double(g.playoutb[66]);
			//lea.teach_data_y[i][0] = winvalue(g.playoutb[66]);
		}
	}

	//clock_t mid = clock();

	//std::string filename = folder + "testdata.csv";
	//std::ofstream writing_file;
	//writing_file.open(filename, std::ios::app);
	//for (int m(0);m < 50;++m) {
	//	for (int l(0); l < inputs;++l) {
	//		writing_file << lea.test_data_x[m][l] << ",";
	//	}
	//	writing_file << lea.test_data_y[m][0] << "," << endl;
	//}

	//Rotate_board rtb;
	//Reflect_board rfb;

	if (opt_level > 63) {
		neu[level].set_network(net);
		double e(0);

		while (e == 0) {
			lea.t = 0;
			neu[level].initialize_bw();
			lea.test_accuracy(neu[level]);
			tea(lea, neu[level], opt, 0);
			lea.test_accuracy(neu[level]);
			e = lea.get_accuracy_e();
		}
		opt_level = level;
	}
	opt.a = opt_a[level] * as;

	if (opt_level > level) {
		int rl(opt_level - 1);
		while (rl >= level) {
			neu[rl] = neu[rl + 1];
			--rl;
		}
		opt_level = level;
	}

	i = 0;
	lea.ai_accuracy.resize(0);
	lea.ai_accuracy_sign.resize(0);
	while (i < lea.times*lea.batch) {
		for (int k(0);k < 1000;++k) {
			tea(lea, neu[level], opt, i);
			i += lea.batch;
		}
		lea.test_accuracy(neu[level]);
		lea.test_accuracy_sign(neu[level]);
	}
	lea.write_accuracy(acc_file[level]);
	lea.write_accuracy_sign(sigacc_file[level]);
	neu[level].save(bw_file[level]);


	if (folder1 != folder2) {
		if (opt_level2 > 63) {
			neu2[level].set_network(net2);
			double e(0);

			while (e == 0) {
				lea2.t = 0;
				neu2[level].initialize_bw();
				lea2.test_accuracy(neu2[level]);
				tea(lea2, neu2[level], opt, 0);
				lea2.test_accuracy(neu2[level]);
				e = lea2.get_accuracy_e();
			}
			opt_level2 = level;
		}
		opt2.a = opt_a2[level] * as;

		//2も学習
		if (opt_level2 > level) {
			int rl(opt_level2 - 1);
			while (rl >= level) {
				neu2[rl] = neu2[rl + 1];
				--rl;
			}
			opt_level2 = level;
		}
		opt2.a = opt_a2[level] * as;


		i = 0;
		lea2.ai_accuracy.resize(0);
		lea2.ai_accuracy_sign.resize(0);
		while (i < lea.times*lea.batch) {
			for (int k(0);k < 1000;++k) {
				tea(lea2, neu2[level], opt2, i);
				i += lea.batch;
			}
			lea2.test_accuracy(neu2[level]);
			lea2.test_accuracy_sign(neu2[level]);
		}
		lea2.write_accuracy(acc_file2[level]);
		lea2.write_accuracy_sign(sigacc_file2[level]);
		neu2[level].save(bw_file2[level]);
	}

	//lea.save(para_file);
	//cout << pl << "%" << endl;
	//}

	//clock_t end = clock();
	//std::string filename2 = folder1 + "Result.txt";
	//std::ofstream writing_file2;
	//writing_file2.open(filename2, std::ios::app);
	//writing_file2 << "Level    : " << level << std::endl;
	//writing_file2 << "TDG time : " << mid - start << std::endl;
	//writing_file2 << "OPT time : " << end - mid << std::endl;
	//writing_file2 << std::endl;
	//writing_file2 << "OPT time : " << end - start << std::endl;

	//writing_file2.close();
}

void TDL::test(const int level)
{
	if (!readjobdone) {
		readjob();
	}
	if (!readdone) {
		read();
	}
	clock_t start = clock();

	//for (int pl(0); pl < 1000;++pl) {

	Learning lea;
	lea.set_paras(para, net);

	cout << " test " << level << " >> ";

	int inputs(net[0]);
	int i(0);
	int tr(0);
	int elim_errs(0);
	int choices_sum(0);
	int elim_choices_sum(0);
	double max_abs_sum(0);
	double max_squ_sum(0);
	while (i < lea.tests) {
		Game g;
		g.bias = bias;
		Board b;
		b.initialize();
		//if (level > 54) {
		//	if (level == 64) {
		//		g.playoutvalue74(b, level);
		//	}
		//	else {
		//		g.playoutvalue74test(b, level, neu[level + 1], w[level + 1]);
		//	}
		//}
		//else {
		//	g.playoutvalue74wdtest(b, level, neu, w, shrink);
		//}


		if (biaslearning) {
			g.ABoutbiatest(b, level, neu, neu3, w, shrink, depth);
		}
		else {
			g.ABouttest(b, level, neu, w, shrink, depth);
		}

		if (g.elim_err) { ++elim_errs; }
		choices_sum += g.choices;
		elim_choices_sum += g.elim_choices;
		max_abs_sum += g.max_abs;
		max_squ_sum += g.max_squ;

		//b.write_board_65();
		//g.playoutvalue(b, chr, chr, level);
		//g.playout(b, chr, chr, level);
		++tr;
		if (g.disks == level) {

			for (int ni(0);ni < 8;++ni) {
				for (int nj(0);nj < 8;++nj) {

				}
			}
			//vector<double> v(65, 0);
			vector<double> v(inputs, 0);
			lea.test_data_x[i].resize(inputs);
			for (int j(0);j < inputs;++j) {
				lea.test_data_x[i][j] = g.playoutb[j];
			}
			lea.test_data_y[i].resize(1, g.playoutb[inputs]);
			//lea2.test_data_y[i][0] = double(g.playoutb[66]);
			//lea2.teach_data_y[i][0] = winvalue(g.playoutb[66]);
			++i;
		}
	}
	err[level] = double(elim_errs) / double(lea.tests);
	choices_ave[level] = double(choices_sum) / double(lea.tests);
	elim_choices_ave[level] = double(elim_choices_sum) / double(lea.tests);
	max_ave[level] = max_abs_sum / double(lea.tests);
	max_squ_ave[level] = max_squ_sum / double(lea.tests);
	clock_t mid = clock();

	//std::string filename = folder + "testdata.csv";
	//std::ofstream writing_file;
	//writing_file.open(filename, std::ios::app);
	//for (int m(0);m < 50;++m) {
	//	for (int l(0); l < inputs;++l) {
	//		writing_file << lea2.test_data_x[m][l] << ",";
	//	}
	//	writing_file << lea2.test_data_y[m][0] << "," << endl;
	//}

	//lea2.load(para_file);

	lea.test_accuracy(neu[level]);
	lea.test_accuracy_sign(neu[level]);
	acc[level] = lea.ai_accuracy[0];
	sigacc[level] = lea.ai_accuracy_sign[0];
	//lea2.save(para_file);

	//cout << pl << "%" << endl;
	//}
}

void TDL::sum(const int level)
{
	if (!readjobdone) {
		readjob();
	}
	std::string filename = folder1 + "summary" + to_string(level) + ".csv";
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);

	writing_file << "level" << "," << "acc" << "," << "sigacc" << "," << "err" << "," << "choices" << "," << "elim_choices" << "," << "max_ave" << "," << "max_squ_ave" << "," << endl;
	test(63);
	for (int l(63);l >= level;--l) {
		test(l);
		writing_file << l << "," << acc[l] << "," << sigacc[l] << "," << max_ave[l] << "," << max_squ_ave[l] << "," << err[l] << "," << choices_ave[l] << "," << elim_choices_ave[l] << "," << endl;
	}
	//cout << "level: " << 61 << "," << endl;
	//cout << 64 << "," << tdl.accuracy("vn_74\\", 64) << "," << endl;
	//cout << level + 2 << "," << tdl.accuracy("vn_74\\", level + 2) << "," << endl;
	//cout << level + 1 << "," << tdl.accuracy("vn_74\\", level + 1) << "," << endl;
	//cout << level << "," << tdl.accuracy("vn_74\\", level) << "," << endl;
	//cout << level - 1 << "," << tdl.accuracy("vn_74\\", level - 1) << "," << endl;
	//cout << level - 2 << "," << tdl.accuracy("vn_74\\", level - 2) << "," << endl;
	//cout << 1 << endl;

}

void TDL::addsum(const int level)
{
	if (!readjobdone) {
		readjob();
	}

	test(level);
	std::string filename = folder1 + "summary.csv";
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::app);
	writing_file << level << "," << acc[level] << "," << sigacc[level] << "," << max_ave[level] << "," << max_squ_ave[level] << "," << err[level] << "," << choices_ave[level] << "," << elim_choices_ave[level] << "," << endl;

	//cout << "level: " << 61 << "," << endl;
	//cout << 64 << "," << tdl.accuracy("vn_74\\", 64) << "," << endl;
	//cout << level + 2 << "," << tdl.accuracy("vn_74\\", level + 2) << "," << endl;
	//cout << level + 1 << "," << tdl.accuracy("vn_74\\", level + 1) << "," << endl;
	//cout << level << "," << tdl.accuracy("vn_74\\", level) << "," << endl;
	//cout << level - 1 << "," << tdl.accuracy("vn_74\\", level - 1) << "," << endl;
	//cout << level - 2 << "," << tdl.accuracy("vn_74\\", level - 2) << "," << endl;
	//cout << 1 << endl;
	writing_file.close();
}

void TDL::renewcontrol_start()
{
	if (thispc == mypc) {
		std::ifstream ifs("control\\status4.txt");
		if (!ifs) {
			std::cout << "入力エラー";
			return;
		}
		string str;
		getline(ifs, str);
		ifs.close();
		if (str != "STOPPED") {
			running_condition = false;
			ifs.close();
		}
		std::ofstream ofs;
		ofs.open("control\\control4.txt", std::ios::out);
		ofs << "STOP" << endl;
		ofs.close();
	}
}

void TDL::renewcontrol_end()
{
	if (thispc == mypc) {
		std::ofstream ofs;
		ofs.open("control\\control4.txt", std::ios::out);
		ofs << "START" << endl;
		ofs.close();
	}
}

Test::Test()
{
}

Test::~Test()
{
}

void Test::run(string folder, const int level)
{
	Random_choose rc;
	Game g;
	Board b;
	b.initialize();
	Neuralnet neu[65];

	std::ifstream ifs(folder + "policy.csv");
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	double opt_a[65];
	double w[65];
	int depth[65];
	double shrink[65];

	std::string str;
	getline(ifs, str);
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		getline(stream, token, ',');
		int lev = stoi(token);
		getline(stream, token, ',');
		opt_a[lev] = stod(token);
		getline(stream, token, ',');
		w[lev] = stod(token);
		getline(stream, token, ',');
		depth[lev] = stoi(token);
		getline(stream, token, ',');
		shrink[lev] = stod(token);
	}
	ifs.close();


	for (int i(6);i < 65;++i) {
		neu[i].load(folder + "bw\\bw" + to_string(i) + ".csv");
	}
	g.ABoutbia(b, level, neu, neu, shrink, depth);
	//g.playoutvalue74(b, level);
	Learning lea2;
	int inputs(neu[63].nodes[0]);
	lea2.test_data_x.resize(10);
	lea2.test_data_x[0].resize(inputs);
	lea2.test_data_y.resize(5);
	lea2.test_data_y[0].resize(5);
	lea2.test_data_y[0][0] = g.playoutb[inputs];
	for (int j(0);j < inputs;++j) {
		lea2.test_data_x[0][j] = g.playoutb[j];
	}

	std::string filename = folder + "test.csv";
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);

	b.playerturnboard();
	for (int i(0);i < 8;++i) {
		writing_file << "row " + to_string(i) << ",";
		for (int j(0);j < 8;++j) {
			writing_file << b.square[i][j] << ",";
		}
		writing_file << endl;
	}
	writing_file << endl;

	writing_file << "spot disks1" << ",";
	for (int i(0);i < 10;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "spot disks2" << ",";
	for (int i(10);i < 20;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "spot moves1" << ",";
	for (int i(20);i < 29;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "spot moves2" << ",";
	for (int i(29);i < 38;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "spot fixed1" << ",";
	for (int i(38);i < 47;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "spot fixed2" << ",";
	for (int i(47);i < 56;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "spot openness1" << ",";
	for (int i(56);i < 65;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "spot openness2" << ",";
	for (int i(65);i < 74;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "moutains1" << ",";
	writing_file << lea2.test_data_x[0][74] << ",";
	writing_file << endl;
	writing_file << "moutains2" << ",";
	writing_file << lea2.test_data_x[0][75] << ",";
	writing_file << endl;
	writing_file << "wings1" << ",";
	writing_file << lea2.test_data_x[0][76] << ",";
	writing_file << endl;
	writing_file << "wings2" << ",";
	writing_file << lea2.test_data_x[0][77] << ",";
	writing_file << endl;
	writing_file << "sides1" << ",";
	for (int i(78);i < 82;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "sides2" << ",";
	for (int i(82);i < 86;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << "corner" << ",";
	for (int i(86);i < 90;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << "xcorner" << ",";
	for (int i(90);i < 94;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << "bycorner" << ",";
	for (int i(94);i < 102;++i) {
		writing_file << lea2.test_data_x[0][i] << ",";
	}
	writing_file << endl;
	writing_file << "dif" << "," << lea2.test_data_y[0][0] << "," << endl;



	//writing_file << "spot disks1" << ",";
	//for (int i(0);i < 10;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "spot disks2" << ",";
	//for (int i(10);i < 20;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "spot moves1" << ",";
	//for (int i(20);i < 29;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "spot moves2" << ",";
	//for (int i(29);i < 38;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "spot fixed1" << ",";
	//for (int i(38);i < 47;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "spot fixed2" << ",";
	//for (int i(47);i < 56;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "spot openness1" << ",";
	//for (int i(56);i < 65;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "spot openness2" << ",";
	//for (int i(65);i < 74;++i) {
	//	writing_file << g.playoutb[i] << ",";
	//}
	//writing_file << endl;
	//writing_file << "dif" << "," << g.playoutb[74] << "," << endl;
	writing_file.close();
}

void TDL::write_test()
{
	if (!readjobdone) {
		readjob();
	}
	if (!readdone) {
		read();
	}
	std::ofstream writing_file;
	writing_file.open(test_file, std::ios::out);
	for (int i(63); i > opt_level; --i) {
		double yout(0);
		double yout2(0);
		for (int j(0); j < 1000; ++j) {
			Random_choose rc;
			Board b;
			b.initialize();
			Game g;
			g.bias = bias;
			g.playto(b, rc, rc, i);
			yout += abs(neu[i].Forward(b.x206())[0]);
			yout2 += pow(neu[i].Forward(b.x206())[0], 2);

		}
		yout /= 1000;
		yout2 /= 1000;
		writing_file << i << "," << yout << "," << yout2 << endl;
	}
	writing_file.close();
}