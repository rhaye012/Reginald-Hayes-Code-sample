#include "TrafficLight.h"


TrafficLight::TrafficLight(void)
{
}


TrafficLight::~TrafficLight(void)
{
}

void TrafficLight::setMaterials()
{
	// In this function, you are supposed to assign values to the variables redOn, redOff,
	// yellowOn, yellowOff, greenOn, greenOff.
	float Ka[3];
	float GKd[3], YKd[3], RKd[3];
	float Ks[3], OffKs[3];
	Ka[0] = 0.000000; Ka[1] = 0.000000; Ka[2] = 0.000000;
	RKd[0] = 1.000000; RKd[1] = 0.000000; RKd[2] = 0.000000;
	YKd[0] = 1.000000; YKd[1] = 1.000000; YKd[2] = 0.000000;
	GKd[0] = 0.000000; GKd[1] = 1.000000; GKd[2] = 0.000000;
	Ks[0] = 0.330000; Ks[1] = 0.330000; Ks[2] = 0.330000;
	OffKs[0] = 0.000000; OffKs[1] = 0.000000; OffKs[2] = 0.000000;

	for (int i = 0; i < 3; i++)
	{
		redOff.Kd[i] = RKd[i];
		redOn.Kd[i] = RKd[i];

		greenOff.Kd[i] = GKd[i];
		greenOn.Kd[i] = GKd[i];

		yellowOff.Kd[i] = YKd[i];
		yellowOn.Kd[i] = YKd[i];
		//
		redOff.Ka[i] = Ka[i];
		redOn.Ka[i] = Ka[i];

		greenOff.Ka[i] = Ka[i];
		greenOn.Ka[i] = Ka[i];

		yellowOff.Ks[i] = Ka[i];
		yellowOn.Ks[i] = Ka[i];
		//
		redOff.Ks[i] = OffKs[i];
		redOn.Ks[i] = Ks[i];

		greenOff.Ks[i] = OffKs[i];
		greenOn.Ks[i] = Ks[i];

		yellowOff.Ks[i] = OffKs[i];
		yellowOn.Ks[i] = Ks[i];
	}


}

void TrafficLight::setSignal(Signal signal)
{
	// You are supposed to assign the materials used in the ObjModel class based on
	// values of the input signal.

	if (signal == Red)
	{
		materials[mtlNames[6]].Kd[0] = 1; materials[mtlNames[6]].Kd[1] = 0; materials[mtlNames[6]].Kd[2] = 0;
		for (int i = 0; i < 3; i++)
		{
			
			materials[mtlNames[7]].Kd[i] = 0;
			materials[mtlNames[8]].Kd[i] = 0;
		}
	}
	else if (signal == Green)
	{
		materials[mtlNames[8]].Kd[0] = 0; materials[mtlNames[8]].Kd[1] = 1; materials[mtlNames[8]].Kd[2] = 0;
		for (int i = 0; i < 3; i++)
		{
			materials[mtlNames[6]].Kd[i] = 0;
			materials[mtlNames[7]].Kd[i] = 0;
		}
	}
	else if (signal == Yellow)
	{
		materials[mtlNames[7]].Kd[0] = 1; materials[mtlNames[7]].Kd[1] = 1; materials[mtlNames[7]].Kd[2] = 0;
		for (int i = 0; i < 3; i++)
		{
			materials[mtlNames[6]].Kd[i] = 0;
			materials[mtlNames[8]].Kd[i] = 0;
		}
	}
}

void TrafficLight::ReadFile(string fileName)
{
	ObjModel::ReadFile(fileName);
	setMaterials();
}