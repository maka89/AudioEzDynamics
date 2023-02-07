#pragma once
#include <vector>
#include <string>
using namespace std;
vector<double> genStep(int bef, int after, int len, double level);
vector<double> genImpulse(int bef, int after, double level);
vector<double> genTestTone(double befd, double afterd, double lend, double sr, double freq, double level);
vector<double> genTestToneSteps(double befd, double afterd, double lend, double sr, double freq, vector<double> levels);
void printData(vector<double> x, vector<double> y, string fn);