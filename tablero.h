#ifndef TABLERO_H
#define TABLERO_H

// librerias necesarias
#include <string>
#include "jugador.h"
#include <vector>

using namespace std;

struct Casilla {
    int numCasilla;
    string nombre;
    string color;
    string tipo; // propiedad, impuesto, cárcel, ...
    int precio;
    Casilla *siguiente;
    Casilla *anterior;
}; // acceder a atributos se usa el .

// crear nueva casilla
Casilla* crearCasilla(int numCasilla, const string& nombre, const string& tipo, const string& color, int precio);

// muestra las casilla del tablero
Casilla* cargarCasillas(const string& archivo);

// mostrar las casillas
void mostrarCasillas(Casilla* inicio, int n);

// liberar memoria
void liberarTablero(Casilla* inicio);

// muestra el tablero de manera visual a los jugadores
void tableroVisual(Casilla* inicio, const vector<jugador*>& jugadores);

// funcion aux para mostrar el numero de casilla y los jugadores en ella
string casillaConJugadores(int numCasilla, const vector<jugador*>& jugadores);

#endif //TABLERO_H
