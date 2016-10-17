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
#ifndef BOARD_H_
#define BOARD_H_

#include "typedefs.h"

#include <string>
#include <vector>

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "util/Strings.h"

/**INCLUDE ALL TYPE FILES
 *
 * */
#include "nodes/Node.h"
#include "workbench/Workbench.h"
#include "nodes/sources/ac_source/ACSource.h"
#include "nodes/consumptions/acmotor/ACmotor.h"
#include "nodes/consumptions/rlc/RLC.h"
#include "nodes/joints/analyzer/Analyzer.h"
#include "nodes/joints/transformer/Transformer.h"
#include "nodes/joints/rlc-series/rlc-series.h"

class Board{

public:
	Board(vector<parameters>& param);
	status NewStep(void);
	virtual ~Board();

	status AddConnection(vector<parameters>& param);

	status AddNode(vector<parameters>& param);

	void ShowGraph(vector<parameters> &param);

private:

	//Root workbench pointer
	Node* rootworkbench;

	//current workbench pointer
	Workbench* workbench;

	float timecurrent;
	float timestep;
	float timeend;

	vector<float> tempI;

};

#endif /* BOARD_H_ */
