#include <iostream>
#include <cstdlib> // Para usar system()
#include <fstream>
#include <vector>
#include <string>
#include "Formulario.cpp"

using namespace std;

// Função para listar os formulários disponíveis
void exibirFormularios() {
    cout << "\nFormularios disponíveis:\n";
    system("ls *.bin");
    //cout << "\n";
}

// Função para salvar um formulário
void salvarFormulario(Formulario* formulario) {
    ofstream outFile(formulario->getNomeFormulario() + ".bin", ios::binary);
    formulario->salvar(outFile);
    outFile.close();
    cout << "Formulario '" << formulario->getNomeFormulario() << "' salvo com sucesso.\n";
}

// Função para carregar um formulário
Formulario* carregarFormulario(const string& nomeArquivo) {
    ifstream inFile(nomeArquivo + ".bin", ios::binary);
    if (!inFile) {
        cout << "Arquivo não encontrado. Digite um nome de arquivo existente!\n";
        return nullptr;
    }
    Formulario* formulario = Formulario::carregar(inFile);
    inFile.close();
    cout << "->Formulario: " << nomeArquivo << "\n";
    return formulario;
}

// Função principal
int main() {
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

            salvarFormulario(formulario);
            delete formulario;

        } else if (opcao == 2) {
            exibirFormularios();
             int erro = 1;
            do{
            //listarFormularios();  // Exibir formulários disponíveis
            string nome;
            cout << "\nDigite o nome do formulario que queira responder (Digite sair para voltar ao menu): ";
            getline(cin, nome);
            cout << "\n";
            int verif = 0;
            if(nome =="sair"){
               verif += 1;
               erro -= 1;
            }
            if(verif == 0){
                Formulario* formulario = carregarFormulario(nome);
                if (formulario) {
                    formulario->coletarRespostas();
                    formulario->salvarRespostas(nome + "_respostas.txt");
                    delete formulario;
                    erro -= 1;
                }
            }
            }while(erro==1);
        } else if (opcao != 3) {
            cout << "\nERRO: Opcao invalida! Escolha entre 1, 2 ou 3!\n";
        }

    } while (opcao != 3);

    return 0;
}
