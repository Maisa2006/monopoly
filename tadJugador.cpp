#include "tadJugador.h"

#include <string>

jugador::jugador(string nombre, int dineroInicial) 
    : nombre(nombre), posicion(0), dinero(dineroInicial) {}

//para conectar los getters
string jugador::getNombre() const {
    return nombre;
}

int jugador::getPosicion() const {
    return posicion;
}

int jugador::getDinero() const {
    return dinero;
}

//para las operaciones, tambien las conectamos con el archivo .h
void jugador::mover(int espacios){
    posicion = (posicion + espacios) % 40; //se utiliza 40 pq es el numero exactos d casillas que lleva el moopoly
   //se necesita una condicion para cuando el jugador retroceda
    if(posicion < 0) {
        posicion += 40; 
    }
}