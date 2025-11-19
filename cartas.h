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
























/*
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>  // necesario para usar vector
#include "json.hpp"
#include "jugador.h"// corregido: usa el nombre exacto de tu archivo
#include "tablero.h"  // corregido: usa el nombre exacto de tu archivo

using namespace std;
using json = nlohmann::json;

// estructura que representa una carta del juego
struct Carta {
    string texto;   // texto descriptivo de la carta
    string accion;  // código que identifica la acción a ejecutar
    int valor;      // monto de dinero asociado (si aplica)
};

// clase que maneja las cartas de Suerte y Comunidad
class Cartas {
public:
    // carga cartas desde un archivo JSON hacia una cola específica
    void cargarCartas(const string &archivo, queue<Carta> &cola) {
        ifstream f(archivo); // abre el archivo
        if (!f.is_open()) { // verifica si se abrió correctamente
            cerr << "No se pudo abrir el archivo: " << archivo << endl;
            return;
        }
        json j; // objeto JSON
        f >> j; // lee el contenido del archivo al objeto JSON
        // itera sobre cada elemento del array JSON
        for (auto &item : j) {
            Carta c; // crea una nueva carta
            c.texto = item.value("texto", ""); // extrae el texto
            c.accion = item.value("accion", "NINGUNA"); // extrae la acción
            c.valor = item.value("valor", 0); // extrae el valor
            cola.push(c); // agrega la carta a la cola
        }
    }

    // inicializa ambas colas de cartas
    void inicializar() {
        cargarCartas("cartasSuerte.json", suerte); // carga cartas de Suerte
        cargarCartas("cartasComunidad.json", comunidad); // carga cartas de Comunidad
    }

    // toma una carta de Suerte y ejecuta su acción
    void tomarSuerte(jugador* jugador, Casilla* tablero, vector<jugador*>& todosJugadores) {
        Carta carta = tomarCarta(suerte); // saca carta de la cola
        ejecutarAccion(carta, jugador, tablero, todosJugadores); // ejecuta la acción
    }

    // toma una carta de Comunidad y ejecuta su acción
    void tomarComunidad(jugador* jugador, Casilla* tablero, vector<jugador*>& todosJugadores) {
        Carta carta = tomarCarta(comunidad); // saca carta de la cola
        ejecutarAccion(carta, jugador, tablero, todosJugadores); // ejecuta la acción
    }

private:

    queue<Carta> suerte;       // cola de cartas de Suerte
    queue<Carta> comunidad;    // cola de cartas de Comunidad
    // saca una carta de la cola, la muestra y la devuelve al final
    Carta tomarCarta(queue<Carta> &cola) {
        if (cola.empty()) return {"No hay cartas.", "NINGUNA", 0}; // verifica si hay cartas

        Carta carta = cola.front(); // obtiene la carta del frente
        cola.pop(); // la saca de la cola

        // muestra la carta al jugador
        cout << "\n+--------------------------------------------------------+" << endl;
        cout << "| CARTA: " << carta.texto << endl;
        cout << "+--------------------------------------------------------+\n" << endl;

        // si no es la carta de salir de cárcel, la regresa al final de la cola
        if (carta.accion != "SALIRCARCEL") {
            cola.push(carta); // la carta vuelve al final
        }

        return carta; // retorna la carta
    }

    // ejecuta la acción correspondiente según el código de la carta
    void ejecutarAccion(const Carta &carta, jugador* jug, Casilla* tablero, vector<jugador*>& todosJugadores) {
        const string &a = carta.accion; // obtiene el código de acción

        // acciones de movimiento
        if (a == "GO") {
            int posActual = jug->getPosicion(); // obtiene posición actual
            jug->mover(-posActual); // mueve a casilla 0 (GO)
            jug->actualizarDinero(200); // cobra $200
            cout << jug->getNombre() << " avanza a Salida y cobra $200" << endl;
        }
        else if (a == "RETROCEDER3") {
            jug->mover(-3); // retrocede 3 casillas
            cout << jug->getNombre() << " retrocede 3 casillas" << endl;
        }
        else if (a == "STCHARLES") {
            moverACasilla(jug, tablero, "St. Charles Place", true); // mueve a St. Charles
        }
        else if (a == "ILLINOIS") {
            moverACasilla(jug, tablero, "Illinois Avenue", true); // mueve a Illinois
        }
        else if (a == "READING") {
            moverACasilla(jug, tablero, "Reading Railroad", true); // mueve a Reading
        }
        else if (a == "BOARDWALK") {
            moverACasilla(jug, tablero, "Boardwalk", false); // mueve a Boardwalk
        }
        else if (a == "FERROCARRIL") {
            int cercano = buscarFerrocarrilMasCercano(jug->getPosicion()); // busca ferrocarril más cercano
            moverAPosicion(jug, cercano, true); // mueve al ferrocarril
            cout << jug->getNombre() << " va al ferrocarril mas cercano" << endl;
        }
        else if (a == "UTILIDAD") {
            int cercana = buscarUtilidadMasCercana(jug->getPosicion()); // busca utilidad más cercana
            moverAPosicion(jug, cercana, false); // mueve a la utilidad
            cout << jug->getNombre() << " va a la utilidad mas cercana" << endl;
        }
        else if (a == "CARCEL") {
            int posActual = jug->getPosicion(); // obtiene posición actual
            jug->mover(10 - posActual); // mueve a casilla 10 (cárcel)
            cout << jug->getNombre() << " va a la CARCEL directamente!" << endl;
        }
        else if (a == "SALIRCARCEL") {
            jug->agregarCarta("Salir de la Carcel Gratis"); // guarda la carta
            cout << jug->getNombre() << " guarda la carta de 'Salir de la carcel gratis'" << endl;
        }

        // acciones de cobro
        else if (a.find("COBRAR") == 0 || a == "DIVIDENDO50") {
            jug->actualizarDinero(carta.valor); // suma el dinero
            cout << jug->getNombre() << " cobra $" << carta.valor << endl;
        }

        // acciones de pago
        else if (a.find("PAGAR") == 0 && a != "PAGAR50JUGADORES") {
            jug->actualizarDinero(-carta.valor); // resta el dinero
            cout << jug->getNombre() << " paga $" << carta.valor << endl;
        }

        // acciones que involucran a otros jugadores
        else if (a == "PAGAR50JUGADORES") {
            // itera sobre todos los jugadores
            for (auto* otroJug : todosJugadores) {
                if (otroJug != jug) { // excepto el jugador actual
                    jug->actualizarDinero(-50); // el jugador actual paga
                    otroJug->actualizarDinero(50); // el otro jugador recibe
                }
            }
            cout << jug->getNombre() << " paga $50 a cada jugador" << endl;
        }
        else if (a == "COBRAR50JUGADORES") {
            // itera sobre todos los jugadores
            for (auto* otroJug : todosJugadores) {
                if (otroJug != jug) { // excepto el jugador actual
                    otroJug->actualizarDinero(-50); // el otro jugador paga
                    jug->actualizarDinero(50); // el jugador actual recibe
                }
            }
            cout << jug->getNombre() << " cobra $50 de cada jugador" << endl;
        }
        else if (a == "REPARACIONES") {
            // acción especial para reparaciones (requiere info de propiedades)
            cout << jug->getNombre() << " debe pagar reparaciones (implementar segun propiedades)" << endl;
        }
        else {
            // acción no reconocida
            cout << "Accion no implementada: " << a << endl;
        }

        // muestra el dinero actualizado del jugador
        cout << "Dinero actual de " << jug->getNombre() << ": $" << jug->getDinero() << endl;
    }

    // mueve al jugador a una casilla específica por nombre
    void moverACasilla(jugador* jug, Casilla* tablero, const string& nombreCasilla, bool pasaPorSalida) {
        int numCasilla = buscarNumeroCasilla(tablero, nombreCasilla); // busca el número de la casilla
        moverAPosicion(jug, numCasilla, pasaPorSalida); // mueve a esa posición
        cout << jug->getNombre() << " se mueve a " << nombreCasilla << endl;
    }

    // mueve al jugador a una posición específica
    void moverAPosicion(jugador* jug, int destino, bool pasaPorSalida) {
        int posActual = jug->getPosicion(); // obtiene la posición actual
        int espacios = destino - posActual; // calcula cuántos espacios mover

        if (espacios < 0) espacios += 40; // ajusta si da la vuelta al tablero

        // verifica si pasa por Salida (casilla 0)
        if (pasaPorSalida && (posActual + espacios >= 40 || destino < posActual)) {
            jug->actualizarDinero(200); // cobra $200 por pasar por Salida
            cout << "Paso por Salida y cobro $200" << endl;
        }

        jug->mover(espacios); // mueve al jugador
    }

    // busca el número de casilla por su nombre
    int buscarNumeroCasilla(Casilla* tablero, const string& nombre) {
        Casilla* actual = tablero; // empieza desde el inicio
        do {
            if (actual->nombre == nombre) { // compara nombres
                return actual->numCasilla; // retorna el número si lo encuentra
            }
            actual = actual->siguiente; // avanza a la siguiente casilla
        } while (actual != tablero); // recorre toda la lista circular
        return 0; // si no encuentra, retorna 0 (GO)
    }

    // busca el ferrocarril más cercano adelante
    int buscarFerrocarrilMasCercano(int posActual) {
        // los ferrocarriles están en las casillas: 5, 15, 25, 35
        int ferrocarriles[] = {5, 15, 25, 35};
        for (int f : ferrocarriles) { // itera sobre los ferrocarriles
            if (f > posActual) return f; // retorna el primero que encuentra adelante
        }
        return 5; // si no hay ninguno adelante, vuelve al primero
    }

    // busca la utilidad más cercana adelante
    int buscarUtilidadMasCercana(int posActual) {
        // las utilidades están en las casillas: 12, 28
        if (posActual < 12) return 12; // si está antes de la primera
        if (posActual < 28) return 28; // si está entre las dos
        return 12; // si pasó las dos, vuelve a la primera
    }
};
 */