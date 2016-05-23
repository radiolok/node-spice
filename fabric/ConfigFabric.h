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


#ifndef CONFIGFABRIC_H_
#define CONFIGFABRIC_H_


#include "../typedefs.h"

#include <string>
#include <vector>

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "../util/Strings.h"

/**INCLUDE ALL TYPE FILES
 *
 * */

#include "board/Board.h"


using namespace std;

class Joint;

class ConfigFabric {
public:

	ConfigFabric(int argc, char** argv);
	virtual ~ConfigFabric();

	Board* GetBoardPtr(void) {return boardPtr;};

private:

	status ParseFile(char* filepath);
	void PrepareString(string& line);
	status ParseLine(string& line, vector<string>& commandline);
	vector<string> split(string str, string delim);

	///statr's parsing a command
	status PrepareCommand(vector<string>& cmdblock, string& command, vector<parameters>& param);

	status IsACommand(string line);

	status ParseCommand(string& cmd, vector<parameters>& param);

	Board* boardPtr;

	char** _argv;
	int _argc;
};

#endif /* CONFIGFABRIC_H_ */
