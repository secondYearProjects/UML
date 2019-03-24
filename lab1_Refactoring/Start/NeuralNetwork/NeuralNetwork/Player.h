#pragma once

#ifndef Player_H_
#define Player_H_

#include <ctime>
#include <stdlib.h>
#include <string>
#include "NeuralNetwork.h"

using namespace std;

#define MAXFIELD 1000.0f
#define DEFAULTHEALTH 100.0f
#define MAXHEALTH 400.0f
#define STEPDECREACE -5.0f

#define MUTATEEVERY 10
#define MINEFF ((DEFAULTHEALTH/ STEPDECREACE)*MUTATEEVERY)

#define EMPTY_  ' '
#define PLAYER_ '±'
#define FOOD_   '+'

/// COMMANDS ///

class LifeGame;

class Player
{
public:
	Player(int n, int m, int playerID, LifeGame* game );
	Player(int n, int m, int playerID, LifeGame* game, bool echo);
	~Player();

	NeuralNetwork * neuro = nullptr;
	void setHealth(float value);
	int get_X();
	int get_Y();
	int getID();
	float getHealth();
	void copyNeuro(const Player& right);
	void clearNeuroData();
	void activateNeuro(float* input);
	void mutate(float a, float b);
	void addHealth(float value);
	void saveWeights();
	void mutatePartly();

private:
	int playerID = 0;
	bool _echo = 1;
	float health = DEFAULTHEALTH;

	LifeGame* currGame = nullptr;

	int pos_x = 0;
	int pos_y = 0;
};

#endif // !Player_H_