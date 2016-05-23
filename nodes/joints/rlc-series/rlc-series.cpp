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
#include "rlc-series.h"

RLCseries::RLCseries(vector<parameters>& param): Node(param) {


	float tmp;

		//load resistor array
		string r[STORED_VARS]={"-Ra", "-Rb", "-Rc"};
		status St = GetFloatParam(param, "-R", tmp, 0);
		if (St == SUCCESS){
			for (int i=0; i<STORED_VARS; i++){
				R.push_back(tmp);
			}
		}
		else{
			for (int i=0; i<STORED_VARS; i++){
				GetFloatParam(param, r[i], tmp, 0);
				R.push_back(tmp);
			}
		}

		//load inductance array

		string l[STORED_VARS]={"-La", "-Lb", "-Lc"};
		St = GetFloatParam(param, "-L", tmp, 0);
		if (St == SUCCESS){
			for (int i=0; i<STORED_VARS; i++){
				L.push_back(tmp);
			}
		}
		else{
			for (int i=0; i<STORED_VARS; i++){
				GetFloatParam(param, l[i], tmp, 0);
				L.push_back(tmp);
			}
		}

		string c1[STORED_VARS]={"-C1a", "-C1b", "-C1c"};
		St = GetFloatParam(param, "-C1", tmp, 0);
		if (St == SUCCESS){
			for (int i=0; i<STORED_VARS; i++){
				C1.push_back(tmp/1000000);
			}
		}
		else{
			for (uint8_t i=0; i < STORED_VARS; i++){
				GetFloatParam(param, c1[i], tmp, 0);
				if (tmp){
					C1.push_back(tmp/1000000);//capacitance in uF
				}
				else{
					C1.push_back(0);//capacitance in uF
				}
			}
		}
		string c2[STORED_VARS]={"-C2a", "-C2b", "-C2c"};
		St = GetFloatParam(param, "-C2", tmp, 0);
		if (St == SUCCESS){
			for (int i=0; i<STORED_VARS; i++){
				C2.push_back(tmp/1000000);
			}
		}
		else{
			for (uint8_t i=0; i < STORED_VARS; i++){
				GetFloatParam(param, c2[i], tmp, 0);
				if (tmp){
					C2.push_back(tmp/1000000);//capacitance in uF
				}
				else{
					C2.push_back(0);//capacitance in uF
				}
			}
		}

		for (uint8_t i=0; i < STORED_VARS; i++){
				I_L_old.push_back(0);
				U_C_old.push_back(0);
		}

		vector<string> U_momentary_name;
		U_momentary_name.push_back("U_A out, V");
		U_momentary_name.push_back("U_A in, V");
		GraphVoltagePhaseAMomentary = new ShowGraph(GetName()+"-U_A", string("t"), U_momentary_name, width, height, font, 2 );//for phi only

		vector<string> I_momentary_name;
		I_momentary_name.push_back("I_A out, A");
		I_momentary_name.push_back("I_A in, A");
		GraphCurrentPhaseAMomentary = new ShowGraph(GetName()+"-I_A", string("t"), I_momentary_name, width, height, font, 2 );//for phi only


			cout <<"Series RLC branch created!" << endl;
			loadstatus = SUCCESS;

}

RLCseries::~RLCseries() {

}

void RLCseries::Solve(float t, float dt){

	for (int i=0; i< STORED_VARS; i++){
		//add C2 current
		if (C2[i]){
			I_in[i] = C2[i] * (U_out[i] - U_out_old[i])/dt;
		}
		else{
			I_in[i] = 0;
		}

		//add children nodes current to capacitance current
		I_in[i] += I_out[i];
		//calc voltage drop:

		float dU = 0;
		if (L[i] == 0){
			dU = I_in[i]*R[i];
		}
		else{
			dU = I_in[i] * R[i] + L[i] * ((I_in[i] - I_L_old[i])/dt);
		}
		I_L_old[i] = I_in[i];

		//input voltage to output
		U_out[i] = U_in[i] - dU;//if resistanse if too high no voltage on the output
		//add C1 current
		if (C1[i]){
			I_in[i] += C1[i] * (U_in[i] - U_C_old[i])/dt;
		}
		else{

		}
		U_C_old[i] = U_in[i];
	}
	GraphVoltagePhaseAMomentary->AddData(0, t, U_out[A]);
	GraphVoltagePhaseAMomentary->AddData(1, t, U_in[A]);
	GraphCurrentPhaseAMomentary->AddData(0, t, I_out[A]);
	GraphCurrentPhaseAMomentary->AddData(1, t, I_in[A]);
}

void RLCseries::ShowCustomGraph(const string &style,
							status mode){
	GraphVoltagePhaseAMomentary->Plot(style, mode, saveraw);
	GraphCurrentPhaseAMomentary->Plot(style, mode, saveraw);
}
