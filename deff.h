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

#pragma once


#ifndef DEFF_H_
#define DEFF_H_


using namespace std;
#include <stdio.h>

#if defined(WIN32)
#include <tchar.h>

#endif

#include <fstream>
#include <iostream>
#include <sstream>      // std::ostringstream
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdint.h>

#include "gnuplot_i.hpp"

#include "typedefs.h"

#include "util/Strings.h"
#include "fabric/ConfigFabric.h"

//base class
#include "nodes/Node.h"
#include "nodes/sources/ac_source/ACSource.h"
#include "nodes/consumptions/acmotor/ACmotor.h"
#include "nodes/consumptions/rlc/RLC.h"
#include "nodes/joints/analyzer/Analyzer.h"
#include "board/Board.h"



#endif
