#pragma once
#include <cmath>
#include <limits>



namespace dynamics {

	const double lowest_number = std::numeric_limits<double>::denorm_min();

	inline double gain_to_db(const double& x) { return 20.0 * std::log10(x+ lowest_number); }
	inline double db_to_gain(const double& x) { return std::pow(10.0, x * 0.05); }


	// Compression curve for a downward compressor / upward expander.
	// I.e. ratio affects area above threshold.
	// x - Audio level in ( in db)
	// w - Knee width
	// t - Threshold
	// r - Ratio
	inline double curve_above(const double& x, const double& w, const double& t, const double& r) {
		double os = x - t;
		double ret;
		if (2.0 * os < -w)
			ret = x;
		else if (2.0 * os > w)
			ret = t + (x - t) / r;
		else
			ret = x + (1.0 / r - 1.0) * std::pow(x - t + 0.5 * w, 2) * 0.5 / w;
		return ret;
	}

	// Compression curve for a upward compressor / downward expander.
	// I.e. ratio affects area below threshold.
	// x - Audio level in ( in db)
	// w - Knee width
	// t - Threshold
	// r - Ratio
	inline double curve_below(const double& x, const double& w, const double& t, const double& r) {
		return (curve_above(x, w, t, r) - t) * r + t;
	}

	// t1, w1, r1 is for down compression / up expansion.
	// t2, w2, r2 is for down expansion / up compression.
	inline double curve_above_below(const double& x, const double& w1, const double& t1, const double& r1, const double& w2, const double& t2, const double& r2) {
		return curve_below(curve_above(x, w1, t1, r1), w2, t2, r2);
	}


	
	class ParameterSmoother {
	public:
		ParameterSmoother() { setFs(48000.0); setTime(50.0); reset(); }
		double process(double x) {
			val = a * val + b * x;
			return val;
		}
		void setTime(double t) { this->t = t; recalc(); }
		void setFs(double fs) { this->fs = fs; recalc(); }
		void reset() { setVal(0.0); }
		void setVal(double x) { val = x; }
	private:
		void recalc() {
			this->a = std::exp(-std::log(9) * 1.0e3 / (this->t * this->fs));
			this->b = 1.0 - a;
		}
		double fs, a, b, val,t;
	};
}