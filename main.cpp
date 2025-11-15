#include <iostream>
#include "tadJugador.h"
using namespace std;

int main() {

    // Crear un jugador
    jugador p("Pepito", 1500);

    // Probar mover()
    p.mover(5);
    p.mover(3); // ahora deberia estar en posición 8

    // Probar actualizarDinero()
    p.actualizarDinero(200);   // dinero 1700
    p.actualizarDinero(-500);  // dinero 1200

    // Probar agregarPropiedad()
    p.agregarPropiedad("Avenida Siempre Viva");
    p.agregarPropiedad("Parque Central");

    // Probar agregarCarta()
    p.agregarCarta("Salir de la cárcel");

    // Mostrar info
    cout << p.mostrarJugador() << endl;

    // Probar bancarrota
    cout << "¿Está en bancarrota?: "
         << (p.estaBancarrota() ? "Sí" : "No") << endl;

    return 0;
}
