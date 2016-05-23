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
#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <string>
#include <vector>

#include <math.h>

typedef int status;

using namespace std;

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

//#define PI M_PI
//#define TWOPI M_PI_2

//TODO: change all places in code to phases enumerator
#define STORED_VARS 3 //DO NOT EDIT! magic number 3 exist in code!

struct parameters {
	string param;
	vector<string> elements;
};

enum edgetypes{
	undefinededge,
	risingedge,
	fallingedge
};

enum phases {
		N,
		A,
		B,
		C};


enum StatusTypes{
		OFF,
		ON,
		DATA,
		WAIT,
		NONE,
		FINISH,
		SUCCESS,
		NO_FILE,
		COMMAND,
		CONTINUE,
		END_CALC,
		BAD_FILE,
		INIT_FAIL,
		UNDEFINED,
		NOT_FOUND,
		PARAMETER,
		BAD_STRING,
		SAVE_GRAPH,
		EMPTY_NODE,
		UNSUPPORTED,
		EMPTY_STRING,
		NOT_A_COMMAND,
		UNSUPPORTED_COMMAND,
		UNSUPPORTED_DEVICE,
		WORKBENCH_ERROR
};


#endif /* TYPEDEFS_H_ */
