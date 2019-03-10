#pragma once

#ifndef NeuralNetwork_H_
#define NeuralNetwork_H_

#include <fstream>
#include "Neuron.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <iterator>
#include <random>

using namespace std;

#define ID_ALREADY_EXISTS 1
#define ID_HAS_ERRORS     2

class FileSystem;

class Layer
{
public:
	Layer(int n, string id);
	~Layer();

	wstring         getPath();
	string          getID();
	vector<Neuron>* getNeurons();
	float           getNeuronData(unsigned int n);
	unsigned int    getSize();

	bool            setLayerData(float mas[], unsigned int n);
	void            setPath(const wstring& path);
	void            setNetworkName(const string& name);
	void            setActivationFunction(float(*f)(float));

	void            activateFunction();
	void            linkWithLayer(Layer *linkWith);
	void            outputData(const wstring& pathToFile);
	void            activate();
	void            clearData();
	void            setLayerWeights(float value);
	void            crossing(Layer& crossWith, int divides); // crossing of (1/divider) weights for layers with same size

private:
	vector<Neuron> *neurons = nullptr;
	string id = "";                    // unique identifier for layer in network
	unsigned int size = 0;
	wstring pathToLayer = L"";
	string networkName = "";           // must be without _.<>,\/?* symbols
	float(*activationFunction)(float); // pointer on function that returns float
									   // value and has one float argument
};

#define layerPair pair< string, Layer* >

class NeuralNetwork
{
public:
	NeuralNetwork(const string& name);
	NeuralNetwork(const string& name, bool echo);
	~NeuralNetwork();

	void echo(bool value);
	bool setLayerData(float mas[], unsigned int n, const string& ID);
	bool setLayerWeights(const string& ID, float value);
	bool setActivationFunction(const string& ID, float(*f)(float));

	vector<float> getData(const string& ID);

	Layer* getLayer(const string& ID);

	void addLayer(unsigned int  neuronQuantity,
		const string& layerID);

	bool connectLayers(const string& ID1,
		const string& ID2);

	bool loadWeightsFromFile(const wstring& pathToFile,
		const  string& IDFrom,
		const  string& IDTo);

	bool saveWeightsToDirectory(const wstring& pathToDirectory,
		const  string& IDFrom,
		const  string& IDTo);

	bool randomizeWeights(const  string& IDFrom,
		const  string& IDTo,
		const   float& a,
		const   float& b); // set layer weights to random [a..b]

	bool addRandomValueToWeights(const  string& IDFrom,
		const  string& IDTo,
		const   float& a,
		const   float& b); // adds to layer weights random floats [a..b]

	void mutate(const float &a, const float &b);

	void mutateLayerPartly(const string&IDFrom, const  string& IDTo, const float &a, const float &b, int divides);

	void activateLayer(const string& ID);

	void outputDataToFile(const string& ID, const wstring& path);

	void activatonFunction(const string &ID);

	bool clearLayerData(const string& ID);

	void deleteNetworkFiles();

	void crossLayers(const string&ID, Layer& crossWith, int divides);


private:
	unordered_map<string, Layer*> layers; // key, point to the layer of nodes
	string networkName = "a";
	FileSystem fs;
	bool _echo = 1; // 1 - create directory&files 0 - do not

	inline bool checkLayerExist(const string& ID);
};

//////////////////** templates **///////////////////////////

#endif // NeuralNetwork_H_ 