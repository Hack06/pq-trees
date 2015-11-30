#include "PQTree.h"
#include <stdlib.h>
#include <sstream>


//#pragma GCC diagnostic ignored "-Wc++11-extensions"
//#pragma GCC diagnostic ignored "-Wpadded"

void testConsectuive() //purpose: tests a consectutive ones matrix example
{
	bool pass = true;
	std::vector< std::vector<int> > mat =
	{
//		{2, 3, 4}, //values that are one in our matrix
//		{1, 2, 3},
//		{1, 4, 5},
//		{2, 3},
//		{3, 4},
//		{1},
//		{5}
			{1,2},
			{2,3},
			{1,3}
	};
//	PQTree *tree = new PQTree("{1, 2, 3, 4, 5}");
//	int setSize = 5;
	PQTree *tree = new PQTree("{1, 2, 3}");
	int setSize = 3;
	
	for(size_t i=0; i<mat.size(); ++i)
	{
		if(!tree->set_consecutive(mat[i]))
		{
			pass = false;
			break;
		}
	}
	delete tree;
	
	//print out the matrix
	printf("Matrix to check:\n");
	for(size_t i=0; i<mat.size(); ++i) {
		int k=1;
		for(unsigned int j=0; j<mat[i].size(); ++j) {
			while (mat[i][j] != k++) {
				printf("0 ");
			}
			printf("1 ");
		}
		while (k++ <= setSize) {
			printf("0 ");
		}
		printf("\n");
	}
	printf("\n");

	//print out the result of the algorithm
	if (pass) {
		printf("\nThe matrix has Consecutive-Ones property!\n");
	} else {
		printf("\nThe matrix does NOT have Consecutive-Ones property!\n");
	}
	
}

int main( int argc, char **argv)
{
    testConsectuive();
    return 0;
}


