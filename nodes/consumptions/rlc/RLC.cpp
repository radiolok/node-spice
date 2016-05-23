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
#include <nodes/consumptions/rlc/RLC.h>



RLC::RLC(vector<parameters>& param) : Consumption(param) {

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

	string c[STORED_VARS]={"-Ca", "-Cb", "-Cc"};
	St = GetFloatParam(param, "-C", tmp, 0);
	if (St == SUCCESS){
		for (int i=0; i<STORED_VARS; i++){
			C.push_back(tmp/1000000);
		}
	}
	else{
		for (uint8_t i=0; i < STORED_VARS; i++){
			GetFloatParam(param, c[i], tmp, 0);
			if (tmp){
				C.push_back(tmp/1000000);//capacitance in uF
			}
			else{
				C.push_back(0);//capacitance in uF
			}
		}
	}
	for (uint8_t i=0; i < STORED_VARS; i++){
			I_L_old.push_back(0);
	}

		cout <<"Asymmetric Load Created!" << endl;
		loadstatus = SUCCESS;

}

RLC::~RLC(){

}


void RLC::Solve(float t, float dt){

	for (int i=0; i< STORED_VARS; i++){

		//input voltage to output
		U_out[i] = U_in[i];

		I_in[i] = 0;
		if (R[i]){
			//add Active current
			I_in[i] += U_out[i] / R[i];
		}
		///add inductive current:
		if (L[i]){

			I_L_old[i] += U_out[i]*dt /L[i];

			I_in[i] = I_L_old[i];

		}
		else{
		//none
		}
		//add capacitive current
		if (C[i]){
			I_in[i] += C[i]* (U_out[i]-U_out_old[i])/dt;
		}
		else{
			//none
		}
	}
}

