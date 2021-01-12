#include "ptable.h"

int PeriodicTable::getNumberFromSymbol(string symbol) {
    for (auto elem : PeriodicTable::PERIODIC_TABLE) {
        if (symbol.compare(elem.symbol) == 0)
            return elem.num;
    }
    return NOT_FOUND;
}


string PeriodicTable::getNameFromNumber(int number) {
    if (number < 1 || number > NUM_ELEMENTS)
        return "";
    return PeriodicTable::PERIODIC_TABLE[number-1].name;
}


int PeriodicTable::getVanDerWallsRadiusFromSymbol(string symbol) {
    return PeriodicTable::getVanDerWallsRadiusFromNumber(PeriodicTable::getNumberFromSymbol(symbol));
}


int PeriodicTable::getVanDerWallsRadiusFromNumber(int number) {
    if (number < 1 || number > NUM_ELEMENTS)
        return NOT_FOUND;
    return PeriodicTable::PERIODIC_TABLE[number-1].vanderWalls;
}