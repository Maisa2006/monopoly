#include "THPropiedades.h"
#include <fstream>
#include <sstream>
#include <iostream>

PropiedadDetallada::PropiedadDetallada() {
    nombre = "";
    color = "";
    precio = 0;
    valorCasa = 0;
    esFerrocarril = false;
    esServicio = false;
    existe = false;
    for(int i=0; i<6; i++) alquiler[i] = 0;
}

TablaPropiedades* crearTabla() {
    TablaPropiedades* tabla = new TablaPropiedades;
    for(int i=0; i<40; i++) tabla->propiedades[i] = PropiedadDetallada();
    return tabla;
}

TablaPropiedades* cargarPropiedades(const string& archivo) {
    ifstream fin(archivo);
    if(!fin.is_open()) {
        cout << "No se pudo abrir el archivo\n";
        return nullptr;
    }

    TablaPropiedades* tabla = crearTabla();
    string linea;
    PropiedadDetallada p;
    int numCasilla = -1;
    int estado = 0;

    while(getline(fin, linea)) {
        if(linea.empty()) continue;

        if(linea == "===") {
            if(numCasilla>=0 && numCasilla<40) {
                p.existe = true;
                tabla->propiedades[numCasilla] = p;
            }
            p = PropiedadDetallada();
            numCasilla = -1;
            estado = 0;
            continue;
        }

        try {
            switch(estado) {
                case 0: numCasilla = stoi(linea); estado++; break;
                case 1: p.nombre = linea; estado++; break;
                case 2: p.color = linea; estado++; break;
                case 3: p.precio = stoi(linea); estado++; break;
                case 4: {
                    stringstream ss(linea);
                    string a;
                    int i=0;
                    while(getline(ss, a, ',') && i<6) p.alquiler[i++] = stoi(a);
                    estado++; 
                    break;
                }
                case 5:
                    if(linea.substr(0,6) == "CASAS=") p.valorCasa = stoi(linea.substr(6));
                    else if(linea == "FERR") p.esFerrocarril = true;
                    else if(linea == "SERV") p.esServicio = true;
                    estado++; 
                    break;
            }
        } catch(...) {
            cout << "Error leyendo línea: " << linea << endl;
        }
    }

    fin.close();
    return tabla;
}

PropiedadDetallada* buscarPorNumCasilla(TablaPropiedades* tabla, int numCasilla) {
    if(numCasilla>=0 && numCasilla<40 && tabla->propiedades[numCasilla].existe)
        return &tabla->propiedades[numCasilla];
    return nullptr;
}

void liberarTabla(TablaPropiedades* tabla) {
    if(tabla) delete tabla;
}

int cantidadPorColor(TablaPropiedades* tabla, const string& color) {
    int total = 0;
    for (int i = 0; i < 40; ++i) {
        if (tabla->propiedades[i].existe && tabla->propiedades[i].color == color)
            total++;
    }
    return total;
}

string colorDePropiedad(TablaPropiedades* tabla, const string& nombre) {
    for (int i = 0; i < 40; ++i) {
        if (tabla->propiedades[i].existe && tabla->propiedades[i].nombre == nombre)
            return tabla->propiedades[i].color;
    }
    return "";
}
