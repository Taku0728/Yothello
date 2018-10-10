// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ��L�q���܂��B
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

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[����ŎQ�Ƃ��Ă�������
