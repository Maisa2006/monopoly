#ifndef TADJUEGO_H
#define TADJUEGO_H

#include <string>
#include <vector>
#include <map>

#include "tadAccionEspecial.h"
#include "tadJugador.h"
#include "tablero.h"
#include "Cartas.h"
#include "THPropiedades.h"

using namespace std;

class Juego {
private: 
    vector<jugador*> jugadores;
    int turnoActual;
    Casilla* tablero;
    TablaPropiedades* tablaPropiedades;
    AccionEspecial accionEspecial;
    Cartas cartas;
    map<jugador*, int> turnosEnCarcel;
    bool juegoTerminado;

    //todo lo de dados
    int ultimoDado1;
    int ultimoDado2;
    int vecesDoble;

    void procesarCasilla(jugador* jugador);
    void enviarACarcel(jugador* jugador);
    void eliminarJugador(jugador* jugador);
    int buscarcasillaPorNum(int numCasilla);
    PropiedadDetallada* obtenerInfoPropiedad(int numCasilla);

public:
    Juego();
    ~Juego(); //destructor para liberar memoria

    void agregarJugador(const string& nombre, int dineroInicial);
    void iniciarJuego();

    jugador* obtenerJugadorActual() const;
    void siguienteTurno();

    int lanzarDados();
    void turnoJugador();
    void moverJugador(int espacios, jugador* j); 

    void cobrarAlquiler(jugador* jugador, const string& casilla);
    bool tieneMonopolio(jugador* jugador, const string& color);
    void verificarBancarrota(jugador* jugador);


    void registrarUltimaJugada(Jugada& jugada, jugador* jugador);
    void registrarNuevaJugada(Jugada& jugada, jugador* jugador);
    void deshacerTurno();

    void turnoEnCarcel(jugador* jugador);


    bool Victoria() const;
    string obtenerGanador() const;

    void mostrarEstadoJuego() const;


    void mostrarTablero() const;
    void infoPropiedadPorNumero(int num) const;
    int getUltimoDado1() const;
    int getUltimoDado2() const;
    AccionEspecial& getAccionEspecial();
    jugador* buscarJugadorPorNombre(const string& nombre) const;
    bool venderPropiedad(jugador* j, const string& nombrePropiedad);
    bool comprarPropiedad(jugador* j);
    void sacarCartaSuerteDeTurno(jugador* j);
    void sacarCartaComunidadDeTurno(jugador* j);
    Casilla* getTablero() const;


};




#endif