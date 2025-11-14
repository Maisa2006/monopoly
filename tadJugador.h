#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <vector>

using namespace std;

class jugador { //la clase con los atributos de un jugador
    private: 

        string nombre;
        int posicion; 
        int dinero; 
        vector<string> propiedades;
        vector<string> cartas;

    public:
        //constructor 
        jugador(string nombre, int dineroInicial);


        //getters
        string getNombre() const;
        int getPosicion() const;
        int getDinero() const;


        //las operaciones a ejecutar 
        void mover(int espacios);
        void actualizarDinero(int cantidad);
        void agregarPropiedad(const string& propiedad);
        void agregarCarta(const string& carta);

        //y ahora hay que mostrar la info del jugador segun sus atributos

        string mostrarJugador() const;


};

#endif
