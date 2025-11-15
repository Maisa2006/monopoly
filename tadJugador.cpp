#include "tadJugador.h"

#include <string>

jugador::jugador(string nombre, int dineroInicial) 
    : nombre(nombre), posicion(0), dinero(dineroInicial) {}

//para conectar los getters
string jugador::getNombre() const {
    return nombre;
}

int jugador::getPosicion() const {
    return posicion;
}

int jugador::getDinero() const {
    return dinero;
}

//para las operaciones, tambien las conectamos con el archivo .h
void jugador::mover(int espacios){
    posicion = (posicion + espacios) % 40; //se utiliza 40 pq es el numero exactos d casillas que lleva el moopoly
   //se necesita una condicion para cuando el jugador retroceda
    if(posicion < 0) {
        posicion += 40; 
    }
}

void jugador::actualizarDinero(int cantidad){
    dinero += cantidad;
    if(dinero < 0) {
        dinero = 0; // Evitar que el dinero sea negativo
    }
}

void jugador::agregarPropiedad(const string& propiedad){
    propiedades.push_back(propiedad);
}

void jugador::agregarCarta(const string& carta){
    cartas.push_back(carta);
}

string jugador::mostrarJugador() const {
    string info = "Nombre: " + nombre + "\n";
    info += "Posicion: " + to_string(posicion) + "\n";
    info += "Dinero: $" + to_string(dinero) + "\n";

    info += "Propiedades: ";
    if(propiedades.empty()) {
        info += "Ninguna\n";
    } else {
        info += "[";
        for (size_t i = 0; i < propiedades.size(); ++i) {
            info += propiedades[i];
            if (i < propiedades.size() - 1) {
                info += ", ";
            }
        }
    }
    info += "]\n";

    info += "Cartas: ";
    if(cartas.empty()) {
        info += "Ninguna\n";
    } else {
        info += "[";
        for (size_t i = 0; i < cartas.size(); ++i) {
            info += cartas[i];
            if (i < cartas.size() - 1) {
                info += ", ";
            }
        }
    }
    info += "]\n";

    return info;
}

bool jugador::estaBancarrota() const {
    return (dinero <= 0 && propiedades.empty()); //para estar en bancarrota debe no tener dinero ni propiedades
} 