#include "InputHandler.h"

InputHandler::InputHandler() {

}

std::string InputHandler::getInput() {
    std::string input_;
    std::cin >> input_;
    return input_;
}

int InputHandler::getPositiveInteger(int mistakeCode) {
    auto input_ = getInput();
    output->printEndl();
    if (!checkDigits(input_)) {
        return WrongInputValue;
    }
    try {
        int posInt = std::stoi(input_);
        if (posInt < 0)
            return mistakeCode;
        else return posInt;
    }
    catch (...) {
        return mistakeCode;
    }
}

int InputHandler::handleBuyAkrs() {
    output->printInputBuy();
    return getPositiveInteger(WrongBuyAkrs);
}

int InputHandler::handleEatBushels() {
    output->printInputEat();
    return getPositiveInteger(WrongEatBushels);
}

int InputHandler::handleSeedAkrs() {
    output->printInputSeed();
    return getPositiveInteger(WrongSeedAkrs);
}

int InputHandler::handleSellAkrs() {
    output->printInputSell();
    return getPositiveInteger(WrongSellAkrs);
}

bool InputHandler::checkError(int value) {
    if (value == WrongBuyAkrs) return false;
    if (value == WrongSellAkrs) return false;
    if (value == WrongEatBushels) return false;
    if (value == WrongSeedAkrs) return false;
    return true;
}

int InputHandler::cycleFunctionTillNotTrue(int(InputHandler::*ptrToFunction)()) {
    auto result = (this->*ptrToFunction)();
    while (result == WrongInputValue) {
        if (output != nullptr)
            output->printInputMistake("My Lord, I'm afraid I didn't got your decision. Repeat it please.\n");
        result = (this->*ptrToFunction)();
    }
    return result;
}

InputHandler::correct_input InputHandler::handleInput() {
    output->printInputStart();
    auto akrsToBuy = cycleFunctionTillNotTrue(&InputHandler::handleBuyAkrs);
    auto akrsToSell = cycleFunctionTillNotTrue(&InputHandler::handleSellAkrs);
    auto akrsToSeed = cycleFunctionTillNotTrue(&InputHandler::handleSeedAkrs);
    auto bushelsToEat = cycleFunctionTillNotTrue(&InputHandler::handleEatBushels);
    return correct_input(akrsToBuy, akrsToSell, akrsToSeed, bushelsToEat);
}

bool InputHandler::checkDigits(const std::string& str) {
    bool success_ = true;
    for (auto symb : str) {
        if (!isdigit(symb)) {
            success_ = false;
        }
    }
    return success_;
}

void InputHandler::setOutputHandler(OutputHandler* ptr) {
    output = ptr;
}

bool InputHandler::askForPause() {
    std::string answer;
    do {
        std::cin >> answer;
    } while (answer != "yes" && answer != "no");
    return answer == "yes" ? true : false;
}