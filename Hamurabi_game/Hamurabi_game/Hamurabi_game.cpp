#include "OutputHandler.h"
#include "InputHandler.h"
#include "FileSystem.h"
#include "God.h"

class GameManager {
public:
    GameManager() {
        output = new OutputHandler();
        input = new InputHandler();
        fs = new FileSystem();
        input->setOutputHandler(output);
    }
    ~GameManager() {
        delete output;
        delete input;
        delete fs;
    }
    void RunGame() {
        if (fs->checkSaveExists()) {
            output->printAskForContinue();
            if (!input->askForPause()) {
                round = RoundInfo();
            } else {
                auto save_ = fs->loadGame();
                round = RoundInfo(save_.round, save_.akrs, save_.bushels,
                    save_.population, save_.starvedPercent, save_.akrsToSell,
                    save_.akrsToBuy, save_.akrsToSeed, save_.bushelsToEat);
            }
        } else {
            round = RoundInfo();
        }

        while (round.roundIdx < GameRounds) {
            output->printAskForPause();
            if (input->askForPause()) {
                StopGame();
                return;
            }
            CountNextRound();
            if (Lost) {
                LoseGame();
                return;
            }
            OutputHandler::TurnReport report(round.roundIdx, params.StarvedPeople, 
                params.IncomePeople, params.PlagueHappened, round.currentPopulation, 
                params.BushelsRaised, params.BushelsPerAkr, params.RatsRation, 
                round.currentAkrs, params.AkrCost, round.currentBushels, params.StartRound);
            output->printTurnReport(report);
            CountInputValues();
        }
        EndGame();
    }

    void CountNextRound() {
        params.StartRound = round.roundIdx++ == 0 ? true : false;
        params.BushelsPerAkr = God::ask().sendBushelsProduction();
        params.BushelsRaised = round.akrsToSeed * params.BushelsPerAkr;
        round.currentBushels += params.BushelsRaised - int(float(round.akrsToSeed) * SeedCost);
        params.RatsRation = God::ask().sendRatsRation(round.currentBushels);
        if (!params.StartRound) {
            round.currentBushels -= params.RatsRation;
            int canEat = round.bushelsToEat / PeopleRation;
            params.StarvedPeople = std::max(0, round.currentPopulation - canEat);
            if (float(params.StarvedPeople) / float(round.currentPopulation) >= loseThreashold) {
                Lost = true;
                return;
            }
            round.starvedPercent += float(params.StarvedPeople) / float(round.currentPopulation);
            round.currentPopulation -= params.StarvedPeople;
            round.currentBushels -= PeopleRation * canEat;
            params.IncomePeople = params.StarvedPeople / 2 + 
                (5 - params.BushelsPerAkr) * round.currentBushels / 600 + 1;
            params.IncomePeople = std::max(0, params.IncomePeople);
            params.IncomePeople = std::min(50, params.IncomePeople);
            round.currentPopulation += params.IncomePeople;
            params.PlagueHappened = God::ask().sendPlague();
            if (params.PlagueHappened)
                round.currentPopulation /= 2;
        }
        params.AkrCost = God::ask().sendAkrCost();
    }
    void CountInputValues() {
        bool inputIsCorrect = false;
         do {
            auto inputValues = input->handleInput();
            round.akrsToBuy = inputValues.akrsToBuy;
            round.akrsToSeed = inputValues.akrsToSeed;
            round.akrsToSell = inputValues.akrsToSell;
            round.bushelsToEat = inputValues.bushelsToEat;
            inputIsCorrect = checkInputValuesCost();
         } while (!inputIsCorrect);
        int diffLand = round.akrsToBuy - round.akrsToSell;
        round.currentAkrs += diffLand;
        round.currentBushels -= diffLand * params.AkrCost;
    }

    void LoseGame() {
        output->printLoseGame();
    }

    void EndGame() {
        int aph = round.currentAkrs / round.currentPopulation; // AkrPerHuman
        int percent = round.starvedPercent * 100 / GameRounds;
        OutputHandler::FinalMark mark;
        std::cout << "\nDEBUG _____" << aph << " " << percent << "\n";
        if (percent > 33 || aph < 7) mark = OutputHandler::BadMark;
        else if (percent > 10 || aph < 9) mark = OutputHandler::SatisfyingMark;
        else if (percent > 3 || aph < 10) mark = OutputHandler::GoodMark;
        else mark = OutputHandler::TerrificMark;
        output->printFinalReport(mark);
    }

    void StopGame() {
        FileSystem::SaveFile save_;
        save_.akrs = round.currentAkrs;
        save_.akrsToBuy = round.akrsToBuy;
        save_.akrsToSeed = round.akrsToSeed;
        save_.akrsToSell = round.akrsToSell;
        save_.bushels = round.currentBushels;
        save_.bushelsToEat = round.bushelsToEat;
        save_.population = round.currentPopulation;
        save_.round = round.roundIdx;
        save_.starvedPercent = round.starvedPercent;
        fs->saveGame(save_);
    }

    bool checkInputValuesCost() {
        bool result = true;
        int totalCost = (round.akrsToBuy - round.akrsToSell) * params.AkrCost;
        totalCost += float(round.akrsToSeed) * SeedCost;
        totalCost += round.bushelsToEat;
        if (totalCost > round.currentBushels)     
            result = false;
        if (round.akrsToSeed > BushProduction * round.currentPopulation)
            result = false;
        if (round.currentAkrs - round.akrsToSell + round.akrsToBuy < round.akrsToSeed)
            result = false;
        if (!result) {
            OutputHandler::MainResourses res(round.currentPopulation, round.currentBushels, round.currentAkrs);
            output->printWrongInputCount(res);
        }
        return result;
    }
private: 
    struct RoundCountedParams {
        int IncomePeople;
        bool PlagueHappened; // by God will
        int RatsRation; // by God will
        int BushelsPerAkr; // by God will
        int AkrCost; // by God will
        int BushelsRaised;
        int StarvedPeople;
        bool StartRound = false;
    };
    struct RoundInfo {
        int roundIdx;
        int currentAkrs;
        int currentBushels;
        int currentPopulation;
        float starvedPercent;
        int akrsToSell;
        int akrsToBuy;
        int akrsToSeed;
        int bushelsToEat;


        RoundInfo() : roundIdx(0), currentAkrs(startAkrs), currentBushels(startBushels),
            currentPopulation(startPopulation), starvedPercent(0.f), akrsToBuy(StartInit),
            akrsToSell(StartInit), akrsToSeed(StartInit), bushelsToEat(StartInit)
        {};
        RoundInfo(int idx, int akrs, int bush, int pop, float percent, 
            int toSell, int toBuy, int toSeed, int toEat) :
            roundIdx(idx), currentAkrs(akrs), currentBushels(bush),
            currentPopulation(pop),starvedPercent(percent), akrsToSell(toSell),
            akrsToBuy(toBuy), akrsToSeed(toSeed), bushelsToEat(toEat)
        {}
        RoundInfo& operator=(RoundInfo& other) {
            this->roundIdx = other.roundIdx;
            this->currentAkrs = other.currentAkrs;
            this->currentBushels = other.currentBushels;
            this->currentPopulation = other.currentPopulation;
            this->starvedPercent = other.starvedPercent;
            this->akrsToBuy = other.akrsToBuy;
            this->akrsToSell = other.akrsToSell;
            this->akrsToSeed = other.akrsToSeed;
            this->bushelsToEat = other.bushelsToEat;
            return *this;
        }
        RoundInfo& operator=(RoundInfo&& other) noexcept {
            this->roundIdx = other.roundIdx;
            this->currentAkrs = other.currentAkrs;
            this->currentBushels = other.currentBushels;
            this->currentPopulation = other.currentPopulation;
            this->starvedPercent = other.starvedPercent;
            this->akrsToBuy = other.akrsToBuy;
            this->akrsToSell = other.akrsToSell;
            this->akrsToSeed = other.akrsToSeed;
            this->bushelsToEat = other.bushelsToEat;
            return *this;
        }
        static const int startPopulation = 100;
        static const int startBushels = 2800;
        static const int startAkrs = 1000;
        static const int StartInit = 0;
    };

    RoundInfo round;
    RoundCountedParams params;
    OutputHandler* output = nullptr;
    InputHandler* input = nullptr;
    FileSystem* fs = nullptr;
    const int GameRounds = 10;
    const float SeedCost = 0.5f;
    const int PeopleRation = 20;
    const float loseThreashold = 0.45f;
    const int BushProduction = 20;
    bool Lost = false;
};

int main()
{
    GameManager game;
    game.RunGame();
}
