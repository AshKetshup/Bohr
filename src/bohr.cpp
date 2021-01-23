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

/* Settings */
const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 900;

using namespace structur;

int main(int argc, char const *argv[]) {
    Bohr bohr(SCR_WIDTH, SCR_HEIGHT);
    if (!bohr.launchSuccessful())
        return -1;
    bohr.main();
    bohr.terminate();
    return 0;
}
