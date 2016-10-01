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
#ifndef NUMERIC_H_
#define NUMERIC_H_

#include <vector>
#include <stddef.h>



///return abs value for float data
float Abs(float val);


///return 1 if val greather than zero, -1 if below zero and 0 if zero
float sign(float val);

int isPowerOfTwo (unsigned int x);

inline void VectorsCopy(std::vector<float> &from, std::vector<float> &to, size_t length);

#endif /* NUMERIC_H_ */
