#ifndef TADJUEGO_H
#define TADJUEGO_H

#include <string>
#include <vector>
#include "tadAccionEspecial.h"
#include "tadJugador.h"
//#include "tablero.h"
//#include "Cartas.h"

using namespace std;

class Juego {
private: 
    vector<jugador*> jugadores;
    int turnoActual;
    //Tablero tablero;
    AccionEspecial accionEspecial;
    //Cartas cartas;
    bool juegoTerminado;

public:
    Juego();
    void agregarJugador(const string& nombre, int dineroInicial);
    void iniciarJuego();

    jugador* obtenerJugadorActual() const;
    void siguienteTurno();

    int lanzarDados() const;
    void moverJugador(int espacios); 

    void cobrarAlquiler(jugador* jugador, const string& casilla);
    bool tieneMonopolio(jugador* jugador, const string& color);
    void verificarBancarrota(jugador* jugador);


    void registrarUltimaJugada(Jugada& jugada, jugador* jugador);
    void registrarNuevaJugada(Jugada& jugada, jugador* jugador);
    void deshacerTurno();


    bool Victoria() const;
    string obtenerGanador() const;


    void mostrarEstadoJuego() const;

};




#endif