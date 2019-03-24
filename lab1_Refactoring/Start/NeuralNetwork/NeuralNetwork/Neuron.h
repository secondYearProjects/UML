#pragma once

#ifndef Neuron_H_
#define Neuron_H_

#include <vector>
#include <cmath>
#include <map>
#include "FileSystem.h"

using namespace std;

#define e 2.71828f

class Neuron;
class Layer;

struct TEdge
{
	Neuron *son = nullptr;
	float weight = 0.0f;
	string layerTo = "";
};

class Neuron
{
public:
	Neuron();
	~Neuron();

	void copyWeights(const Neuron& copyFrom); // copy weights from same type neuron
	void setInput(float input);
	void setWeights(float value);
	void activate();
	inline void increase(float value);
	float activateFunction();
	void setActivationFunction(float(*f)(float));
	void createLink(Neuron& connectWith, const string& layerTo);
	float getData();
	map< string, vector< TEdge* > >* getEdges();
private:
	float data = 0.0f;
	map< string, vector< TEdge* > > edges;
	float(*activationFunction)(float);
};

#endif // !Neuron_H_
