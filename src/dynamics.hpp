#pragma once
#include "level_detector.hpp"
#include "utils.hpp"

#include <cmath>
namespace dynamics {

	class BaseCompressor {
	public:
		BaseCompressor() :knee(3.0), makeup(0.0), threshold(0.0), ratio(1.0){ }
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
	class Compressor:public BaseCompressor {
	public:
		Compressor() :BaseCompressor() { l.setAttack(5.0); l.setFs(48000.0); l.setRelease(100.0); }
		double calc_gain(const double& x) override{
			double tmp = std::abs(x);
			tmp = gain_to_db(tmp);
			tmp = tmp - this->compression_curve(tmp, this->knee, this->threshold, this->ratio);
			tmp = this->makeup - l.process(tmp);
			return db_to_gain(tmp);
		}
		void setAttack(double atk) { l.setAttack(atk); }
		void setRelease(double rel) { l.setRelease(rel); }
		void setFs(double fs) { l.setFs(fs); }
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_above(x, w, t, r); }
	protected:
		BranchingLevelDetector l;
	};

	class RMSCompressor :public BaseCompressor {
	public:
		RMSCompressor() :BaseCompressor() { p.setFs(48000.0); p.setTime(50.0); l.setAttack(5.0); l.setFs(48000.0); l.setRelease(100.0); }
		double calc_gain(const double& x) override {
			double tmp = std::sqrt(p.get(x * x));// std::abs(x);
			tmp = gain_to_db(tmp);
			tmp = tmp - this->compression_curve(tmp, this->knee, this->threshold, this->ratio);
			tmp = this->makeup - l.process(tmp);
			return db_to_gain(tmp);
		}
		void setRMSTimefactor(double time) { this->p.setTime(time); }
		void setFs(double fs) { this->l.setFs(fs); this->p.setFs(fs); }
		void setAttack(double atk) { this->l.setAttack(atk); }
		void setRelease(double rel) { this->l.setRelease(rel); }

	double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_above(x, w, t, r); }
	private:
		BranchingLevelDetector l;
		ParameterSmoother p;
	};


}