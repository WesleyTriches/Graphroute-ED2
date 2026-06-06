#include "digraph.cpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


char menu(){
    char resposta;
    while (true){
        cout << "\nGrafo de roteamento inicializado!\n";
        cout << "==============================================\n";
        cout << "1. Exibir Grafo Completo\n"
        << "2. Encontrar Menor Caminho\n"
        << "3. Calcular o Diâmetro do Grafo\n"
        << "4. Identificar Roteadores Críticos\n"
        << "0. Sair\n";
        cout << "==============================================\n";
        cin >> resposta;
        cin.ignore();
        if(resposta == '0' || resposta == '1' || resposta == '2' || resposta == '3' || resposta == '4'){

            return resposta;
        }
        else{
            cout << "Opção Inválida! Digite um valor válido!";
        };
    }

}

bool validateInput(){

}



int main(){
    
    menu();
    graph::digraph g;

    g.insert_link("A", "B");
    g.insert_link("A", "C");
    g.insert_link("B", "D");
    g.insert_link("A", "B"); 
    g.insert_link("*", "E"); 
    g.insert_link("F", "*"); 

    return 0;

}   


