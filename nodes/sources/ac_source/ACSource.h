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
#ifndef ACSOURCE_H_
#define ACSOURCE_H_

#include "typedefs.h"
#include <string>
#include <vector>
#include <math.h>

#include "nodes/Node.h"

#include "util/Strings.h"
#include "gnuplot_i.hpp"

class ACSource : public Node {

public:

	ACSource(vector<parameters>& param);

	virtual ~ACSource();

protected:
	///Do new calculation
	virtual void Solve(float t, float dt);

private:

	float f;
	float omega;

	//TODO different voltages by vector
	float Ua;

	vector<float> R;

	vector<float> L;
	float phi;
};

#endif /* ACSOURCE_H_ */
