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

#ifndef TRANSFORMER_H_
#define TRANSFORMER_H_

#include "nodes/Node.h"

#include "typedefs.h"
#include <string>
#include <vector>
#include <inttypes.h>

#include "util/Numeric.h"


#include <math.h>


class Transformer : public Node{
public:
	Transformer(vector<parameters> &param);
	virtual ~Transformer();

protected:

	virtual void Solve(float t, float dt);

private:

	///Параметр кривой намагничивания , по умолчанию =70
	float a;

	///Параметр кривой намагничивания, по умолчанию = 6
	float b;

	///параметр кривой намагничивания. По умолчанию = 10
	float sigma;

	///индукция насыщения, по умолчанию 1,73
	float Bs;

	///длина сердечника, м. по умолчанию 1м.
	float ls;

	//длина ярма, м, по умолчанию 1м.
	float ly;

	///Сопротивление первичной обмотки трансформатора, Ом, по умолчанию 100
	float R0;

	///площадь поперечного сечения сердечника м^2, по умолчанию 1000
	float S;

	///число витков первичной обмотки, по умолчанию 1000
	float N1;

	///коэффициент трансформации, по умолчанию 1
	float k;


	vector<float> B;

	float H(float _B);
	void newB(void);
};

#endif /* TRANSFORMER_H_ */
