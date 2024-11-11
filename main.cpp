#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Classe base Pergunta
class Pergunta {
protected:
    string textoPergunta;

public:
    Pergunta(const string& texto) : textoPergunta(texto) {}
    virtual ~Pergunta() {}

    virtual void exibirPergunta() const = 0;
    virtual void registrarResposta() = 0;
    virtual void salvarResposta(ofstream& outFile) const = 0;

    virtual void salvar(ofstream& outFile) const = 0;
    virtual void carregar(ifstream& inFile) = 0;
};

// Pergunta aberta
class PerguntaAberta : public Pergunta {
private:
    string resposta;

public:
    PerguntaAberta(const string& texto) : Pergunta(texto) {}

    void exibirPergunta() const override {
        cout << textoPergunta << " (Resposta aberta): ";
    }

    void registrarResposta() override {
        getline(cin, resposta);
    }

    void salvarResposta(ofstream& outFile) const override {
        outFile << textoPergunta << " Resposta: " << resposta << "\n";
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

// Pergunta de múltipla escolha
class PerguntaMultiplaEscolha : public Pergunta {
private:
    vector<string> opcoes;
    int resposta;

public:
    PerguntaMultiplaEscolha(const string& texto, const vector<string>& opcoes)
        : Pergunta(texto), opcoes(opcoes), resposta(-1) {}

    void exibirPergunta() const override {
        cout << textoPergunta << "\n";
        for (size_t i = 0; i < opcoes.size(); ++i) {
            cout << i + 1 << ". " << opcoes[i] << "\n";
        }
        cout << "Escolha uma opção: ";
    }

    void registrarResposta() override {
        cin >> resposta;
        cin.ignore();
    }

    void salvarResposta(ofstream& outFile) const override {
        outFile << textoPergunta << " Resposta: " << opcoes[resposta - 1] << "\n";
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

// Pergunta de escala
class PerguntaEscala : public Pergunta {
private:
    int resposta;
    int escalaMin, escalaMax;

public:
    PerguntaEscala(const string& texto, int min, int max)
        : Pergunta(texto), escalaMin(min), escalaMax(max), resposta(-1) {}

    void exibirPergunta() const override {
        cout << textoPergunta << " (De " << escalaMin << " a " << escalaMax << "): ";
    }

    void registrarResposta() override {
        cin >> resposta;
        cin.ignore();
    }

    void salvarResposta(ofstream& outFile) const override {
        outFile << textoPergunta << " Resposta: " << resposta << "\n";
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

// Classe Formulario
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
            pergunta->exibirPergunta();
        }
    }

    void coletarRespostas() {
        for (Pergunta* pergunta : perguntas) {
            pergunta->exibirPergunta();
            pergunta->registrarResposta();
        }
    }

    void salvarRespostas(const string& filename) const {
        ofstream outFile(filename, ios::app);
        outFile << "\n";
        for (const Pergunta* pergunta : perguntas) {
            pergunta->salvarResposta(outFile);
        }
        outFile << "\n------------------------------";
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

void salvarFormulario(Formulario* formulario) {
    ofstream outFile(formulario->getNomeFormulario() + ".bin", ios::binary);
    formulario->salvar(outFile);
    outFile.close();
    cout << "Formulario '" << formulario->getNomeFormulario() << "' salvo com sucesso.\n";
}

Formulario* carregarFormulario(const string& nomeArquivo) {
    ifstream inFile(nomeArquivo + ".bin", ios::binary);
    if (!inFile) {
        cout << "Arquivo não encontrado.\n";
        return nullptr;
    }
    Formulario* formulario = Formulario::carregar(inFile);
    inFile.close();
    return formulario;
}

int main() {
    int opcao;
    do {
        cout << "Menu:\n";
        cout << "1. Criar novo formulario\n";
        cout << "2. Responder um formulario\n";
        cout << "3. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore();

        if (opcao == 1) {
            string nome;
            cout << "Nome do formulario: ";
            getline(cin, nome);

            Formulario* formulario = new Formulario(nome);

            int numPerguntasAbertas, numPerguntasMultipla, numPerguntasEscala;

            cout << "Quantas perguntas abertas deseja adicionar? ";
            cin >> numPerguntasAbertas;
            cin.ignore();

            for (int i = 0; i < numPerguntasAbertas; ++i) {
                string texto;
                cout << "Digite o texto da pergunta aberta " << i + 1 << ": ";
                getline(cin, texto);
                formulario->adicionarPergunta(new PerguntaAberta(texto));
            }

            cout << "Quantas perguntas de multipla escolha deseja adicionar? ";
            cin >> numPerguntasMultipla;
            cin.ignore();

            for (int i = 0; i < numPerguntasMultipla; ++i) {
                string texto;
                cout << "Digite o texto da pergunta de multipla escolha " << i + 1 << ": ";
                getline(cin, texto);

                int numOpcoes;
                cout << "Quantas opcoes? ";
                cin >> numOpcoes;
                cin.ignore();

                vector<string> opcoes;
                for (int j = 0; j < numOpcoes; ++j) {
                    string opcao;
                    cout << "Digite a opcao " << j + 1 << ": ";
                    getline(cin, opcao);
                    opcoes.push_back(opcao);
                }
                formulario->adicionarPergunta(new PerguntaMultiplaEscolha(texto, opcoes));
            }

            cout << "Quantas perguntas de escala deseja adicionar? ";
            cin >> numPerguntasEscala;
            cin.ignore();

            for (int i = 0; i < numPerguntasEscala; ++i) {
                string texto;
                int min, max;
                cout << "Digite o texto da pergunta de escala " << i + 1 << ": ";
                getline(cin, texto);
                cout << "Digite o valor minimo da escala: ";
                cin >> min;
                cout << "Digite o valor maximo da escala: ";
                cin >> max;
                cin.ignore();
                formulario->adicionarPergunta(new PerguntaEscala(texto, min, max));
            }

            salvarFormulario(formulario);
            delete formulario;

        } else if (opcao == 2) {
            string nome;
            cout << "Digite o nome do formulario que deseja responder: ";
            getline(cin, nome);

            Formulario* formulario = carregarFormulario(nome);
            if (formulario) {
                formulario->coletarRespostas();
                formulario->salvarRespostas(nome + "_respostas.txt");
                delete formulario;
            }

        } else if (opcao != 3) {
            cout << "Opcao invalida!\n";
        }

    } while (opcao != 3);

    return 0;
}
