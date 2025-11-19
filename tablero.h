#ifndef TABLERO_H
#define TABLERO_H

// librerias necesarias
#include <string>
#include "tadJugador.h"
#include <vector>
#include "THPropiedades.h"

using namespace std;

struct Casilla {
    int numCasilla;
    string nombre;
    string color;
    string tipo; // propiedad, impuesto, cárcel, ...
    int precio;

    string propietario; // nombre del jugador que posee la propiedad, vacío si no tiene propietario
    int numCasas; 
    bool tieneHotel;

    Casilla *siguiente;
    Casilla *anterior;
}; // acceder a atributos se usa el .

//Operaciones que hacian falta en tablero
bool seCompra(Casilla* casilla);


//para saber si tiene dueño 
bool tienePropietario(Casilla* casilla);

void asignarPropietario(Casilla* casilla, const string& jugador);

//cuando se esta en banca rota
void elimPropietario(Casilla* casilla);

bool construirCasa(Casilla* casilla);

//para esto se requieren 4 casa ya construidas 
bool construirHotel(Casilla* casilla);

int AlquilerAPagar(const Casilla* casilla, PropiedadDetallada* info);

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
