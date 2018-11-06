
#include "stdafx.h"
#include "Learning.h"
#include "maths.h"

Neuralnet::Neuralnet()
	:layers(0),
	nodes(0, 0),
	bia(0, vector<double>(0, 0)),
	wei(0, vector<vector<double>>(0, vector<double>(0, 0)))
{

}

Neuralnet::~Neuralnet()
{
}

void Neuralnet::set_network(vector<int> &n)
{
	nodes = n;
	layers = nodes.size();
	bia.resize(layers - 1);
	wei.resize(layers - 1);
	for (int i = 0;i < layers - 1;++i) {
		bia[i].resize(nodes[i + 1]);
		wei[i].resize(nodes[i + 1]);
		for (int j = 0;j < nodes[i + 1];++j) {
			wei[i][j].resize(nodes[i]);
		}
	}
}

void Neuralnet::initialize_bw()
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < layers - 1;++i) {
		for (int j = 0;j < nodes[i + 1];++j) {
			bia[i][j] = 0;
			//bia[i][j] = distribution(mt);
			for (int k = 0;k < nodes[i];++k) {
				wei[i][j][k] = distribution(mt) / pow(double(nodes[i]), 0.5);
			}
		}
	}
}

//vector<double> Neuralnet::Forward(const vector<double> &x) const
//{
//	vector<vector<double>> sig;
//	const int lo_layers(layers);
//	auto itr_nodes = nodes.begin();
//	auto itr_wei = wei.begin();
//	auto itr_bia = bia.begin();
//	sig.resize(lo_layers);
//	for (int i = 0;i < lo_layers;++i) {
//		sig[i].resize(itr_nodes[i]);
//	}
//	for (int j = 0;j < itr_nodes[0];++j) {
//		sig[0][j] = x[j];
//	}
//	for (int i = 1;i < lo_layers - 1;++i) {
//		for (int n1 = 0;n1 < itr_nodes[i];++n1) {
//			sig[i][n1] = itr_bia[i - 1][n1];
//			for (int n0 = 0;n0 < itr_nodes[i - 1];++n0) {
//				sig[i][n1] = sig[i][n1] + itr_wei[i - 1][n1][n0] * sig[i - 1][n0];
//			}
//			sig[i][n1] = max(0.0, sig[i][n1]);
//		}
//	}
//	int i(lo_layers - 1);
//	for (int n1 = 0;n1 < itr_nodes[i];++n1) {
//		sig[i][n1] = itr_bia[i - 1][n1];
//		for (int n0 = 0;n0 < itr_nodes[i - 1];++n0) {
//			sig[i][n1] = sig[i][n1] + itr_wei[i - 1][n1][n0] * sig[i - 1][n0];
//		}
//	}
//	return sig[lo_layers - 1];
//}

vector<double> Neuralnet::Forward(const vector<double> &x) const
{
	vector<vector<double>> sig;
	const int lo_layers(layers);
	auto itr_nodes = nodes.begin();
	auto itr_wei = wei.begin();
	auto itr_bia = bia.begin();
	sig.resize(lo_layers);
	for (int i = 0;i < lo_layers;++i) {
		sig[i].resize(itr_nodes[i]);
	}
	for (int j = 0;j < itr_nodes[0];++j) {
		sig[0][j] = x[j];
	}
	for (int i(0); i < lo_layers - 2; ++i) {
		for (int n1(0); n1 < itr_nodes[i + 1];++n1) {
			sig[i + 1][n1] = itr_bia[i][n1];
		}
		for (int n0(0); n0 < itr_nodes[i]; ++n0) {
			double lo_sig = sig[i][n0];
			if (lo_sig != 0) {
				for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
					sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
				}
			}
		}
		for (int n1(0); n1 < itr_nodes[i + 1];++n1) {
			sig[i + 1][n1] = max(0.0, sig[i + 1][n1]);
		}
	}

	int i(lo_layers - 2);
	for (int n1(0); n1 < itr_nodes[i + 1];++n1) {
		sig[i + 1][n1] = itr_bia[i][n1];
	}
	for (int n0(0); n0 < itr_nodes[i]; ++n0) {
		double lo_sig = sig[i][n0];
		if (lo_sig != 0) {
			for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
				sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
			}
		}
	}
	return sig[lo_layers - 1];
}

//vector<double> Neuralnet::Forward(const vector<double> &x) const
//{
//	vector<vector<double>> sig;
//	const int lo_layers(layers);
//	vector<int> lo_nodes(nodes);
//	auto itr_nodes = nodes.begin();
//	vector<vector<vector<double>>> lo_wei(wei);
//	vector<vector<double>> lo_bia(bia);
//	sig.resize(lo_layers);
//	for (int i = 0;i < lo_layers;++i) {
//		sig[i].resize(lo_nodes[i]);
//	}
//	for (int j = 0;j < lo_nodes[0];++j) {
//		sig[0][j] = x[j];
//	}
//	for (int i = 1;i < lo_layers - 1;++i) {
//		for (int n1 = 0;n1 < lo_nodes[i];++n1) {
//			sig[i][n1] = lo_bia[i - 1][n1];
//			for (int n0 = 0;n0 < lo_nodes[i - 1];++n0) {
//				sig[i][n1] = sig[i][n1] + lo_wei[i - 1][n1][n0] * sig[i - 1][n0];
//			}
//			sig[i][n1] = max(0.0, sig[i][n1]);
//		}
//	}
//	int i(lo_layers - 1);
//	for (int n1 = 0;n1 < lo_nodes[i];++n1) {
//		sig[i][n1] = lo_bia[i - 1][n1];
//		for (int n0 = 0;n0 < lo_nodes[i - 1];++n0) {
//			sig[i][n1] = sig[i][n1] + lo_wei[i - 1][n1][n0] * sig[i - 1][n0];
//		}
//	}
//	return sig[lo_layers - 1];
//}

//void Neuralnet::Backward(vector<vector<double>> &SdEdb, vector<vector<vector<double>>> &SdEdw, const vector<double> &x, const vector<double> &y) const
//{
//	vector<vector<double>> sig;
//	vector<vector<double>> del;
//	vector<vector<double>> dEdb;
//	vector<vector<vector<double>>> dEdw;
//	sig.resize(layers);
//	del.resize(layers - 1);
//	dEdb.resize(layers - 1);
//	dEdw.resize(layers - 1);
//	for (int i = 0;i < layers - 1;++i) {
//		del[i].resize(nodes[i + 1]);
//		dEdw[i].resize(nodes[i + 1]);
//		dEdb[i].resize(nodes[i + 1]);
//		for (int j = 0;j < nodes[i + 1];++j) {
//			dEdw[i][j].resize(nodes[i]);
//		}
//	}
//	for (int i = 0;i < layers;++i) {
//		sig[i].resize(nodes[i]);
//	}
//	//入力
//	for (int j = 0;j < nodes[0];++j) {
//		sig[0][j] = x[j];
//	}
//
//	//出力
//	for (int i = 1;i < layers - 1;++i) {
//		for (int n1 = 0;n1 < nodes[i];++n1) {
//			sig[i][n1] = bia[i - 1][n1];
//			for (int n0 = 0;n0 < nodes[i - 1];++n0) {
//				sig[i][n1] = sig[i][n1] + wei[i - 1][n1][n0] * sig[i - 1][n0];
//			}
//			sig[i][n1] = max(0.0, sig[i][n1]);
//		}
//	}
//	for (int n1 = 0;n1 < nodes[layers - 1];++n1) {
//		sig[layers - 1][n1] = bia[layers - 2][n1];
//		for (int n0 = 0;n0 < nodes[layers - 2];++n0) {
//			sig[layers - 1][n1] = sig[layers - 1][n1] + wei[layers - 2][n1][n0] * sig[layers - 2][n0];
//		}
//	}
//
//
//	//誤差逆伝播ppp
//	for (int n1 = 0;n1 < nodes[layers - 1];++n1) {
//		del[layers - 2][n1] = sig[layers - 1][n1] - y[n1];
//		dEdb[layers - 2][n1] = del[layers - 2][n1];
//		SdEdb[layers - 2][n1] += dEdb[layers - 2][n1];
//		for (int n0 = 0;n0 < nodes[layers - 2];++n0) {
//			dEdw[layers - 2][n1][n0] = del[layers - 2][n1] * sig[layers - 2][n0];
//			SdEdw[layers - 2][n1][n0] += dEdw[layers - 2][n1][n0];
//		}
//	}
//	for (int i = layers - 3;i >= 0;--i) {
//		for (int n1 = 0;n1 < nodes[i + 1];++n1) {
//			del[i][n1] = 0;
//			if (sig[i + 1][n1] != 0) {
//				for (int n2 = 0;n2 < nodes[i + 2];++n2) {
//					del[i][n1] = del[i][n1] + del[i + 1][n2] * wei[i + 1][n2][n1];
//				}
//				dEdb[i][n1] = del[i][n1];
//				SdEdb[i][n1] += dEdb[i][n1];
//				for (int n0 = 0;n0 < nodes[i];++n0) {
//					dEdw[i][n1][n0] = del[i][n1] * sig[i][n0];
//					SdEdw[i][n1][n0] += dEdw[i][n1][n0];
//				}
//			}
//		}
//	}
//}

void Neuralnet::Backward(vector<vector<double>> &SdEdb, vector<vector<vector<double>>> &SdEdw, const vector<double> &x, const vector<double> &y) const
{
	vector<vector<double>> del;
	vector<vector<double>> dEdb;
	vector<vector<vector<double>>> dEdw;
	del.resize(layers - 1);
	dEdb.resize(layers - 1);
	dEdw.resize(layers - 1);
	for (int i = 0;i < layers - 1;++i) {
		del[i].resize(nodes[i + 1]);
		dEdw[i].resize(nodes[i + 1]);
		dEdb[i].resize(nodes[i + 1]);
		for (int j = 0;j < nodes[i + 1];++j) {
			dEdw[i][j].resize(nodes[i]);
		}
	}
	
	//入力
	vector<vector<double>> sig;
	const int lo_layers(layers);
	auto itr_nodes = nodes.begin();
	auto itr_wei = wei.begin();
	auto itr_bia = bia.begin();
	sig.resize(lo_layers);
	for (int i = 0;i < lo_layers;++i) {
		sig[i].resize(itr_nodes[i]);
	}
	for (int j = 0;j < itr_nodes[0];++j) {
		sig[0][j] = x[j];
	}
	for (int i(0); i < lo_layers - 2; ++i) {
		for (int n1(0); n1 < itr_nodes[i + 1];++n1) {
			sig[i + 1][n1] = itr_bia[i][n1];
		}
		for (int n0(0); n0 < itr_nodes[i]; ++n0) {
			double lo_sig = sig[i][n0];
			if (abs(lo_sig) > 1e-10) {
				for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
					sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
				}
			}
		}
		for (int n1(0); n1 < itr_nodes[i + 1];++n1) {
			sig[i + 1][n1] = max(0.0, sig[i + 1][n1]);
		}
	}

	int i(lo_layers - 2);
	for (int n1(0); n1 < itr_nodes[i + 1];++n1) {
		sig[i + 1][n1] = itr_bia[i][n1];
	}
	for (int n0(0); n0 < itr_nodes[i]; ++n0) {
		double lo_sig = sig[i][n0];
		if (abs(lo_sig) > 1e-10) {
			for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
				sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
			}
		}
	}


	//誤差逆伝播ppp
	for (int n1 = 0;n1 < nodes[layers - 1];++n1) {
		del[layers - 2][n1] = sig[layers - 1][n1] - y[n1];
		dEdb[layers - 2][n1] = del[layers - 2][n1];
		SdEdb[layers - 2][n1] += dEdb[layers - 2][n1];
		for (int n0 = 0;n0 < nodes[layers - 2];++n0) {
			dEdw[layers - 2][n1][n0] = del[layers - 2][n1] * sig[layers - 2][n0];
			SdEdw[layers - 2][n1][n0] += dEdw[layers - 2][n1][n0];
		}
	}
	for (int i = layers - 3;i >= 0;--i) {
		for (int n1 = 0;n1 < nodes[i + 1];++n1) {
			del[i][n1] = 0;
			if (abs(sig[i + 1][n1]) > 1e-10) {
				for (int n2 = 0;n2 < nodes[i + 2];++n2) {
					del[i][n1] = del[i][n1] + del[i + 1][n2] * wei[i + 1][n2][n1];
				}
				dEdb[i][n1] = del[i][n1];
				SdEdb[i][n1] += dEdb[i][n1];
				for (int n0 = 0;n0 < nodes[i];++n0) {
					dEdw[i][n1][n0] = del[i][n1] * sig[i][n0];
					SdEdw[i][n1][n0] += dEdw[i][n1][n0];
				}
			}
		}
	}
}

void Neuralnet::save(string filename) const
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	for (int i = 0;i < layers;++i) {
		writing_file << nodes[i] << ",";
	}
	writing_file << std::endl;


	for (int i(0);i < layers - 1;++i) {
		for (int n1(0);n1 < nodes[i + 1];++n1) {
			writing_file << bia[i][n1] << ",";
		}
		writing_file << std::endl;
	}
	for (int i(0);i < layers - 1;++i) {
		for (int n1(0);n1 < nodes[i + 1];++n1) {
			for (int n0 = 0;n0 < nodes[i];++n0) {
				writing_file << wei[i][n1][n0] << ",";
			}
			writing_file << std::endl;
		}
	}
	writing_file.close();
}

void Neuralnet::load(string filename)
{
	std::ifstream ifs(filename);
	if (!ifs) {
		return;
	}
	std::vector<int> hp(0, 0);
	std::string str;
	getline(ifs, str);
	std::string token;
	std::istringstream stream(str);
	while (getline(stream, token, ',')) {
		hp.emplace_back(std::stoi(token));
	}

	set_network(hp);

	int i(0);
	int n1(0);
	while (i < layers - 1) {
		getline(ifs, str);
		n1 = 0;
		std::istringstream stream(str);
		while (getline(stream, token, ',')) {
			bia[i][n1] = std::stod(token);
			++n1;
		}
		++i;
	}

	i = 0;
	int n0(0);
	while (i < layers - 1) {
		n1 = 0;
		while (n1 < nodes[i + 1]) {
			getline(ifs, str);
			n0 = 0;
			std::istringstream stream(str);
			while (getline(stream, token, ',')) {
				wei[i][n1][n0] = std::stod(token);
				++n0;
			}
			++n1;
		}
		++i;
	}

	ifs.close();
}

Optimizer::Optimizer()
	:a(0.001)
{
}

Optimizer::Optimizer(double a1)
	: a(a1)
{
}

Optimizer::~Optimizer()
{
}

void Optimizer::operator()(Neuralnet &net, Learning &le)
{
	for (int i = 0;i < (net.layers - 1);++i) {
		for (int n1 = 0;n1 < net.nodes[i + 1];++n1) {
			net.bia[i][n1] -= a*le.SdEdb[i][n1] / le.batch;
			for (int n0 = 0;n0 < net.nodes[i];++n0) {
				net.wei[i][n1][n0] -= a*le.SdEdw[i][n1][n0] / le.batch;
			}
		}
	}
	le.clearS();
}

Adam::Adam()
	://a(0.001), //default 0.001
	b1(0.9),   //default 0.9
	b2(0.999), //default 0.999
	e(1E-8)	   //default 1E-8
{
}

Adam::~Adam()
{
}

void Adam::operator()(Neuralnet &net, Learning_Adam &le)
{
	double mp;
	double vp;
	for (int i = 0;i < (net.layers - 1);++i) {
		for (int n1 = 0;n1 < net.nodes[i + 1];++n1) {
			for (int n0 = 1;n0 < net.nodes[i];++n0) {
				le.mAdamw[i][n1][n0] = le.mAdamw[i][n1][n0] * b1 + (1 - b1) * le.SdEdw[i][n1][n0] / le.batch;
				le.vAdamw[i][n1][n0] = le.vAdamw[i][n1][n0] * b2 + (1 - b2) * pow(le.SdEdw[i][n1][n0] / le.batch, 2);
				mp = le.mAdamw[i][n1][n0] / (1 - pow(b1, le.tA));
				vp = le.vAdamw[i][n1][n0] / (1 - pow(b2, le.tA));
				net.wei[i][n1][n0] -= a*mp / (sqrt(vp) + e);
			}
			le.mAdamb[i][n1] = le.mAdamb[i][n1] * b1 + (1 - b1) * le.SdEdb[i][n1] / le.batch;
			le.vAdamb[i][n1] = le.vAdamb[i][n1] * b2 + (1 - b2) * pow(le.SdEdb[i][n1] / le.batch, 2);
			mp = le.mAdamb[i][n1] / (1 - pow(b1, le.tA));
			vp = le.vAdamb[i][n1] / (1 - pow(b2, le.tA));
			net.bia[i][n1] -= a*mp / (sqrt(vp) + e);
		}
	}
	le.clearS();
	++le.tA;
}

SDG::SDG()
{
}

SDG::SDG(double a1) : Optimizer(a1)
{
}

SDG::~SDG()
{
}

void SDG::operator()(Neuralnet &net, Learning &le)
{
	for (int i = 0;i < (net.layers - 1);++i) {
		for (int n1 = 0;n1 < net.nodes[i + 1];++n1) {
			net.bia[i][n1] -= a*le.SdEdb[i][n1] / le.batch;
			for (int n0 = 1;n0 < net.nodes[i];++n0) {
				net.wei[i][n1][n0] -= a*le.SdEdw[i][n1][n0] / le.batch;
			}
		}
	}
	le.clearS();
}

Learning::Learning()
	:times(100),
	batch(10),
	tests(0),
	t(0)
{
}

Learning::~Learning()
{
}

void Learning::read_teach_data(const string file)
{
	//ファイルの読み込み
	std::ifstream ifs(file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	teach_data_all.clear();
	teach_data_all.reserve(10000);
	//csvファイルを1行ずつ読み込む
	std::string str;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		std::vector<double> tempv(0, 0);
		tempv.reserve(20);
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			tempv.emplace_back(std::stod(token));
		}
		teach_data_all.emplace_back(tempv);
	}
	teach_data_all.shrink_to_fit();
	ifs.close();
}

void Learning::read_test_data(const string file)
{
	//ファイルの読み込み
	std::ifstream ifs(file);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	test_data_all.clear();
	test_data_all.reserve(10000);
	//csvファイルを1行ずつ読み込む
	std::string str;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		std::vector<double> tempv(0, 0);
		tempv.reserve(20);
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			tempv.emplace_back(std::stod(token));
		}
		test_data_all.emplace_back(tempv);
	}
	test_data_all.shrink_to_fit();
	ifs.close();
}

void Learning::separate_data()
{
	teach_data_x.reserve(10000);
	teach_data_y.reserve(10000);
	int end(teach_data_all.size() - 1);
	for (int j = 0;j <= end;++j) {
		std::vector<double> tempv(0, 0);
		for (int i = x_begin;i <= x_end;++i) {
			tempv.emplace_back(teach_data_all[j][i]);
		}
		teach_data_x.emplace_back(tempv);
		tempv.clear();
		for (int i = y_begin;i <= y_end;++i) {
			tempv.emplace_back(teach_data_all[j][i]);
		}
		teach_data_y.emplace_back(tempv);
	}
	teach_data_x.shrink_to_fit();
	teach_data_y.shrink_to_fit();

	test_data_x.reserve(10000);
	test_data_y.reserve(10000);
	end = test_data_all.size() - 1;
	for (int j = 0;j <= end;++j) {
		std::vector<double> tempv(0, 0);
		for (int i = x_begin;i <= x_end;++i) {
			tempv.emplace_back(test_data_all[j][i]);
		}
		test_data_x.emplace_back(tempv);
		tempv.clear();
		for (int i = y_begin;i <= y_end;++i) {
			tempv.emplace_back(test_data_all[j][i]);
		}
		test_data_y.emplace_back(tempv);
	}
	test_data_x.shrink_to_fit();
	test_data_y.shrink_to_fit();
}

void Learning::set_parameters(const vector<double> &x, const vector<int> &n)
{
	times = int(x[0]);
	batch = int(x[1]);
	x_begin = int(x[2]);
	x_end = int(x[3]);
	y_begin = int(x[4]);
	y_end = int(x[5]);
	tests = int(x[6]);
	t = int(x[7]);
	a = x[8];
	net = n;
	ai_accuracy.reserve(times*batch + 1);
	ai_accuracy_sign.reserve(times*batch + 1);
	SdEdb.resize(net.size() - 1);
	SdEdw.resize(net.size() - 1);
	for (unsigned int i = 0;i < net.size() - 1;++i) {
		SdEdb[i].resize(net[i + 1]);
		SdEdw[i].resize(net[i + 1]);
		for (int j = 0;j < net[i + 1];++j) {
			SdEdw[i][j].assign(net[i], 0);
		}
	}
}

void Learning::set_paras(const vector<double> &x, const vector<int> &n)
{
	times = int(x[0]);
	batch = int(x[1]);
	tests = int(x[2]);
	t = int(x[3]);
	net = n;
	x_begin = n[0];
	x_end = n[n.size() - 1];
	y_begin = 0;
	y_end = 0;
	ai_accuracy.resize(0);
	ai_accuracy_sign.resize(0);
	teach_data_x.resize(0);
	teach_data_y.resize(0);
	test_data_x.resize(0);
	test_data_y.resize(0);
	ai_accuracy.reserve(tests + 50);
	ai_accuracy_sign.reserve(tests + 50);
	SdEdb.resize(net.size() - 1);
	SdEdw.resize(net.size() - 1);
	for (unsigned int i = 0;i < net.size() - 1;++i) {
		SdEdb[i].resize(net[i + 1], 0);
		SdEdw[i].resize(net[i + 1]);
		for (int j = 0;j < net[i + 1];++j) {
			SdEdw[i][j].assign(net[i], 0);
		}
	}
	teach_data_x.resize(times*batch);
	teach_data_y.resize(times*batch);
	test_data_x.resize(tests);
	test_data_y.resize(tests);
}

void Learning_Adam::set_parameters(const vector<double> &x, const vector<int> &n)
{
	times = int(x[0]);
	batch = int(x[1]);
	x_begin = int(x[2]);
	x_end = int(x[3]);
	y_begin = int(x[4]);
	y_end = int(x[5]);
	tests = int(x[6]);
	t = int(x[7]);
	a = x[8];
	tA = int(x[9]);
	net = n;
	ai_accuracy.reserve(times*batch + 1);
	SdEdb.resize(net.size() - 1);
	SdEdw.resize(net.size() - 1);
	mAdamb.resize(net.size() - 1);
	vAdamb.resize(net.size() - 1);
	mAdamw.resize(net.size() - 1);
	vAdamw.resize(net.size() - 1);
	for (unsigned int i = 0;i < net.size() - 1;++i) {
		SdEdb[i].resize(net[i + 1]);
		SdEdw[i].resize(net[i + 1]);
		mAdamb[i].resize(net[i + 1]);
		vAdamb[i].resize(net[i + 1]);
		mAdamw[i].resize(net[i + 1]);
		vAdamw[i].resize(net[i + 1]);
		for (int j = 0;j < net[i + 1];++j) {
			SdEdw[i][j].assign(net[i], 0);
			mAdamw[i][j].assign(net[i], 0);
			vAdamw[i][j].assign(net[i], 0);
		}
	}
}

void Learning::test_accuracy(Neuralnet &ne)
{
	double E(0);
	vector<double> yout;
	for (int i = 0;i < tests;++i) {
		yout = ne.Forward(test_data_x[i]);
		for (int j = 0; j <= (y_end - y_begin);++j) {
			E += pow((test_data_y[i][j] - yout[j]), 2) / 2;
		}
	}
	ai_accuracy.emplace_back(E / tests);
	++t;
}

void Learning::test_accuracy_sign(Neuralnet &ne)
{
	double E(0);
	double yout;
	double yth;
	//cout << test_data_x.size() << "," << test_data_x[0].size() << "," << test_data_x[0][0] << endl;
	//cout << test_data_y.size() << "," << test_data_y[0].size() << "," << test_data_y[0][0] << endl;
	//cout << test_data_x.size() << "," << ne.Forward(test_data_x[0]).size() << "," << ne.Forward(test_data_x[0])[0] << endl;
	for (int i = 0;i < tests;++i) {
		yout = ne.Forward(test_data_x[i])[0];
		yth = test_data_y[i][0];
		if (yout < 0) {
			yout = -1;
		}
		else if (yout == 0) {
			yout = 0;
		}
		else {
			yout = 1;
		}

		if (yth < 0) {
			yth = -1;
		}
		else if (yth == 0) {
			yth = 0;
		}
		else {
			yth = 1;
		}
		E += pow((yth - yout), 2);
	}
	ai_accuracy_sign.emplace_back(E / tests);
}

void Learning::write_accuracy(string file)
{
	std::ofstream writing_file;
	writing_file.open(file, std::ios::app);
	int ti(ai_accuracy.size());
	for (int i = 0;i < ti;++i) {
		writing_file << ai_accuracy[i] << ",";
	}
	writing_file.close();
}

void Learning::write_accuracy_sign(string file)
{
	std::ofstream writing_file;
	writing_file.open(file, std::ios::app);
	int ti(ai_accuracy_sign.size());
	for (int i = 0;i < ti;++i) {
		writing_file << ai_accuracy_sign[i] << ",";
	}
	writing_file.close();
}

double Learning::get_accuracy_e() const
{
	return pow((ai_accuracy[t - 1] - ai_accuracy[t - 2]), 2) / 2;
}

void Learning::clearS()
{
	for (unsigned int i = 0;i < net.size() - 1;++i) {
		for (int n1 = 0; n1 < net[i + 1];++n1) {
			SdEdb[i][n1] = 0;
			for (int n0 = 0; n0 < net[i];++n0) {
				SdEdw[i][n1][n0] = 0;
			}
		}
	}
}

void Learning::save(string filename) const
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);

	writing_file << times << "," << batch << "," << x_begin << ","
		<< x_end << "," << y_begin << "," << y_end << "," << tests
		<< "," << t << "," << a << "," << std::endl;
	for (unsigned int i = 0;i < net.size();++i) {
		writing_file << net[i] << ",";
	}
	writing_file << std::endl;
	writing_file << t << "," << std::endl;
	writing_file.close();
}

void Learning_Adam::save(string filename) const
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);

	writing_file << times << "," << batch << "," << x_begin << ","
		<< x_end << "," << y_begin << "," << y_end << "," << tests << ","
		<< t << "," << a << "," << tA << "," << std::endl;
	for (unsigned int i = 0;i < net.size();++i) {
		writing_file << net[i] << ",";
	}
	writing_file << std::endl;
	for (unsigned int i(0);i < net.size() - 1;++i) {
		for (int n1(0);n1 < net[i + 1];++n1) {
			writing_file << mAdamb[i][n1] << ",";
		}
		writing_file << std::endl;
	}
	for (unsigned int i(0);i < net.size() - 1;++i) {
		for (int n1(0);n1 < net[i + 1];++n1) {
			writing_file << vAdamb[i][n1] << ",";
		}
		writing_file << std::endl;
	}
	for (unsigned int i(0);i < net.size() - 1;++i) {
		for (int n1(0);n1 < net[i + 1];++n1) {
			for (int n0 = 0;n0 < net[i];++n0) {
				writing_file << mAdamw[i][n1][n0] << ",";
			}
			writing_file << std::endl;
		}
	}
	for (unsigned int i(0);i < net.size() - 1;++i) {
		for (int n1(0);n1 < net[i + 1];++n1) {
			for (int n0 = 0;n0 < net[i];++n0) {
				writing_file << vAdamw[i][n1][n0] << ",";
			}
			writing_file << std::endl;
		}
	}
	writing_file << t << "," << std::endl;
	writing_file << tA << "," << std::endl;
	writing_file.close();
}

void Learning::load(string filename)
{
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	std::vector<double> hp(0, 0);
	std::string str;
	getline(ifs, str);
	std::string token;
	std::istringstream stream(str);
	while (getline(stream, token, ',')) {
		hp.emplace_back(std::stod(token));
	}
	getline(ifs, str);
	while (getline(stream, token, ',')) {
		net.emplace_back(std::stoi(token));
	}
	getline(ifs, str);

	set_parameters(hp, net);

	std::istringstream stream2(str);
	while (getline(stream2, token, ',')) {
		t = std::stoi(token);
	}

	ifs.close();
}

void Learning_Adam::load(string filename)
{
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cout << "入力エラー";
		return;
	}
	std::vector<double> hp(0, 0);
	std::string str;
	getline(ifs, str);
	std::string token;
	std::istringstream stream(str);
	while (getline(stream, token, ',')) {
		hp.emplace_back(std::stoi(token));
	}
	getline(ifs, str);
	while (getline(stream, token, ',')) {
		net.emplace_back(std::stoi(token));
	}

	set_parameters(hp, net);

	unsigned int i(0);
	int n1(0);
	int n0(0);
	while (i < net.size() - 1) {
		getline(ifs, str);
		std::istringstream stream(str);
		n1 = 0;
		while (getline(stream, token, ',')) {
			mAdamb[i][n1] = std::stod(token);
			++n1;
		}
		++i;
	}

	i = 0;
	while (i < net.size() - 1) {
		getline(ifs, str);
		std::istringstream stream(str);
		n1 = 0;
		while (getline(stream, token, ',')) {
			vAdamb[i][n1] = std::stod(token);
			++n1;
		}
		++i;
	}

	i = 0;
	while (i < net.size() - 1) {
		n1 = 0;
		while (n1 < net[i + 1]) {
			getline(ifs, str);
			n0 = 0;
			std::istringstream stream(str);
			while (getline(stream, token, ',')) {
				mAdamw[i][n1][n0] = std::stod(token);
				++n0;
			}
			++n1;
		}
		++i;
	}

	i = 0;
	while (i < net.size() - 1) {
		n1 = 0;
		while (n1 < net[i + 1]) {
			getline(ifs, str);
			n0 = 0;
			std::istringstream stream(str);
			while (getline(stream, token, ',')) {
				vAdamw[i][n1][n0] = std::stod(token);
				++n0;
			}
			++n1;
		}
		++i;
	}
	getline(ifs, str);
	std::istringstream stream2(str);
	while (getline(stream2, token, ',')) {
		t = std::stoi(token);
	}
	while (getline(stream2, token, ',')) {
		tA = std::stoi(token);
	}

	ifs.close();
}

Learning_Adam::Learning_Adam()
	:tA(0)
{
}

Learning_Adam::~Learning_Adam()
{
}
