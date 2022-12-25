#include "FileSystem.h"


FileSystem::FileSystem() {
	path = "saveFile.txt"; 
}

bool FileSystem::checkSaveExists() {
	fs.open(path, std::fstream::in || std::fstream::out);
	if (!fs.is_open() || fs.peek() == EOF)
		return false;
	fs.close();
	return true;
}

FileSystem::SaveFile FileSystem::loadGame() {
	fs.open(path, std::fstream::in);
	FileSystem::SaveFile file;
	fs >> file.round;
	fs >> file.population;
	fs >> file.bushels;
	fs >> file.akrs;
	fs >> file.starvedPercent;
	fs >> file.akrsToSell;
	fs >> file.akrsToBuy;
	fs >> file.akrsToSeed;
	fs >> file.bushelsToEat;
	fs.close();
	return file;
}

void FileSystem::saveGame(SaveFile & file) {
	using std::endl;
	fs.open(path, std::fstream::out);
	fs << file.round << endl;
	fs << file.population << endl;
	fs << file.bushels << endl;
	fs << file.akrs << endl;
	fs << file.starvedPercent << endl;
	fs << file.akrsToSell << endl;
	fs << file.akrsToBuy << endl;
	fs << file.akrsToSeed << endl;
	fs << file.bushelsToEat << endl;
	fs.close();
}