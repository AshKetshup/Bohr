//------------------------------------------------------------------------------
//
// UNIVERSIDADE DA BEIRA INTERIOR
// Licenciatura em Engenharia Informática, Computação Gráfica
// Projeto Prático: BOHR
//
//     41266 --- Diogo Castanheira Simões (diogo.c.simoes@ubi.pt)
//     41381 --- Igor Cordeiro Bordalo Nunes (igor.bordalo.nunes@ubi.pt)
//
//------------------------------------------------------------------------------

#include "sism.h"

const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 900;

using namespace structur;

int main(int argc, char const *argv[]) {
    unsigned int width;
    unsigned int height;

    if (argc == 3) {
        width  = atoi(argv[1]);
        height = atoi(argv[2]);
    } else {
        width  =  SCR_WIDTH;
        height = SCR_HEIGHT;
    }

    Bohr bohr(width, height);

    if (!bohr.launchSuccessful())
        return -1;
    bohr.main();
    return 0;
}