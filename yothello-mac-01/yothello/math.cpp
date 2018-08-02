//
//  math.cpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#include "math.hpp"


Func1::Func1()
{
}

Func1::~Func1()
{
}

double Func1::operator()(double x, double y)
{
	return 20 + pow(x, 2) - 10 * std::cos(2 * M_PI*x) + pow(y, 2) - 10 * std::cos(2 * M_PI*y);
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
