// librerias necesaria
#include <fstream>
#include <iostream>
#include <sstream>
#include "tablero.h"
#include "Cartas.h"
#include "tadJugador.h"
using namespace std;


// crear nueva casilla
Casilla* crearCasilla(int numCasilla, const string& nombre, const string& tipo, const string& color, int precio){
  Casilla* nueva = new Casilla;
  nueva->numCasilla = numCasilla;
  nueva->nombre = nombre;
  nueva->color = color;
  nueva->tipo = tipo;
  nueva->precio = precio;
  nueva->siguiente = nullptr;
  nueva->anterior = nullptr;
  return nueva;
}


//las funciones logicas que faltaban por implementar 

bool seCompra(Casilla* casilla){
    return (casilla->tipo == "Propiedad" || casilla->tipo == "Estación" || casilla->tipo == "Servicio") && casilla->propietario.empty();
}

bool tienePropietario(Casilla* casilla){
    return (!casilla->propietario.empty());
}

void asignarPropietario(Casilla* casilla, const string& jugador){
    casilla->propietario = jugador;
}

void elimPropietario(Casilla* casilla){
    casilla->propietario = "";
    casilla->numCasas = 0;
    casilla->tieneHotel = false;
}

bool construirCasa(Casilla* casilla){
    if(casilla->tipo != "Propiedad"){
        return false;
    }  // solo propiedades pueden tener casas
    if(casilla->tieneHotel) {
        return false; 
    }// no se pueden construir mas casas si ya hay hotel
    if(casilla->numCasas >= 4) {
        return false;
    }  // maximo 4 casas

    casilla->numCasas++; // construye una casa
    return true;
}

bool construirHotel(Casilla* casilla){
    if(casilla->tipo != "Propiedad"){
        return false;
    }  // solo propiedades pueden tener hotel
    if(casilla->tieneHotel) {
        return false; 
    } // ya tiene hotel
    if(casilla->numCasas < 4) {
        return false;
    }  // necesita 4 casas para construir hotel

    casilla->numCasas = 0; // remueve las casas
    casilla->tieneHotel = true; // construye el hotel
    return true;
}

int AlquilerAPagar(const Casilla* casilla, PropiedadDetallada* info){
    
if (casilla == NULL || info == NULL) { //verificar si los punteros son nulos
        return 0;
    }

    // alquiler nivel 5
    if (casilla->tieneHotel) {
        return info->alquiler[5];
    }

    // CASAS (1 a 4)
    if (casilla->numCasas >= 1 && casilla->numCasas <= 4) {
        return info->alquiler[casilla->numCasas];
    }

    // alquiler básico
    return info->alquiler[0]; 
}

// muestra las casilla del tablero
Casilla* cargarCasillas(const string& archivo) {
    ifstream fin(archivo); // abrir archivo en modo lectura
    if (!fin.is_open()) { // verifica si el archivo se abrió de forma correcta
        cout << "No se pudo abrir el archivo\n";
        return nullptr; // retorna puntero nulo porque la función es de tipo puntero
    }

    Casilla* primera = nullptr; // primera casilla
    Casilla* ultima = nullptr; // última casilla
    string linea; // variable que va a almacenar cada linea del casillas.txt

    // leer el archivo linea a linea
    while (getline(fin, linea)) {
        if (linea.empty()) continue; // en caso de que haya una linea vacía

        // eliminar el ; final si existe
        if (linea.back() == ';') linea.pop_back();

        stringstream ss(linea); // convertir en flujo de texto para leer cada campo separado por ,
        string numCasillaStr, nombre, tipo, color, precioStr; // variables que van a almacenar cada campo de la linea

        // leer campos separados por ,
        getline(ss, numCasillaStr, ',');
        int num = stoi(numCasillaStr); // casteo de string a entero

        getline(ss, nombre, ',');
        getline(ss, tipo, ',');

        // inicializar valores por defecto
        color = "-"; // si no hay color, se usa -
        int precio = 0; //precio inicial

        // si queda más parámetros, leer color y precio según el tipo de casilla
        if (ss.good()) {
            string resto; // variable temporal
            getline(ss, resto, ',');
            if (tipo == "Propiedad" || tipo == "Estación" || tipo == "Servicio") {
                color = resto; // campo correspondiente al color
                if (ss.good()) {
                    string precioStr2;
                    getline(ss, precioStr2, ',');// lee el siguiente campo separado por coma y lo guardar en la variable temporal
                    precio = stoi(precioStr2); // casteo
                }
            } else if (tipo == "Especial" && ss.good()) {
                // para casos como Income Tax o Luxury Tax
                precio = stoi(resto);
            }
        }

        Casilla* nueva = crearCasilla(num, nombre, tipo, color, precio);

        // enlazar lista
        if (!primera) {
            primera = nueva; // si es la primera casilla, inicializar punteros
            ultima = nueva;
        } else { // si ya hay casillas enlazar al final
            ultima->siguiente = nueva;
            nueva->anterior = ultima;
            ultima = nueva;
        }
    }

    // hacer la lista circular
    if (primera) {
        primera->anterior = ultima;
        ultima->siguiente = primera;
    }

    fin.close(); // cierra el archivo
    return primera;
}


// mostrar las casillas
void mostrarCasillas(Casilla* inicio, int n) {
    if (!inicio) return; // si el puntero de inicio es nulo, no hay tablero

    Casilla* actual = inicio; // puntero aux para recorrer la lista

    // mostrar las primeras n casillas
    for (int i = 0; i < n; i++) {
        cout << actual->numCasilla << ": " << actual->nombre
             << " - " << actual->tipo
             << " - " << actual->color
             << " - $" << actual->precio << "\n";
        actual = actual->siguiente;
    }
}


// liberar memoria
void liberarTablero(Casilla* inicio) {
    if (!inicio) return; // si el puntero de inicio es nulo, no hay tablero

    Casilla* actual = inicio; // puntero aux para recorrer la lista
    Casilla* siguiente;

    // deshacer la lista circular
    inicio->anterior->siguiente = nullptr;
    inicio->anterior = nullptr;

    // recorrer la lista normal para liberar cada nodo
    while (actual) {
        siguiente = actual->siguiente; // guarda el sig nodo
        delete actual; // libera nodo actual
        actual = siguiente; // avanza al sig nodo
    }
}


// muestra el tablero de manera visual a los jugadores
void tableroVisual(Casilla* inicio, const vector<jugador*>& jugadores) {
    if (!inicio) return; // si el puntero de inicio es nulo, no hay tablero

    // array con todas las casillas
    Casilla* casillas[40];
    Casilla* actual = inicio;

    // recorre la lista circular y guarda cada casilla en el arreglo
    for (int i = 0; i < 40; i++) {
        casillas[i] = actual; // casilla actual
        actual = actual->siguiente; // sig casila
    }

    // formato
    // fila superior (casillas 20-30)
    cout << "+----+----+----+----+----+----+----+----+----+----+----+\n";
    cout << "|";
    for (int i = 20; i <= 30; i++){
        cout << " " << casillaConJugadores(casillas[i]->numCasilla, jugadores) << " |";

    }
    cout << "\n+----+----+----+----+----+----+----+----+----+----+----+\n";

    // filas del medio (lado izquierdo y derecho)
    for (int fila = 0; fila < 9; fila++) {
        int izq = 19 - fila;
        int der = 31 + fila;

        cout << "| " << casillaConJugadores(casillas[izq]->numCasilla, jugadores) << " |";

        // espacio central
        cout << "                                            ";

        // casilla derecha
        cout << "| " << casillaConJugadores(casillas[der]->numCasilla, jugadores) << " |\n";
        cout << "+----+                                            +----+\n";
    }

    // fila inferior (casillas 10-0)
    cout << "|    |----+----+----+----+----+----+----+----+----|    |\n";
    cout << "|";
    for (int i = 10; i >= 0; i--) {
        if (casillas[i]->numCasilla < 10) // números de una cifra, espacio de más
            cout << " " << casillaConJugadores(casillas[i]->numCasilla, jugadores) << "  |";
        else
            cout << " " << casillaConJugadores(casillas[i]->numCasilla, jugadores) << " |";
    }
    cout << "\n+----+----+----+----+----+----+----+----+----+----+----+\n";
}


// funcion aux para mostrar el numero de casilla y los jugadores en ella
string casillaConJugadores(int numCasilla, const vector<jugador*>& jugadores) {
    string celda = to_string(numCasilla); // empieza con el numero de la casilla
    for (auto j : jugadores) {
        if (j->getPosicion() == numCasilla)
            celda += j->getNombre().substr(0, 2); // dos primeras iniciales del jugador
    }
    return celda;
}