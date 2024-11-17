#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Pergunta.cpp"
using namespace std;

class PerguntaMultiplaEscolha : public Pergunta {
private:
    vector<string> opcoes;
    int resposta;

public:
    PerguntaMultiplaEscolha(const string& texto, const vector<string>& opcoes)
        : Pergunta(texto), opcoes(opcoes), resposta(-1) {}

    void exibirPergunta(int cont) const override {
        cout << cont << "ª) " <<  textoPergunta << "\n";
        for (size_t i = 0; i < opcoes.size(); ++i) {
            cout << i + 1 << ". " << opcoes[i] << "\n";
        }
        cout << "Escolha uma das alternativas: " ;
    }

    void registrarResposta(int cont) override {
        do{
        cin >> resposta;
        cin.ignore();
        if(opcoes.size() < resposta || resposta == 0){
            cout << "\nERRO: Escolha uma opção válida! (Entre 1 e "<< opcoes.size() << ")\n\n";
            int contador = 1;
            exibirPergunta(cont);
        }
        }while(opcoes.size() < resposta || resposta == 0);
    }

    void salvarResposta(ofstream& outFile) const override {
        outFile << textoPergunta << "\nResposta: " << opcoes[resposta - 1] << "\n";
    }

    void salvar(ofstream& outFile) const override {
        int tipo = 2;
        outFile.write(reinterpret_cast<const char*>(&tipo), sizeof(tipo));
        size_t tamanhoTexto = textoPergunta.size();
        outFile.write(reinterpret_cast<const char*>(&tamanhoTexto), sizeof(tamanhoTexto));
        outFile.write(textoPergunta.c_str(), tamanhoTexto);

        size_t numOpcoes = opcoes.size();
        outFile.write(reinterpret_cast<const char*>(&numOpcoes), sizeof(numOpcoes));
        for (const string& opcao : opcoes) {
            size_t tamanhoOpcao = opcao.size();
            outFile.write(reinterpret_cast<const char*>(&tamanhoOpcao), sizeof(tamanhoOpcao));
            outFile.write(opcao.c_str(), tamanhoOpcao);
        }
    }

    void carregar(ifstream& inFile) override {
        size_t tamanhoTexto;
        inFile.read(reinterpret_cast<char*>(&tamanhoTexto), sizeof(tamanhoTexto));
        textoPergunta.resize(tamanhoTexto);
        inFile.read(&textoPergunta[0], tamanhoTexto);

        size_t numOpcoes;
        inFile.read(reinterpret_cast<char*>(&numOpcoes), sizeof(numOpcoes));
        opcoes.resize(numOpcoes);
        for (size_t i = 0; i < numOpcoes; ++i) {
            size_t tamanhoOpcao;
            inFile.read(reinterpret_cast<char*>(&tamanhoOpcao), sizeof(tamanhoOpcao));
            opcoes[i].resize(tamanhoOpcao);
            inFile.read(&opcoes[i][0], tamanhoOpcao);
        }
    }
};
