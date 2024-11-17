#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Pergunta {
protected:
    string textoPergunta;

public:
    Pergunta(const string& texto) : textoPergunta(texto) {}
    virtual ~Pergunta() {}

    virtual void exibirPergunta(int cont) const = 0;
    virtual void registrarResposta(int cont) = 0;
    virtual void salvarResposta(ofstream& outFile) const = 0;

    virtual void salvar(ofstream& outFile) const = 0;
    virtual void carregar(ifstream& inFile) = 0;
};
