#pragma once

#include <string>
#include <iostream>
#include "OutputHandler.h"

class InputHandler {
public:
	struct correct_input {
		int akrsToBuy;
		int akrsToSell;
		int akrsToSeed;
		int bushelsToEat;

		correct_input(int buy, int sell, int seed, int eat) :
			akrsToBuy(buy), akrsToSell(sell), akrsToSeed(seed),
			bushelsToEat(eat) { };
	};
	enum Troubles {
		WrongBuyAkrs = -100,
		WrongSellAkrs,
		WrongEatBushels,
		WrongInputValue,
		WrongSeedAkrs,
	};
	InputHandler();
	
	correct_input handleInput();
	bool askForPause();
	void setOutputHandler(OutputHandler * ptr);
private:
	OutputHandler* output = nullptr;

	void(OutputHandler::*printWrongInput)(std::string) = nullptr;
	int handleBuyAkrs();
	int handleEatBushels();
	int handleSeedAkrs();
	int handleSellAkrs();
	bool checkDigits(const std::string& str);
	std::string getInput();
	int getPositiveInteger(int mistakeCode);
	bool checkError(int value);
	int cycleFunctionTillNotTrue(int(InputHandler::*ptrToFunction)());
};