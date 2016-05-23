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

#include <workbench/Workbench.h>
#include <omp.h>


Workbench::Workbench(vector<parameters>& param ) : Node(param){

	currentnode = 0;

	nodeID = 0;

}

Workbench::~Workbench() {

}

status Workbench::AddNode(Node* nodePtr){
	//let's create new block


	if (nullptr == nodePtr){//check if a node pinter is not null

		return EMPTY_NODE;
	}

	currentnode = new worknode;

	currentnode->node = nodePtr;
	currentnode->name = nodePtr->GetName();
	currentnode->nodeid = nodeID;

	cout << "Node \"" << currentnode->name ;
	cout << "\" ptr= \"" << currentnode->node;
	cout << "\" Added to workbench \"" << GetName() << "\"" << endl;
	//partial support for multiworkbench suppot

	if (nodePtr->GetType() == "workbench"){
		currentnode->isworkbench = true;
	}
	else{
		currentnode->isworkbench = false;
	}

	nodeslist.push_back(currentnode);

	nodeID++;

	return SUCCESS;
}

status Workbench::AddWorkbench(Workbench* workbenchPtr){


	return UNSUPPORTED_COMMAND;


}

status Workbench::AddConnection(vector<parameters>& param){

	string inputname = " ";
	string outputname = " ";

	Node* inputnodePtr = 0;
	Node* outputnodePtr = 0;


	PrintParameters(param);

	//find parameters

	status St = GetStringParam(param, "-input", inputname, "none");
	if (St !=SUCCESS){

		return BAD_STRING;

	}

	St = GetStringParam(param, "-output", outputname, "none");
	if (St !=SUCCESS){

		return BAD_STRING;

	}

	for (auto obj : nodeslist){

		if (inputname == (obj->name)){

			inputnodePtr = obj->node;
			cout << "input node!  ptr = " << inputnodePtr << endl;
		}

		if (outputname == (obj->name)){

			outputnodePtr = obj->node;
			cout << "output node!  ptr = " << outputnodePtr << endl;
		}

	}

	if ((outputnodePtr != nullptr) && (inputnodePtr != nullptr)){

		outputnodePtr->AddLink(inputnodePtr, param);

		inputnodePtr->AddLink(outputnodePtr, param);

		return SUCCESS;

	}

	return BAD_STRING;
}

void Workbench::NewStep(float t, float dt){

	for (auto obj : nodeslist){
		obj->node->UpdateState(t,dt);
	}

	int nodescount = nodeslist.size();
#pragma omp parallel for 
	for (int it = 0; it < nodescount; ++it) {
			nodeslist[it]->node->NewStep(t, dt);
		}
}


void Workbench::ShowGraph(vector<parameters> &param){
	string tmp;
	status mode = 0;
	string style;

	GetStringParam(param, "-control", tmp, "none");

	GetStringParam(param, "-style", style, "lines");

	if (tmp == "wait"){
		mode = WAIT;
	}else{
		mode = SAVE_GRAPH;
	}

	for (auto iter : nodeslist){
		iter->node->Plot(style, mode);
	}
}

