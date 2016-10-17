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

#include "board/Board.h"

Board::Board(vector<parameters>& param) {

	//Reset Pointers
	rootworkbench = nullptr;
	workbench = nullptr;



	status state =  GetFloatParam(param, "-dt", timestep, 1/8192);

	if (state == NOT_FOUND){
		float frequency = 0;
		state =  GetFloatParam(param, "-f", frequency, 8192);
		timestep = 1 / frequency;
	}
	GetFloatParam(param, "-Off", timeend, 1);


	timecurrent = timestep;

	for (uint8_t s = 0; s < STORED_VARS ; s++){

		tempI.push_back(0);

	}

	cout << "Time Setup! Tmax = " << timeend << " c, dt = " << timestep << endl;
}

Board::~Board() {

}

///BLOCK CONFIGURATION SECTION
status Board::AddNode(vector<parameters>& param){

	//check, that we have current workbench
	string sourcetype;

	GetStringParam(param, "-t", sourcetype, "none");

	//check, has we normal workbench or not
	if (workbench == nullptr){

		if ("workbench" == sourcetype){

			workbench = new Workbench(param);
			rootworkbench = workbench;

		}
		return SUCCESS;
	}
	if ("acsource" == sourcetype){

		workbench->AddNode(new ACSource(param));

	}
	else if ("motor" == sourcetype){

		workbench->AddNode(new AC_motor(param));

	}
	else if ("rlc" == sourcetype){

		workbench->AddNode(new RLC(param));

	}
	else if ("analyzer" == sourcetype){

		workbench->AddNode(new Analyzer(param));

	}
	else if ("rlc_series" == sourcetype){

		workbench->AddNode(new RLCseries(param));
	}

	else if ("transformer" == sourcetype){

		workbench->AddNode(new Transformer(param));

	}
	else{
		cout << sourcetype <<" UNSUPPORTED" << endl;
		return UNSUPPORTED_DEVICE;

	}

		return SUCCESS;

}

void Board::ShowGraph(vector<parameters> &param){

	workbench->ShowGraph(param);

}

status Board::AddConnection(vector<parameters>& param){

	workbench->AddConnection(param);

	return SUCCESS;
}


status Board::NewStep(void){

	status St= SUCCESS;
	while (timecurrent < timeend){
		workbench->NewStep(timecurrent, timestep);

		St = workbench->GetStatus();

		timecurrent +=timestep;
	}

	return St;

}

