#define _CRT_SECURE_NO_WARNINGS
#pragma once

#ifndef FileSystem_H_
#define FileSystem_H_

#include <iostream>
#include <windows.h>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <fstream>

using namespace std;

class NeuralNetwork;
class Layer;
class Neuron;

typedef const string& Acstr;
typedef const wstring& Acwstr;

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	wstring getCurDir();                                 // Returns current directory path.

	wstring organizeLayer(const string& networkName,
		const string& layerID);        // Returns path to layer dir.

	void organizeNetwork(const string& networkName);     // Creates directory in current directory with network name.

	void createWeightFile(vector<Neuron>* neurons,       // Creates file with weights of neurons.
		const wstring&  path,
		const string&   connectedWith);

	void deleteNetwork();

private:
	wstring currentPath = L"";                          // Path to .exe file.
	wstring networkPath = L"";                          // path to network dir.

	void createNetworkDir(const wstring& name);         // Creates network directory and put path value into networkPath field.

	wstring createLayerDir(const wstring& name);        // Returns path to layer dir.

	int StringToWString(wstring& ws, const string& s);  // Changes wstring argument.
	wstring strToWstr(const string& s);                 // Converts and returns wstring.
};

#endif //  FileSystem_H_