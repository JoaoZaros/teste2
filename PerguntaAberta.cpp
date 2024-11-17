#include <iostream>
#include <fstream>
#include <string>
#include "Pergunta.cpp"
using namespace std;

class PerguntaAberta : public Pergunta {
private:
    string resposta;

public:
    PerguntaAberta(const string& texto) : Pergunta(texto) {}

    void exibirPergunta(int cont) const override {
        cout << cont <<"ª) "<<textoPergunta << "\n";
    }

    void registrarResposta(int cont) override {
        getline(cin, resposta);
    }

    void salvarResposta(ofstream& outFile) const override {
        outFile << textoPergunta << "\nResposta: " << resposta << "\n";
    }

    void salvar(ofstream& outFile) const override {
        int tipo = 1;
        outFile.write(reinterpret_cast<const char*>(&tipo), sizeof(tipo));
        size_t tamanhoTexto = textoPergunta.size();
        outFile.write(reinterpret_cast<const char*>(&tamanhoTexto), sizeof(tamanhoTexto));
        outFile.write(textoPergunta.c_str(), tamanhoTexto);
    }

    void carregar(ifstream& inFile) override {
        size_t tamanhoTexto;
        inFile.read(reinterpret_cast<char*>(&tamanhoTexto), sizeof(tamanhoTexto));
        textoPergunta.resize(tamanhoTexto);
        inFile.read(&textoPergunta[0], tamanhoTexto);
    }
};
