#include "tadAccionEspecial.h"

AccionEspecial::AccionEspecial() {
    // Constructor vacío
}

void AccionEspecial::registrarjugada(const string& jugada) {
    acciones.push_back(jugada);
}

string AccionEspecial::deshacer() {
    if (!acciones.empty()) {
        string ultimaAccion = acciones.back();
        acciones.pop_back();
        return ultimaAccion;
    }
    return ""; // Retorna cadena vacía si no hay acciones
}

bool AccionEspecial::estaVacio() const {

    if(acciones.empty()){
        return true;
    } else {
        return false;
    }
    
}