#include "tadJugador.h"

#include <string>

Jugador::Jugador(string nombre, int dineroInicial) 
    : nombre(nombre), posicion(0), dinero(dineroInicial) {}