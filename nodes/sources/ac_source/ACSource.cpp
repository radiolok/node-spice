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
#include "ACSource.h"


ACSource::ACSource(vector<parameters>& param)  : Node(param){
	//get frequency
	GetFloatParam(param, "-f", f, 50);

	//get amplitude voltage
	status St = GetFloatParam(param, "-Ua", Ua, 0);
	if (NOT_FOUND == St){//Ua NOT_FOUND? find Ud
		float Ud=0;
		St = GetFloatParam(param, "-Ud", Ud, 0);
		Ua = 1.41 * Ud;
	}

	float tmp;
	//load resistor array
	string r[STORED_VARS]={"-Ra", "-Rb", "-Rc"};
	St = GetFloatParam(param, "-R", tmp, 0);
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

	GetFloatParam(param, "-phi", phi, 0);

	omega = 2*M_PI*f;//314 for 50Hz

	for (int i=0; i<3 ; i++){
		I_out[i] = 0;
		I_out_old[i] = 0;
	}
	Solve(0, 0.00000001);

}

ACSource::~ACSource() {

}


void ACSource::Solve(float t, float dt){

	for (int i=0; i<3 ; i++){

		float dU = 0;
		if (!L[i]){
			dU = I_out[i]*R[i];

		}
		else{
			dU = I_out[i]*R[i]+L[i]*((I_out[i]-I_out_old[i])/dt);
		}

		float absdU = dU > 0?  dU : -dU;

		float absU = U_out[i] > 0? U_out[i] : -U_out[i];

		if (0 > (absU - absdU)){

			dU = U_out[i];
		}
		U_out[i] = (Ua)*sin(omega*t - (2*M_PI/3)*i + phi)-dU;

	}
}



