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
#include "nodes/Node.h"

Node::Node(vector<parameters>& param) {


	GetStringParam(param, "-name", name, "noname");

	//on and off time
	GetFloatParam(param, "-On", timeon, 0);

	GetFloatParam(param, "-Off", timeoff, 0);

	//store type in string variable
	GetStringParam(param, "-t", type, "-");

	GetFloatParam(param, "-Imax", Imax, 0);

	GetStringParam(param, "-width", width, "800");

	GetStringParam(param, "-height", height, "600");

	GetStringParam(param, "-font", font, "arial,10");
	
	GetBoolParam(param, "-raw", saveraw);


	//init vectors
	for (uint8_t i=0; i < STORED_VARS; i++){


		I_0.push_back(0);
		tempI.push_back(0);

		U_out.push_back(0);
		U_out_old.push_back(0);

		U_in.push_back(0);
		U_in_old.push_back(0);

		I_in.push_back(0);
		I_in_old.push_back(0);

		I_out.push_back(0);
		I_out_old.push_back(0);

	}

	PrintParameters(param);

}

status Node::AddLink(Node* nodeptr, vector<parameters>& param){

	string linkname;
	//check input connections
	status St = GetStringParam(param, "-input", linkname, "empty");//when we create node, default name is noname

	if (SUCCESS != St){
		//-input is requred command!
		return UNSUPPORTED_COMMAND;

	}

	if (linkname == name){//store pointer to input array

		//just only store pointer and return
		inputnodesPtr.push_back(nodeptr);
		cout << GetName() << ": inputnodesPtr<-" <<inputnodesPtr.back() <<endl;
		return SUCCESS;
	}

	//check for output connections
	St = GetStringParam(param, "-output", linkname, "empty");//when we create node, default name is noname

	if (SUCCESS != St){

		return UNSUPPORTED_COMMAND;

	}

	if (linkname == name){//store pointer to output array

		//just only store pointer and return
		outputnodesPtr.push_back(nodeptr);
		cout << GetName() << ": outputnodesPtr<-" <<outputnodesPtr.back() <<endl;
		return SUCCESS;
	}

	return UNSUPPORTED_COMMAND;
}


void Node::PrintParameters(vector<parameters>& param){
//We need to print all parameters:
	vector<string> elements;
	for (auto it = param.begin(); it < param.end();	++it){
		cout << it->param << "\t";
		elements = it->elements;
		for (auto jt = elements.begin();
				jt < elements.end();
				++jt){
			cout << jt->c_str() << "\t";
		}
		cout << endl;
	}
}


status Node::CheckStatement(float t){
	//MAGIC! Do NOT Touch and simplify!
	status workstate=OFF;

	if (timeon == timeoff){
		workstate = ON;
	}
	else{
		if (timeon < timeoff){
			if ((t > timeon) && (t < timeoff)){
				workstate = ON;
			}
			else{
				workstate = OFF;
			}
		}
		else{
			if ((t < timeon)&& (t > timeoff)){
				workstate = OFF;
			}
			else{
				workstate = ON;
			}
		}
	}
	return workstate;
}

void Node::UpdateState(float t, float dt){

	UpdateOutput();

	UpdateInput();

}

void Node::NewStep(float t, float dt){

	//cout << GetName() << endl;
	if (!CheckStatement(t)){
		for (int s=0; s<STORED_VARS;s++){
			U_in[s] = 0;
		}
	}

	Solve(t, dt);

	if (!CheckStatement(t)){
		for (int s=0; s<STORED_VARS;s++){
			I_in[s] = 0;
		}
	}

	SaveBaseGraphData(t);
	for (int s = 0; s<STORED_VARS; s++){
		U_in_old[s] = U_in[s];
		U_out_old[s] = U_out[s];
		I_out_old[s] = I_out[s];
		I_in_old[s] = I_in[s];
	}
}


void Node::SaveBaseGraphData(float t){

	SaveCustomGraphData(t);

}

void Node::UpdateInput(void){

	for (auto obj : inputnodesPtr){
		//TODO ADD multiple input voltage support
		U_in = obj->GetVoltage();

	}
}

void Node::UpdateOutput(void){
	for (int s=0; s<STORED_VARS;s++){
		I_out[s] = 0;
	}
	for (auto obj : outputnodesPtr){
		tempI = obj->GetCurrent();
		for (uint8_t i = 0; i < STORED_VARS; i++){
			I_out[i] += tempI[i];
		}
	}
}

void Node::Plot(const string &style, status mode){
	ShowBaseGraphs(style, mode);
}

void Node::ShowBaseGraphs(const string &style,
					status mode){

	ShowCustomGraph(style, mode);

}

Node::~Node() {

}

