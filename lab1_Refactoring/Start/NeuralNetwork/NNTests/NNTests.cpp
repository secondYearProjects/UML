#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <windows.h>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <fstream>
#include <string.h>

#include "../NeuralNetwork/NeuralNetwork.h"
#include "../NeuralNetwork/NeuralNetwork.cpp"

#include "../NeuralNetwork/Neuron.h"
#include "../NeuralNetwork/Neuron.cpp"

#include "../NeuralNetwork/FileSystem.h"
#include "../NeuralNetwork/FileSystem.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool cmp(std::vector<float> a1, std::vector<float> a2) {
	for (int i = 0; i < a1.size();i++) {
		if (a1[i] != a2[i])
			return false;
	}
	return true;
}

float eqFunction(float x) { return x; }

float halfFunction(float x) { return x / 2.0f; }

namespace NNTests
{
	TEST_CLASS(NNTests)
	{
	public:
		
		TEST_METHOD(LayerCreationConnection)
		{
			NeuralNetwork a("d");
			//NeuralNetwork n1{ "n1" };

			//n1.addLayer(2, "n1.1");
			//n1.addLayer(3, "n1.2");
			
			// 1 -> 2
			//n1.connectLayers("n1.1", "n1.2");

			//float eq[] = { 1.0f,1.0f,1.0f };

			//n1.setLayerData(eq, 3, "n1.1");
			
			//n1.setLayerWeights("n1.1", 1.0f);
			//n1.activateLayer("n1.1");

			std::vector<float> expected1 = { 1.0f,1.0f,1.0f };
			//Assert::IsTrue(cmp(expected1, n1.getData("n1.2")));

			//n1.setActivationFunction("n1.2", eqFunction);
			//n1.activatonFunction("n1.2");
			// Activation with eq function doesn't change values
			//Assert::IsTrue(cmp(expected1, n1.getData("n1.2")));

			std::vector<float> expected2 = { 0.5f, 0.5f, 0.5f };
			//n1.setActivationFunction("n1.2", halfFunction);
			//n1.activatonFunction("n1.2");
			// Activation with half function
			//Assert::IsTrue(cmp(expected2, n1.getData("n1.2")));


		}
	};
}
