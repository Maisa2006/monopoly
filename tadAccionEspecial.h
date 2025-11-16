#ifndef ACCIOENESPECIAL_H
#define ACCIOENESPECIAL_H

#include <vector>
#include <string>
using namespace std;

class AccionEspecial {
private:
    vector<string> acciones;

public:
    // constructor
    AccionEspecial();

    void registrarjugada(const string& jugada);

    string deshacer(); 

    bool estaVacio() const;

};


#endif