#pragma once

#include <fstream>


class FileSystem
{
public:
	struct SaveFile {
		int round;
		int population;
		int bushels;
		int akrs;
		float starvedPercent;
		int akrsToSell;
		int akrsToBuy;
		int akrsToSeed;
		int bushelsToEat;
	};
	FileSystem();
	bool checkSaveExists();
	SaveFile loadGame();
	void saveGame(SaveFile & file);
private:
	std::fstream fs;
	std::string path;
};

