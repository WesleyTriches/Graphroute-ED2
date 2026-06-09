#include "digraph.cpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

vector<string> split(const string& line, char sep)
{
    vector<string> fields;
    stringstream ss(line);
    string campo;
    while (getline(ss, campo, sep))
        fields.push_back(campo);
    return fields;
}

char menu()
{
    char resposta;
    while (true) {
        cout << "\n==============================================\n";
        cout << "1. Exibir Grafo Completo\n"
             << "2. Encontrar Menor Caminho\n"
             << "3. Calcular o Diâmetro do Grafo\n"
             << "4. Identificar Roteadores Críticos\n"
             << "0. Sair\n";
        cout << "==============================================\n";
        cin >> resposta;
        cin.ignore();
        if (resposta == '0' || resposta == '1' || resposta == '2' || resposta == '3' || resposta == '4')
            return resposta;
        else
            cout << "Opção inválida! Digite um valor válido!\n";
    }
}

char submenu()
{
    char resposta;
    while (true) {
        cout << "Selecione o formato de saída do Graphviz:\n";
        cout << "1. Tela\n"
             << "2. Imagem (PNG)\n"
             << "3. Documento (PDF)\n";
        cout << "Opção: ";
        cin >> resposta;
        cin.ignore();
        if (resposta == '1' || resposta == '2' || resposta == '3')
            return resposta;
        else
            cout << "Opção inválida!\n";
    }
}

void run_graphviz(const string& base, char format)
{
    string dot = base + ".dot";
    string command;

    if (format == '1') {
        command = "dot -Tx11 " + dot + " &";
    }
    else if (format == '2') {
        command = "dot -Tpng " + dot + " -o " + base + ".png";
    }
    else if (format == '3') {
        command = "dot -Tpdf " + dot + " -o " + base + ".pdf";
    }

    system(command.c_str()); // converte uma std::string do C++ para texto no formato C
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo.log>\n";
        return 1;
    }

    graph::digraph g;

    ifstream in(argv[1]);
    if (!in) {
        cerr << "Erro: não foi possivel abrir o arquivo '" << argv[1] << "'\n";
        return 1;
    }

    string line;
    getline(in, line);
    while (getline(in, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back(); 
        vector<string> fields = split(line, ',');
        if (fields.size() < 6)
            continue;
        g.insert_link(fields[4], fields[5]); 
    }

    cout << "\nGrafo de roteamento inicializado!\n";
    cout << "Vertices unicos (IPs): " << g.node_count()
         << " | Arestas: " << g.edge_count() << "\n";

    char opcao;
    do {
        opcao = menu();

        if (opcao == '1') {
            char formato = submenu();
            g.show(argv[1], formato);
            
        }
        else if (opcao == '2') {
            string origem, destino;
            cout << "Digite o IP de Origem: ";
            getline(cin, origem);
            cout << "Digite o IP de Destino: ";
            getline(cin, destino);
            char formato = submenu();
            // TODO: generate_output(formato);
        }
        else if (opcao == '3') {
            // TODO: calculation_diameters();
        }
        else if (opcao == '4') {
            vector<pair<string, int>> ranking = g.critical_routers();

            cout << "\nTop 5 roteadores criticos:\n";

            int limit = 5;
            if ((int)ranking.size() < 5)
                limit = ranking.size();

            for (int i = 0; i < limit; i++) {
                cout << i + 1 << ". " << ranking[i].first
                     << " - Grau de entrada: " << ranking[i].second << "\n";
            }
        }

    } while (opcao != '0');

    cout << "Encerrando...\n";
    return 0;
}