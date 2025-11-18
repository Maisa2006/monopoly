#ifndef THPROPIEDADES_H
#define THPROPIEDADES_H

#include <string>
using namespace std;

struct PropiedadDetallada {
    string nombre;
    string color;
    int precio;
    int alquiler[6];
    int valorCasa;
    bool esFerrocarril;
    bool esServicio;
    bool existe;

    PropiedadDetallada(); // constructor por defecto
};

struct TablaPropiedades {
    PropiedadDetallada propiedades[40];
};

// funciones
TablaPropiedades* crearTabla();
TablaPropiedades* cargarPropiedades(const string& archivo);
PropiedadDetallada* buscarPorNumCasilla(TablaPropiedades* tabla, int numCasilla);
void liberarTabla(TablaPropiedades* tabla);

#endif // THPROPIEDADES_H