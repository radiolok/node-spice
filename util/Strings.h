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
#ifndef STRINGS_H_
#define STRINGS_H_

#include <vector>
#include <stdio.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "typedefs.h"

	vector<parameters>::iterator FindParam(vector<parameters>& v,string param);
	status GetFloatParam(vector<parameters>& v,string p, float& result, float def);
	status GetStringParam(vector<parameters>& v,string p, string& result, string def);

	bool GetBoolParam(vector<parameters>& v,const string p);
	bool GetBoolParam(vector<parameters>& v,const string p, bool &ret);
	void PrintVec(string s, vector<float> v);

	void PrintStrVec(string name, vector<string> vec);

	#endif /* STRINGS_H_ */
