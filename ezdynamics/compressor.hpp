#pragma once
#include "level_detector.hpp"
#include "utils.hpp"

#include <cmath>
namespace ezdynamics {

	class BaseCompressor {
	public:
		BaseCompressor(){ setKnee(3.0); setMakeup(0.0); setThreshold(0.0); setRatio(1.0); }
		void setRatio(double ratio) { this->ratio = ratio; }
		void setKnee(double knee) { this->knee = knee; }
		void setMakeup(double makeup) { this->makeup = makeup; }
		void setThreshold(double threshold) { this->threshold = threshold; }

		virtual double compression_curve(const double& x, const double& w, const double& t, const double& r) { return x; };
		virtual double calc_gain(const double& x) { return 1.0; }

	protected:
		double ratio, knee, makeup,threshold;
	};
	


	// Log domain compressor
	// Fig. 7c in "Digital Dynamic Range Compressor Design—A Tutorial and Analysis".
	class PeakCompressor:public BaseCompressor {
	public:
		PeakCompressor() :BaseCompressor() { l.setAttack(5.0); l.setFs(48000.0); l.setRelease(100.0); this->setRatio(1.0); }
		double calc_gain(const double& x) override{
			double tmp = std::abs(x);
			tmp = gain_to_db(tmp);
			tmp = tmp - this->compression_curve(tmp, this->knee, this->threshold, this->ratio);
			tmp = this->makeup - f*l.process(f*tmp);
			return db_to_gain(tmp);
		}
		void setAttack(double atk) { l.setAttack(atk); }
		void setRelease(double rel) { l.setRelease(rel); }
		void setFs(double fs) { l.setFs(fs); }
		void setRatio(double ratio) { BaseCompressor::setRatio(ratio); this->f = (ratio >= 1.0) ? 1.0 : -1.0; }
		void reset() { l.reset(); }
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_above(x, w, t, r); }
	protected:
		double f;
		SmoothBranchingLevelDetector l;
	};




}