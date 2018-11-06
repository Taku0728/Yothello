#include "stdafx.h" 
#include "othello.h"
#include "maths.h"

int main()
{
	std::string folder("test");
	std::string file("Game5x");
	int randomize(1);

	const int trial(20);
	clock_t start = clock();
	Random_choose ch;
	const int files(1);
	for (int n(0);n < files;++n) {
		for (int i = 0;i < trial;++i) {
			Game g;
			g.playgame(ch, ch);
			g.writegame2(n, folder, file, randomize);
			//	da.adddata(g.get_game_matrix().begin(), g.get_turns());
		}
		//	da.writedata(writing_file);
	}

	//Board b;
	//b.read_board_v();
	//b.write_board_full();

	clock_t end = clock();
	std::string filename2 = "Result.txt";
	std::ofstream writing_file2;
	writing_file2.open(filename2, std::ios::app);
	//	writing_file.open(filename, std::ios::out);
	writing_file2 << "Time Cost: " << end - start << std::endl;
	writing_file2 << "Trial    : " << trial*files << std::endl << std::endl;
	return 0;

	//std::string filename = "mathtest2\\mathfunc.csv";
	//std::ofstream writing_file;
	//writing_file.open(filename, std::ios::app);

	//std::random_device rnd;
	//std::mt19937 mt(rnd());
	//std::uniform_real_distribution<> rand(-5.12, 5.12);
	//Func2 f;
	//double x(0);
	//double y(0);
	//double z(0);

	//const int trial(10000);
	//clock_t start = clock();
	//Random_choose ch;
	//const int files(1);
	//for (int n(0);n < files;++n) {
	//	for (int i = 0;i < trial;++i) {
	//		x = rand(mt);
	//		y = rand(mt);
	//		z = f(x, y);
	//		writing_file << x << "," << y << "," << z << "," << std::endl;
	//	}
	//}

	////Board b;
	////b.read_board_v();
	////b.write_board_full();

	//clock_t end = clock();
	//std::string filename2 = "Result.txt";
	//std::ofstream writing_file2;
	//writing_file2.open(filename2, std::ios::app);
	////	writing_file.open(filename, std::ios::out);
	//writing_file2 << "Time Cost: " << end - start << std::endl;
	//writing_file2 << "Trial    : " << trial*files << std::endl << std::endl;
	//return 0;

}