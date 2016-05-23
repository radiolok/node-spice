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

#include "ConfigFabric.h"


ConfigFabric::ConfigFabric(int argc,char** argv):_argv(argv),_argc(argc) {

	//reset all pointers
	boardPtr = 0;
	//search fo file path - any other types are prohibited:
	int i=0;
	while ( i < argc){//we need next after -f parameter
		if (!strcmp("-f", argv[i++])){
			break;
		}
	}
	//TODO: rewrite this spagetti code to normal type
	ParseFile(argv[i]);
}

ConfigFabric::~ConfigFabric() {
	//
}



/********************************************************************************
 *
 * FILE PARSING SECTION
 *
 * *****************************************************************************/
status ConfigFabric::ParseFile(char* filepath){
	ifstream inputfile(filepath);
	string line;
	status parse;
	vector<string> vectorline;
	if (inputfile.good()){//if our file is good
		cout << "File "<< filepath <<" opened!" << endl;
		while (getline( inputfile, line)){//get line
			string cmd;
			vector<parameters> prmelem;
			parse = ParseLine(line, vectorline);
			switch (parse){
			case SUCCESS:
				parse = PrepareCommand(vectorline,cmd,prmelem);
				if ((SUCCESS == parse) || (EMPTY_STRING == parse)){
					status parse = ParseCommand(cmd,prmelem);
					if (SUCCESS!=parse){
						continue;
					}
				}
				else{
					continue;
				}


				break;
			//TODO make some new parser features enabled
			default:
				continue;
				break;
			}
		}
	}
	else{
		cout << "File not found, exit"<< endl;
		return NO_FILE;
	}
	return SUCCESS;
}

vector<string> ConfigFabric::split(string str, string delim){
      unsigned start = 0;
      unsigned end = str.length();
      unsigned pos = 0;
      vector<string> v;
      while( (pos = str.find(delim, start)) < end )
      {
            v.push_back(str.substr(start, pos-start));
            start = pos + delim.length();
      }
      v.push_back(str.substr(start));
      return v;
}

void ConfigFabric::PrepareString(string& line){
	//delete commentary:
	size_t start = line.find("/");
	if (start != string::npos){
		line.erase(line.begin()+start,line.end());
	}
	char chars[] = "\"\r";
	unsigned int chars_quantity = strlen(chars);
   for (unsigned int i = 0; i < chars_quantity; ++i)
   {
	  // you need include <algorithm> to use general algorithms like std::remove()
	  line.erase (remove(line.begin(), line.end(), chars[i]), line.end());
   }
}

status  ConfigFabric::ParseLine(string& line, vector<string>& commandline){
	if (line.empty()){
		return EMPTY_STRING;
	}
	cout << "Line: " <<  line << endl;
	PrepareString(line);
	if (line.empty()){
		return EMPTY_STRING;
	}
	string separate = " "; //separate line by spaces
	commandline = split(line,separate);
	return SUCCESS;
}


/************************************************************************
 *
 * COMMAND PARSING SECTION
 *
 ***********************************************************************/

status ConfigFabric::PrepareCommand(vector<string>& cmdblock, string& command, vector<parameters>& params){

	int paramid=-1;

	unsigned statetype=UNSUPPORTED;

	//check all elements in vector

	for (auto cmdptr : cmdblock){

		if (cmdptr == cmdblock.front()){
			command = cmdptr;
			continue;
		}

		parameters block;
		if (SUCCESS == IsACommand(cmdptr)){//we work with a command
			paramid++;//for first loop it's a zero
			params.push_back(block);
			params[paramid].param = cmdptr;//copy  command to string
			statetype = COMMAND;
		}
		else{//not a  command - parameter?

			if (UNSUPPORTED == statetype){//non supported

				continue;
			}
			else{
				params[paramid].elements.push_back(cmdptr);//collect all parameters in vector
				statetype = PARAMETER;
			}
		}

	}

	if (0 > paramid){//we has no input data, return

		return EMPTY_STRING;

	}
	return SUCCESS;
}

status ConfigFabric::IsACommand(string line){
	size_t found = line.find("-",0,1);
	if (found!=std::string::npos){
		return SUCCESS;
	}
	else{
		return NOT_A_COMMAND;
	}
}

status ConfigFabric::ParseCommand(string& cmd, vector<parameters>& params){

	status St= SUCCESS;
	cout << "cmd: >" << cmd << "<"<< endl;
	if ("setup"==cmd){

		boardPtr = new Board(params);
		return SUCCESS;

	}
	else{
		//nothing
	}

	if (boardPtr != nullptr){
		if ("load" == cmd){//create new load element
			St = boardPtr->AddNode(params);
			return St;
		}
		else {
			if ("link" == cmd){
				St = boardPtr->AddConnection(params);
				return St;

			}
			else {
				if ("solve" == cmd){//start calc!
					St = boardPtr->NewStep();
					return St;

				}
				else {
					if ("graph" == cmd){//show graphs!
						boardPtr->ShowGraph(params);
						return St;
					}
					else{
						cout << "UNSUPPORTED COMMAND" <<endl;
						St =  UNSUPPORTED_COMMAND;

					}
				}
			}
		}
	}
	else{
		cerr << "No setup command defined" << endl;
	}

	return UNSUPPORTED_COMMAND;

}

