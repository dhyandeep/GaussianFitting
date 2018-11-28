#include "CSVReader.h"
#include <string>
#include <sstream>
#include <cstring>
using namespace std;
CSVReader::CSVReader():m_MZIFileName("scans.csv"), m_RetentionFileName("retention_time.csv")
{
	m_MZIFile.open(m_MZIFileName, ofstream::in);
	m_RetentionFile.open(m_RetentionFileName, ofstream::in);
	if ((!m_MZIFile) || (!m_RetentionFile))
	{
		cout << __FUNCTION__ << " m_FileInStream.open() failed" << endl;
		throw std::runtime_error("Unable to open file");
	}
}
CSVReader::CSVReader(string fName1,string fName2):m_MZIFileName(fName1), m_RetentionFileName(fName2)
{
	m_MZIFile.open(m_MZIFileName, ofstream::in);
	m_RetentionFile.open(m_RetentionFileName, ofstream::in);
	if ((!m_MZIFile) || (!m_RetentionFile))
	{
		cout << __FUNCTION__ << " m_FileInStream.open() failed" << endl;
		throw std::runtime_error("Unable to open file");
	}
}

int g_count = 0;
float arr1[2000] = { 0 };
float arr2[2000] = { 0 };
float retent = 0;
void CSVReader::ReadCSV(int flag, ScanData obj[], int size)
{
	char c;
	float mbyz, intensity;
	string input,inputRetention;
	int row = 0;

	while ((m_MZIFile && m_RetentionFile)&&(row<ScanCount))
	{
		g_count = 0;
		input.clear();
		inputRetention.clear();
		getline(m_MZIFile, input);
		getline(m_RetentionFile, inputRetention);
		stringstream ssRet(inputRetention);
		float fRetentionInput;
		ssRet >> c >> c >> fRetentionInput>>c;
		obj[row].retentionTime = fRetentionInput;
		if(! input.empty())
		{
			stringstream ss(input);
			if (ss)
			{
				ss >> arr1[g_count] >> c >> arr2[g_count]; ;
				while (ss)
				{
					++g_count;
					ss >> c >> arr1[g_count] >> c >> arr2[g_count]; ;
				}

				obj[row].mbyz = new float[g_count + 1];
				memcpy(obj[row].mbyz, arr1, (g_count + 1) * sizeof(float));
				
				obj[row].intensity = new float[g_count + 1];;
				memcpy(obj[row].intensity, arr2, (g_count + 1) * sizeof(float));
				obj[row].rowsize = g_count + 1;
			}

		}
		row++;
	}

}


CSVReader::~CSVReader()
{
	if (m_MZIFile.is_open())
	{
		m_MZIFile.close();
	}
}
