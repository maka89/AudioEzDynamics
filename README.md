# AudioCompressorCpp

## Intro
Classes for controlling dynamic range of audio. Compressors + gates
Based on https://www.eecs.qmul.ac.uk/~josh/documents/2012/GiannoulisMassbergReiss-dynamicrangecompression-JAES2012.pdf

## Usage

### Gate
```c++
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
```
