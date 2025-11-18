#include "tadJugador.h"

#include <string>
#include <algorithm> // para usar find en eliminar propiedad

//constructor

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

//las propiedades y cartas son vectores por la facilidad de manejo que tienen en STL, se recorre, se agregan, se eliminan, etc... elementos facilmente.
vector<string> jugador::getPropiedades() const {
    return propiedades;
}

vector<string> jugador::getCartas() const {
    return cartas;
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

bool jugador::pagar(int cantidad) {
    if (dinero >= cantidad) {
        dinero -= cantidad;
        return true; // Pago exitoso
    } 

    dinero = 0;//manejo de errores para que el jugador no quede en negativo
    return false; // No se pudo pagar
}

//trabaja como un anspropiedad, agrega una propiedad al final del vector propiedades

void jugador::agregarPropiedad(const string& propiedad){
    propiedades.push_back(propiedad);
}

/*aqui en eliminar como utilizamos los iteradores, ent basicamente
funciona buscando la propiedad en el vector y si la encuentra 
lo elimina usando el metodo de STL erase()
*/
void jugador::eliminarPropiedad(const string& propiedad){ //recibe el parametro propiedad sin copiarlo
    //it = iterador, si encuentra la propiedad en el vector, este apunta al elemento
    //auto : detecta el tipo 
    auto it = find(propiedades.begin(), propiedades.end(), propiedad); //busca el valor propiedad en el vector propiedades
    if (it != propiedades.end()){ //si el iterador no esta en el ultimo elemento significa que encontro la propiedad
        propiedades.erase(it); //ent aqui se elimina esa propiedad 
    }
}

//esto trabaja como un anscarta, ya que agrega una carta al final del vector cartas 

void jugador::agregarCarta(const string& carta){
    cartas.push_back(carta);
}

//con un string mostramos toda la informacion del jugador

string jugador::mostrarJugador() const {
    string info = "Nombre: " + nombre + "\n";
    info += "Posicion: " + to_string(posicion) + "\n";
    info += "Dinero: $" + to_string(dinero) + "\n";

    info += "Propiedades: ";
    if(propiedades.empty()) { //si no hay ninguna propiedad
        info += "Ninguna\n";
    } else {
        info += "[";
        for (size_t i = 0; i < propiedades.size(); ++i) {
            info += propiedades[i];
            if (i < propiedades.size() - 1) { //para no poner la coma al final
                info += ", ";
            }
        }
    }
    info += "]\n";

    //lo mismo que propiedad pero para cartas
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
    if(dinero <= 0 && propiedades.empty()){
        return true; 

    } else {
        return false; 

    }
    
} 