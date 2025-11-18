#include "tadAccionEspecial.h"

void AccionEspecial::registrarJugada(const Jugada& jugada) {
    historial.push(jugada);
}

Jugada AccionEspecial::deshacer() {
    if (historial.empty()){
        return Jugada();
    }
    Jugada ultima = historial.top();
    historial.pop();
    return ultima;
}

bool AccionEspecial::estaVacio() const {
    if (historial.empty()){
        return true;
    } else {
        return false;
    }
}

void AccionEspecial::reiniciar() {
    while (!historial.empty()) {
        historial.pop();
    }
}
