#include "CSVReader.h"
#include <algorithm>
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
					//peaks.push_back(new Peak{ sCounter, data[sCounter].intensity[j] });
					peaksInRange.push_back(Point{ sCounter, data[sCounter].intensity[j] });
				}
				IAndScanNo.push_back(Point{ sCounter, data[sCounter].intensity[j] });
				cout << data[sCounter].retentionTime.c_str() <<"    "<<data[sCounter].intensity[j] << endl;
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
int main()
{
	try
	{
		CSVReader CReader("scans.csv", "retention_time.csv");
		ScanData DataSet[ScanCount];

		CReader.ReadCSV(0, DataSet, ScanCount);
		PrintScanAt(DataSet,610);
		vector<Point> resultPeaks;
		vector<Point> resultIntensities;
		GetIntensityAndRetentionForMbyZRange(DataSet, 128.0340, 128.0366, resultPeaks, resultIntensities);
		cout << "**********peaks**********" << endl;
		cout << "Intensity   " << "Retention" << endl;
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
			cout << i.intensity << " " << DataSet[i.scanNo].retentionTime.c_str()<<endl;
		}

		//vector<Point> resultIntensities1{ {1,1488}, {2,15}, {3, 20}, {4,80},/*4,8 should hit*/ {5,90}, {6,100}, {7,100}, {8,101}, {9,110}, \
		//{10, 110}, { 11, 150 }, { 12, 152 }, { 13, 150 }, { 14, 150 }, { 15, 140 }, { 16, 102 }, { 17, 20 }, { 18, 16 },/*should hit*/{ 19,15 }, \
		//{ 20,222 } };
		//Point max1{12,152 };
		pair<Point, Point> resultBounds = FindLowerAndUpper(max, resultIntensities);
		//pair<Point, Point> resultBounds = FindLowerAndUpper(max1, resultIntensities1);
		cout << "Lower Bound = " << resultBounds.first.intensity << "UpperBound = " << resultBounds.second.intensity;
	}
	catch (...)
	{
		cout << "Exception" << endl;
	}
	
	return 0;
}