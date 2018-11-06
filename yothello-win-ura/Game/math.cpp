//
//  math.cpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#include "math.hpp"

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

vector< vector<int> > square_to_black(const vector< vector<int> > s) {
	vector< vector<int> > b(8, vector<int> (8, 0));
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (s[i][j] == 1) {
				b[i][j] = 1;
			}
		}
	}
	return b;
}

vector< vector<int> > square_to_white(const vector< vector<int> > s) {
	vector< vector<int> > w(8, vector<int> (8, 0));
	for (int i(0); i < 8; ++i) {
		for (int j(0); j < 8; ++j) {
			if (s[i][j] == 2) {
				w[i][j] = 1;
			}
		}
	}
	return w;
}

ostringstream matrix_to_hex(const vector< vector<int> > v) {
	ostringstream ss;
	for (auto i : v) {
		int a(0);
		int d(1);
		for (int j(0); j < 4; ++j) {
			a += d * i[3 - j];
			d *= 2;
		}
		ss << std::hex << char16_t(a);
		a = 0;
		d = 1;
		for (int j(0); j < 4; ++j) {
			a += d * i[7 - j];
			d *= 2;
		}
		ss << std::hex << char16_t(a);
	}
	return ss;
}

Func1::Func1()
{
}

Func1::~Func1()
{
}

double Func1::operator()(double x, double y)
{
	return 1;
	// return 20 + pow(x, 2) - 10 * std::cos(2 * M_PI*x) + pow(y, 2) - 10 * std::cos(2 * M_PI*y);
}

Func2::Func2()
{
}

Func2::~Func2()
{
}

double Func2::operator()(double x, double y)
{
	return pow((x - 2), 2) + pow((x*y + y + 3), 2);
}

Functor::Functor()
{
}

Functor::~Functor()
{
}

double Functor::operator()(double x, double y)
{
	return 0;
}

Rotate_board::Rotate_board()
{
}

Rotate_board::~Rotate_board()
{
}

void Rotate_board::operator()(vector<double> &x)
{
	vector<double> temp(64, 0);
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			temp[8 * i + j] = x[8 * (7 - j) + i];
		}
	}
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			x[8 * i + j] = temp[8 * i + j];
		}
	}
}

Reflect_board::Reflect_board()
{
}

Reflect_board::~Reflect_board()
{
}

void Reflect_board::operator()(vector<double> &x)
{
	vector<double> temp(64, 0);
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			temp[8 * i + j] = x[63 - 8 * j - i];
		}
	}
	for (int i = 0;i < 8;++i) {
		for (int j = 0;j < 8;++j) {
			x[8 * i + j] = temp[8 * i + j];
		}
	}
}

Translate::Translate()
{
}

Translate::~Translate()
{
}

int Translate::operator()(const char *p)
{
	switch (*p)
	{
		case 'A':
			return 0;
		case 'B':
			return 1;
		case 'C':
			return 2;
		case 'D':
			return 3;
		case 'E':
			return 4;
		case 'F':
			return 5;
		case 'G':
			return 6;
		case 'H':
			return 7;
		default:
			return 0;
			break;
	}
}

Reverse::Reverse()
{
}

Reverse::~Reverse()
{
}

void Reverse::operator()(vector<int> &v)
{
	for (unsigned int i(0); i < v.size();++i) {
		switch (v[i]) {
			case 1:
				v[i] = 2;
				break;
			case 2:
				v[i] = 1;
				break;
		}
	}
	
}

Reverse2::Reverse2()
{
}

Reverse2::~Reverse2()
{
}

void Reverse2::operator()(vector<int> &v)
{
	for (unsigned int i(0); i < v.size() - 1;++i) {
		switch (v[i]) {
			case 1:
				v[i] = 2;
				break;
			case 2:
				v[i] = 1;
				break;
		}
	}
	v[v.size() - 1] = -v[v.size() - 1];
}

Fibc::Fibc()
{
}

Fibc::~Fibc()
{
}

int Fibc::operator()(int n)
{
	if (n == 0) {
		return 1;
	}
	if (n == 1) {
		return 1;
	}
	Fibc fibc;
	return fibc(n - 1) + fibc(n - 2);
}

int fib(int n) {
	if (n == 0) {
		return 1;
	}
	if (n == 1) {
		return 1;
	}
	return fib(n - 1) + fib(n - 2);
}
