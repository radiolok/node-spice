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
#ifndef RLC_H_
#define RLC_H_

#include "typedefs.h"
#include <string>
#include <vector>

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <util/Strings.h>

#include <nodes/consumptions/Consumption.h>

class RLC : public Consumption{

public:
	virtual ~RLC();
	RLC(vector<parameters>& param);

protected:
		///Do new calculation
	void Solve(float t, float dt);

private:

	vector<float> R;
	vector<float> L;
	vector<float> C;
	vector<float> I_L_old;
	status loadstatus;

};

#endif /* RLC_H_ */
