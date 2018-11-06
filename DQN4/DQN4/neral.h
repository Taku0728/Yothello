#include "stdafx.h"

class Newralnet
{
public:
	Newralnet();
	~Newralnet();

private:
	int layers;
	int nodes[10];
	double wei[10][100][100];
	double bia[10][100];
	double dEdw[10][100][100];
	double dEdb[10][100];

	void initialize();

};

Newralnet::Newralnet()
{
}

Newralnet::~Newralnet()
{
}