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
        vector<string> getPropiedades() const;
        vector<string> getCartas() const;


        //las operaciones a ejecutar 
        void mover(int espacios);
        void setPosicion(int nuevaPosicion);
        void actualizarDinero(int cantidad);
        bool pagar(int cantidad); //devuelve true si se pudo pagar, false si no

        //el operador por referencia en este caso se usa para no trabajar sobre una copia y que no se pueda modificar el valor original
        void agregarPropiedad(const string& propiedad);
        void eliminarPropiedad(const string& propiedad);
        void agregarCarta(const string& carta);

        //ver si esta en bancarrota
        bool estaBancarrota() const;

        //mostrar inforrmacion del jugador
        string mostrarJugador() const;
        
        void setDinero(int nuevoDinero);


};

#endif
