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
#ifndef NODE_H_
#define NODE_H_


#include "typedefs.h"
#include <cstdint>
#include "util/Strings.h"
#include <string>
#include <vector>
#include <iostream>

#include "util/ShowGraph.h"

using namespace std;

class Node {
public:

	///Base constructor.
	Node(vector<parameters>& param);

	///Function takes current time and timestep and do one calculate iteration
	///this function is NOT virtual.
	void UpdateState(float t, float dt);
	virtual void NewStep(float t, float dt);

	status GetStatus(void){return SUCCESS;};

	///Return reference to vector with voltage in every phase
	virtual vector<float> &GetVoltage(void){return U_out;};

	///Return reference to vector with voltage in every phase
	virtual vector<float> &GetCurrent(void){return I_in;};

	///Set voltage in every phase
	virtual void SetVoltage(vector<float> u){};

	///Set current in every phase
	virtual void SetCurrent(vector<float> i){};

	virtual ~Node();

	string GetName(void){return name;};

	status AddLink(Node* nodeptr, vector<parameters>& param);

	string GetType(void){return type;};

	void Plot(const string &style,
			status mode);

protected:

	status CheckStatement(float t);

	void PrintParameters(vector<parameters>& param);
//overloaded function, which is consist core
//	mathematic function
	virtual void Solve(float t, float dt){};

	vector<float> U_out;

	///line old U var
	vector<float> U_out_old;

	vector<float> U_in;

	vector<float> U_in_old;

	///line current var
	vector<float> I_out;
	///line old var
	vector<float> I_out_old;

	vector<float> I_in;
	vector<float> I_in_old;

	vector<float> I_0;

	vector<float> tempI;

	float GetOffTime(void){return timeoff;};

	/*Graph Section
	 *
	 * */
	virtual void SaveBaseGraphData(float t);

	virtual void SaveCustomGraphData(float t){return;};

	virtual void ShowBaseGraphs(const string &style,
						status mode);

	virtual void ShowCustomGraph(const string &style,
								status mode){return;};

	string width;

	string height;

	string font;

	string fontsize;

	bool saveraw;

	/* Nodes connection section
	 *
	 * */

	virtual void UpdateInput(void);

	virtual void UpdateOutput(void);

	vector<Node* > inputnodesPtr;

	vector<Node* > outputnodesPtr;

	enum phases {N,A,B,C};

private:


	float timeon;
	float timeoff;

	string name;

	///nodetype
	string type;

	///store max value. after it node failed
	float Imax;
};

#endif /* NODE_H_ */
