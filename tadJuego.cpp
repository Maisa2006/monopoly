#include "tadJuego.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

//se implementa el construtor y el destructor de la clase jeugo
//para inicializar los atributos y cuando ya se acabe el juego liberar la memoria

Juego::Juego() {
    turnoActual = 0; 
    juegoTerminado = false;

    tablero = cargarCasillas("casillas.txt"); // carga el tablero desde archivo
    tablaPropiedades = cargarPropiedades("propiedades.txt"); // carga la tabla de propiedades

    cartas.inicializar(); // inicializa las cartas

    ultimoDado1 = 0;
    ultimoDado2 = 0;
    vecesDoble = 0;
}

Juego::~Juego(){

}

void Juego::agregarJugador(const string& nombre, int dineroInicial) {
    jugadores.push_back(new jugador(nombre, dineroInicial)); 
    turnosEnCarcel[jugadores.back()] = 0; // inicializa los turnos en cárcel para el nuevo jugador
}

void Juego::iniciarJuego() {
    turnoActual = 0; 
    juegoTerminado = false;
    cout << "Juego iniciado con " << jugadores.size() << " jugadores.\n";
}

jugador* Juego::obtenerJugadorActual() const {
    return jugadores[turnoActual];
}

void Juego::siguienteTurno() {
    turnoActual = (turnoActual + 1) % jugadores.size();
    vecesDoble = 0; // reinicia el conteo de dobles al cambiar de turno
}

int Juego::lanzarDados() {
    ultimoDado1 = rand() % 6 + 1; // dado 1
    ultimoDado2 = rand() % 6 + 1; // dado 2
    int suma = ultimoDado1 + ultimoDado2;
    return suma;
}

void Juego::turnoJugador(){
    jugador* jugador = obtenerJugadorActual();

    //primero se verifica si esta en la carcel 
    if(turnosEnCarcel[jugador] > 0){
        turnoEnCarcel(jugador);
        return; //si esta en la carcel no puede hacer nada mas en su turno 
    }

    cout << "\nTurno de " << jugador->getNombre() << endl;

    int suma = lanzarDados();

    cout << "Dado 1: " << ultimoDado1 << ", Dado 2: " << ultimoDado2 << endl;

    if (ultimoDado1 == ultimoDado2) {
        vecesDoble++;
        
        if (vecesDoble == 3) {
            cout << jugador->getNombre() << " ha sacado 3 dobles y va a la cárcel!" << endl;
            enviarACarcel(jugador);
            return; // termina el turno
        }
    } else {
        vecesDoble = 0; // reinicia el conteo de dobles
    }

    moverJugador(suma); // mueve al jugador

    if(ultimoDado1 == ultimoDado2) {
        cout << "sacaste par, vuelve a lanzar los dados!" << endl;
        turnoJugador(); // turno extra por sacar dobles
    }

}

void Juego::moverJugador(int espacios) {
    jugador* jugador= obtenerJugadorActual();

    int posActual = jugador->getPosicion();
    int nueva = (posActual + espacios) % 40;

    if(posActual + espacios >= 40){
        jugador->actualizarDinero(200); //cobra 200 por pasar por salida
        cout << jugador->getNombre() << " paso por salida y cobra $200!" << endl;
    }

    jugador->mover(espacios);
    procesarCasilla(jugador);
}



void Juego::procesarCasilla(jugador* jugador) {
    Casilla* casilla = tablero;

    for(int i = 0; i < jugador->getPosicion(); i++) {
        casilla = casilla->siguiente;
    }

    cout << jugador->getNombre() << " ha caido en " << casilla->nombre << endl;

    if(casilla->tipo == "propiedad") {
        if(casilla -> propietario == ""){
            cout << "Desea comprar " << casilla->nombre << " por $" << casilla->precio << "? (1 = si 0 = no): ";
            int opcion;
            cin >> opcion;
            if(opcion == 1){
                if(jugador->pagar(casilla->precio)){
                    jugador->agregarPropiedad(casilla->nombre);
                    casilla->propietario = jugador->getNombre();       
                    
                } else {
                    cout << "No tienes suficiente dinero para comprar esta propiedad." << endl;
                }
            }
        } else if(casilla->propietario != jugador->getNombre()) {
            cobrarAlquiler(jugador, casilla->nombre);
        }
    }

    if(casilla->tipo == "Suerte"){
        cartas.tomarSuerte(jugador, tablero, jugadores);
    } else if(casilla->tipo == "Comunidad"){
        cartas.tomarComunidad(jugador, tablero, jugadores);
    }
}

void Juego::enviarACarcel(jugador* jugador){
    cout << jugador->getNombre() << " es enviado a la cárcel!" << endl;
    jugador->setPosicion(10); // posición de la cárcel
    turnosEnCarcel[jugador] = 3; // establece 3 turnos en cárcel
}

void Juego::eliminarJugador(jugador* jugador) {
    cout << jugador->getNombre() << " ha quebrado y sale del juego.\n";
    turnosEnCarcel.erase(jugador);

    for (auto it = jugadores.begin(); it != jugadores.end(); it++) {
        if (*it == jugador) {
            jugadores.erase(it);
            return;
        }
    }
}

int Juego::buscarcasillaPorNum(int numCasilla) {
    Casilla* aux = tablero;
    for (int i = 0; i < 40; i++){
        if(aux->numCasilla == numCasilla){
            return i;
        }
        aux = aux->siguiente;
    }
    return -1; // no encontrada
}

PropiedadDetallada* Juego::obtenerInfoPropiedad(int numCasilla) {
    return buscarPorNumCasilla(tablaPropiedades, numCasilla);
}

void Juego::cobrarAlquiler(jugador* jugador, const string& nombreCasilla) {
    Casilla* casilla = tablero;
    while (casilla->nombre != nombreCasilla) {
        casilla = casilla->siguiente;
    }

    PropiedadDetallada* info = obtenerInfoPropiedad(casilla->numCasilla);
    if (!info || !info->existe) {
        cout << "Error: Información de propiedad no encontrada para " << nombreCasilla << endl;
        return;
    }

    int alquiler = AlquilerAPagar(casilla, info);
    cout << jugador->getNombre() << " debe pagar un alquiler de $" << alquiler << " a " << casilla->propietario << endl;

    if (jugador->pagar(alquiler)) {
        for (auto* otroJug : jugadores) {
            if (otroJug->getNombre() == casilla->propietario) {
                otroJug->actualizarDinero(alquiler);
                break;
            }
        }
    } else {
        cout << jugador->getNombre() << " no tiene suficiente dinero para pagar el alquiler!" << endl;
        verificarBancarrota(jugador);
    }
}

bool Juego::tieneMonopolio(jugador* jugador, const string& color){
    int total = 0; 
    int propias= 0; 

    Casilla* aux = tablero;

    for(int i = 0; i < 40; i++){
        if(aux->color == color){
            total++;
            if(aux->propietario == jugador->getNombre()){
                propias++;
            }
        }
        aux = aux->siguiente;
    }

    return (total == propias && total > 0);
}

void Juego::verificarBancarrota(jugador* jugador){
    if(jugador->estaBancarrota()){
        eliminarJugador(jugador);
        if(jugadores.size() == 1){
            juegoTerminado = true;
        }
    }
}

void Juego::registrarUltimaJugada(Jugada& jugada, jugador* jugador){
    accionEspecial.registrarJugada(jugada);
}

void Juego::registrarNuevaJugada(Jugada& jugada, jugador* jugador){
    accionEspecial.registrarJugada(jugada);
}

void Juego::deshacerTurno(){
    if(!accionEspecial.estaVacio()){
        Jugada jugada = accionEspecial.deshacer();
        cout << "jugada deshecha" << endl;
    }
}

void Juego::turnoEnCarcel(jugador* jugador){
    cout << jugador->getNombre() << " esta en la carcel. Turnos restantes: " << turnosEnCarcel[jugador] << endl;

    cout << "Elija una opcion para salir de la carcel:\n";
    cout << "1. Pagar $50\n";
    cout << "2. Intentar sacar dobles con los dados\n";
    int opcion;
    cin >> opcion;

    if(opcion == 1){
        if(jugador->pagar(50)){
            turnosEnCarcel[jugador] = 0; // sale de la carcel
            cout << jugador->getNombre() << " ha pagado $50 y sale de la carcel.\n";
        } else {
            cout << "No tienes suficiente dinero para pagar la multa.\n";
        }
    } else if(opcion == 2){
        int dado1 = rand() % 6 + 1;
        int dado2 = rand() % 6 + 1;
        cout << "Dado 1: " << dado1 << ", Dado 2: " << dado2 << endl;

        if(dado1 == dado2){
            turnosEnCarcel[jugador] = 0; // sale de la carcel
            cout << jugador->getNombre() << " ha sacado dobles y sale de la carcel!\n";
        } else {
            turnosEnCarcel[jugador]--;
            cout << jugador->getNombre() << " no saco dobles. Turnos restantes en la carcel: " << turnosEnCarcel[jugador] << endl;
        }
    }

    if(turnosEnCarcel[jugador] == 0){
        cout << jugador->getNombre() << " ya puede moverse en su proximo turno.\n";
    }
}

bool Juego::Victoria() const {
    return juegoTerminado;
}

string Juego::obtenerGanador() const {
    if (jugadores.size() == 1) {
        return jugadores[0]->getNombre();
    }
    return "No hay ganador aun.";
}

void Juego::mostrarEstadoJuego() const {
    cout << "Estado actual del juego:\n";
    for (const auto& jug : jugadores) {
        cout << jug->mostrarJugador() << endl;
    }
}






