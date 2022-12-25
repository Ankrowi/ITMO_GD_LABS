#include "OutputHandler.h"

OutputHandler::OutputHandler() {
    FinalReport.insert(std::pair<int, std::string>(BadMark,
        "You were so incompetent ruler that the citizens revolted \
        and drove you out of their city!"));
    FinalReport.insert(std::pair<int, std::string>(SatisfyingMark,
        "You ruled with an iron fist, like Nero and Ivan the Terrible. \
        The people breathed a sigh of relief, and no one else wants to \
        see you as a ruler!"));
    FinalReport.insert(std::pair<int, std::string>(TerrificMark,
        "You were so incompetent ruler that the citizens revolted \
        and drove you out of their city!"));
    FinalReport.insert(std::pair<int, std::string>(GoodMark,
        "Fantastic! Charlemagne, Disraeli and Jefferson couldn't \
        have done better together"));
}

OutputHandler::~OutputHandler() {
	
}

void OutputHandler::printInputMistake(std::string mistake) {
    std::cout << mistake << std::endl;
}

void OutputHandler::printFinalReport(FinalMark mark) {
    std::cout << FinalReport[mark] << std::endl;
}

void OutputHandler::printTurnReport(TurnReport report) {
    using namespace std;
    printLine();
    cout << "My Lord, let me tell you that \n";
    cout << "   In the " << report.TurnIdx << " of your highest rule\n";
    if (!report.StartReport)
    cout << "   " << report.Starved << " People died of hunger and " <<
        report.IncomePeople << " people arrived our great city;\n";
    if (!report.StartReport) {
        if (report.PlagueHappened)
            cout << "   Plague killed half the people;\n";
        else cout << "   This year plague passed our city;\n";
    }
    cout << "   Population of our city is now " << report.CurrentPopulation << " people;\n";
    if (!report.StartReport)
    cout << "   We gathered " << report.BushelsRaised << " bushels of wheet, it's " <<
        report.BushelsPerAkre << " bushels per akre;\n";
    if (!report.StartReport)
        cout << "   Rats consumed " << report.BushelsEatenByRats << " bushels of wheet" <<
        ", leaving " << report.TotalBushels << " bushels in our barns;\n";
    else cout << "   Total amount of wheet is now " << report.TotalBushels << " bushels;\n";
    cout << "   The city now occupies " << report.CityAkrs << " akrs;\n";
    cout << "   1 akr of land now costs " << report.AkrCost << " bushels;\n";
}

void OutputHandler::printInputStart() {
    using namespace std;
    cout << "What do you wish, your majesty ?\n";
}

void OutputHandler::printInputBuy() {
    std::cout << "How many akrs do you command to buy: ";
}

void OutputHandler::printInputSell() {
    std::cout << "How many akrs do you command to sell: ";
}

void OutputHandler::printInputEat() {
    std::cout << "How many bushels of wheet do you command to eat: ";
}

void OutputHandler::printInputSeed() {
    std::cout << "How many akrs do you command to seed: ";
}

void OutputHandler::printWrongInputCount(MainResourses res) {
    std::cout << "Oh lord, spare us!We have only " << res.population << " people, " <<
        res.bushels << " bushels of wheat and " << res.akrs << " acres of land!\n";
}

void OutputHandler::printAskForPause() {
    printLine();
    std::cout << "My Lord, would you like to take a break? ( leave the game )\n\
Print \"yes\" or \"no\" as the answer, please: ";
}

void OutputHandler::printEndl() {
    //std::cout << std::endl;
}

void OutputHandler::printLoseGame() {
    printLine();
    std::cout << "So many people died of starving because of your fault, \nthat you were sentenced to death! \n \
        It was the last year of your horrible rule and life!";
}

void OutputHandler::printLine() {
    std::cout << "_____________________________________________________________________\n";
}

void OutputHandler::printAskForContinue() {
    std::cout << "My Lord, we've found chronicles of yout highes rule;\nDo you want to continue?\nPrint \"yes\" or \"no\" as the answer: ";
}