# GaussianFitting


## Getting Started

Use  this code with visual studio 2017 (set c++11 features enabled). copy the entire folder to your local system and open the .vcxproj file with visual studio. copy the dataset files "scans.csv" and "retention_time.csv" to the same folder. 

Build the project with visual studio and run the same.

Wait for several minutes for reading the dataset. Then input the scan number to display table of intensity and m/z. 

It will output the following

1. it will print the table for Intensity and retention for the range between the given limit of m/z.

2. it will print the peak values above 10e4,

3. lower and upper bounds for the maximum peak will be printed.

4. it will print the coefficients of the gaussian which will fit this curve approximately.

## Built With

Visual studio 2017

VC++ version 14.13

## Algorithms used

Gaussian approximation of curve : caruana's algorithm  https://www.researchgate.net/publication/252062037_A_Simple_Algorithm_for_Fitting_a_Gaussian_Function_DSP_Tips_and_Tricks

Algorithms used in solving caruana's algorithm: GaussElimination for solving linear system of equation: https://en.wikipedia.org/wiki/Gaussian_elimination






