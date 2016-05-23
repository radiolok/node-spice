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
#include "ShowGraph.h"

ShowGraph::ShowGraph(const string& name,
					const string& _xlabel,
					vector<string>& _ylabel,
					const string& _width,
					const string& _height,
					const string& _font,
					int channels) {

	data = new vector<double>[channels];
	numberofchannels =  channels;

	graphname << name << "_"<< this;

	graph = nullptr;

	xlabel = _xlabel;
	ylabel = _ylabel;
	width = _width;
	height = _height;
	font = _font;
	//default collecting factors
	collectchannels = false;
	collectscale = 1.0;

}

ShowGraph::ShowGraph(const string& name,
					const string& _xlabel,
					vector<string>& _ylabel,
					const string& _width,
					const string& _height,
					const string& _font,
					int channels,
					bool collect,
					float scale) {

	data = new vector<double>[channels];
	numberofchannels =  channels;

	graphname << name << "_" << this;

	graph = nullptr;

	xlabel = _xlabel;
	ylabel = _ylabel;
	width = _width;
	height = _height;
	font = _font;
	collectchannels = collect;
	collectscale = scale;

}

ShowGraph::~ShowGraph() {
	delete[] data;
}

void ShowGraph::AddData(int channel, float timestamp, double inputdata){
	if (0 == channel){
		time.push_back(timestamp);
	}
	data[channel].push_back(inputdata);

}

void ShowGraph::AddData(float timestamp,const vector<double> inputdata){

	time.push_back(timestamp);

	if (inputdata.size() != numberofchannels){
		return;
	}
	for (unsigned int s = 0; s < numberofchannels; s++){
		data[s].push_back(inputdata[s]);
	}
}

double ShowGraph::Last(int channel)
{
	if (data[channel].empty() == true)
	{
		return 0;
	}
	return data[channel].back();
}


void ShowGraph::Plot(const string &style,
					status mode, bool saveraw){
	if (!data[0].size()){
		cout << graphname.str() << " no data" << endl;
		return;
	}
	graph = new Gnuplot(graphname.str());
	graph->SaveRawFile(saveraw);
	graph->set_style(style);
	graph->set_xlabel(graphname.str()+",  t");
	graph->show_grid();

	unsigned int channelstoplot = numberofchannels;
	if ((collectchannels) && (numberofchannels > 1)){

		channelstoplot = 1;
		for (decltype(data[0].size()) i = 0; i < data[0].size(); i++){
			for (unsigned int s = 1; s < numberofchannels; s++){
				data[0][i] += data[s][i];
			}
			if (collectscale){
				data[0][i] /= collectscale;
			}
			else{
				data[0][i] /= numberofchannels;
			}
		}

	}
	//find min and max values:
	double ymin = 0;
	double ymax = 0;
	for (unsigned int s=0; s < channelstoplot; s++)
	{
		for (auto iter : data[s])
		{
			if (ymin > iter)
			{
				ymin = iter;
			}
			if (ymax < iter)
			{
				ymax = iter;
			}
		}
	}
	if (ymin < 0)
	{
		ymin  = 1.2 * ymin;
	}
	else
	{
		ymin = 0;
	}

	if (ymax > 0)
	{
		ymax = 1.2 * ymax;
	}
	else
	{
		ymax = 0;
	}
	if (ymin == ymax){
		if (ymin == 0){
			ymax = 1;
		}
		else{
			ymin = ymin - 0.5;
			ymax = ymax * 1.2;
		}
	}
	graph->set_yrange(ymin, ymax);
	for (unsigned int i = 0; i < channelstoplot; i++){
		graph->plot_xy(time, data[i], ylabel[i]);
	}
	switch (mode){
	case WAIT:
		cout << "Press enter to continue" << endl;
		while (getchar() != '\n'){}
		break;
	case SAVE_GRAPH:

		cout << "save graph " << graphname.str() << endl;
		graph->savePNG(width, height, font, 1, graphname.str());

		break;
	}
}
