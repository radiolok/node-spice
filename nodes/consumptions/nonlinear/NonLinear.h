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
#ifndef NONLINEAR_H_
#define NONLINEAR_H_

#include "typedefs.h"

#include <string>
#include <vector>

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <util/Strings.h>

#include <nodes/consumptions/Consumption.h>

class NonLinear :public Consumption {
public:

	NonLinear(vector<parameters>& param);
	virtual ~NonLinear();


protected:
	///Do new calculation
	void Solve(float t, float dt);

private:


};

#endif /* NONLINEAR_H_ */
