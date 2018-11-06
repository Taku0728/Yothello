// Play.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h" 
#include "othello.h"
#include "maths.h"
#include "Learning.h"
#include "Tree.h"

int main()
{
	Play p;
	p();
	p.record();
	return 0;
}