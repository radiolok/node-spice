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
#ifndef SHOWGRAPH_H_
#define SHOWGRAPH_H_

#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iterator>
#include "gnuplot_i.hpp"

#include "typedefs.h"


using namespace std;

class ShowGraph {
public:

	ShowGraph(const string& name,
			const string& _xlabel,
			vector<string>& _ylabel,
			const string& _width,
			const string& _height,
			const string& _font,
			int channels);

	///init graph class with number of channels
	ShowGraph(const string& name,
			const string& _xlabel,
			vector<string>& _ylabel,
			const string& _width,
			const string& _height,
			const string& _font,
			int channels,
			bool collect,
			float scale);

	virtual ~ShowGraph();

	///add new data
	void AddData(int channel, float timestamp, double data);

	///add new data for all channels
	void AddData(float timestamp, vector<double> inputdata);

	double Last(int channel);

	void Plot(const string &style,
			status mode, bool saveraw);

private:

	unsigned int numberofchannels;
	//need to collect all channels to one graph
	bool collectchannels;
	//scaling factor of graph sum
	float collectscale;

	vector<double> time;
	vector<double>* data;

	string width;

	string height;

	string font;

	ostringstream graphname;

	string xlabel;
	vector<string> ylabel;

	Gnuplot* graph;

};

#endif /* SHOWGRAPH_H_ */
