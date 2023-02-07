#pragma once
#include "../src/dynamics.hpp"
#include "../src/level_detector.hpp"
#include "../src/gate.hpp"

#include <iostream>
#include <cstdlib>
#include "testutils.h"
using namespace std;
using namespace dynamics;
int main2() {

	const int N = 100;
	double x[N],y[N];
	for (int i = 0; i < N; i++) 
		x[i] = ((float)rand()) / RAND_MAX;

	
	Compressor c;
	c.setFs(48000.0);
	c.setAttack(1.0);
	c.setRelease(100.0);

	c.setKnee(6.0);
	c.setRatio(1.2);
	c.setMakeup(0.0);
	c.setThreshold(-30.0);


	for (int i = 0; i < N; i++) 
		y[i] = x[i]*c.calc_gain(x[i]);
	
	
}

vector<double> getTestTone() {
	double fs = 48000.0;
	double freq = 20.0;
	double bef = 0.1;
	double len = 0.5;
	double after = 2.0;

	vector<double> levels;
	levels.push_back(-10.0);
	levels.push_back(-20.0);
	levels.push_back(-29.99);
	levels.push_back(-31.0);
	levels.push_back(-35.0);

	vector<double> x = genTestToneSteps(bef, after, len, fs, freq, levels);
	return x;
}
void stepGate() {

	vector<double> x = getTestTone();
	vector<double> y = vector<double>();

	BasicGate g;
	g.setFs(48000.0);
	g.setAttack(500.0);
	g.setRelease(5.0);
	g.setFloor(-20.0);
	g.setThreshold(-30.0);
	g.setHold(50.0);


	for (int i = 0; i < x.size(); i++) {
		double tmp;
		tmp = g.calc_gain(x[i]);
		y.push_back(tmp);
	}
	printData(x, y,"stepGate.txt");

}
void stepComp() {
	vector<double> x = getTestTone();
	vector<double> y = vector<double>();

	
	
	auto c = Compressor();
	c.setAttack(100.0);
	c.setRelease(300.0);


	c.setKnee(0.0);
	c.setMakeup(0.0);
	c.setRatio(2.0);
	c.setThreshold(-25.0);
	c.setFs(48000.0);
	//c.setRMSTimefactor(40.0);
	for (int i = 0; i < x.size(); i++) {
		double tmp;
		tmp = c.calc_gain(x[i]);
		y.push_back(tmp);
	}
	printData(x, y, "stepComp.txt");

}

void plotCurve() {
	double xmin = -90.0;
	double xmax = 0.0;
	int N = 1000;
	double dx = (xmax - xmin) / (N - 1.0);
	double t = -30.0;
	double w = 3.0;
	double r = 1.5;

	vector<double> xv, yv;
	for (int i = 0; i < N; i++) {
		double x = xmin + i * dx;
		xv.push_back(x);
		yv.push_back(curve_below(x, w, t, r));
	}
	printData(xv, yv, "curve.txt");
}
int main() {
	stepComp();
	stepGate();
	plotCurve();
}
