// librerias necesarias
#include "cartas.h"
#include <vector>
// #include "jugador.h"

// cargar cartas desde json
void Cartas::cargarCartas(const string &archivo, queue<Carta> &cola) {
    ifstream f(archivo); // abrir el archivo json
    if (!f.is_open()) { // verificar si se pudo abrir
        cerr << "No se pudo abrir el archivo: " << archivo << endl;
        return;
    }

    json j; // variable json
    f >> j; // leer json del archivo

    for (auto &item : j) {
        Carta c; // crear carta
        c.texto = item.value("texto", ""); // obtener texto
        c.accion = item.value("accion", "NINGUNA"); // obtener accion
        c.valor  = item.value("valor", 0); // obtener valor
        cola.push(c); // agregar carta a la cola
    }
}

// carga todas las cartas de suerte y comunidad desde sus archivos json
void Cartas::inicializar() {
    cargarCartas("cartasSuerte.json", suerte); // cargar cartas de suerte
    cargarCartas("cartasComunidad.json", comunidad); // cargar cartas de comunidad
}


// agarra carta de suerte
void Cartas::tomarSuerte(jugador* jug, Casilla* tablero, vector<jugador*>& todosJugadores) {
    Carta carta = tomarCarta(suerte); // tomar carta de suerte
    cout << jug->getNombre() << " sacó una carta de Suerte: " << carta.texto << endl;
    ejecutarAccion(carta, jug, tablero, todosJugadores); // ejecutar accion de la carta
}


// agarra carta de la caja de comunidad
void Cartas::tomarComunidad(jugador* jug, Casilla* tablero, vector<jugador*>& todosJugadores) {
    Carta carta = tomarCarta(comunidad); // tomar carta de comunidad
    cout << jug->getNombre() << " sacó una carta de Comunidad: " << carta.texto << endl;
    ejecutarAccion(carta, jug, tablero, todosJugadores); // ejecutar accion de la carta
}


// muestra el contenido de la carta
Carta Cartas::tomarCarta(queue<Carta> &cola) {
    if (cola.empty()) return {"No hay cartas.", "NINGUNA", 0}; // si no hay cartas, retornar carta vacia

    Carta carta = cola.front(); // obtiene carta del frente
    cola.pop(); // elimina la carta

    cout << " CARTA: " << carta.texto << endl; // muestra el texto de la carta

    if (carta.accion != "SALIRCARCEL") // si no es carta de salir de la carcel
        cola.push(carta); // agrega la carta al final

    return carta;
}


// hacer la acción de cada carta
void Cartas::ejecutarAccion(const Carta &carta, jugador* jug, Casilla* tablero, vector<jugador*>& todosJugadores) {
    const string &a = carta.accion;

    if (a == "GO") {
        int posActual = jug->getPosicion();
        jug->mover(-posActual);
        jug->actualizarDinero(200);
        cout << jug->getNombre() << " avanza a Salida y cobra $200" << endl;
    }
    else if (a == "RETROCEDER3") {
        jug->mover(-3);
        cout << jug->getNombre() << " retrocede 3 casillas" << endl;
    }
    else if (a == "STCHARLES") moverACasilla(jug, tablero, "St. Charles Place", true);
    else if (a == "ILLINOIS") moverACasilla(jug, tablero, "Illinois Avenue", true);
    else if (a == "READING") moverACasilla(jug, tablero, "Reading Railroad", true);
    else if (a == "BOARDWALK") moverACasilla(jug, tablero, "Boardwalk", false);

    else if (a == "FERROCARRIL") { // ferrocarril mas cercano
        int pos = buscarFerrocarrilMasCercano(jug->getPosicion()); // buscar posición
        moverAPosicion(jug, pos, true); // mueve al jugador
        cout << jug->getNombre() << " va al ferrocarril mas cercano" << endl;
    }
    else if (a == "SERVICIO") { // ir al ferrocarril máas cercano
        int pos = buscarServicioMasCercano(jug->getPosicion());
        moverAPosicion(jug, pos, false);
        cout << jug->getNombre() << " va al servicio más cercano" << endl;
    }
    else if (a == "CARCEL") { // va a la carcel
        int posActual = jug->getPosicion();
        jug->mover(10 - posActual);
        cout << jug->getNombre() << " va a la CARCEL directamente!" << endl;
    }
    else if (a == "SALIRCARCEL") { // carta se guarda
        jug->agregarCarta("Salir de la Carcel Gratis"); // se agrega al jugador
        cout << jug->getNombre() << " guarda la carta de 'Salir de la carcel gratis'" << endl;
    }

    // COBRAR

    else if (a.rfind("COBRAR", 0) == 0 || a == "DIVIDENDO50") {
        jug->actualizarDinero(carta.valor);
        cout << jug->getNombre() << " cobra $" << carta.valor << endl;
    }
    else if (a.rfind("PAGAR", 0) == 0 && a != "PAGAR50JUGADORES") {
        jug->actualizarDinero(-carta.valor);
        cout << jug->getNombre() << " paga $" << carta.valor << endl;
    }
    else if (a == "PAGAR50JUGADORES") {
        for (auto* otro : todosJugadores) {
            if (otro != jug) {
                jug->actualizarDinero(-50); // se le descuenta
                otro->actualizarDinero(50); // se le agrega al otro jugador
            }
        }
        cout << jug->getNombre() << " paga $50 a cada jugador" << endl;
    }
    else if (a == "COBRAR50JUGADORES") { // cobra a los jugadores 50
        for (auto* otro : todosJugadores) {
            if (otro != jug) {
                otro->actualizarDinero(-50); // se le descuenta a los jugadores
                jug->actualizarDinero(50); // se le suma al jugador
            }
        }
        cout << jug->getNombre() << " cobra $50 de cada jugador" << endl;
    }
    else if (a == "REPARACIONES") {
        cout << jug->getNombre() << " debe pagar reparaciones (por implementar)" << endl;
    }
    else { // acción no implementada
        cout << "Accion no implementada: " << a << endl;
    }

    cout << "Dinero actual de " << jug->getNombre() << ": $" << jug->getDinero() << endl;
}


// para las cartes que desplazan a los jugadores
void Cartas::moverACasilla(jugador* jug, Casilla* tablero, const string& nombreCasilla, bool pasaPorSalida) {
    int num = buscarNumeroCasilla(tablero, nombreCasilla); // buscar numero de la casilla
    moverAPosicion(jug, num, pasaPorSalida); // mover jugador a posicion
    cout << jug->getNombre() << " se mueve a " << nombreCasilla << endl; // mostrar movimiento
}


// mover al jugador a una casilla especifica, teniendo en ceunta la salida
void Cartas::moverAPosicion(jugador* jug, int destino, bool pasaPorSalida) {
    int posActual = jug->getPosicion(); // posicion actual
    int espacios = destino - posActual; // calcular diferencia
    if (espacios < 0) espacios += 40; // ajustar si da negativo

    if (pasaPorSalida && (posActual + espacios >= 40 || destino < posActual)) { // si pasa por salida
        jug->actualizarDinero(200); // cobrar 200
        cout << "Paso por Salida y cobro $200" << endl;
    }

    jug->mover(espacios); // mover jugador
}


// busca el número de la casilla que tenga el nombre indicado dentro del tablero
int Cartas::buscarNumeroCasilla(Casilla* tablero, const string& nombre) {
    Casilla* actual = tablero; // empezar desde el inicio
    do {
        if (actual->nombre == nombre) // si coincide
            return actual->numCasilla; // retornar numero
        actual = actual->siguiente; // siguiente casilla
    } while (actual != tablero); // hasta volver al inicio

    return 0; // si no se encontro
}


// devuelve el ferrocarril más cercano hacia adelante según la posición actual del jugador
int Cartas::buscarFerrocarrilMasCercano(int posActual) {
    int fer[] = {5, 15, 25, 35}; // posiciones de ferrocarriles
    for (int f : fer)
        if (f > posActual) return f; // retornar primero mayor
    return 5; // si no hay mayor, volver al primero
}


// devuelve el servicio más cercano hacia adelante según la posición actual del jugador
int Cartas::buscarServicioMasCercano(int posActual) {
    if (posActual < 12) return 12; // primer servicio
    if (posActual < 28) return 28; // segundo servico
    return 12; // vuelve al segundo
}
