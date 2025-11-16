#ifndef TABLERO_H
#define TABLERO_H


#include <string>

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

Casilla* cargarCasillas(const string& archivo);

// mostrar las casillas
void mostrarCasillas(Casilla* inicio, int n);

// liberar memoria
void liberarTablero(Casilla* inicio);

void tableroVisual(Casilla* inicio);

#endif //TABLERO_H
