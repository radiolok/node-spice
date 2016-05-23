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
#include "Analyzer.h"

Analyzer::Analyzer(vector<parameters>& param): Node(param) {

		string tmp;

		//RMS sliding mode
		t_old = 0;
		rmsinit = 0;
		rmsitercount = 0;

		savedt = new vector<double>;

		savedU = new vector<double>[STORED_VARS];

		savedI = new vector<double>[STORED_VARS];

		U_array = new vector<double>[STORED_VARS];

		I_array = new vector<double>[STORED_VARS];

		GraphCurrentRMS = nullptr;
		GraphVoltageRMS = nullptr;
		GraphActivePowerRMS = nullptr;
		GraphReactivePowerRMS = nullptr;
		GraphPowerFactorRMS = nullptr;
		GraphFullPowerRMS = nullptr;
		GraphActiveEnergyRMS = nullptr;
		GraphActiveResistanceRMS = nullptr;
		GraphReactiveResistanceRMS = nullptr;
		GraphActiveResistanceDerivativeRMS = nullptr;
		GraphReactiveResistanceDerivativeRMS = nullptr;

		int ringBuffer_Start = 0;
		int ringBuffer_End = 0;

		graphchannels = STORED_VARS;

		//set RMS sample time for graphs. Default is 1 second
		GetFloatParam(param, "-tRMS", rmsperiod, 1);
		if (rmsperiod > 1)
		{
			rmsperiod = 1;
		}
		float ftmp = 0;
		if (GetFloatParam(param, "-fs", ftmp, 0) == SUCCESS){
			t_RMS_counting  = 1/ftmp;
		}
		else{
			t_RMS_counting  = 0;
		}
		t_RMS_last_count = 0;

		GetBoolParam(param, "-Collect", collectgraph);
		//set nominal RMS Voltage for voltage drop definition
		GetFloatParam(param, "-Unom", Unominal, 220);

		if (GetBoolParam(param, "-U", saveU)){//need to  store U
			if (collectgraph)
			{
				savedUnames.push_back("U RMS, V");
			}
			else
			{
				savedUnames.push_back("U_A, V");
				savedUnames.push_back("U_B, V");
				savedUnames.push_back("U_C, V");
			}
			GraphVoltageRMS = new ShowGraph(GetName()+"-U", string("t"), savedUnames, width, height, font, graphchannels, collectgraph, (1.0/graphchannels));//A, B, C, N
		}

		if (GetBoolParam(param, "-I", saveI)){//need to  store I graph
			if (collectgraph)
			{
				savedInames.push_back("I RMS, A");
			}
			else
			{
				savedInames.push_back("I_A, A");
				savedInames.push_back("I_B, A");
				savedInames.push_back("I_C, A");
			}
			GraphCurrentRMS = new ShowGraph(GetName()+"-I", string("t"), savedInames, width, height, font,graphchannels, collectgraph, 1);//A, B, C, N
		}

		if (GetBoolParam(param, "-S", saveS)){//need to  store S
			if (collectgraph)
			{
				savedSnames.push_back("S, VA");
			}
			else
			{
				savedSnames.push_back("S_A, VA");
				savedSnames.push_back("S_B, VA");
				savedSnames.push_back("S_C, VA");
			}
			GraphFullPowerRMS = new ShowGraph(GetName()+"-S", string("t"), savedSnames, width, height, font,graphchannels, collectgraph, 1);//A, B, C, N
		}

		if (GetBoolParam(param, "-Phi", savePhi))
		{//need to  store phase
			if (collectgraph)
			{
				savedPhinames.push_back("Phi");
			}
			else
			{
				savedPhinames.push_back("Phi_A");
				savedPhinames.push_back("Phi_B");
				savedPhinames.push_back("Phi_C");
			}
			GraphPowerFactorRMS = new ShowGraph(GetName()+"-Phi", string("t"), savedPhinames, width, height, font, graphchannels, collectgraph, (1.0/graphchannels) );//for phi only
		}

		if (GetBoolParam(param, "-P", saveP)){//need to  store P
			if (collectgraph)
			{
				savedPnames.push_back("P, W");
			}
			else
			{
				savedPnames.push_back("P_A, W");
				savedPnames.push_back("P_B, W");
				savedPnames.push_back("P_C, W");
			}
			GraphActivePowerRMS = new ShowGraph(GetName()+"-P", string("t"), savedPnames, width, height, font,graphchannels, collectgraph, 1);//A, B, C, N
		}

		if (GetBoolParam(param, "-Q", saveQ)){//need to  store Q
			if (collectgraph)
			{
				savedQnames.push_back("Q, VAr");
			}
			else
			{
				savedQnames.push_back("Q_A, VAr");
				savedQnames.push_back("Q_B, VAr");
				savedQnames.push_back("Q_C, VAr");
			}
			GraphReactivePowerRMS = new ShowGraph(GetName()+"-Q", string("t"), savedQnames, width, height, font,graphchannels, collectgraph, 1);//A, B, C, N
		}

		if (GetBoolParam(param, "-E", saveE)){//need to  store E
			if (collectgraph)
			{
				savedEnames.push_back("E, kWh");
			}
			else
			{
				savedEnames.push_back("E_A, kWh");
				savedEnames.push_back("E_B, kWh");
				savedEnames.push_back("E_C, kWh");
			}
			GraphActiveEnergyRMS = new ShowGraph(GetName()+"-E", string("t"), savedEnames, width, height, font, graphchannels, collectgraph, 1 );//for phi only
		}

		if (GetBoolParam(param, "-R", saveR)){//need to  store R
			if (collectgraph)
			{
				savedRnames.push_back("R, Om");
			}
			else
			{
				savedRnames.push_back("R_A, Om");
				savedRnames.push_back("R_B, Om");
				savedRnames.push_back("R_C, Om");
			}
			GraphActiveResistanceRMS = new ShowGraph(GetName()+"-R", string("t"), savedRnames, width, height, font, graphchannels, collectgraph, (1.0/graphchannels) );
		}

		if (GetBoolParam(param, "-X", saveX)){//need to  store U
			if (collectgraph)
			{
				savedLnames.push_back("X, Om");
			}
			else
			{
				savedLnames.push_back("X_A, Om");
				savedLnames.push_back("X_B, Om");
				savedLnames.push_back("X_C, Om");
			}
			GraphReactiveResistanceRMS = new ShowGraph(GetName()+"-L", string("t"), savedLnames, width, height, font, graphchannels, collectgraph, (1.0/graphchannels) );
		}

		if (GetBoolParam(param, "-dR", saveDR)){//need to  store R
			if (collectgraph)
			{
				savedDRnames.push_back("dR, Om/c");
			}
			else
			{
				savedDRnames.push_back("dR_A, Om/c");
				savedDRnames.push_back("dR_B, Om/c");
				savedDRnames.push_back("dR_C, Om/c");
			}
			GraphActiveResistanceDerivativeRMS = new ShowGraph(GetName()+"-dR", string("t"), savedDRnames, width, height, font, graphchannels, collectgraph, (1.0/graphchannels) );
		}

		if (GetBoolParam(param, "-dX", saveDX)){//need to  store U
			if (collectgraph)
			{
				savedDXnames.push_back("dX, Om/c");
			}
			else
			{
				savedDXnames.push_back("dX_A, Om/c");
				savedDXnames.push_back("dX_B, Om/c");
				savedDXnames.push_back("dX_C, Om/c");
			}
			GraphReactiveResistanceDerivativeRMS = new ShowGraph(GetName()+"-dL", string("t"), savedDXnames, width, height, font, graphchannels, collectgraph, (1.0/graphchannels) );
		}

		for (uint8_t i=0; i < STORED_VARS; i++){
			U_very_old[i] = 0;
			I_very_old[i] = 0;
			U_squared_sum[i] = 0;
			I_squared_sum[i] = 0;
			U_mul_I_sum[i] = 0;
			U90_mul_I_sum[i] = 0;
		}
}

Analyzer::~Analyzer() {

}

void Analyzer::Solve(float t, float dt){

	U_out = U_in;
	//collect data in any way
	//exclude initial data, when time = 0
	if (t_old < dt/8)
	{
		rmsitercount = static_cast<int>(static_cast<float>(rmsperiod) / (static_cast<float>(dt)));
		cout << "RMS period "<< rmsperiod << " RMS counts " << rmsitercount << " dt " << dt <<endl;
	}
	else
	{
		if (!rmsinit){//old RMS data is enabled
			//init RMS counting with setting data array count
			for (int s = 0; s < STORED_VARS; ++s){
				//initial copy
				U_array[s].push_back(U_out[s]);
				I_array[s].push_back(I_out[s]);

				//add to RMS counter
				U_squared_sum[s] += U_array[s].back() * U_array[s].back();
				I_squared_sum[s] += I_array[s].back() * I_array[s].back();

				U_mul_I_sum[s] += U_array[s].back() * I_array[s].back();
			}

			//collect fist pack;
			if (U_array[0].size() < rmsitercount){
				rmsinit = 0;
			}
			else{
				ringBuffer_Start = 0;
				ringBuffer_End = U_array[0].size() - 1;
				for (int s = 0; s < STORED_VARS; ++s){
					for (int i = 0; i < 20; ++i){
						//initial copy
						U_array[s].push_back(0);
						I_array[s].push_back(0);
					}
				}
				rmsinit = 1;
			}
		}
		else{
			if ( (t - t_RMS_last_count) > t_RMS_counting ){
				CalcRMS(t, dt);
				CalcPower(t, dt);
				t_RMS_last_count = t;
			}
		}
	}
	t_old = t;
	I_in = I_out;
}

void Analyzer::CalcRMS(float t, float dt){
	for (int s = 0; s < STORED_VARS; s++){
		//shift to left old data array (ring buffer from ass)
		/*Store first data*/
		U_very_old[s] = U_array[s][ringBuffer_Start];
		I_very_old[s] = I_array[s][ringBuffer_Start];
		//delete old from RMS count for U and I
		U_squared_sum[s] -= U_array[s][ringBuffer_Start] * U_array[s][ringBuffer_Start];
		I_squared_sum[s] -= I_array[s][ringBuffer_Start] * I_array[s][ringBuffer_Start];

		//for P and Q
		U_mul_I_sum[s] -= U_array[s][ringBuffer_Start] * I_array[s][ringBuffer_Start];

		//and add new data to the end
		U_array[s][ringBuffer_End] = U_out[s];
		I_array[s][ringBuffer_End] = I_out[s];

		//add new to RMS count
		U_squared_sum[s] += U_array[s][ringBuffer_End] * U_array[s][ringBuffer_End];
		I_squared_sum[s] += I_array[s][ringBuffer_End] * I_array[s][ringBuffer_End];

		U_mul_I_sum[s] += U_array[s][ringBuffer_End] * I_array[s][ringBuffer_End];

	}
	ringBuffer_Start++;
	if (ringBuffer_Start >= U_array[0].size()){
		ringBuffer_Start = 0;
	}
	ringBuffer_End++;
	if (ringBuffer_End >= U_array[0].size()){
		ringBuffer_End = 0;
	}
	for (int s = 0; s < STORED_VARS; s++){
		if (saveU)
		{
			GraphVoltageRMS->AddData(s, t, sqrt(U_squared_sum[s]/U_array[s].size()));
		}

		if (saveI)
		{
			GraphCurrentRMS->AddData(s, t, sqrt(I_squared_sum[s]/I_array[s].size()));
		}
	}
}

void Analyzer::CalcPower(float t, float dt)
{
	double tmp = 0;
	for (int s=0; s<STORED_VARS;s++)
	{
		if ((saveU)&&(saveI)){

			if (saveP){
				GraphActivePowerRMS->AddData(s, t, U_mul_I_sum[s]/U_array[s].size());

				if (saveE){
					GraphActiveEnergyRMS->AddData(s, t, GraphActiveEnergyRMS->Last(s) + GraphActivePowerRMS->Last(s)*rmsperiod/3600000);
				}
			}//if saveP

			if (saveR){
				if (GraphActivePowerRMS->Last(s)){
					tmp = GraphVoltageRMS->Last(s)*GraphVoltageRMS->Last(s)/ GraphActivePowerRMS->Last(s);
				}
				else{
					tmp = 0;
				}
				if (saveDR){
					GraphActiveResistanceDerivativeRMS->AddData(s,t,(GraphActiveResistanceRMS->Last(s) - tmp)/dt);
				}
				GraphActiveResistanceRMS->AddData(s,t, tmp);
			}//if saveR

			if (saveS){
				GraphFullPowerRMS->AddData(s, t, GraphCurrentRMS->Last(s) * GraphVoltageRMS->Last(s));
				if (savePhi){
					if ((GraphFullPowerRMS->Last(s))){
						GraphPowerFactorRMS->AddData(s, t, (GraphActivePowerRMS->Last(s)) / (GraphFullPowerRMS->Last(s)));
					}
					else{
						GraphPowerFactorRMS->AddData(s, t, 0);
					}
				}
				if (saveQ){
					GraphReactivePowerRMS->AddData(s, t, sqrt(GraphFullPowerRMS->Last(s)*GraphFullPowerRMS->Last(s)-
															GraphActivePowerRMS->Last(s)*GraphActivePowerRMS->Last(s)));
					if (saveX){
						if (GraphReactivePowerRMS->Last(s)){
							tmp = GraphVoltageRMS->Last(s)*GraphVoltageRMS->Last(s)/ GraphReactivePowerRMS->Last(s);
						}
						else{
							tmp = 0;
						}
						if (saveDX){
							GraphReactiveResistanceDerivativeRMS->AddData(s,t,(GraphReactiveResistanceRMS->Last(s) - tmp)/dt);
						}
						GraphReactiveResistanceRMS->AddData(s,t, tmp);
					}//if saveX
				}//if saveQ
			}//if saveS
		}//if saveU && saveI
	}//for
}

void Analyzer::ShowCustomGraph(const string &style,	status mode)
{
	if (true == saveU){
		GraphVoltageRMS->Plot(style, mode, saveraw);
	}
	if (true ==saveI){
		GraphCurrentRMS->Plot(style, mode, saveraw);
	}
	if (true == saveS){
		GraphFullPowerRMS->Plot(style, mode, saveraw);
	}
	if (true == saveP){
		GraphActivePowerRMS->Plot(style, mode, saveraw);
	}
	if (true == savePhi){
		GraphPowerFactorRMS->Plot(style, mode, saveraw);
	}
	if (true == saveQ){
		GraphReactivePowerRMS->Plot(style, mode, saveraw);
	}
	if (true == saveE){
		GraphActiveEnergyRMS->Plot(style, mode, saveraw);
	}
	if (true == saveR){
		GraphActiveResistanceRMS->Plot(style, mode, saveraw);
	}
	if (true == saveX){
		GraphReactiveResistanceRMS->Plot(style, mode, saveraw);
	}
	if (true == saveDR){
		GraphActiveResistanceDerivativeRMS->Plot(style, mode, saveraw);
	}
	if (true == saveDX){
		GraphReactiveResistanceDerivativeRMS->Plot(style, mode, saveraw);
	}
}
