#ifndef PTABLE_H
#define PTABLE_H

#include <string>
#include <glm/glm.hpp>
using namespace std;

#define ND               -1
#define NA               -2
#define NF               -3
#define NO_DATA          ND
#define NOT_AVAILABLE    NA
#define NOT_FOUND        NF

#define ERROR_ATOMIC_NUMBER_OUT_OF_BOUNDS      -255

#define NUM_ELEMENTS    118

typedef struct {
    float r, g, b;
    glm::vec3 toVec3() const;
} AtomColorF;

typedef struct {
    int r, g, b;
    AtomColorF toFloat() const;
} AtomColor;

#define DEFAULT_COLOR   ((AtomColor){221, 119, 255}).toFloat()

class PeriodicTable {
public:
    static int getNumberFromSymbol(string symbol);
    static string getNameFromNumber(int number);
    static int getVanDerWallsRadiusFromSymbol(string symbol);
    static int getVanDerWallsRadiusFromNumber(int number);
    static AtomColorF getColorFromSymbol(string symbol);
    static AtomColorF getColorFromNumber(int number);

private:
    typedef struct {
        int  num;
        char symbol[3];
        char name[14];
        int  empirical;
        int  calculated;
        int  vanderWalls;
        int  covalent1;
        int  covalent3;
        int  metallic;
        AtomColor color;
    } Element;

    static constexpr Element PERIODIC_TABLE[NUM_ELEMENTS] = {
        {  1,     "H" ,     "hydrogen"     ,   25,      53,     120,      38,      ND,      NA,     {255, 255, 255}},
        {  2,     "He",     "helium"       ,  120,      31,     140,      32,      ND,      NA,     {  0, 255, 255}},
        {  3,     "Li",     "lithium"      ,  145,     167,     182,     134,      ND,     152,     {119,   0, 255}},
        {  4,     "Be",     "beryllium"    ,  105,     112,     153,      90,      85,     112,     {  0, 119,   0}},
        {  5,     "B" ,     "boron"        ,   85,      87,     192,      82,      73,      NA,     {255, 170, 119}},
        {  6,     "C" ,     "carbon"       ,   70,      67,     170,      77,      60,      NA,     { 34,  34,  34}},
        {  7,     "N" ,     "nitrogen"     ,   65,      56,     155,      75,      54,      NA,     { 34,  51, 255}},
        {  8,     "O" ,     "oxygen"       ,   60,      48,     152,      73,      53,      NA,     { 31, 240,  31}},
        {  9,     "F" ,     "fluorine"     ,   50,      42,     147,      71,      53,      NA,     {221, 119, 255}},
        { 10,     "Ne",     "neon"         ,  160,      38,     154,      69,      ND,      NA,     {  0, 255, 255}},
        { 11,     "Na",     "sodium"       ,  180,     190,     227,     154,      ND,     186,     {119,   0, 255}},
        { 12,     "Mg",     "magnesium"    ,  150,     145,     173,     130,     127,     160,     {  0, 119,   0}},
        { 13,     "Al",     "aluminium"    ,  125,     118,     184,     118,     111,     143,     {221, 119, 255}},
        { 14,     "Si",     "silicon"      ,  110,     111,     210,     111,     102,      NA,     {221, 119, 255}},
        { 15,     "P" ,     "phosphorus"   ,  100,      98,     180,     106,      94,      NA,     {255, 153,   0}},
        { 16,     "S" ,     "sulfur"       ,  100,      88,     180,     102,      95,      NA,     {255, 229,  34}},
        { 17,     "Cl",     "chlorine"     ,  100,      79,     175,      99,      93,      NA,     { 31, 240,  31}},
        { 18,     "Ar",     "argon"        ,   71,      71,     188,      97,      96,      NA,     {  0, 255, 255}},
        { 19,     "K" ,     "potassium"    ,  220,     243,     275,     196,      ND,     227,     {119,   0, 255}},
        { 20,     "Ca",     "calcium"      ,  180,     194,     231,     174,     133,     197,     {  0, 119,   0}},
        { 21,     "Sc",     "scandium"     ,  160,     184,     211,     144,     114,     162,     {221, 119, 255}},
        { 22,     "Ti",     "titanium"     ,  140,     176,      ND,     136,     108,     147,     {153, 153, 153}},
        { 23,     "V" ,     "vanadium"     ,  135,     171,      ND,     125,     106,     134,     {221, 119, 255}},
        { 24,     "Cr",     "chromium"     ,  140,     166,      ND,     127,     103,     128,     {221, 119, 255}},
        { 25,     "Mn",     "manganese"    ,  140,     161,      ND,     139,     103,     127,     {221, 119, 255}},
        { 26,     "Fe",     "iron"         ,  140,     156,      ND,     125,     102,     126,     {221, 119,   0}},
        { 27,     "Co",     "cobalt"       ,  135,     152,      ND,     126,      96,     125,     {221, 119, 255}},
        { 28,     "Ni",     "nickel"       ,  135,     149,     163,     121,     101,     124,     {221, 119, 255}},
        { 29,     "Cu",     "copper"       ,  135,     145,     140,     138,     120,     128,     {221, 119, 255}},
        { 30,     "Zn",     "zinc"         ,  135,     142,     139,     131,      ND,     134,     {221, 119, 255}},
        { 31,     "Ga",     "gallium"      ,  130,     136,     187,     126,     121,     135,     {221, 119, 255}},
        { 32,     "Ge",     "germanium"    ,  125,     125,     211,     122,     114,      NA,     {221, 119, 255}},
        { 33,     "As",     "arsenic"      ,  115,     114,     185,     119,     106,      NA,     {221, 119, 255}},
        { 34,     "Se",     "selenium"     ,  115,     103,     190,     116,     107,      NA,     {221, 119, 255}},
        { 35,     "Br",     "bromine"      ,  115,      94,     185,     114,     110,      NA,     {153,  34,   0}},
        { 36,     "Kr",     "krypton"      ,   ND,      88,     202,     110,     108,      NA,     {  0, 255, 255}},
        { 37,     "Rb",     "rubidium"     ,  235,     265,     303,     211,      ND,     248,     {119,   0, 255}},
        { 38,     "Sr",     "strontium"    ,  200,     219,     249,     192,     139,     215,     {  0, 119,   0}},
        { 39,     "Y" ,     "yttrium"      ,  180,     212,      ND,     162,     124,     180,     {221, 119, 255}},
        { 40,     "Zr",     "zirconium"    ,  155,     206,      ND,     148,     121,     160,     {221, 119, 255}},
        { 41,     "Nb",     "niobium"      ,  145,     198,      ND,     137,     116,     146,     {221, 119, 255}},
        { 42,     "Mo",     "molybdenum"   ,  145,     190,      ND,     145,     113,     139,     {221, 119, 255}},
        { 43,     "Tc",     "technetium"   ,  135,     183,      ND,     156,     110,     136,     {221, 119, 255}},
        { 44,     "Ru",     "ruthenium"    ,  130,     178,      ND,     126,     103,     134,     {221, 119, 255}},
        { 45,     "Rh",     "rhodium"      ,  135,     173,      ND,     135,     106,     134,     {221, 119, 255}},
        { 46,     "Pd",     "palladium"    ,  140,     169,     163,     131,     112,     137,     {221, 119, 255}},
        { 47,     "Ag",     "silver"       ,  160,     165,     172,     153,     137,     144,     {221, 119, 255}},
        { 48,     "Cd",     "cadmium"      ,  155,     161,     158,     148,      ND,     151,     {221, 119, 255}},
        { 49,     "In",     "indium"       ,  155,     156,     193,     144,     146,     167,     {221, 119, 255}},
        { 50,     "Sn",     "tin"          ,  145,     145,     217,     141,     132,      NA,     {221, 119, 255}},
        { 51,     "Sb",     "antimony"     ,  145,     133,     206,     138,     127,      NA,     {221, 119, 255}},
        { 52,     "Te",     "tellurium"    ,  140,     123,     206,     135,     121,      NA,     {221, 119, 255}},
        { 53,     "I" ,     "iodine"       ,  140,     115,     198,     133,     125,      NA,     {102,   0, 187}},
        { 54,     "Xe",     "xenon"        ,   ND,     108,     216,     130,     122,      NA,     {  0, 255, 255}},
        { 55,     "Cs",     "caesium"      ,  265,     298,     343,     225,      ND,     265,     {119,   0, 255}},
        { 56,     "Ba",     "barium"       ,  215,     253,     268,     198,     149,     222,     {  0, 119,   0}},
        { 57,     "La",     "lanthanum"    ,  195,      ND,      ND,     169,     139,     187,     {221, 119, 255}},
        { 58,     "Ce",     "cerium"       ,  185,      ND,      ND,     165,     131,     182,     {221, 119, 255}},
        { 59,     "Pr",     "praseodymium" ,  185,     247,      10,      11,     128,     182,     {221, 119, 255}},
        { 60,     "Nd",     "neodymium"    ,  185,     206,      20,      20,      ND,     181,     {221, 119, 255}},
        { 61,     "Pm",     "promethium"   ,  185,     205,      ND,      ND,      ND,     183,     {221, 119, 255}},
        { 62,     "Sm",     "samarium"     ,  185,     238,      ND,      ND,      ND,     180,     {221, 119, 255}},
        { 63,     "Eu",     "europium"     ,  185,     231,      ND,      ND,      ND,     180,     {221, 119, 255}},
        { 64,     "Gd",     "gadolinium"   ,  180,     233,      ND,      ND,     132,     180,     {221, 119, 255}},
        { 65,     "Tb",     "terbium"      ,  175,     225,      ND,      ND,      ND,     177,     {221, 119, 255}},
        { 66,     "Dy",     "dysprosium"   ,  175,     228,      ND,      ND,      ND,     178,     {221, 119, 255}},
        { 67,     "Ho",     "holmium"      ,  175,     226,      ND,      ND,      ND,     176,     {221, 119, 255}},
        { 68,     "Er",     "erbium"       ,  175,     226,      ND,      ND,      ND,     176,     {221, 119, 255}},
        { 69,     "Tm",     "thulium"      ,  175,     222,      ND,      ND,      ND,     176,     {221, 119, 255}},
        { 70,     "Yb",     "ytterbium"    ,  175,     222,      ND,      ND,      ND,     176,     {221, 119, 255}},
        { 71,     "Lu",     "lutetium"     ,  175,     217,      ND,     160,     131,     174,     {221, 119, 255}},
        { 72,     "Hf",     "hafnium"      ,  155,     208,      ND,     150,     122,     159,     {221, 119, 255}},
        { 73,     "Ta",     "tantalum"     ,  145,     200,      ND,     138,     119,     146,     {221, 119, 255}},
        { 74,     "W" ,     "tungsten"     ,  135,     193,      ND,     146,     115,     139,     {221, 119, 255}},
        { 75,     "Re",     "rhenium"      ,  135,     188,      ND,     159,     110,     137,     {221, 119, 255}},
        { 76,     "Os",     "osmium"       ,  130,     185,      ND,     128,     109,     135,     {221, 119, 255}},
        { 77,     "Ir",     "iridium"      ,  135,     180,      ND,     137,     107,     136,     {221, 119, 255}},
        { 78,     "Pt",     "platinum"     ,  135,     177,     175,     128,     110,     139,     {221, 119, 255}},
        { 79,     "Au",     "gold"         ,  135,     174,     166,     144,     123,     144,     {221, 119, 255}},
        { 80,     "Hg",     "mercury"      ,  150,     171,     155,     149,      ND,     151,     {221, 119, 255}},
        { 81,     "Tl",     "thallium"     ,  190,     156,     196,     148,     150,     170,     {221, 119, 255}},
        { 82,     "Pb",     "lead"         ,  180,     154,     202,     147,     137,      NA,     {221, 119, 255}},
        { 83,     "Bi",     "bismuth"      ,  160,     143,     207,     146,     135,      NA,     {221, 119, 255}},
        { 84,     "Po",     "polonium"     ,  190,     135,     197,      ND,     129,      NA,     {221, 119, 255}},
        { 85,     "At",     "astatine"     ,   ND,     127,     202,      ND,     138,      NA,     {221, 119, 255}},
        { 86,     "Rn",     "radon"        ,   ND,     120,     220,     145,     133,      NA,     {221, 119, 255}},
        { 87,     "Fr",     "francium"     ,   ND,      ND,     348,      ND,      ND,      ND,     {119,   0, 255}},
        { 88,     "Ra",     "radium"       ,  215,      ND,     283,      ND,     159,      ND,     {  0, 119,   0}},
        { 89,     "Ac",     "actinium"     ,  195,      ND,      ND,      ND,     140,      NA,     {221, 119, 255}},
        { 90,     "Th",     "thorium"      ,  180,      ND,      ND,      ND,     136,     179,     {221, 119, 255}},
        { 91,     "Pa",     "protactinium" ,  180,      ND,      ND,      ND,     129,     163,     {221, 119, 255}},
        { 92,     "U" ,     "uranium"      ,  175,      ND,     186,      ND,     118,     156,     {221, 119, 255}},
        { 93,     "Np",     "neptunium"    ,  175,      ND,      ND,      ND,     116,     155,     {221, 119, 255}},
        { 94,     "Pu",     "plutonium"    ,  175,      ND,      ND,      ND,      ND,     159,     {221, 119, 255}},
        { 95,     "Am",     "americium"    ,  175,      ND,      ND,      ND,      ND,     173,     {221, 119, 255}},
        { 96,     "Cm",     "curium"       ,  176,      ND,      ND,      ND,      ND,     174,     {221, 119, 255}},
        { 97,     "Bk",     "berkelium"    ,   ND,      ND,      ND,      ND,      ND,     170,     {221, 119, 255}},
        { 98,     "Cf",     "californium"  ,   ND,      ND,      ND,      ND,      ND,     186,     {221, 119, 255}},
        { 99,     "Es",     "einsteinium"  ,   ND,      ND,      ND,      ND,      ND,     186,     {221, 119, 255}},
        {100,     "Fm",     "fermium"      ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {101,     "Md",     "mendelevium"  ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {102,     "No",     "nobelium"     ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {103,     "Lr",     "lawrencium"   ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {104,     "Rf",     "rutherfordium",   ND,      ND,      ND,      ND,     131,      ND,     {221, 119, 255}},
        {105,     "Db",     "dubnium"      ,   ND,      ND,      ND,      ND,     126,      ND,     {221, 119, 255}},
        {106,     "Sg",     "seaborgium"   ,   ND,      ND,      ND,      ND,     121,      ND,     {221, 119, 255}},
        {107,     "Bh",     "bohrium"      ,   ND,      ND,      ND,      ND,     119,      ND,     {221, 119, 255}},
        {108,     "Hs",     "hassium"      ,   ND,      ND,      ND,      ND,     118,      ND,     {221, 119, 255}},
        {109,     "Mt",     "meitnerium"   ,   ND,      ND,      ND,      ND,     113,      ND,     {221, 119, 255}},
        {110,     "Ds",     "darmstadtium" ,   ND,      ND,      ND,      ND,     112,      ND,     {221, 119, 255}},
        {111,     "Rg",     "roentgenium"  ,   ND,      ND,      ND,      ND,     118,      ND,     {221, 119, 255}},
        {112,     "Cn",     "copernicium"  ,   ND,      ND,      ND,      ND,     130,      ND,     {221, 119, 255}},
        {113,     "Nh",     "nihonium"     ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {114,     "Fl",     "flerovium"    ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {115,     "Mc",     "moscovium"    ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {116,     "Lv",     "livermorium"  ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {117,     "Ts",     "tennessine"   ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}},
        {118,     "Og",     "oganesson"    ,   ND,      ND,      ND,      ND,      ND,      ND,     {221, 119, 255}}
    };
};

#endif