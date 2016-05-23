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
#ifndef CONSUMPTION_H_
#define CONSUMPTION_H_

#include <nodes/Node.h>

class Consumption : public Node{

public:
	Consumption(vector<parameters>& param): Node(param){};

	virtual ~Consumption();

	virtual void UpdateOutput(void);


};

#endif /* CONSUMPTION_H_ */
