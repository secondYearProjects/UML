#include "../NeuralNetwork/NeuralNetwork.h"
#include "../NeuralNetwork/Neuron.h"

#include "pch.h"

TEST(LayerCreation, NeuralNetworkTests) {
	Layer layerA(2, "A");
	Layer layerB(2, "B");

	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(LayerConnection, NeuralNetworkTests) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(LayerActivation, NeuralNetworkTests) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(NeuralNetworkSaveAndLoad, NeuralNetworkTests) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}