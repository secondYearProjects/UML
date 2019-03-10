#include "NeuralNetwork.h"
#include "FileSystem.h"

float sigmoidFunction(float x)    // Standart Activation Function.
{
	return (1.0f / (1.0f + float(pow(e, -x))));
}

inline bool checkIn(const string& strIn, const string& str)
{
	return (strIn.find(str) != string::npos);
}

inline bool checkForFileName(const string& layerID)
{
	return (checkIn(layerID, ".") &&
		checkIn(layerID, "_") &&
		checkIn(layerID, "/") &&
		checkIn(layerID, "\\") &&
		checkIn(layerID, "?") &&
		checkIn(layerID, "<") &&
		checkIn(layerID, ">") &&
		checkIn(layerID, "*"));
}

wstring StringToWString(const string& s)
{
	wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}

///////////////////** NeuralNetwork **/////////////////////////////

NeuralNetwork::NeuralNetwork(const string& name) :networkName(name)
{
	if(this->_echo == 1)
		this->fs.organizeNetwork(name);
}

NeuralNetwork::NeuralNetwork(const string & name, bool echo) :networkName(name)
{
	this->_echo = echo;
	if (this->_echo == 1)
		this->fs.organizeNetwork(name);
}

NeuralNetwork::~NeuralNetwork()
{
	for (auto it = this->layers.begin(); it != this->layers.end(); it++)
	{
		delete (*it).second;
	}
}

vector<float> NeuralNetwork::getData(const string & ID)
{
	Layer* layer = this->layers.find(ID)->second;
	int n = (*layer).getSize();

	vector<float> mas(n);
	//float* mas = new float[n];

	for (int i = 0; i < n; i++)
	{
		mas[i] = layer->getNeuronData(i);
	}
	return mas;
}

Layer * NeuralNetwork::getLayer(const string & ID)
{
	return (*(this->layers.find(ID))).second;
}

void NeuralNetwork::addLayer(unsigned int neuronQuantity, const string& layerID)
{
	if (checkForFileName(layerID)) // check on _ . < > , \ / ? * in id
	{
		throw ID_HAS_ERRORS;
		exit(ID_HAS_ERRORS);
	}
	if (this->layers.find(layerID) == this->layers.cend())
	{
		Layer * newLayer = new Layer(neuronQuantity, layerID);
		newLayer->setNetworkName(this->networkName);
		this->layers.insert(layerPair(layerID, newLayer));

		if(this->_echo == 1)
			newLayer->setPath(this->fs.organizeLayer(this->networkName, layerID));
	}
	else
	{
		throw ID_ALREADY_EXISTS; // DO SOMETHING or write catch in wrapper for network class
		exit(ID_ALREADY_EXISTS);
	}
}

bool NeuralNetwork::connectLayers(const string& ID1, const string& ID2)
{
	if (checkLayerExist(ID1) && checkLayerExist(ID2))
	{
		(*(this->layers.find(ID1))).second->linkWithLayer((*(this->layers.find(ID2))).second);

		wstring pathToWeights = (*(this->layers.find(ID1))).second->getPath();

		pathToWeights += L"\\" + StringToWString(ID1) +
			L"_" + StringToWString(ID2) + L".txt";
		if (this->_echo == 1)
			this->fs.createWeightFile(this->layers.find(ID1)->second->getNeurons(), pathToWeights, ID2);
		return 1;
	}
	else
		return 0;
}

bool NeuralNetwork::loadWeightsFromFile(const wstring&  pathToFile,
	const  string&  IDFrom,
	const  string&  IDTo)
{
	auto idFromNeurons = this->layers.find(IDFrom)->second->getNeurons();

	map< string, vector< TEdge* > >* pVectorEdges =
		idFromNeurons->begin()->getEdges();

	if (pVectorEdges->find(IDTo) == pVectorEdges->end())
		return 0;

	ifstream fin(pathToFile);

	for (auto it  = idFromNeurons->begin();
		      it != idFromNeurons->end(); ++it)
	{
		for (auto vIt = (*it).getEdges()->find(IDTo)->second.begin();
			vIt != (*it).getEdges()->find(IDTo)->second.end(); ++vIt)
		{
			fin >> (*vIt)->weight;
		}
	}
	fin.close();
	return 1;
}

bool NeuralNetwork::saveWeightsToDirectory(const wstring& pathToDirectory,
	const  string& IDFrom,
	const  string& IDTo)
{
	auto idFromNeurons = this->layers.find(IDFrom)->second->getNeurons();

	map< string, vector< TEdge* > >* pVectorEdges =
		idFromNeurons->begin()->getEdges();

	if (pVectorEdges->find(IDTo) == pVectorEdges->end())
		return 0;

	wstring tmpPath = pathToDirectory + L"\\" +
		StringToWString(IDFrom) + L"_" +
		StringToWString(IDTo) + L"_.txt";

	ofstream fout(tmpPath);
	fout << fixed;
	fout.precision(11);

	for (auto it = idFromNeurons->begin();
		it != idFromNeurons->end(); ++it)
	{
		for (auto vIt = (*it).getEdges()->find(IDTo)->second.begin();
			vIt != (*it).getEdges()->find(IDTo)->second.end(); ++vIt)
		{
			fout << (*vIt)->weight << ' ';
		}
		fout << '\n';
	}

	fout.close();
	return 1;
}

bool NeuralNetwork::randomizeWeights(const string & IDFrom, const string & IDTo, const float & a, const float & b)
{
	if (this->layers.find(IDFrom) == this->layers.end())
		return 0;
	else
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(a, std::nextafter(b, DBL_MAX));

		for (auto it = this->layers.find(IDFrom)->second->getNeurons()->begin();
			it != this->layers.find(IDFrom)->second->getNeurons()->end(); ++it)
		{
			for (auto vIt = (*it).getEdges()->find(IDTo)->second.begin();
				vIt != (*it).getEdges()->find(IDTo)->second.end(); ++vIt)
			{
				(*vIt)->weight = float(dist(mt));
			}
		}
		return 1;
	}
}

bool NeuralNetwork::addRandomValueToWeights(const string & IDFrom, const string & IDTo, const float & a, const float & b)
{
	if (this->layers.find(IDFrom) == this->layers.end())
		return 0;
	else
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(a, std::nextafter(b, DBL_MAX));

		for (auto it = this->layers.find(IDFrom)->second->getNeurons()->begin();
			it != this->layers.find(IDFrom)->second->getNeurons()->end(); ++it)
		{
			for (auto vIt = (*it).getEdges()->find(IDTo)->second.begin();
				vIt != (*it).getEdges()->find(IDTo)->second.end(); ++vIt)
			{
				(*vIt)->weight += float(dist(mt));
			}
		}
		return 1;
	}
}

void NeuralNetwork::mutate(const float & a, const float & b)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(a, std::nextafter(b, DBL_MAX));

	for (auto layer = this->layers.begin();
		layer != this->layers.end(); ++layer)
	{
		for (auto neuron = (*layer).second->getNeurons()->begin();
			neuron != (*layer).second->getNeurons()->end(); ++neuron)
		{
			for (auto vEdges = (*neuron).getEdges()->begin();
				vEdges != (*neuron).getEdges()->end(); ++vEdges)
			{
				for (auto connection = vEdges->second.begin();
					connection != vEdges->second.end(); ++connection)
				{
					(*connection)->weight += float(dist(mt));
				}
			}
		}
	}
}

void NeuralNetwork::mutateLayerPartly(const string & IDFrom, const string & IDTo, const float & a, const float & b, int divides)
{
	if (this->layers.find(IDFrom) == this->layers.end())
		return;
	else
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(a, std::nextafter(b, DBL_MAX));

		vector<Neuron> * tmpNeur = this->layers.find(IDFrom)->second->getNeurons();

		mt19937 gen(unsigned int(time(0)));
		int tmpSize = int(tmpNeur->size() - 1);
		uniform_int_distribution<int> chromo{ 0,tmpSize };

		int tmpCounter = divides;
		for (int i = 0; i < tmpCounter; i++)
		{
			int tmpChromo = chromo(gen);
			for (auto edgeIt  = ((*tmpNeur)[tmpChromo]).getEdges()->find(IDTo)->second.begin();
				      edgeIt != ((*tmpNeur)[tmpChromo]).getEdges()->find(IDTo)->second.end(); ++edgeIt)
			{
				(*edgeIt)->weight = float(dist(mt));
			}
		}
	}
	
}


void NeuralNetwork::activateLayer(const string & ID)
{
	(*(this->layers.find(ID))).second->activate();
}

void NeuralNetwork::outputDataToFile(const string & ID, const wstring & path)
{
	(*(this->layers.find(ID))).second->outputData(path);
}

void NeuralNetwork::activatonFunction(const string & ID)
{
	(*(this->layers.find(ID))).second->activateFunction();
}

bool NeuralNetwork::clearLayerData(const string & ID)
{
	auto layer = this->layers.find(ID);
	if (layer == this->layers.end())
		return 0;
	else
	{
		(*layer).second->clearData();
		return 1;
	}
}

void NeuralNetwork::deleteNetworkFiles()
{
	if (this->_echo == 1)
		this->fs.deleteNetwork();
}

void NeuralNetwork::crossLayers(const string & ID, Layer & crossWith, int divides)
{
	if (checkLayerExist(ID) == 0)
	{
		return;
	}
	(*(this->layers.find(ID))).second->crossing(crossWith, divides);
}

void NeuralNetwork::echo(bool value)
{
	this->_echo = value;
}

bool NeuralNetwork::setLayerData(float mas[], unsigned int n, const string & ID)
{
	{
		if (checkLayerExist(ID))
		{
			(*(this->layers.find(ID))).second->setLayerData(mas, n);
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

bool NeuralNetwork::setLayerWeights(const string & ID, float value)
{
	if (checkLayerExist(ID))
		return 0;
	(*(this->layers.find(ID))).second->setLayerWeights(value);
	return 1;
}

bool NeuralNetwork::setActivationFunction(const string & ID, float(*f)(float))
{
	auto layer = this->layers.find(ID);
	if (layer != this->layers.end())
	{
		(*layer).second->setActivationFunction(f);
		return 1;
	}
	else
		return 0;
}

inline bool NeuralNetwork::checkLayerExist(const string&  ID)
{
	if (this->layers.find(ID) != this->layers.end())
		return 1;

	else
		return 0;
}

Layer::Layer(int n, string id) :id(id), size(n)
{
	this->activationFunction = &sigmoidFunction;
	this->neurons = new vector<Neuron>(n);
}

Layer::~Layer()
{
	delete this->neurons;
	this->neurons = nullptr;
}

wstring Layer::getPath()
{
	return this->pathToLayer;
}

void Layer::setPath(const wstring& path)
{
	this->pathToLayer = move(path);
}

void Layer::setNetworkName(const string& name)
{
	this->networkName = move(name);
}

string Layer::getID()
{
	return this->id;
}

unsigned int Layer::getSize()
{
	return (this->size);
}

void Layer::setActivationFunction(float(*f)(float))
{
	for (auto it = this->neurons->begin();
		it != this->neurons->end(); ++it)
	{
		(*it).setActivationFunction(f);
	}
}

void Layer::activateFunction()
{
	for (auto it = this->neurons->begin();
		it != this->neurons->end(); ++it)
	{
		(*it).activateFunction();
	}
}

float Layer::getNeuronData(unsigned int n)
{
	if (n < this->size)
	{
		return (this->neurons->at(n).getData());
	}
	else
		return 0.0f;
}

void Layer::linkWithLayer(Layer * linkWith)
{
	for (auto layer1 = this->neurons->begin();
		layer1 != this->neurons->end(); ++layer1)
	{
		for (auto layer2 = linkWith->neurons->begin();
			layer2 != linkWith->neurons->end(); ++layer2)
		{
			(*layer1).createLink(*layer2, linkWith->getID());
		}
	}
}

void Layer::outputData(const wstring&  pathToFile)
{
	ofstream fout(pathToFile);
	fout << fixed;
	fout.precision(11);
	for (auto it = this->neurons->begin();
		it != this->neurons->end(); ++it)
	{
		fout << (*it).getData() << '\n';
	}

	fout.close();
}

void Layer::activate()
{
	for (auto it = this->neurons->begin();
		it != this->neurons->end(); ++it)
	{
		(*it).activate();
	}
}

void Layer::clearData()
{
	for (auto neurons__ = this->neurons->begin();
		neurons__ != this->neurons->end(); ++neurons__)
	{
		(*neurons__).setInput(0.0f);
	}
}

void Layer::setLayerWeights(float value)
{
	for (auto it = this->neurons->begin(); it != this->neurons->end(); ++it)
	{
		(*it).setWeights(value);
	}
}

void Layer::crossing(Layer & crossWith, int divides)
{
	if (this->neurons->size() != crossWith.neurons->size())
		return;

	mt19937 gen(unsigned int(time(0)));
	int tmpSize = int(this->neurons->size() - 1);
	uniform_int_distribution<int> chromo{ 0,tmpSize };
	int counter = divides;
	for (int i = 0; i < counter; i++)
	{
		int tmpChromo = chromo(gen);
		vector<Neuron>* tmpNeur = (crossWith.getNeurons());
		(*this->neurons)[tmpChromo].copyWeights((*tmpNeur)[tmpChromo]);
	}
}

bool Layer::setLayerData(float mas[], unsigned int n)
{
	{
		if (n < this->size)
			return 0;
		else
		{
			for (auto it = this->neurons->begin();
				it != this->neurons->end(); ++it)
			{
				(*it).setInput(mas[distance(this->neurons->begin(), it)]);
			}
			return 1;
		}
	}
}

vector<Neuron>* Layer::getNeurons()
{
	return this->neurons;
}