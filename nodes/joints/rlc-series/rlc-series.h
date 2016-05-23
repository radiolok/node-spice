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
#ifndef NODES_JOINTS_RLCSERIES_RLCSERIES_H_
#define NODES_JOINTS_RLCSERIES_RLCSERIES_H_


#include "typedefs.h"
#include <string>
#include <vector>

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "util/Strings.h"


#include "util/ShowGraph.h"

#include "nodes/Node.h"


class RLCseries : public Node {
public:
	RLCseries(vector<parameters>& param);
	virtual ~RLCseries();

protected:

	void Solve(float t, float dt);

private:
	ShowGraph* GraphVoltagePhaseAMomentary;
	ShowGraph* GraphCurrentPhaseAMomentary;
	void ShowCustomGraph(const string &style,status mode);

		vector<float> R;
		vector<float> L;
		vector<float> C1;
		vector<float> C2;
		vector<float> I_L_old;
		vector<float> U_C_old;
		status loadstatus;

};

#endif /* NODES_JOINTS_ANALYZER_ANALYZER_H_ */
