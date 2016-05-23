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
#ifndef NODES_H_
#define NODES_H_

#include <vector>
#include "nodes/Node.h"
#include "workbench/Workbench.h"
#include <string>
#include <stdint.h>

struct worknode{

	uint8_t nodeid;
	bool isworkbench;
	Node* node;//pointer to current node
	string name;//we collect it in the node, but copy it here to simplify search

};


struct Comparator {
   const char* expected_name;

   Comparator(const char* _expected_name)
     : expected_name(_expected_name)
   {}

   bool operator()(const worknode* node) const
   { return !strcmp(node->name.c_str(), expected_name); } // Just an example using strcmp
};

#endif /* NODES_H_ */
