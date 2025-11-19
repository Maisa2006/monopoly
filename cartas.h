#ifndef CARTAS_H
#define CARTAS_H

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include "json.hpp"
#include "tablero.h"
#include "tadJugador.h"

using json = nlohmann::json;
using namespace std;

// estructura de carta
struct Carta {
    string texto;
    string accion;
    int valor;
};

class Cartas {
public:
    // carga todas las cartas de suerte y comunidad desde sus archivos json
    void inicializar();

    // agarra carta de suerte
    void tomarSuerte(jugador* jug, Casilla* tablero, vector<jugador*>& todosJugadores);

    // agarra carta de la caja de comunidad
    void tomarComunidad(jugador* jug, Casilla* tablero, vector<jugador*>& todosJugadores);

private:
    queue<Carta> suerte;
    queue<Carta> comunidad;

    // cargar cartas desde json
    void cargarCartas(const string &archivo, queue<Carta> &cola);

    // muestra el contenido de la carta
    Carta tomarCarta(queue<Carta> &cola);

    // hacer la acción de cada carta
    void ejecutarAccion(const Carta &carta, jugador* jug, Casilla* tablero, vector<jugador*>& todosJugadores);

    // para las cartes que desplazan a los jugadores
    void moverACasilla(jugador* jug, Casilla* tablero, const string& nombreCasilla, bool pasaPorSalida);

    // mover al jugador a una casilla especifica, teniendo en ceunta la salida
    void moverAPosicion(jugador* jug, int destino, bool pasaPorSalida);

    // busca el número de la casilla que tenga el nombre indicado dentro del tablero
    int buscarNumeroCasilla(Casilla* tablero, const string& nombre);

    // devuelve el ferrocarril más cercano hacia adelante según la posición actual del jugador
    int buscarFerrocarrilMasCercano(int posActual);

    // devuelve el servicio más cercano hacia adelante según la posición actual del jugador
    int buscarServicioMasCercano(int posActual);
};

#endif






















>>>>>>> fcf99fe (algunos cambios)
