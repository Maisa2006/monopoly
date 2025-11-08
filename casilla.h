//
// Created by Nicol on 8/11/2025.
//

#ifndef CASILLA_H
#define CASILLA_H

// librerias necesarias
#include <cstudio>

struct Casilla {
  int posicion;
  int precio;
  bool dueño;
  string nombrePropietario;
  int renta;
  bool estaHipotecada;
  bool esFerrocarril;
  bool esServicio;

}

#endif //CASILLA_H
