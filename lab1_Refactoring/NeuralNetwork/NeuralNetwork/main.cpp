#include <cstdlib>
#include <iostream>
#include "NeuralNetwork.h"
#include "FileSystem.h"
#include "LifeGame.h"
#include "Player.h"

using namespace std;

int main()
{
	system("MODE CON: COLS=101 LINES=51");

	int numPlayers = 10;

	vector<int> res(numPlayers);
	vector<int> tmpVec(10);

	string savesPath = "F:\\work\\Git\\MachineLearning\\NeuralNetwork\\NeuralNetwork\\PlayersSaves\\player_";

	for (int i = 0; i < 800; i++)
	{
		LifeGame* ldf = new LifeGame(40, 40, numPlayers, i, savesPath, savesPath,0);
		if (i == 0)
		{
			//ldf->initializeAllWithRnd(-1.0f, 1.0f);
		}
		ldf->setPause(0);
		ldf->echoPrint(1);
		ldf->play(res);
		tmpVec = ldf->getResult();
		delete ldf;
		for (int k = 0; k < 10; k++)
		{
			res[k] += tmpVec[k];
		}
	}
	cout << "\nTOTAL:\n";
	for (int i = 0; i < 10; i++)
	{
		cout << "player " << i << " : " << res[i] << '\n';
	}
	system("pause");
	return 0;
}

/// TODO ///
//1 filework (write/read weights from files +++ "interpretator" for network)
//2 write wrapper for NeuralNetwork class
//3 optimize file system

// write exeptions
// divide headers and cpp parts of code

/*
FILE SYSTEM:
name of network:
	layer name:
		id1_id2.txt ///

*/