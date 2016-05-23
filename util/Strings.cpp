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
#include "Strings.h"

vector<parameters>::iterator FindParam(vector<parameters>& v,const string param){
	for (auto it = v.begin(); it<v.end(); ++it){
		if (param == (it->param)){
			return it;
		}
	}
		return v.end();
}

status GetFloatParam(vector<parameters>& v, const string p, float& result, float def){
	string tmp = " ";

	status St = GetStringParam(v, p, tmp, " ");
	if (SUCCESS == St){
		result = atof(tmp.c_str());
	}
	else{
		result = def;
		}
	return St;

}

status GetStringParam(vector<parameters>& v,const string p, string& result, const string def){
	status St;
	auto it = FindParam(v,p);
	if (it != v.end()){//find!
		if (it->elements.begin() != it->elements.end()){
			result = it->elements.front();
		}
		else{
			result = def;
		}
		St = SUCCESS;
	}
	else{
		result = def;
		St = NOT_FOUND;
	}
	return St;
}

bool GetBoolParam(vector<parameters>& v,const string p, bool &ret){
	ret = false;
	if (FindParam(v,p) != v.end()){//find!
		ret = true;
	}
	return ret;
}


bool GetBoolParam(vector<parameters>& v,const string p){
	bool St = false;
	if (FindParam(v,p) != v.end()){//find!
		St = true;
	}
	return St;
}

void PrintVec(const string s,const  vector<float> v){
	cout << s ;
	for (auto it : v){
			cout << "\t" << it ;
		}
	cout << endl;
}

void PrintStrVec(const string name, const vector<string> vec){
	cout << name << '\n';
	for (auto it : vec)
	 	  cout << ' ' << it << '\n';
	cout << '\n';
}

	/*For human-looklike values convert this code from js to C++
	 *   //------------------------------------------------------------------------------------------------------------------
    Helper:
    {
        idCounter: 0,
        getUniqueId: function() {  this.idCounter++; return "bsid"+this.idCounter; },


        strToNumber: function(str) {
            str = str.toString();
            str = str.toLowerCase();
            var myArray = str.match(/([-0-9\.,]+)(g|meg|k|m|u|n|p)?/);

            if (myArray == null) return str;
            var val = parseFloat(myArray[1]);

            if (myArray[2] != undefined)
            {
                var multiplier = myArray[2];

                switch(multiplier)
                {
                    case "p": val *= 1E-12; break;
                    case "n": val *= 1E-9; break;
                    case "u": val *= 1E-6; break;
                    case "m": val *= 1E-3; break;
                    case "k": val *= 1E3; break;
                    case "meg": val *= 1E6; break;
                    case "g": val *= 1E9; break;
                }
            }
            return val;
        },


        numberToStr: function(num) {
            var str = "";

            if ( num >= 1E9 	) { num /= 1E9; str = "G"; }
            else if ( num >= 1E6 	) { num /= 1E6; str = "Meg"; }
            else if ( num >=  1E3   ) { num /= 1E3;	str = "k";   }
            else if ( num >=  1     ) { }
            else if ( num >=  1E-3  ) { num /= 1E-3; str = "m"; }
            else if ( num >=  1E-6  ) { num /= 1E-6; str = "u"; }
            else if ( num >=  1E-9  ) { num /= 1E-9; str = "n";  }
            else if ( num >=  1E-12 )  { num /= 1E-12; str = "p";  }
            str = num.toFixed(2) + str;
            return str;
        },
	 *
	 *
	 *
	 *
	 * */
