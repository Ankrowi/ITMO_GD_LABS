#pragma once

#include <iostream>
#include <string>
#include <map>

class OutputHandler {
public:
    struct TurnReport {
        int TurnIdx;
        int Starved;
        int IncomePeople;
        bool PlagueHappened;
        int CurrentPopulation;
        int BushelsRaised;
        int BushelsPerAkre;
        int BushelsEatenByRats;
        int CityAkrs;
        int AkrCost;
        int TotalBushels;
        bool StartReport;

        TurnReport(int idx, int starved, int income, bool plague, int pop,
            int bushRaised, int bushPerAkr, int ratsAte, int cityAkrs,
            int akrCost, int bush, int start = false) :
            TurnIdx(idx), Starved(starved), IncomePeople(income),
            PlagueHappened(plague), CurrentPopulation(pop), BushelsRaised(bushRaised),
            BushelsPerAkre(bushPerAkr), BushelsEatenByRats(ratsAte), CityAkrs(cityAkrs),
            AkrCost(akrCost), TotalBushels(bush), StartReport(start) {};
    };
    enum FinalMark {
        BadMark = 1,
        SatisfyingMark,
        GoodMark,
        TerrificMark
    };
    struct MainResourses {
        int population;
        int bushels;
        int akrs;

        MainResourses(int pop_, int bush_, int akrs_) :
            population(pop_), bushels(bush_), akrs(akrs_) {}
    };
    OutputHandler();
    ~OutputHandler();
    //methods for GameManager
    void printTurnReport(TurnReport report);
    void printFinalReport(FinalMark mark);
    void printAskForPause();
    void printAskForContinue();
    void printLoseGame();
    //methods for InputHandler
    void printInputBuy();
    void printInputSell();
    void printInputSeed();
    void printInputEat();
    void printWrongInputCount(MainResourses res);
    void printInputMistake(std::string);
    void printInputStart();
    void printEndl();
   
private:
    std::map<int, std::string> FinalReport;
    // inner used
    
    void printLine();
    
};