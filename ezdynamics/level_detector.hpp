#pragma once
#include <cmath>
namespace ezdynamics {

	class AbstractLevelDetector {
	public:
		virtual double process(const double& x) { return 0.0; }
		virtual void setFs(double) { this->fs = fs; }
	protected:
		double fs;
	};

	// Abstract Standard level detector with attack/release parameters and exponential smoothing.
	class StdLevelDetector: public AbstractLevelDetector {
	protected:
		double a_atk, a_rlse;
		double t_atk, t_rlse;
		double last_y;
		
		void recalcConstants() {
			this->a_atk = std::exp(-std::log(9)*1.0e3 / (this->t_atk * this->fs));
			this->a_rlse = std::exp(-std::log(9) * 1.0e3 / (this->t_rlse * this->fs));
		}
	public:
		StdLevelDetector() { setFs(48000.0); setAttack(1.0); setRelease(100.0); recalcConstants(); reset();  };

		// Sample rate in Hertz
		void setFs(double fsi) override {
			this->fs = fsi;
			recalcConstants();
		}

		// Attack in milliseconds
		void setAttack(double t_attack) {
			this->t_atk = t_attack;
			recalcConstants();
		}
		// Release in milliseconds
		void setRelease(double t_release) {
			this->t_rlse = t_release;
			recalcConstants();
		}

		//Reset buffer.
		void reset() { this->last_y = 0.0; }
	};


	// Eq. 16  from Digital Dynamic Range Compressor Design—A Tutorial and Analysis.
	// Author DIMITRIOS GIANNOULIS et.al.

	class SmoothBranchingLevelDetector : public StdLevelDetector {
	public:
		double process(const double& x) override{
			double ret;
			if (x <= this->last_y)
				ret = this->a_rlse * this->last_y + (1.0 - this->a_rlse) * x;
			else
				ret = this->a_atk * this->last_y + (1.0 - this->a_atk) * x;
			this->last_y = ret;
			return ret;
		}
	};

	
	class SmoothAttack : public StdLevelDetector {
	public:
		double process(const double& x) override {
			this->last_y = this->a_atk * this->last_y + (1.0 - this->a_atk) * x;
			return this->last_y;
		}
		void setRelease(double) = delete;
	};
	class SmoothRelease : public StdLevelDetector {
	public:
		double process(const double& x) override {
			double a = this->a_rlse * this->last_y + (1.0 - this->a_rlse) * x;
			this->last_y = (a >= x) ? a : x;
			return this->last_y;
		}
		void setAttack(double) = delete;
	};
	
}