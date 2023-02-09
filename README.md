# AudioCompressorCpp

## Intro
Classes for controlling dynamic range of audio.

### Compressors
Repos has classes a PeakCompressor() and RMSCompressor() based on Fig. 7c  in ref. [1].
Can also be used as an upward expander.

It applies attack and release to the calculated gain in the log (dB) domain.


### DynamicsHi
![DynamicsHi](/doc/img/hi.png)

PeakDynamicsHi() and RMSDynamicsHi() are added for completeness. But are probably not too useful.
Used for DownwardCompression (aka. regular compression) or Upward Expansion.
Based on design outlined in Fig.7a in ref. [1] The same paper outlines some issues with this design:

- Sudden increases in volume leads to a "lag" in the attack. Since it needs time to ramp up to the threshold.
- Unnatural sounding release, due to release being applied in linear domain. This applies to compression. if it also applies to upward expansion, Im not sure.


### DynamicsLo
![DynamicsLo](/doc/img/lo.png)

PeakDynamicsLo() and RMSDynamicsLo() class.
Same as DynamicsHi classes, but used for Downward Expansion or Upwards Compression.

Note that the issues with DynamicsHi, does not show up in the same way here.


- Not sure if release applied in the linear domain sounds bad for upward compression or downward expansion. Needs to be investigated.
  But implementing release in the dB domain seems hard, without complicating the design.
  The magnitude of the gain (in dB) gets higher the more silent the input signal. And thus the release time can get very short if the soundlevel drops to silence.
  
- No lag in the attack. There is a lag in the release, since sound level needs to fall below the threshold before gain starts being applied. 

#### Discussion on release lag
If the lag in the release is undesireable, the LevelDetector can be moved to the end of the sidechain, in the linear domain. 
This will remove the lag entirely, but might lead to distortion, when the soundlevel is above the threshold. 
The distortion could for instance be removed by removing the softKnee functionality and introducing a "hold time" as is know from gates.

Another option would be to keep the level detector at the start of the sidechain, but set a max. level above the threshold the soundlevel is "allowed" to go.


## Usage

### Compressor
```c++
#include <dynamics.hpp>

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
[1] [Giannoulis et al. (2012) *Digital Dynamic Range Compressor Design—A Tutorial and Analysis*](https://www.eecs.qmul.ac.uk/~josh/documents/2012/GiannoulisMassbergReiss-dynamicrangecompression-JAES2012.pdf)
