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

#include "Numeric.h"


float Abs(float val){
	if (val >= 0){
		return val;
	}
	else{
		return -val;
	}
}


float sign(float val){
	if (val > 0){
		return 1;
	}
	else if (val < 0){
		return -1;
	}
	else{
		return 0;
	}
}


int isPowerOfTwo (unsigned int x)
{
 while (((x & 1) == 0) && x > 1) /* While x is even and > 1 */
   x >>= 1;
 return (x == 1);
}

inline void VectorsCopy(std::vector<float> &from, std::vector<float> &to, size_t length){
	for (size_t i = 0; i < length; ++i){
		to[i] = from[i];
	}
}


