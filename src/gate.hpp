#pragma once
#include <cmath>
#include "utils.hpp"

namespace dynamics {


class BasicGate {
public:
	BasicGate() {
		this->setFs(48000.0);
		this->setAttack(1.0);
		this->setRelease(100.0);
		this->setFloor(-20.0);
		this->last_gain = this->floor;
		this->setThreshold(-40.0);
		this->setHold(100.0);
	}
	void setFs(double fs) { this->fs = fs;  recalcConstants(); }
	void setAttack(double atk) { this->t_atk = atk;  recalcConstants();}
	void setRelease(double rlse) { this->t_rlse = rlse;  recalcConstants();}
	void setHold(double hold) { this->t_hold = hold; }
	void setThreshold(double threshold) { this->threshold = db_to_gain(threshold); }
	void setFloor(double floor) { this->floor = db_to_gain(floor);  }
	double calc_gain(const double& x) {
		double signal = std::abs(x);
		double tmp = (signal > this->threshold) ? 1.0 : floor ;
		if (tmp <= last_gain){
			if (attack_counter > t_hold) {
				tmp = this->a_atk * last_gain + this->a_atk2 * tmp;
			}
			else {
				tmp = last_gain;
				attack_counter += this->dt*1e3;
			}
		}
		else {
			tmp = this->a_rlse * last_gain + this->a_rlse2 * tmp;
			attack_counter = 0.0;
		}
		this->last_gain = tmp;


		return  tmp; 
	}

private:
	double t_atk, t_rlse, t_hold;
	double a_atk, a_rlse, a_atk2, a_rlse2;
	double attack_counter;
	double fs,dt;
	double threshold, floor;
	double last_gain;
	void recalcConstants() {
		this->a_atk = std::exp(-std::log(9) * 1.0e3 / (this->t_atk * this->fs));
		this->a_rlse = std::exp(-std::log(9) * 1.0e3 / (this->t_rlse * this->fs));

		this->a_atk2 = 1.0 - this->a_atk;
		this->a_rlse2 = 1.0 - this->a_rlse;
		this->dt = 1.0 / this->fs;
	}
};

}