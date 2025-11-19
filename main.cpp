#include "tadJuego.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib> 
#include <ctime>

using namespace std;

int main() {
    system("chcp 65001 > nul");
    srand(time(nullptr));

    Juego juego;

    int numJugadores;
    cout << "¿Cuántos jugadores van a jugar? ";
    cin >> numJugadores;
    cin.ignore();

    for (int i = 0; i < numJugadores; i++) {
        string nombre;
        cout << "Nombre del jugador " << (i + 1) << ": ";
        getline(cin, nombre);
        juego.agregarJugador(nombre, 1500);
    }

    juego.iniciarJuego();

    cout << "MOSTRAR TABLERO: Dibuja el tablero ASCII con jugadores\n";
    cout << "MOSTRAR ESTADO: Muestra dinero, propiedades y cartas de cada jugador\n";
    cout << "TURNO: Ejecuta el turno completo del jugador actual\n";
    cout << "CARCEL: Ejecuta lógica de cárcel para el jugador actual\n";
    cout << "INFO PROPIEDAD <num>: Muestra detalles de la propiedad en la casilla <num>\n";
    cout << "MONOPOLIO <color>: Verifica monopolio del color\n";
    cout << "DESHACER: Deshace la última jugada\n";
    cout << "COMPRAR: Comprar la propiedad en la casilla actual\n";
    cout << "VENDER <propiedad>: Vender propiedad\n";
    cout << "MOSTRAR CASILLAS: Lista todas las casillas del tablero\n";
    cout << "SALIR: Termina el juego\n";

    string linea;
    bool salir = false;

    while (!juego.Victoria() && !salir) {
        cout << "\n> ";
        getline(cin, linea);
        if (linea.empty()) continue;

        string comando;
        string args;
        {
            stringstream ss(linea);
            ss >> comando;
            string next;
            if (ss >> next) {
                comando += " " + next;
                getline(ss, args);
                if (!args.empty() && args[0] == ' ') args.erase(0, 1);
            }
        }

        if (comando == "MOSTRAR TABLERO") {
            juego.mostrarTablero();
        }
        else if (comando == "MOSTRAR ESTADO") {
            juego.mostrarEstadoJuego();
        }
        else if (comando == "TURNO") {
            juego.turnoJugador();
        }
        else if (comando == "INFO PROPIEDAD") {
            if (args.empty()) {
                cout << "Uso: INFO PROPIEDAD <num>\n";
            } else {
                try {
                    int num = stoi(args);
                    juego.infoPropiedadPorNumero(num);
                } catch (...) {
                    cout << "Número de casilla inválido.\n";
                }
            }
        }
        else if (comando == "MONOPOLIO") {
            if (args.empty()) {
                cout << "Uso: MONOPOLIO <color>\n";
            } else {
                auto* j = juego.obtenerJugadorActual();
                bool tiene = juego.tieneMonopolio(j, args);
                cout << j->getNombre() << (tiene ? " tiene" : " no tiene")
                     << " monopolio del color " << args << ".\n";
            }
        }
        else if (comando == "DESHACER") {
            juego.deshacerTurno();
        }
        else if (comando == "COMPRAR") {
            auto* j = juego.obtenerJugadorActual();
            juego.comprarPropiedad(j);
        }
        else if (comando == "VENDER") {
            if (args.empty()) {
                cout << "Uso: VENDER <nombre_propiedad>\n";
            } else {
                auto* j = juego.obtenerJugadorActual();
                juego.venderPropiedad(j, args);
            }
        }
        else if (comando == "MOSTRAR CASILLAS") {
            Casilla* inicio = juego.getTablero(); 
            mostrarCasillas(inicio, 40); 
        }
        else if (comando == "SALIR") {
            cout << "Juego terminado.\n";
            salir = true;
        }
        else {
            cout << "Comando no reconocido.\n";
        }
    }

    if (juego.Victoria()) {
        cout << "El ganador es: " << juego.obtenerGanador() << "\n";
    }

    return 0;
}
