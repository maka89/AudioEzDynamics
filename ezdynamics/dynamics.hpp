#pragma once
#include "level_detector.hpp"
#include "utils.hpp"
#include "dynamics_base.hpp"
namespace ezdynamics {




	// Log domain compressor
	// Fig. 7c in "Digital Dynamic Range Compressor Design—A Tutorial and Analysis".
	// Level detector on gain in db domain.
	class PeakCompressor : public DynamicsBase {
	public:
		PeakCompressor() :DynamicsBase() { setRatio(1.0); }
		double calc_gain(const double& x) override {
			double tmp = this->inpStage(x);
			tmp = gain_to_db(tmp);
			tmp = tmp - this->compression_curve(tmp, this->knee, this->threshold, this->ratio);
			tmp = this->makeup - f * l.process(f * tmp);
			return db_to_gain(tmp);
		}

		void setRatio(double ratio) { DynamicsBase::setRatio(ratio);  this->f = (ratio >= 1.0) ? 1.0 : -1.0; }
	protected:
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_above(x, w, t, r); }
		double f;
	};

	//RMS Edition
	class RMSCompressor : public RMSDynamics {
	public:
		RMSCompressor() :RMSDynamics() { setRatio(1.0); }
		double calc_gain(const double& x) override {
			double tmp = this->inpStage(x);
			tmp = gain_to_db(tmp);
			tmp = tmp - this->compression_curve(tmp, this->knee, this->threshold, this->ratio);
			tmp = this->makeup - f * l.process(f * tmp);
			return db_to_gain(tmp);
		}

		void setRatio(double ratio) { RMSDynamics::setRatio(ratio);  this->f = (ratio >= 1.0) ? 1.0 : -1.0; }
	protected:
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_above(x, w, t, r); }
		double f;
	};


	//
	// These applies level detector attack/release in the linear domain. At start of sidechain (Return to zero- from paper).
	// Hi: Delayed attack, unnatural release.
	// Lo: Delayed release.
	class PeakDynamicsLo :public DynamicsBase {
	protected:
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_below(x, w, t, r); }
	};
	class PeakDynamicsHi :public DynamicsBase {
	protected:
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_above(x, w, t, r); }
	};


	//RMS edition
	class RMSDynamicsLo : public RMSDynamics {
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_below(x, w, t, r); }
	};

	class RMSDynamicsHi : public RMSDynamics {
		double compression_curve(const double& x, const double& w, const double& t, const double& r) override { return curve_above(x, w, t, r); }
	};

	


	

}