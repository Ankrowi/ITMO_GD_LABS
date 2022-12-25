#pragma once

struct City {
    //три основных ресурса
    int population;
    int bushels;
    int akrs;
    //детали изменений популяции людей
    int starved;
    int newCommers;
    //детали изменения бушелей пшеницы
    int bushelsForRats;
    int bushelsForPeople;
    int bushelsRaised;
    //детали изменения акров земли
    int akrsToSeed;
    int akrsToSell;
    int akrsToBuy;
 };