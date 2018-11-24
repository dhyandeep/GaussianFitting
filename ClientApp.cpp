#include "CSVReader.h"
#include <algorithm>
#include "CMatrix.h"
void PrintScanAt(ScanData* data, int scanNo)
{
	if (data)
	{
		cout << "m/z     " << "Intensity" << endl;
		for (int i = 0; i < data[scanNo].rowsize; i++)
		{
			cout << data[scanNo].mbyz[i] << "\t" << data[scanNo].intensity[i] << endl;
		}
	}
}

void GetIntensityAndRetentionForMbyZRange(ScanData* data, float lower, float upper, vector<Point>& peaksInRange, vector<Point>& IAndScanNo )
{
	cout << "**********Retention and Intensity for range " << lower << " to " << upper << "**********" << endl;
	cout << "Retention     " << "intensity" << endl;
	
	int ThresholdIntensityCounter = 0;
	for (int sCounter = 0; sCounter < ScanCount; sCounter++)
	{
		for (int j = 0; j < data[sCounter].rowsize; j++)
		{
			if ((data[sCounter].mbyz[j] >= lower) && (data[sCounter].mbyz[j] <= upper))
			{
				if (data[sCounter].intensity[j] > IntensityThreshold)
				{
					ThresholdIntensityCounter++;
					peaksInRange.push_back(Point{ sCounter, data[sCounter].intensity[j] });
				}
				IAndScanNo.push_back(Point{ sCounter, data[sCounter].intensity[j] });
				cout << data[sCounter].retentionTime <<"    "<<data[sCounter].intensity[j] << endl;
			}
		}
	}
	cout << "ThresholdIntensityCounter " << ThresholdIntensityCounter << endl;
	
}
pair<Point,Point>& FindLowerAndUpper(Point& GlobalPeak, vector<Point>& DataSet)
{
	//calculation of lower bound

	int currScanNo = GlobalPeak.scanNo;
	pair<Point,Point> bounds;
	
	int maxIndex = 0;
	while (DataSet[maxIndex].scanNo!=GlobalPeak.scanNo)
	{
		maxIndex++;
	}
	int i = maxIndex;
	while ( (i>0) && (DataSet[i].intensity >= DataSet[i - 1].intensity) )
		i--;
	bounds.first.scanNo = DataSet[i].scanNo;
	bounds.first.intensity= DataSet[i].intensity;

	i = maxIndex;
	
	while ( (i<DataSet.size()-1)  && (DataSet[i].intensity >= DataSet[i + 1].intensity) )
		i++;
	bounds.second.scanNo = DataSet[i].scanNo;
	bounds.second.intensity = DataSet[i].intensity;

	return bounds;
}
void FitGaussian(pair<Point, Point>& resultBounds, vector<Point>& Intensities, ScanData* DataSet)
{
	double* xArr = (double*)malloc(Intensities.size()*sizeof(double));
	double* logyArr = (double*)malloc(Intensities.size()*sizeof(double));
	int iter = 0;
	for (auto i : Intensities)
	{
		xArr[iter] = i.intensity;
		logyArr[iter] = log(DataSet[i.scanNo].retentionTime);
		iter++;
	}
	vector<vector<double>> augMatrix;
	FindParameters(xArr, logyArr, iter, augMatrix);
	vector<double> result = gaussElimination(augMatrix);
	
	cout << "a, b, c values:" << endl;
	for (auto i : result)
	{
		cout << i << "\t";
	}
	cout << endl;
	
	double mu = -result[1] / (2 * result[2]);						// mu = -b/(2*c);
	double sigma = sqrt(-1 / (2*result[2]));						// sigma = sqrt(-1/(2*c))
	double temp = result[0] - pow(result[1], 2) / (4 * result[2]);	//(a-((b^2)/4*c)
	double A = pow(2.71828, temp);									//e^(a-((b^2)/4*c))

	cout << "******************Gaussian Fitting ****************"<<endl;
	cout << "Mu = " << mu << endl;
	cout << "Sigma = " << sigma << endl;
	cout << "A = " << A << endl;
}
int main()
{
	try
	{
		CSVReader CReader("scans.csv", "retention_time.csv");
		ScanData DataSet[ScanCount];
		cout << "Reading from CSV files, please wait..." << endl;
		CReader.ReadCSV(0, DataSet, ScanCount);
		cout << "Input scan number" << endl;;
		int inputNo;
		cin >> inputNo;
		cout << "******printing values at scan number****" << inputNo<<endl;
		PrintScanAt(DataSet, inputNo);
		vector<Point> resultPeaks;
		vector<Point> resultIntensities;
		GetIntensityAndRetentionForMbyZRange(DataSet, 128.0340, 128.0366, resultPeaks, resultIntensities);
		cout << "**********peaks**********" << endl;
		cout << "Intensity  " << "Retention" << endl;
		Point max;
		max.intensity = resultPeaks[0].intensity;
		max.scanNo = 0;
		for (auto i : resultPeaks)
		{
			if (i.intensity > max.intensity)
			{
				max.intensity = i.intensity;
				max.scanNo = i.scanNo;
			}
			cout << i.intensity << "\t" << DataSet[i.scanNo].retentionTime <<endl;
		}

		pair<Point, Point> resultBounds = FindLowerAndUpper(max, resultIntensities);

		cout << "************* Upper and Lower Bounds ************" << endl;
		cout << "upper Bound = " << resultBounds.first.intensity << " lowerBound = " << resultBounds.second.intensity<<endl;
		FitGaussian(resultBounds, resultIntensities, DataSet);
		
	}
	catch (...)
	{
		cout << "Exception" << endl;
	}
	
	return 0;
}