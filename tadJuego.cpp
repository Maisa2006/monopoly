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



void Juego::turnoJugador() {
    jugador* j = obtenerJugadorActual();
    cout << "\nTurno de " << j->getNombre() << endl;

    
    
    if (turnosEnCarcel[j] > 0) {
        cout << j->getNombre() << " está en la cárcel. Turnos restantes: " << turnosEnCarcel[j] << endl;
        cout << "1. Pagar $50\n2. Intentar sacar dobles\n";
        int opcion; cin >> opcion;

        if (opcion == 1) {
            if (j->pagar(50)) {
                turnosEnCarcel[j] = 0;
                cout << j->getNombre() << " pagó $50 y salió de la cárcel.\n";
            } else {
                cout << "No tienes suficiente dinero.\n";
            }
        } else if (opcion == 2) {
            int d1 = lanzarDados();
            int d2 = lanzarDados();
            cout << "Dado 1: " << d1 << ", Dado 2: " << d2 << endl;
            if (d1 == d2) {
                turnosEnCarcel[j] = 0;
                cout << j->getNombre() << " sacó dobles y salió de la cárcel!\n";
                j->mover(d1 + d2);
                procesarCasilla(j);
            } else {
                turnosEnCarcel[j]--;
                cout << j->getNombre() << " no sacó dobles. Turnos restantes: " << turnosEnCarcel[j] << endl;
                if (turnosEnCarcel[j] == 0) {
                    j->pagar(50);
                    cout << j->getNombre() << " cumplió 3 turnos y pagó $50 para salir.\n";
                }
            }
        }
        siguienteTurno();
        return; // termina aquí el turno si estaba en cárcel
    }


    vecesDoble = 0;
    bool repetir = true;
    while (repetir) {
        int suma = lanzarDados();
        cout << "Dado 1: " << ultimoDado1 << ", Dado 2: " << ultimoDado2 << endl;

        bool esDoble = (ultimoDado1 == ultimoDado2);
        j->mover(suma);
        procesarCasilla(j);

        if (esDoble) {
            vecesDoble++;
            if (vecesDoble == 3) {
                cout << j->getNombre() << " sacó 3 dobles y va a la cárcel!\n";
                enviarACarcel(j);
                siguienteTurno();
                return;
            }
        } else {
            repetir = false;
            vecesDoble = 0;
            siguienteTurno();
        }
    }
}



void Juego::moverJugador(int espacios, jugador* j) {
    jugador* jugador = obtenerJugadorActual();

    // Crear jugada y registrar estado antes
    Jugada jugada;
    registrarUltimaJugada(jugada, jugador);

    int posActual = jugador->getPosicion();
    int nueva = (posActual + espacios) % 40;

    if (posActual + espacios >= 40) {
        jugador->actualizarDinero(200); // cobra 200 por pasar por salida
        cout << jugador->getNombre() << " pasó por salida y cobra $200!" << endl;
    }

    jugador->mover(espacios);
    procesarCasilla(jugador);

    // Registrar estado después
    registrarNuevaJugada(jugada, jugador);
    accionEspecial.registrarJugada(jugada);
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

bool Juego::tieneMonopolio(jugador* j, const string& color) {
    vector<string> props = j->getPropiedades();
    int totalColor = cantidadPorColor(tablaPropiedades, color);
    int tiene = 0;
    for (const string& p : props) {
        if (colorDePropiedad(tablaPropiedades, p) == color)
            tiene++;
    }
    return tiene == totalColor;
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

void Juego::deshacerTurno() {
    if (accionEspecial.estaVacio()) {
        cout << "No hay jugadas para deshacer.\n";
        return;
    }

    Jugada ultima = accionEspecial.deshacer();
    jugador* j = buscarJugadorPorNombre(ultima.jugador);

    if (!j) {
        cout << "No se encontró al jugador " << ultima.jugador << " para deshacer.\n";
        return;
    }

    // Restaurar solo lo que está permitido
    j->setPosicion(ultima.posicionAnterior);
    j->setDinero(ultima.dineroAnterior);

    cout << "Se deshizo la jugada de " << j->getNombre() << ".\n";
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








void Juego::mostrarTablero() const {
    tableroVisual(tablero, jugadores);
}

void Juego::sacarCartaSuerteDeTurno(jugador* j) {
    cartas.tomarSuerte(j, tablero, jugadores);
}
void Juego::sacarCartaComunidadDeTurno(jugador* j) {
    cartas.tomarComunidad(j, tablero, jugadores);
}


void Juego::infoPropiedadPorNumero(int num) const {
    PropiedadDetallada* info = buscarPorNumCasilla(tablaPropiedades, num);
    if (!info || !info->existe) {
        cout << "No hay información de propiedad para la casilla " << num << ".\n";
        return;
    }

    cout << " Propiedad en casilla " << num << ":\n";
    cout << " Nombre: " << info->nombre << "\n";
    cout << " Color: " << info->color << "\n";
    cout << " Precio: $" << info->precio << "\n";

    cout << " Alquiler: $" << info->alquiler[0] << "\n";
    cout << " 1 casa: $" << info->alquiler[1] << "\n";
    cout << " 2 casas: $" << info->alquiler[2] << "\n";
    cout << " 3 casas: $" << info->alquiler[3] << "\n";
    cout << " 4 casas: $" << info->alquiler[4] << "\n";
    cout << " Hotel: $" << info->alquiler[5] << "\n";
    cout << " Valor por construcción de cada casa: $" << info->valorCasa << "\n";
   
    cout << " Ferrocarril: " << (info->esFerrocarril ? "Sí" : "No") << "\n";
    cout << " Servicio: " << (info->esServicio ? "Sí" : "No") << "\n";
}


int Juego::getUltimoDado1() const {
    return ultimoDado1;
}

int Juego::getUltimoDado2() const {
    return ultimoDado2;
}

AccionEspecial& Juego::getAccionEspecial() {
    return accionEspecial;
}


jugador* Juego::buscarJugadorPorNombre(const string& nombre) const {
    for (jugador* j : jugadores) {
        if (j->getNombre() == nombre) {
            return j;
        }
    }
    return nullptr;
}


bool Juego::venderPropiedad(jugador* j, const string& nombrePropiedad) {
    vector<string> props = j->getPropiedades();
    bool laTiene = false;
    for (const string& p : props) {
        if (p == nombrePropiedad) {
            laTiene = true;
            break;
        }
    }

    if (!laTiene) {
        cout << j->getNombre() << " no tiene " << nombrePropiedad << ".\n";
        return false;
    }

    int precioBase = 0;
    for (int i = 0; i < 40; ++i) {
        if (tablaPropiedades->propiedades[i].existe &&
            tablaPropiedades->propiedades[i].nombre == nombrePropiedad) {
            precioBase = tablaPropiedades->propiedades[i].precio;
            break;
        }
    }

    if (precioBase == 0) {
        cout << "No se encontró información de " << nombrePropiedad << ".\n";
        return false;
    }

    int valorVenta = precioBase / 2;
    j->actualizarDinero(valorVenta);
    j->eliminarPropiedad(nombrePropiedad);
    cout << j->getNombre() << " vendió " << nombrePropiedad << " por $" << valorVenta << ".\n";

    return true;
}

bool Juego::comprarPropiedad(jugador* j) {
    int pos = j->getPosicion();
    PropiedadDetallada* info = buscarPorNumCasilla(tablaPropiedades, pos);
    if (!info || !info->existe) return false;

    if (j->getDinero() < info->precio) {
        cout << j->getNombre() << " no tiene suficiente dinero para comprar " << info->nombre << ".\n";
        return false;
    }

    j->actualizarDinero(-info->precio);
    j->agregarPropiedad(info->nombre);
    cout << j->getNombre() << " compró " << info->nombre << " por $" << info->precio << ".\n";
    return true;
}

Casilla* Juego::getTablero() const {
    return tablero;
}


