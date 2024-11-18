#include <iostream>
#include <cstdlib> // Para usar system()
#include <fstream>
#include <vector>
#include <string>
#include "Formulario.cpp"

using namespace std;

// Função para obter o timestamp atual
string getTimestamp() {
    time_t now = time(nullptr);
    now -= 3 * 3600; 
    char buf[80];
    strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", localtime(&now));
    return string(buf);
}

// Classe para gerenciar o log
class Logger {
private:
    ofstream logFile;

public:
    Logger(const string& fileName) {
        logFile.open(fileName, ios::app); // Abrir em modo de anexação
        if (!logFile) {
            cerr << "Erro ao abrir o arquivo de log!" << endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const string& message) {
        if (logFile.is_open()) {
            logFile << getTimestamp() << " " << message << endl;
        }
    }
};

// Função para listar os formulários disponíveis
void exibirFormularios(Logger& logger) {
    cout << "\nFormularios disponíveis:\n";
    logger.log("Listando formulários disponíveis.");
    system("ls *.bin");
}

// Função para salvar um formulário
void salvarFormulario(Formulario* formulario, Logger& logger) {
    ofstream outFile(formulario->getNomeFormulario() + ".bin", ios::binary);
    formulario->salvar(outFile);
    outFile.close();
    logger.log("Formulario '" + formulario->getNomeFormulario() + "' salvo com sucesso.");
    cout << "Formulario '" << formulario->getNomeFormulario() << "' salvo com sucesso.\n";
}

// Função para carregar um formulário
Formulario* carregarFormulario(const string& nomeArquivo, Logger& logger) {
    logger.log("Tentativa de carregar formulário: " + nomeArquivo);
    ifstream inFile(nomeArquivo + ".bin", ios::binary);
    if (!inFile) {
        logger.log("Erro: Arquivo não encontrado - " + nomeArquivo);
        cout << "Arquivo não encontrado. Digite um nome de arquivo existente!\n";
        return nullptr;
    }
    Formulario* formulario = Formulario::carregar(inFile);
    inFile.close();
    logger.log("Formulario carregado: " + nomeArquivo);
    cout << "->Formulario: " << nomeArquivo << "\n";
    return formulario;
}

// Função principal
int main() {
    Logger logger("log_execucao.txt");
    logger.log("Programa iniciado.");

    int opcao;
    do {
        cout << "\n";
        cout << "-------------Menu-------------\n";
        cout << "1. Criar novo formulario\n";
        cout << "2. Responder um formulario\n";
        cout << "3. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore();

        if (opcao == 1) {
            string nome;
            cout << "\nNome do formulario: ";
            getline(cin, nome);

            Formulario* formulario = new Formulario(nome);
            logger.log("Criando novo formulário: " + nome);

            int numPerguntasAbertas, numPerguntasMultipla, numPerguntasEscala;

            cout << "\nQuantas perguntas abertas deseja adicionar? ";
            cin >> numPerguntasAbertas;
            cin.ignore();

            for (int i = 0; i < numPerguntasAbertas; ++i) {
                string texto;
                cout << "Digite o texto da " << i + 1 << "ª pergunta aberta: ";
                getline(cin, texto);
                formulario->adicionarPergunta(new PerguntaAberta(texto));
            }

            cout << "\nQuantas perguntas de multipla escolha deseja adicionar? ";
            cin >> numPerguntasMultipla;
            cin.ignore();

            for (int i = 0; i < numPerguntasMultipla; ++i) {
                string texto;
                cout << "Digite o texto da " << i + 1 << "ª pergunta de multipla escolha: ";
                getline(cin, texto);

                int numOpcoes;
                do {
                    cout << "Número de Alternativas (Máximo 5): ";
                    cin >> numOpcoes;
                    cin.ignore();
                    if (numOpcoes > 5 || numOpcoes < 1) {
                        cout << "\nERRO: Coloque um número de opção válido!\n\n";
                    }
                } while (numOpcoes > 5 || numOpcoes < 1);

                vector<string> opcoes;
                for (int j = 0; j < numOpcoes; ++j) {
                    string opcao;
                    cout << "Digite a opcao " << j + 1 << ": ";
                    getline(cin, opcao);
                    opcoes.push_back(opcao);
                }
                formulario->adicionarPergunta(new PerguntaMultiplaEscolha(texto, opcoes));
            }

            cout << "\nQuantas perguntas de escala deseja adicionar? ";
            cin >> numPerguntasEscala;
            cin.ignore();

            for (int i = 0; i < numPerguntasEscala; ++i) {
                string texto;
                int min, max;
                cout << "\n";
                cout << "Digite o texto da " << i + 1 << "ª pergunta de escala: ";
                getline(cin, texto);
                do {
                    cout << "Digite o valor minimo da escala: ";
                    cin >> min;
                    cout << "Digite o valor maximo da escala: ";
                    cin >> max;
                    cin.ignore();
                    if (max <= min) {
                        cout << "Erro: o valor maximo deve ser maior que o valor minimo. tente novamente!\n\n";
                    }
                } while (max <= min);
                formulario->adicionarPergunta(new PerguntaEscala(texto, min, max));
            }

            salvarFormulario(formulario, logger);
            delete formulario;

        } else if (opcao == 2) {
            exibirFormularios(logger);
            int erro = 1;
            do {
                string nome;
                cout << "\nDigite o nome do formulario que queira responder (Digite sair para voltar ao menu): ";
                getline(cin, nome);
                cout << "\n";

                if (nome == "sair") {
                    erro = 0;
                    logger.log("Usuário optou por sair da opção de responder formulário.");
                } else {
                    Formulario* formulario = carregarFormulario(nome, logger);
                    if (formulario) {
                        formulario->coletarRespostas();
                        formulario->salvarRespostas(nome + "_respostas.txt");
                        logger.log("Respostas salvas para o formulário: " + nome);
                        delete formulario;
                        erro = 0;
                    }
                }
            } while (erro == 1);
        } else if (opcao != 3) {
            cout << "\nERRO: Opcao invalida! Escolha entre 1, 2 ou 3!\n";
            logger.log("Erro: Opção inválida escolhida.");
        }

    } while (opcao != 3);

    logger.log("Programa encerrado.");
    return 0;
}
