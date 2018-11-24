#pragma once
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
//const int ScanCount = 5610;
const int ScanCount = 5610;
const float IntensityThreshold = 10e4;

struct Point
{
	int scanNo;
	float intensity;
};

struct ScanData
{
	
	float retentionTime;
	//map<float, float> MByZvsIntensity;
	int rowsize;
	float* mbyz;
	float* intensity;
};

class CSVReader
{
	string m_MZIFileName;
	string m_RetentionFileName;
	ifstream m_MZIFile;
	ifstream m_RetentionFile;
	
public:
	CSVReader();
	CSVReader(string fName, string fName2);
	CSVReader(CSVReader& toCpy) = delete;
	CSVReader operator = (CSVReader& toassign) = delete;
	//map<int, pair<float, float >> ReadCSV(int flag);
	void ReadCSV(int flag, ScanData obj[], int size);
	~CSVReader();
};
