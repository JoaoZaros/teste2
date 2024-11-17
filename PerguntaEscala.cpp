#include <iostream>
#include <fstream>
#include <string>
#include "Pergunta.cpp"
using namespace std;

class PerguntaEscala : public Pergunta {
private:
    int resposta;
    int escalaMin, escalaMax;

public:
    PerguntaEscala(const string& texto, int min, int max)
        : Pergunta(texto), escalaMin(min), escalaMax(max), resposta(-1) {}

    void exibirPergunta(int cont) const override {
        cout << cont <<"ª) "<< textoPergunta << "\n(Escolha entre " << escalaMin << " a " << escalaMax << "): ";
    }

    void registrarResposta(int cont) override {
        do{
        cin >> resposta;
        cin.ignore();
        if(escalaMin > resposta || escalaMax < resposta){
            cout <<"\nERRO: Digite uma resposta válida! (Entre "<< escalaMin <<" e "<<escalaMax<<")\n\n";
            int contador = 1;
            exibirPergunta(cont);
        }

        } while(escalaMin > resposta || escalaMax < resposta);
    }

    void salvarResposta(ofstream& outFile) const override {
        outFile << textoPergunta << "\nResposta: " << resposta << "\n";
    }

    void salvar(ofstream& outFile) const override {
        int tipo = 3;
        outFile.write(reinterpret_cast<const char*>(&tipo), sizeof(tipo));
        size_t tamanhoTexto = textoPergunta.size();
        outFile.write(reinterpret_cast<const char*>(&tamanhoTexto), sizeof(tamanhoTexto));
        outFile.write(textoPergunta.c_str(), tamanhoTexto);
        outFile.write(reinterpret_cast<const char*>(&escalaMin), sizeof(escalaMin));
        outFile.write(reinterpret_cast<const char*>(&escalaMax), sizeof(escalaMax));
    }

    void carregar(ifstream& inFile) override {
        size_t tamanhoTexto;
        inFile.read(reinterpret_cast<char*>(&tamanhoTexto), sizeof(tamanhoTexto));
        textoPergunta.resize(tamanhoTexto);
        inFile.read(&textoPergunta[0], tamanhoTexto);
        inFile.read(reinterpret_cast<char*>(&escalaMin), sizeof(escalaMin));
        inFile.read(reinterpret_cast<char*>(&escalaMax), sizeof(escalaMax));
    }
};
