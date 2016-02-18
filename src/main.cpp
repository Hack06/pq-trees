#include "PQTree.h"
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <string>
#include<sstream>
using namespace std;

template <typename T>
string ToString(T val) {
    stringstream stream;
    stream << val;
    return stream.str();
}

void testConsectuive(int n, int m) //purpose: tests a consectutive ones matrix example
{
	bool pass = true;
	int r=0;	//will contain the number of 1s in the random matrix
	string columns = "{1";
	for (int i=2; i<=n; ++i) {
		columns += ", " + ToString(i);
	}
	columns += "}";
	PQTree *tree = new PQTree(columns);
	
	std::vector< std::vector<int> > mat;
	for (int i=0; i<m; ++i) {
		std::vector<int> currentRow;
		int start = rand()%n+1;
		int end = rand()%n+1;
		if (start > end) {	//swap them
			int temp = start;
			start = end;
			end = temp;
		}
		r += end - start + 1;
		for (int j=start; j<=end; ++j) {
			currentRow.push_back(j);
		}
		mat.push_back(currentRow);
	}

	//start counting the timer
	long ms = clock();

	for(size_t i=0; i<mat.size(); ++i) {
		if(!tree->set_consecutive(mat[i])) {
			pass = false;
			break;
		}
	}

	//get the elapsed time
	ms = clock() - ms;
	printf("%d %d %d %ld\n", n, m, r, ms/(CLOCKS_PER_SEC/1000));

	delete tree;
	
}

int main( int argc, char **argv)
{
	printf("n     m     r    t\n");
	for (int n=100; n<2100; n+=100) {
		for (int m=100; m<2100; m+=100) {
			testConsectuive(n,m);
		}
	}
    return 0;
}