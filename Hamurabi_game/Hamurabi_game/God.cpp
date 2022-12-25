#include "God.h"

std::random_device rd;
std::mt19937 gen(rd());

God::God() {
	bushelsGodWill = new std::uniform_int_distribution<int>(minBushelsFromAkr, maxBushelsFromAkr);
	akrCostGodWill = new std::uniform_int_distribution<int>(minAkrCost, maxAkrCost);
	plagueGodWill = new std::uniform_real_distribution<float>(0.f, 1.f);
};

God::~God() {
	delete bushelsGodWill;
	delete akrCostGodWill;
	delete plagueGodWill;
};

God* God::instance_ = nullptr;

God& God::ask() {
	if (!instance_)
		instance_ = new God();
	return *instance_;
}

bool God::sendPlague() {
	return (*plagueGodWill)(gen) > PLAGUE_CHANCE ? false : true;
}

int God::sendBushelsProduction() {
	return (*bushelsGodWill)(gen);
}

int God::sendAkrCost() {
	return (*akrCostGodWill)(gen);
}

int God::sendRatsRation(int bushels_) {
	std::uniform_real_distribution<float> ratsRationGodWill =
		std::uniform_real_distribution<float>(0.f, (float)bushels_ * RATS_MAX_RATION);
	return static_cast<int>(ratsRationGodWill(gen));
}