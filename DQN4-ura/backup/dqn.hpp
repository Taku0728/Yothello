
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

#endif
