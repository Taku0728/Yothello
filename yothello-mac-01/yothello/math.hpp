//
//  math.hpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#ifndef math_hpp
#define math_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>
#include <random>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <cmath>

using  std::cout;
using  std::cin;
using  std::wcout;
using  std::vector;
using  std::random_device;
using  std::endl;
using  std::abs;
using  std::max;
using  std::min;

using std::string;
using std::vector;
using std::random_device;
using std::mt19937;
using std::normal_distribution;

class Functor
{
public:
	Functor();
	virtual ~Functor();
	virtual double operator()(double x, double y);
	
private:
	double data;
	
};

class Func1: public Functor
{
public:
	Func1();
	~Func1();
	double operator()(double x, double y);
	
private:
	double data;
	
};

class Func2 : public Functor
{
public:
	Func2();
	~Func2();
	double operator()(double x, double y);
	
private:
	double data;
	
};

class Rotate_board
{
public:
	Rotate_board();
	~Rotate_board();
	void operator()(vector<double> &x);
	
private:
};

class Reflect_board
{
public:
	Reflect_board();
	~Reflect_board();
	void operator()(vector<double> &x);
private:
	
};

class Translate
{
public:
	Translate();
	~Translate();
	int operator()(const char *p);
	
private:
	
};

class Reverse
{
public:
	Reverse();
	~Reverse();
	void operator()(vector<int> &v);
	
private:
	
};

class Reverse2
{
public:
	Reverse2();
	~Reverse2();
	void operator()(vector<int> &v);
	
private:
	
};

class Fibc
{
public:
	Fibc();
	~Fibc();
	int operator()(int n);
	
private:
	
};

int fib(int n);

string get_pcname();



#endif /* math_hpp */
