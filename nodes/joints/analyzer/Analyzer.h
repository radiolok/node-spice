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
#ifndef NODES_JOINTS_ANALYZER_ANALYZER_H_
#define NODES_JOINTS_ANALYZER_ANALYZER_H_


#include "typedefs.h"
#include <string>
#include <vector>

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "util/Strings.h"

#include "util/ShowGraph.h"

#include "nodes/Node.h"



class Analyzer : public Node {
public:
	Analyzer(vector<parameters>& param);
	virtual ~Analyzer();

protected:

	void Solve(float t, float dt);

	void ShowCustomGraph(const string &style, status mode);

private:

		bool rmsinit;
		unsigned int rmsitercount;
		float rmsperiod;
		//RMS sampling and counting work on different speed
		float t_RMS_counting;
		float t_RMS_last_count;

		//las tRMS period data
		vector<double>* U_array;
		vector<double>* I_array;

		int ringBuffer_Start;
		int ringBuffer_End;

		//previous last array element
		double U_very_old[STORED_VARS];
		double I_very_old[STORED_VARS];

		//sum of U^2
		double U_squared_sum[STORED_VARS];
		//sum of I^2
		double I_squared_sum[STORED_VARS];

		//sum of U * I
		double U_mul_I_sum[STORED_VARS];

		//sum of Hilbert(U) * I
		double U90_mul_I_sum[STORED_VARS];


		float Unominal;

		double t_old;

		bool collectgraph;
		int graphchannels;

		vector<double>* savedt;

		///flag and variable for saving voltage data
		vector<double>* savedU;//gnuplot works with double
		bool saveU;
		ShowGraph* GraphVoltageRMS;//gnuplot works with double
		vector<string> savedUnames;

		bool saveI;
		///flag and variable to save current data
		vector<double>* savedI;//gnuplot works with double
		ShowGraph* GraphCurrentRMS;//gnuplot works with double
		vector<string> savedInames;

		double savedIsum[STORED_VARS];

		///flag and variable to save current data
		bool savePhi;
		ShowGraph* GraphPowerFactorRMS;//gnuplot works with double
		vector<string> savedPhinames;

		///flag and variable to save current data
		bool saveP;
		ShowGraph* GraphActivePowerRMS;//gnuplot works with double
		vector<string> savedPnames;

		///flag and variable to save current data
		bool saveQ;
		ShowGraph* GraphReactivePowerRMS;//gnuplot works with double
		vector<string> savedQnames;

		///flag and variable to save current data
		bool saveS;
		ShowGraph* GraphFullPowerRMS;//gnuplot works with double
		vector<string> savedSnames;

		///flag and variable to save current data
		bool saveE;
		ShowGraph* GraphActiveEnergyRMS;//gnuplot works with double
		vector<string> savedEnames;

		///flag and variable to save current data
		bool saveR;
		ShowGraph* GraphActiveResistanceRMS;//gnuplot works with double
		vector<string> savedRnames;

		///flag and variable to save current data
		bool saveX;
		ShowGraph* GraphReactiveResistanceRMS;//gnuplot works with double
		vector<string> savedLnames;

		///flag and variable to save current data
		bool saveDR;
		ShowGraph* GraphActiveResistanceDerivativeRMS;//gnuplot works with double
		vector<string> savedDRnames;		///flag and variable to save current data

		bool saveDX;
		ShowGraph* GraphReactiveResistanceDerivativeRMS;//gnuplot works with double
		vector<string> savedDXnames;

		void CalcRMS(float t, float dt);

		void CalcPower(float t, float dt);

};

#endif /* NODES_JOINTS_ANALYZER_ANALYZER_H_ */
