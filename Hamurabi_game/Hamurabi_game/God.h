#pragma once

#include <random>

class God {
public:
	static God& ask();
	~God();

	bool sendPlague();
	int sendBushelsProduction();
	int sendAkrCost();
	int sendRatsRation(int bushels_);
	

private:
	static God* instance_;
	int minBushelsFromAkr = 1;
	int maxBushelsFromAkr = 6;
	int  minAkrCost = 17;
	int maxAkrCost = 26;
	static constexpr float PLAGUE_CHANCE = 0.15f;
	static constexpr float RATS_MAX_RATION = 0.07f;
	std::uniform_int_distribution<int>* bushelsGodWill;
	std::uniform_int_distribution<int>* akrCostGodWill;
	std::uniform_real_distribution<float>* plagueGodWill;

	explicit God();
};
