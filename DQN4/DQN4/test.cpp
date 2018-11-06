

#include "stdafx.h" 
#include "othello.h"
#include "maths.h"
#include "Learning.h"
#include "DQN.h"
using namespace std;


int main(){
	
	clock_t start = clock();

	Board b;
	b.initialize();
	b.calculate_moves();
	cout << b.move_num << endl;

	clock_t end = clock();
	cout << end - start << endl;
	cout << endl;
}