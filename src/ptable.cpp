#include "ptable.h"

constexpr PeriodicTable::Element PeriodicTable::PERIODIC_TABLE[NUM_ELEMENTS];

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


AtomColorF PeriodicTable::getColorFromSymbol(string symbol){
    return PeriodicTable::getColorFromNumber(PeriodicTable::getNumberFromSymbol(symbol));
}


AtomColorF PeriodicTable::getColorFromNumber(int number) {
    if (number < 1 || number > NUM_ELEMENTS)
        return DEFAULT_COLOR;
    return PeriodicTable::PERIODIC_TABLE[number-1].color.toFloat();
}


AtomColorF AtomColor::toFloat(void) const {
    return {r / 255.f, g / 255.f, b / 255.f};
}


glm::vec3 AtomColorF::toVec3(void) const {
    return glm::vec3(r, g, b);
}