#pragma once
#include "level_detector.hpp"
#include "utils.hpp"

#include <cmath>
namespace ezdynamics {

	class DynamicsBase {
	public:
		DynamicsBase(){ setKnee(3.0); setMakeup(0.0); setThreshold(0.0); setRatio(1.0); this->setAttack(5.0); this->setFs(48000.0); this->setRelease(100.0); }

		virtual double calc_gain(const double& x)  {

			double tmp = this->inpStage(x);
			tmp = l.process(tmp);
			tmp = gain_to_db(tmp);			
			tmp = tmp - this->compression_curve(tmp, this->knee, this->threshold, this->ratio);
			tmp = this->makeup - tmp;//
			return db_to_gain(tmp);

		}

		void setRatio(double ratio) { this->ratio = ratio;}
		void setKnee(double knee) { this->knee = knee; }
		void setMakeup(double makeup) { this->makeup = makeup; }
		void setThreshold(double threshold) { this->threshold = threshold; }
		void setAttack(double atk) { l.setAttack(atk); }
		void setRelease(double rel) { l.setRelease(rel); }
		void setFs(double fs) {  l.setFs(fs); }
		void reset() { l.reset(); }
		
	protected:
		double ratio, knee, makeup, threshold;
		virtual double inpStage(const double& x) { return std::abs(x); };
		virtual double compression_curve(const double& x, const double& w, const double& t, const double& r){ return x; }
		SmoothBranchingLevelDetector l;
	};


	class RMSDynamics : public DynamicsBase {
	public:
		RMSDynamics() :DynamicsBase() { reset(); setFs(48000.0); setRMSTime(5.0); }
		void reset() { DynamicsBase::reset(); p.reset(); }
		void setFs(double fs) { DynamicsBase::setFs(fs); p.setFs(fs); }
		void setRMSTime(double t) { p.setTime(t); }

	protected:
		double inpStage(const double& x) override { return std::sqrt(p.process(x * x)); }
		ParameterSmoother p;
	};




	


	

}