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
#include "Transformer.h"



Transformer::Transformer(vector<parameters> &param):Node(param) {

	/*
	 * B-H curve parameters
	 * */
	GetFloatParam(param, "-a", a, 70);

	GetFloatParam(param, "-b", b, 6);

	GetFloatParam(param, "-sigma", sigma, 10);

	GetFloatParam(param, "-Bs", Bs, 1.73);

	GetFloatParam(param, "-R0", R0, 100);

	GetFloatParam(param, "-S", S, 1000);

	GetFloatParam(param, "-ls", ls, 1);

	GetFloatParam(param, "-ly", ly, 1);

	GetFloatParam(param, "-N1", N1, 1);

	GetFloatParam(param, "-k", k, 1);

	for (uint8_t i=0; i < STORED_VARS; i++){

		B.push_back(0);

	}

}

Transformer::~Transformer() {

}



void Transformer::newB(void){

	float dB = 0;

	 dB = (R0 * (2 * I_0[1] + I_0[2])/3 + U_in[0] - U_in[1])/(S * N1);
	 B[0] += dB;

	 dB = (R0 * (2 * I_0[2] + I_0[0])/3 + U_in[1] - U_in[2])/(S * N1);
	 B[1] += dB;

	 dB = (R0 * (2 * I_0[0] + I_0[1])/3 + U_in[2] - U_in[0])/(S * N1);
	 B[2] += dB;
}

float Transformer::H(float _B){
	float H = 0.;

	if (Abs(_B)<Bs){
		H = a*_B + b*pow(_B,7);
	}
	else{
		H = a*_B + b*pow(_B, 7) +sign(_B)*(pow((Abs(_B)-Bs),2))/((4* M_PI * 1e-7)*(sigma + Abs(_B)-Bs));
	}

	return H;
}

void Transformer::Solve(float t, float dt){

	for (uint8_t i=0; i < STORED_VARS; i++){

		//take transformer coefficient
		U_out[i] = U_in[i] * k;
		I_in[i] = I_out[i] * k;

	}

}


