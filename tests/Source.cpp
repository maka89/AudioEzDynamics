#pragma once
#include "../ezdynamics/dynamics.hpp"
#include "../ezdynamics/level_detector.hpp"

#include <iostream>
#include <cstdlib>
#include "testutils.h"
using namespace std;
using namespace ezdynamics;


vector<double> getTestTone() {
	double fs = 48000.0;
	double freq = 400.0;
	double bef = 0.0;
	double len = 0.8;
	double after = 0.0;

	vector<double> levels;
	levels.push_back(-300.0);
	levels.push_back(-6.0);
	levels.push_back(-20.0);


	vector<double> x = genTestToneSteps(bef, after, len, fs, freq, levels);
	return x;
}
void stepComp() {
	vector<double> x = getTestTone();
	vector<double> y = vector<double>();

	
	
	auto c = PeakCompressor();
	c.setAttack(40.0);
	c.setRelease(300.0);


	c.setKnee(0.0);
	c.setMakeup(0.0);
	c.setRatio(2.0);
	c.setThreshold(-12.0);
	c.setFs(48000.0);
	c.reset();
	//c.setRMSTime(10.0);
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
	plotCurve();
}
