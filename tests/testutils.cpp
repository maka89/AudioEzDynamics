#pragma once
#include <vector>
#include "../src/utils.hpp"
#include <fstream>
using namespace std;
vector<double> genStep(int bef, int after, int len, double level) {
	auto v = vector<double>();
	for (int i = 0; i < bef; i++) 
		v.push_back(0.0);

	for (int i = 0; i < len; i++) 
		v.push_back(dynamics::db_to_gain(level));

	for (int i = 0; i < after; i++) 
		v.push_back(0.0);
	return v;
}
vector<double> genImpulse(int bef, int after, double level) {
	return genStep(bef, after, 1, level);
}

vector<double> genTestTone(double befd, double afterd, double lend, double sr, double freq, double level) {
	int bef = (int)round(befd * sr);
	int after = (int)round(afterd * sr);
	int len = (int)round(lend * sr);

	double dt = 1.0 / sr;
	double gain = dynamics::db_to_gain(level);
	auto v = vector<double>();
	for (int i = 0; i < bef; i++)
		v.push_back(0.0);
	for (int i = 0; i < len; i++) {
		double t = i * dt;
		v.push_back(gain*sin(2.0 * 3.14159265 * freq * t));
	}
	for (int i = 0; i < after; i++)
		v.push_back(0.0);
	return v;
}
vector<double> genTestToneSteps(double befd, double afterd, double lend, double sr, double freq, vector<double> levels) {
	int bef = (int)round(befd * sr);
	int after = (int)round(afterd * sr);
	int len = (int)round(lend * sr);

	double dt = 1.0 / sr;

	auto v = vector<double>();
	for (int i = 0; i < bef; i++)
		v.push_back(0.0);
	double t = 0.0;
	for (int j = 0; j < levels.size(); j++) {
		
		double gain = dynamics::db_to_gain(levels[j]);
		for (int i = 0; i < len; i++) {
			v.push_back(gain * sin(2.0 * 3.14159265 * freq * t));
			t += dt;
		}
	}
	for (int i = 0; i < after; i++)
		v.push_back(0.0);
	return v;
}


void printData(vector<double> x, vector<double> y,string fn) {
	ofstream myfile;
	myfile.open(fn);
	for (int i = 0; i < x.size(); i++) 
		myfile << x[i] << ", " << y[i] << endl;
	myfile.close();
}