#include <iostream>
#include <cmath>
#include <vector>
#include "CMatrix.h"

using namespace std;

void printMatrix(vector< vector<double>> Matrix)
{
	int n = Matrix.size();
	for (int i = 0; i<n; i++)
	{
		for (int j = 0; j<n + 1; j++)
		{
			cout << Matrix[i][j] << "\t";
			if (j == n - 1)
			{
				cout << "# ";
			}
		}
		cout << "\n";
	}
	cout << endl;
}

vector<double> gaussElimination(vector< vector<double>> Augmented)
{
	int n = Augmented.size();

	for (int i = 0; i<n; i++)
	{
		double maxEl = abs(Augmented[i][i]);
		int maxRow = i;
		for (int k = i + 1; k<n; k++)
		{
			if (abs(Augmented[k][i]) > maxEl)
			{
				maxEl = abs(Augmented[k][i]);
				maxRow = k;
			}
		}

		for (int k = i; k<n + 1; k++)
		{
			double tmp = Augmented[maxRow][k];
			Augmented[maxRow][k] = Augmented[i][k];
			Augmented[i][k] = tmp;
		}


		for (int k = i + 1; k<n; k++)
		{
			double c = -Augmented[k][i] / Augmented[i][i];
			for (int j = i; j<n + 1; j++)
			{
				if (i == j)
				{
					Augmented[k][j] = 0;
				}
				else
				{
					Augmented[k][j] += c * Augmented[i][j];
				}
			}
		}
	}

	vector<double> x(n);
	for (int i = n - 1; i >= 0; i--)
	{
		x[i] = Augmented[i][n] / Augmented[i][i];
		for (int k = i - 1; k >= 0; k--)
		{
			Augmented[k][n] -= Augmented[k][i] * x[i];
		}
	}
	return x;
}

void FindParameters(double* x, double* lny, int N, vector<vector<double>>& augmentedMat)
{
	double xSum = 0.0, x2Sum = 0.0, x3Sum = 0.0, x4Sum = 0.0, lnySum = 0.0, xlnySum = 0.0, x2lnySum = 0.0;
	double tmpPowerd;
	for (int i = 0; i < N; ++i)
	{
		lnySum += lny[i];
		//
		tmpPowerd = x[i];
		xSum += tmpPowerd;
		xlnySum += tmpPowerd * lny[i];
		//
		tmpPowerd *= x[i];
		x2Sum += tmpPowerd;
		x2lnySum += tmpPowerd * lny[i];
		//
		tmpPowerd *= x[i];
		x3Sum += tmpPowerd;
		//
		tmpPowerd *= x[i];
		x4Sum += tmpPowerd;
	}
	vector<double> row1;
	row1.push_back(N);
	row1.push_back(xSum);
	row1.push_back(x2Sum);
	row1.push_back(lnySum);

	vector<double> row2;
	row2.push_back(xSum);
	row2.push_back(x2Sum);
	row2.push_back(x3Sum);
	row2.push_back(xlnySum);

	vector<double> row3;
	row3.push_back(x2Sum);
	row3.push_back(x3Sum);
	row3.push_back(x4Sum);
	row3.push_back(x2lnySum);

	augmentedMat.push_back(row1);
	augmentedMat.push_back(row2);
	augmentedMat.push_back(row3);
}