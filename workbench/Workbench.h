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
#ifndef WORKBENCH_H_
#define WORKBENCH_H_

#include "typedefs.h"
#include "fabric/nodetypes.h"
#include <string>
#include <vector>
#include <stdint.h>

#include "nodes/Node.h"

#include "util/Strings.h"
#include "gnuplot_i.hpp"

///make new class from Node class (to have capability to add new workbenches)
class Workbench : public Node{

public:
	Workbench(vector<parameters>& param);

	virtual ~Workbench();

	virtual void NewStep(float t, float dt);

	///function add new node to current workbench
	status AddNode(Node* nodePtr);

	status AddWorkbench(Workbench* workbenchPtr);

	status AddConnection(vector<parameters>& param);

	void ShowGraph(vector<parameters> &param);

private:

	///array of nodes in this workbench
	vector<worknode*> nodeslist;

	///pointer for current Node
	worknode* currentnode;

	uint8_t nodeID;

};

#endif /* WORKBENCH_H_ */
