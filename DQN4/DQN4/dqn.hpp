
#ifndef dqn_hpp
#define dqn_hpp

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
#include <chrono>
#include <iomanip>
#include "neuralnet.hpp"
#include "math.hpp"
#include "othello.hpp"
// #include <amp.h>

using  std::cout;
using  std::cin;
using  std::wcout;
using  std::endl;
using  std::abs;
using  std::max;
using  std::min;

using std::string;
using std::vector;
using std::random_device;
using std::mt19937;
using std::normal_distribution;
using std::ostringstream;

void auto_play();

void montecarlo_learn(const string neuralnet_folder);

void build_book();

void test_accuracy();

void view_book();

void surf_board(const Board &b,
					map<string, pair<double, double> > &m,
					map<string, pair<double, double> > &m2,
					const double range,
					const double step_cost);

void explore_board(const Neuralnet neuralnet,
					const Board &b,
					map<string, pair<double, double> > &m,
					const int depth,
					const double cut_ratio,
					const double range,
					const double step_cost);

#endif
