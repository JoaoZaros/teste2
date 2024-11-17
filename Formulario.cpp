#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "PerguntaAberta.cpp"
#include "PerguntaEscala.cpp"
#include "PerguntaMultipla.cpp"

using namespace std;

class Formulario {
private:
    string nomeFormulario;
    vector<Pergunta*> perguntas;

public:
    Formulario(const string& nome) : nomeFormulario(nome) {}

    ~Formulario() {
        for (Pergunta* pergunta : perguntas) {
            delete pergunta;
        }
    }

    void adicionarPergunta(Pergunta* pergunta) {
        perguntas.push_back(pergunta);
    }

    void exibirFormulario() const {
        cout << "Formulario: " << nomeFormulario << "\n";
        for (const Pergunta* pergunta : perguntas) {
            int contador = 1;
            pergunta->exibirPergunta(contador);
        }
    }

    void coletarRespostas() {
        int contador = 1; // Inicia a contagem em 1
        for (Pergunta* pergunta : perguntas) {
            cout << "\n"; // Exibe o número da pergunta
            pergunta->exibirPergunta(contador);
            pergunta->registrarResposta(contador);
            contador++;
        }
    }

    void salvarRespostas(const string& filename) const {
        ofstream outFile(filename, ios::app);
        for (const Pergunta* pergunta : perguntas) {
            pergunta->salvarResposta(outFile);
        }
        outFile << "-----------------------------------------\n";
    }

    string getNomeFormulario() const {
        return nomeFormulario;
    }

    void salvar(ofstream& outFile) const {
        size_t tamanhoNome = nomeFormulario.size();
        outFile.write(reinterpret_cast<const char*>(&tamanhoNome), sizeof(tamanhoNome));
        outFile.write(nomeFormulario.c_str(), tamanhoNome);

        size_t numPerguntas = perguntas.size();
        outFile.write(reinterpret_cast<const char*>(&numPerguntas), sizeof(numPerguntas));
        for (const Pergunta* pergunta : perguntas) {
            pergunta->salvar(outFile);
        }
    }

    static Formulario* carregar(ifstream& inFile) {
        size_t tamanhoNome;
        inFile.read(reinterpret_cast<char*>(&tamanhoNome), sizeof(tamanhoNome));

        string nomeFormulario(tamanhoNome, '\0');
        inFile.read(&nomeFormulario[0], tamanhoNome);

        Formulario* formulario = new Formulario(nomeFormulario);

        size_t numPerguntas;
        inFile.read(reinterpret_cast<char*>(&numPerguntas), sizeof(numPerguntas));

        for (size_t i = 0; i < numPerguntas; ++i) {
            int tipo;
            inFile.read(reinterpret_cast<char*>(&tipo), sizeof(tipo));
            Pergunta* pergunta = nullptr;

            if (tipo == 1) {
                pergunta = new PerguntaAberta("");
            } else if (tipo == 2) {
                pergunta = new PerguntaMultiplaEscolha("", {});
            } else if (tipo == 3) {
                pergunta = new PerguntaEscala("", 0, 0);
            }

            pergunta->carregar(inFile);
            formulario->adicionarPergunta(pergunta);
        }
        return formulario;
    }

    
};
