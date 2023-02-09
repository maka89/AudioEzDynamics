# AudioCompressorCpp

## Intro
Classes for controlling dynamic range of audio.

### Compressors
Repos has a Peak- and RMS-Compressor based on Fig.7c  in ref. [1] :


It applies attack and release to the calculated gain in the log (dB) domain.

### DynamicsLo
There is a PeakDynamicsLo() and RMSDynamicsLo() class.

This is used for Downward Expansion or Upwards Compression.
It uses the Return-to-zero method from the paper




## Usage

### Compressor
```c++
auto c = PeakCompressor(); // Or RMSCompressor()
c.setAttack(100.0);
c.setRelease(300.0);


c.setKnee(0.0);
c.setMakeup(0.0);
c.setRatio(2.0);
c.setThreshold(-25.0);
c.setFs(48000.0);
//c.setRMSTime(10.0); //If RMSCompressor()
for (int i = 0; i < x.size(); i++) {
	double tmp = c.calc_gain(x[i]);
	y.push_back(tmp*x[i]);
}
```

## References
[1] ["Digital Dynamic Range Compressor Design—A Tutorial and Analysis", DIMITRIOS GIANNOULIS et al.](https://www.eecs.qmul.ac.uk/~josh/documents/2012/GiannoulisMassbergReiss-dynamicrangecompression-JAES2012.pdf)
