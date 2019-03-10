#include "Neuron.h"
#include "NeuralNetwork.h"

float sigm(float x)  // standart Activation Function
{
	return (1.0f / (1.0f + float(pow(e, -x))));
}

Neuron::Neuron()
{
	this->data = 0.0f;
	this->activationFunction = &sigm;
}

Neuron::~Neuron()
{
	for (auto layerIt = this->edges.begin(); layerIt != this->edges.end(); ++layerIt)
	{
		for (auto vEdges = layerIt->second.begin(); vEdges != layerIt->second.end(); ++vEdges)
		{
			delete *vEdges;
			*vEdges = nullptr;
		}
	}
	this->edges.clear();
}

void Neuron::copyWeights(const Neuron & copyFrom)
{
	if (this->edges.size() != copyFrom.edges.size())
		return;
	auto it1 = this->edges.begin();
	auto it2 = copyFrom.edges.begin();
	while (it1 != this->edges.end() && it2 != copyFrom.edges.end())
	{
		auto connector1 = (*it1).second.begin();
		auto connector2 = (*it2).second.begin();
		while (connector1 != (*it1).second.end() && connector2 != (*it2).second.end())
		{
			(*connector1)->weight = (*connector2)->weight;
			++connector1;
			++connector2;
		}
		++it1;
		++it2;
	}
}

void Neuron::setInput(float input)
{
	this->data = input;
}

void Neuron::setWeights(float value)
{
	for (auto connection = this->edges.begin(); connection != this->edges.end(); ++connection)
	{
		for (auto vEdges = (*connection).second.begin(); vEdges != (*connection).second.end(); ++vEdges)
		{
			(*vEdges)->weight = value;
		}
	}
}

void Neuron::activate()
{
	for (auto it = this->edges.begin();
		it != this->edges.end(); ++it)
	{
		for (auto vectorIt = it->second.begin();
			vectorIt != it->second.end(); ++vectorIt)
		{
			(*vectorIt)->son->increase(((*vectorIt)->weight) * (this->data));
		}
	}
}

inline void Neuron::increase(float value)
{
	this->data += value;
}

float Neuron::activateFunction()
{
	this->data = (this->activationFunction)(this->data);
	return this->data;
}

void Neuron::setActivationFunction(float(*f)(float))
{
	this->activationFunction = f;
}

void Neuron::createLink(Neuron & connectWith, const string& layerTo)
{
	TEdge *newEdge = new TEdge();
	newEdge->son = &connectWith;
	newEdge->weight = 0.0f;
	newEdge->layerTo = move(layerTo);

	this->edges[layerTo].push_back(newEdge);
}

float Neuron::getData()
{
	return (this->data);
}

map< string, vector<TEdge*> >* Neuron::getEdges()
{
	return &this->edges;
}