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
#include "ACmotor.h"

//#include "gnuplot_i.hpp"


AC_motor::AC_motor(vector<parameters>& param) : Consumption(param){

	t_last_show  =0;

	I_sys = VectorXd::Zero(6);

	U_sys = VectorXd::Zero(6);

	dI = VectorXd::Zero(6);

	dU = VectorXd::Zero(6);

	X1 = MatrixXd::Zero(6, 6);

	torque = 0.0;
	speed = 0.0;
	//search data

	status St;
	St = GetFloatParam(param, "-Rs", R_s, 0);
	if (NOT_FOUND == St) sysstatus = BAD_STRING;

	St = GetFloatParam(param, "-Rr", R_r, 0);
	if (NOT_FOUND == St) sysstatus = BAD_STRING;


	St = GetFloatParam(param, "-Ls", L_s, 0);
	if (NOT_FOUND == St) sysstatus = BAD_STRING;

	St = GetFloatParam(param, "-Lr", L_r, 0);
	if (NOT_FOUND == St) sysstatus = BAD_STRING;

	St = GetFloatParam(param, "-Lm", L_m, 0);
	if (NOT_FOUND == St) sysstatus = BAD_STRING;

	St = GetFloatParam(param, "-J", J, 0);
	if (NOT_FOUND == St) sysstatus = BAD_STRING;

	string tmp;
	St = GetStringParam(param, "-saveGraph", tmp, " ");
		if (SUCCESS == St) {
			saveGraph = true;
			savedgraphnames.push_back("omega");
			savedgraphnames.push_back("M");
			savedgraphnames.push_back("Ms");
			savedGraphs = new ShowGraph(GetName(), string("t"), savedgraphnames, width, height, font,STORED_VARS);//A, B, C, N

		}

	float p_float;
	GetFloatParam(param, "-p", p_float, 2);

	p = static_cast<int>(p_float);

	GetFloatParam(param, "-Ms", staticTorque, 0);

	GetFloatParam(param, "-Tload", tload, 0);

	if (BAD_STRING == sysstatus){
		sysstatus = INIT_FAIL;
		cout << "AC motor Init Failed!" <<endl;
		cout <<"check parameters"<< endl;
	}
	else{
		sysstatus = SUCCESS;
	}

	for(int s = 0; s< 3; s++){
		savegraphs.push_back(0);
	}

		InitResistorsArray();
		InitinductanseArrays();

		NewStep(0,0.001);//Init calc!
}

AC_motor::~AC_motor() {


}

void AC_motor::InitResistorsArray(){
	R = MatrixXd::Zero(6, 6);
	R	<< 	R_s		 ,   0         ,      0      ,      0    ,     0   ,     0   ,  
			0       , R_s         ,      0      ,      0    ,     0   ,     0   ,   
			0       ,   0         ,    R_s		 ,      0    ,     0   ,     0   ,   
			0       ,   0         ,      0      ,     R_r   ,     0   ,     0   ,   
			0       ,   0         ,      0      ,      0    ,    R_r  ,     0   ,   
			0       ,   0         ,      0      ,      0    ,     0   ,    R_r     ;
}

void AC_motor::InitinductanseArrays(){
	L1 = MatrixXd::Zero(6, 6);
	L1          << L_s 			 ,  -L_m/2     ,  -L_m/2     ,     L_m    ,   -L_m/2  ,  -L_m/2 ,
                  -L_m/2     , L_s  		,  -L_m/2     ,    -L_m/2  ,    L_m    ,  -L_m/2  ,
                  -L_m/2     ,  -L_m/2     , L_s 		,    -L_m/2  ,   -L_m/2  ,   L_m    ,
                   L_m       ,  -L_m/2     ,  -L_m/2     ,    L_r      ,   -L_m/2  ,  -L_m/2  ,
                  -L_m/2     ,   L_m       ,  -L_m/2     ,    -L_m/2  ,   L_r      ,  -L_m/2  ,
                  -L_m/2     ,  -L_m/2     ,   L_m       ,    -L_m/2   ,  -L_m/2  ,    L_r    ;


	L2 = MatrixXd::Zero(6, 6);
	L2          <<      0       ,       0      ,     0        ,      0         ,        0       ,        0       ,
				      0       ,       0      ,     0        ,      0         ,        0       ,        0      ,
				      0       ,       0      ,     0        ,      0         ,        0       ,        0       ,
				      0       ,   3*L_m/2   ,  -3*L_m/2   ,      0         ,  (L_r +L_m/2) , -(L_r +L_m/2) ,
				  -3*L_m/2   ,       0      ,   3*L_m/2   , -(L_r +L_m/2) ,        0       ,  (L_r +L_m/2) ,
				   3*L_m/2   ,  -3*L_m/2   ,     0        ,  (L_r +L_m/2) , -(L_r +L_m/2) ,        0       ;


}

void AC_motor::SolveSystem(float t, float dt){

	X1 = L2*speed / sqrt(3);

	dU = U_sys - (R+X1)*I_sys;

	dI = L1.colPivHouseholderQr().solve(dU);

	dI *= dt;

	I_sys = I_sys + dI;
}


void AC_motor::CurrentTorque(){

	torque =  p * sqrt(3.0) * L_m * ((I_sys(A)*I_sys(c)+I_sys(B)*I_sys(a)+I_sys(C)*I_sys(b))-
									((I_sys(A)*I_sys(b)+I_sys(B)*I_sys(c)+I_sys(C)*I_sys(a)))) /2;

}

void AC_motor::CurrentSpeed(float t, float dt){

	float staticTorquePassive;
	float torque_after_control;

	if (tload < t){
		torque_after_control = staticTorque;
	}
	else{
		torque_after_control = 0;
	}

	if (speed >= 0){
		staticTorquePassive = torque_after_control;
	}
	else{
		staticTorquePassive = -torque_after_control;
	}

	if (true == saveGraph){
		savegraphs[2] = staticTorquePassive;

	}

	float dspeed = (torque - staticTorquePassive) * p * dt/J;

	if (speed >= 0){
		if ((speed + dspeed) >=0){
			speed +=dspeed;
		}
		else{
			speed =0;
		}
	}
	else{
		if ((speed + dspeed) <0){
			speed +=dspeed;
		}
		else{
			speed = 0;
		}
	}

}


void AC_motor::Solve(float t, float dt){
	if (SUCCESS == sysstatus){
		//copy voltage
		for (int i = 0; i < STORED_VARS; i++){
			//input voltage to output
			U_out[i] = U_in[i];
			U_sys(i) = U_out[i];
		}

		//if motor is off, Put U down
		if (OFF == CheckStatement(t)){
			for (int i = 0; i < STORED_VARS; i++){
				U_sys(i) = 0;
			}
		}
		SolveSystem(t, dt);
		CurrentTorque();
		CurrentSpeed(t, dt);

		if (true == saveGraph ){
			if ((t - t_last_show) > 0.02){
				savegraphs[0] = speed;
				savegraphs[1] = torque;
				//static torque saved in currentspeed

				savedGraphs->AddData(t, savegraphs);
				t_last_show = t;
			}
		}

		//StaticTorque is set in CurrentTorque()

		for (int i=0; i < STORED_VARS; i++){
			I_in[i] = I_sys(i);
		}

	}
	else{//motor  is bad. consumption is zero
		for (int i=0; i < STORED_VARS; i++){
			I_in[i] = 0;
		}
	}
}

void AC_motor::ShowCustomGraph(const string &style,
							status mode){

	if (true == saveGraph ){

		savedGraphs->Plot(style, mode, saveraw);

	}

}//end ShowCustomGraph
