#ifndef ACCIOENESPECIAL_H
#define ACCIOENESPECIAL_H

#include <vector>
#include <string>
#include <stack> //se utiliza stack ya que definimos el objeto abstracto pila

using namespace std;

//para hacerlo mas organizado, el struct va a tener una parte de jugada antes y otra parte despues de la jugada
//se utiliza struct ya que la jugada es como un contenedor de datos, no un objeto con metodos
struct Jugada { 

    //estado antes de la jugada

    string jugador; 
    int posicionAnterior;
    int dineroAnterior;
    vector<string> propiedadesAnterior;
    vector<string> cartasAnterior;

    //estado despues de la jugada
    int posicionNueva;
    int dineroNuevo;
    vector<string> propiedadesNuevas;
    vector<string> cartasNuevas;

    string accion; 

};

class AccionEspecial {
private: 
    stack<Jugada> historial; //utilizamos pila como objeto abstracto para guardar las jugadas

public:
    void registrarJugada(const Jugada& jugada);
    Jugada deshacer();
    bool estaVacio() const;
    void reiniciar();

};


#endif