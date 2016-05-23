/*******************************************************************
Node-SPICE - Power networks modelling system
Copyright (C) 2014-2016  Artem Kashkanov radiolokn@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

******************************************************************/
#ifndef ACMOTOR_H_
#define ACMOTOR_H_

#include "typedefs.h"
#include <string>
#include <vector>

#include "nodes/consumptions/Consumption.h"

#include <Eigen/Dense>
using namespace Eigen;

#include "util/Strings.h"
#include "gnuplot_i.hpp"

class ShowGraph;

class AC_motor : public Consumption{
public:

	AC_motor(vector<parameters>& param);

	virtual ~AC_motor();

protected:
	void Solve(float t, float dt);

	virtual void ShowCustomGraph(const string &style,
								status mode);

private:

	ShowGraph* savedGraphs;
	bool saveGraph;

	vector<double> savegraphs;
	vector<string> savedgraphnames;

	status sysstatus;

	float t_last_show;

	enum {A,B,C,a,b,c};
	void ClearArray(float* array, int size);
	void InitResistorsArray(void);
	void InitinductanseArrays(void);
	void CurrentTorque();
	void CurrentSpeed(float t, float dt);

	void SolveSystem(float t, float dt);

	float tload;

	VectorXd  I_sys;

	VectorXd dI;

	VectorXd U_sys;

	VectorXd dU;

	MatrixXd R;

	MatrixXd L1;

	MatrixXd L2;

	MatrixXd X1;

	float R_s;//stator resistanse
	float R_r;//rotos resistanse
	//float* R_l;//line active resistanse
	float L_s;//stator induction
	float L_r;//rotor induction
	//float * L_l;//line induction array
	float L_m;//stator-stator coil interinduction
	float J;//rotor inercia moment

	unsigned int p;

	float torque;
	float staticTorque;
	float speed;

};

#endif /* ACMOTOR_H_ */
