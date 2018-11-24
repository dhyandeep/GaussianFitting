#pragma once
#include <vector>
using namespace std;
void printMatrix(vector< vector<double> > Mat);
vector<double> gaussElimination(vector< vector<double> > Augmented);
void FindParameters(double* x, double* lny, int N, vector<vector<double>>& augmentedMat);