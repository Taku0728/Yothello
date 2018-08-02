// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#ifndef INCLUDED_stdafx_h_
#define INCLUDED_stdafx_h_
#pragma once

#define _USE_MATH_DEFINES
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
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
#include <windows.h>

template<typename A, size_t N, typename T>
void Fill(A(&array)[N], const T &val) {
	std::fill((T*)array, (T*)(array + N), val);
}



#endif

// TODO: プログラムに必要な追加ヘッダーをここで参照してください
