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
	double tmp = g.calc_gain(x[i]);
	y.push_back(tmp*x[i]);
}
```

### Compressor
```c++
auto c = Compressor(); // Or RMSCompressor()
c.setAttack(100.0);
c.setRelease(300.0);


c.setKnee(0.0);
c.setMakeup(0.0);
c.setRatio(2.0);
c.setThreshold(-25.0);
c.setFs(48000.0);
//c.setRMSTimefactor(40.0); //If RMSCompressor()
for (int i = 0; i < x.size(); i++) {
	double tmp = c.calc_gain(x[i]);
	y.push_back(tmp*x[i]);
}
```
