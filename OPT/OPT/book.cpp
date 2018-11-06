
#include "stdafx.h" 
#include "book.h"


Book::Book()
	:disks0(5),
	sized(12)
{
}

Book::~Book()
{
}

void Book::initialize(const int disks, const int depth)
{
	sized = depth;
	disks0 = disks;
	int(*p_Size)(Size);
	for (int i(0); i < 40;++i) {
		p_Size[i] = 0;
	}
	ReferTable.resize(depth);
	TransTableUpper.resize(depth);
	TransTableLower.resize(depth);
	for (int i(0); i < depth; ++i) {
		//ReferTable[i].reserve(int(500000 / pow(2, depth - i - 1)));
		//TransTableUpper[i].reserve(int(500000 / pow(2, depth - i - 1)));
		//TransTableLower[i].reserve(int(500000 / pow(2, depth - i - 1)));
		ReferTable[i].reserve(10000);
		TransTableUpper[i].reserve(10000);
		TransTableLower[i].reserve(10000);

	}
	//int(*lo_ReferTable)[1000][9](ReferTable);
	//for (int i(0); i <sized;++i) {
	//	for (int j(0); j < 1000;++j) {
	//		for (int k(0); k < 9;++k) {
	//			lo_ReferTable[i][j][k] = 0;
	//		}
	//	}
	//}
}

//void Book::v8(const int turn, const  int(&sq)[8][8], int(&mat)[8])
//{
//	int s[8][8];
//	if (turn == 2) {
//		for (int i(0); i < 8;++i) {
//			for (int j(0); j < 8;++j) {
//				switch (sq[i][j]) {
//				case 1:
//					s[i][j] = 2;
//					break;
//				case 2:
//					s[i][j] = 1;
//					break;
//				case 0:
//					s[i][j] = 0;
//				}
//			}
//		}
//	}
//	else {
//		for (int i(0); i < 8;++i) {
//			for (int j(0); j < 8;++j) {
//				s[i][j] = sq[i][j];
//			}
//		}
//	}
//	int m[8][8];
//	for (int i(0);i < 8;++i) {
//		for (int j(0); j < 8;++j) {
//			m[i][j] = 0;
//		}
//	}
//	for (int i(0);i < 8;++i) {
//		int power(1);
//		for (int j(0); j < 8;++j) {
//			m[0][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int i(0);i < 8;++i) {
//		int power(1);
//		for (int j(7); j >= 0;--j) {
//			m[1][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int i(7);i >= 0;--i) {
//		int power(1);
//		for (int j(0); j < 8;++j) {
//			m[2][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int i(7);i >= 0;--i) {
//		int power(1);
//		for (int j(7); j >= 0;--j) {
//			m[3][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(0);j < 8;++j) {
//		int power(1);
//		for (int i(0); i < 8;++i) {
//			m[4][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(0);j < 8;++j) {
//		int power(1);
//		for (int i(7); i >= 0;--i) {
//			m[5][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(7);j >= 0;--j) {
//		int power(1);
//		for (int i(0); i < 8;++i) {
//			m[6][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(7);j >= 0;--j) {
//		int power(1);
//		for (int i(7); i >= 0;--i) {
//			m[7][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//
//	bool con[8] = { true, true, true, true, true, true, true, true };
//	for (int k(0); k < 8;++k) {
//		int ma(0);
//		for (int l(0);l < 8;++l) {
//			if (con[l]) {
//				ma = max(ma, m[l][k]);
//			}
//		}
//		for (int l(0);l < 8;++l) {
//			if (ma != m[l][k]) {
//				con[l] = false;
//			}
//		}
//	}
//	for (int l(0);l < 8;++l) {
//		if (con[l]) {
//			for (int k(0);k < 8;++k) {
//				mat[k] = m[l][k];
//			}
//		}
//	}
//}
//
//void Book::v82(const int turn, const  int(&sq)[8][8], int(&mat)[8])
//{
//	int m[8][8];
//	int si[8] = { 0,0,0,0,7,7,7,7 };
//	int sj[8] = { 0,0,7,7,7,7,0,0 };
//	int di[8] = { 0,1,1,0,0,-1,-1,0 };
//	int dii[8] = { 1,0,0,1,-1,0,0,-1 };
//	int dj[8] = { 1,0,0,-1,-1,0,0,1 };
//	int djj[8] = { 0,1,-1,0,0,-1,1,0 };
//	bool con[8] = { true, true, true, true, true, true, true, true };
//
//	int s[8][8];
//	if (turn == 2) {
//		for (int i(0); i < 8;++i) {
//			for (int j(0); j < 8;++j) {
//				switch (sq[i][j]) {
//				case 1:
//					s[i][j] = 2;
//					break;
//				case 2:
//					s[i][j] = 1;
//					break;
//				default:
//					s[i][j] = 0;
//					break;
//				}
//			}
//		}
//	}
//	else {
//		for (int i(0); i < 8;++i) {
//			for (int j(0); j < 8;++j) {
//				s[i][j] = sq[i][j];
//			}
//		}
//	}
//
//	for (int i(0);i < 8;++i) {
//		int power(1);
//		for (int l(0);l < 8;++l) {
//			m[l][i] = 0;
//		}
//		for (int j(0);j < 8;++j) {
//			for (int l(0);l < 8;++l) {
//				if (con[l]) {
//					m[l][i] += power * s[si[l] + i*dii[l] + j*di[l]][sj[l] + i*djj[l] + j*dj[l]];
//				}
//			}
//			power *= 4;
//		}
//		int ma(0);
//		for (int l(0);l < 8;++l) {
//			ma = max(ma, m[l][i]);
//		}
//		for (int l(0);l < 8;++l) {
//			if (ma != m[l][i]) {
//				con[l] = false;
//			}
//		}
//	}
//	for (int l(0);l < 8;++l) {
//		if (con[l]) {
//			for (int k(0);k < 8;++k) {
//				mat[k] = m[l][k];
//			}
//		}
//	}
//}
//
//int Book::number(const int disks, const int turn, const int(&s)[8][8])
//{
//	int m[8];
//	v82(turn, s, m);
//	int n(0);
//	int depth = disks - disks0;
//	auto lo_ReferTable = ReferTable[depth].begin();
//	int sizen(Size[depth]);
//	while (n < sizen) {
//		bool match(true);
//		int i(0);
//		while (match && i < 8) {
//			if (lo_ReferTable[n][i] != m[i]) {
//				match = false;
//			}
//			++i;
//		}
//		if (match) {
//			return n;
//		}
//		++n;
//	}
//	return -1;
//}
//
//void Book::newdata(const int disks, const  int turn, const int(&s)[8][8], const double upper, const double lower)
//{
//	int m8[8];
//	v82(turn, s, m8);
//	int m9[9];
//	for (int i(0);i < 8;++i) {
//		m9[i] = m8[i];
//	}
//	int depth = disks - disks0;
//	int n(Size[depth]);
//	m9[8] = n;
//	ReferTable[depth][n].resize(9);
//	ReferTable[depth][n].emplace_back(m9);
//	TransTableUpper[depth].emplace_back(upper);
//	TransTableLower[depth].emplace_back(lower);
//	//for (int i(0); i < 8;++i) {
//	//	ReferTable[depth][n][i] = m[i];
//	//}
//	//ReferTable[depth][n][9] = n;
//	//TransTableUpper[depth][n] = upper;
//	//TransTableLower[depth][n] = lower;
//	++Size[depth];
//
//	display(disks, turn, s, n);
//}

//void Book::v8(const int turn, const  int(&sq)[8][8], unsigned long long(&mat)[2])
//{
//	int s[8][8];
//	if (turn == 2) {
//		for (int i(0); i < 8;++i) {
//			for (int j(0); j < 8;++j) {
//				switch (sq[i][j]) {
//				case 1:
//					s[i][j] = 2;
//					break;
//				case 2:
//					s[i][j] = 1;
//					break;
//				case 0:
//					s[i][j] = 0;
//				}
//			}
//		}
//	}
//	else {
//		for (int i(0); i < 8;++i) {
//			for (int j(0); j < 8;++j) {
//				s[i][j] = sq[i][j];
//			}
//		}
//	}
//	int m[8][8];
//	for (int i(0);i < 8;++i) {
//		for (int j(0); j < 8;++j) {
//			m[i][j] = 0;
//		}
//	}
//	for (int i(0);i < 8;++i) {
//		int power(1);
//		for (int j(0); j < 8;++j) {
//			m[0][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int i(0);i < 8;++i) {
//		int power(1);
//		for (int j(7); j >= 0;--j) {
//			m[1][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int i(7);i >= 0;--i) {
//		int power(1);
//		for (int j(0); j < 8;++j) {
//			m[2][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int i(7);i >= 0;--i) {
//		int power(1);
//		for (int j(7); j >= 0;--j) {
//			m[3][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(0);j < 8;++j) {
//		int power(1);
//		for (int i(0); i < 8;++i) {
//			m[4][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(0);j < 8;++j) {
//		int power(1);
//		for (int i(7); i >= 0;--i) {
//			m[5][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(7);j >= 0;--j) {
//		int power(1);
//		for (int i(0); i < 8;++i) {
//			m[6][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//	for (int j(7);j >= 0;--j) {
//		int power(1);
//		for (int i(7); i >= 0;--i) {
//			m[7][i] += power * s[i][j];
//			power *= 4;
//		}
//	}
//
//	bool con[8] = { true, true, true, true, true, true, true, true };
//	for (int k(0); k < 8;++k) {
//		int ma(0);
//		for (int l(0);l < 8;++l) {
//			if (con[l]) {
//				ma = max(ma, m[l][k]);
//			}
//		}
//		for (int l(0);l < 8;++l) {
//			if (ma != m[l][k]) {
//				con[l] = false;
//			}
//		}
//	}
//	for (int l(0);l < 8;++l) {
//		if (con[l]) {
//			for (int k(0);k < 8;++k) {
//				mat[k] = m[l][k];
//			}
//		}
//	}
//}

void Book::v2(const int turn, const  int(&sq)[8][8], unsigned long long(&mat)[2])
{
	unsigned long long m[8][2];
	int si[8] = { 0,0,0,0,7,7,7,7 };
	int sj[8] = { 0,0,7,7,7,7,0,0 };
	int di[8] = { 0,1,1,0,0,-1,-1,0 };
	int dii[8] = { 1,0,0,1,-1,0,0,-1 };
	int dj[8] = { 1,0,0,-1,-1,0,0,1 };
	int djj[8] = { 0,1,-1,0,0,-1,1,0 };
	bool con[8] = { true, true, true, true, true, true, true, true };

	int s[8][8];
	if (turn == 2) {
		for (int i(0); i < 8;++i) {
			for (int j(0); j < 8;++j) {
				switch (sq[i][j]) {
				case 1:
					s[i][j] = 2;
					break;
				case 2:
					s[i][j] = 1;
					break;
				default:
					s[i][j] = 0;
					break;
				}
			}
		}
	}
	else {
		for (int i(0); i < 8;++i) {
			for (int j(0); j < 8;++j) {
				s[i][j] = sq[i][j];
			}
		}
	}

	unsigned long long power(1);
	int bo(0);
	for (int l(0);l < 8;++l) {
		m[l][0] = 0;
		m[l][1] = 0;
	}
	for (int i(0);i < 8;++i) {
		if (i == 4) {
			power = 1;
			bo = 1;
		}
		for (int j(0);j < 8;++j) {
			for (int l(0);l < 8;++l) {
				if (con[l]) {
					m[l][bo] += power * s[si[l] + i*dii[l] + j*di[l]][sj[l] + i*djj[l] + j*dj[l]];
				}
			}
			power *= 4;
		}
		unsigned long long ma(0);
		for (int l(0);l < 8;++l) {
			if (con[l]) {
				ma = max(ma, m[l][bo]);
			}
		}
		for (int l(0);l < 8;++l) {
			if (ma != m[l][bo]) {
				con[l] = false;
			}
		}
	}
	for (int l(0);l < 8;++l) {
		if (con[l]) {
			for (int k(0);k < 2;++k) {
				mat[k] = m[l][k];
			}
			return;
		}
	}
}

int Book::number(const int disks, const int turn, const int(&s)[8][8])
{
	unsigned long long m[2];
	v2(turn, s, m);
	int n(0);
	int depth = disks - disks0;
	auto lo_ReferTable = ReferTable[depth].begin();
	int sizen(Size[depth]);
	while (n < sizen) {
		bool match(true);
		int i(0);
		while (match && i < 2) {
			if (lo_ReferTable[n][i] != m[i]) {
				match = false;
			}
			++i;
		}
		if (match) {
			return n;
		}
		++n;
	}
	return -1;
};

void Book::newdata(const int disks, const  int turn, const int(&s)[8][8], const double upper, const double lower)
{
	unsigned long long m[2];
	v2(turn, s, m);
	vector<unsigned long long> m2;
	m2.resize(2);
	m2[0] = m[0];
	m2[1] = m[1];
	int depth = disks - disks0;
	int n(Size[depth]);
	ReferTable[depth].emplace_back(m2);
	//ReferTable[depth][n].resize(2);
	//ReferTable[depth][n][0] = m[0];
	//ReferTable[depth][n][1] = m[1];
	TransTableUpper[depth].emplace_back(upper);
	TransTableLower[depth].emplace_back(lower);
	for (int i(0); i < 2;++i) {
		ReferTable[depth][n][i] = m[i];
	}
	++Size[depth];

	display(disks, turn, s, n);
}


void Book::record(const int disks, const  int n, const double upper, const double lower)
{
	int depth = disks - disks0;
	TransTableUpper[depth][n] = upper;
	TransTableLower[depth][n] = lower;
}

double Book::upper(const int disks, const int n)
{
	int depth = disks - disks0;
	return TransTableUpper[depth][n];
}

double Book::lower(const int disks, const  int n)
{
	int depth = disks - disks0;
	return TransTableLower[depth][n];
}

void Book::save()
{
	string bfiler;
	string bfilet;

	for (int i(0);i < sized;++i) {
		bfiler = neu_folder + "Book\\bookr" + to_string(disks0 + i) + ".txt";
		ofstream writing_file;
		writing_file.open(bfiler, ios::out);
		for (int j = 0;j < Size[i];++j) {
			for (int k(0); k < 2;++k) {
				writing_file << ReferTable[i][j][k] << ",";
			}
			writing_file << endl;
		}
		writing_file.close();

		bfilet = neu_folder + "Book\\bookt" + to_string(disks0 + i) + ".txt";
		ofstream writing_file2;
		writing_file2.open(bfilet, ios::out);
		for (int j = 0;j < Size[i];++j) {
			writing_file2 << TransTableLower[i][j] << "," << TransTableUpper[i][j] << "," << endl;
		}
		writing_file2.close();
	}

	//string bfiles;
	//bfiles = "Book\\books.txt";
	//ofstream writing_file;
	//writing_file.open(bfiler, ios::out);
	//for (int i(0);i < sized;++i) {
	//	writing_file << disks0 + i << "," << Size[i] << endl;
	//}
	//writing_file.close();
}

void Book::load()
{
	string bfiler;
	string bfilet;
	//string bfiles;
	//bfiles = "Book\\books.txt";
	//ifstream ifs(bfiles);
	//for (int i(0); i < sized;++i) {
	// Size[i] = 0;
	//}
	//if (ifs) {
	//	int n(0);
	//	string str;
	//	while (getline(ifs, str)) {
	//		string token;
	//		istringstream stream(str);
	//		getline(stream, token, ',');
	//		int l(stoi(token));
	//		getline(stream, token, ',');
	//		if (l >= disks0 && l < (disks0 + sized)) {
	//			Size[l] = stoi(token);
	//		}
	//	}
	//	ifs.close();
	//}

	for (int i(0);i < sized;++i) {
		Size[i] = 0;
		bfiler = neu_folder + "Book\\bookr" + to_string(disks0 + i) + ".txt";
		ifstream ifs(bfiler);
		if (ifs) {
			int n(0);
			string str;
			while (getline(ifs, str)) {
				string token;
				istringstream stream(str);
				//ReferTable[i][n].resize(2);
				//for (int j(0);j < 2;++j) {
				//	getline(stream, token, ',');
				//	ReferTable[i][n][j] = stoull(token);
				//}
				//++n;
				vector<unsigned long long> v(2);
				for (int j(0);j < 2;++j) {
					getline(stream, token, ',');
					v[j] = stoull(token);
				}
				ReferTable[i].emplace_back(v);
				++Size[i];
			}
			ifs.close();
		}

		bfilet = neu_folder + "Book\\bookt" + to_string(disks0 + i) + ".txt";
		ifstream ifs2(bfilet);
		if (ifs2) {
			int n(0);
			string str;
			while (getline(ifs2, str)) {
				string token;
				istringstream stream(str);
				getline(stream, token, ',');
				//TransTableLower[i][n] = stod(token);
				TransTableLower[i].emplace_back(stod(token));
				getline(stream, token, ',');
				//TransTableUpper[i][n] = stod(token);
				TransTableUpper[i].emplace_back(stod(token));
				++n;
			}
			ifs2.close();
		}
	}



}

void Book::display(const int disks, const  int turn, const int(&s)[8][8], int n)
{
	//for (int i(0);i < 8;++i) {
	//	for (int j(0);j < 8;++j) {
	//		std::cout << s[i][j] << " ";
	//	}
	//	std::cout << endl;
	//}
	cout << "  ";
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
	}
	cout << endl;
	for (int i(0); i < 8; ++i) {
		cout << i << " ";
		for (int j(0); j < 8; ++j) {
			switch (s[i][j]) {
			case 1:
				cout << "–";
				break;
			case 2:
				cout << "Z";
				break;
			default:
				cout << "\";
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
	int depth = disks - disks0;
	for (int i(0);i < 2;++i) {
		std::cout << ReferTable[depth][n][i] << " ";
	}
	std::cout << endl;
	std::cout << disks << " " << turn << " " << n << " " << TransTableLower[depth][n] << endl << endl;
}