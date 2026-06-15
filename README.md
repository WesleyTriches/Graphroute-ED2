# GraphRoute - Trabalho Final de Estrutura de Dados II

#### Desenvolvido por: Wesley Triches, Alberto Luiz Marsaro Neto e Luis Gustavo Cavalheiro Rossal.

## O QUE É?

É um projeto desenvolvido para a matéria de Estrutura de Dados II do curso de Ciência da Computação.

O problema trabalhado é a análise de rotas de rede a partir de arquivos de log no formato `.log`.

O programa lê dados de traceroute, monta um grafo direcionado usando os endereços IP encontrados no arquivo e permite realizar algumas análises sobre essa rede, como:

* Exibir o grafo completo;
* Encontrar o menor caminho entre dois IPs;
* Calcular o diâmetro do grafo;
* Identificar os top 5 roteadores mais críticos.

Cada IP é tratado como um nó do grafo, e cada ligação entre dois IPs representa uma aresta direcionada.

Exemplo:

```text
hop_from -> hop_to
```

Isso quer dizer que se uma linha do arquivo indica que o salto foi de um IP para outro, o programa vai criar uma ligação direcionada entre esses dois nós.

---

## COMO USAR?

LINUX:

* Um compilador C++ com suporte ao padrão C++17;
* O Graphviz, usado para gerar e visualizar os grafos.

No Linux, o Graphviz pode ser instalado com:

```bash
sudo apt install graphviz
```

Instalação funcionou?

```bash
dot -V
```

Se aparecer a versão do Graphviz, a instalação foi feita corretamente.

---

## COMPILAÇÃO

Para compilar:

```bash
g++ -std=c++17 graphroute.cpp -o graphroute
```

---

## EXECUÇÃO

Após compilar, execute o programa informando o arquivo `.log` como argumento, exemplo:

```bash
./graphroute input_1.log
```

Se nenhum arquivo for informado, o programa vai mostrar uma mensagem indicando o modo correto de uso:

```text
Uso: ./graphroute <arquivo.log>
```

Se o arquivo informado não puder ser aberto, o programa também exibirá uma mensagem de erro e finalizará.

---

## FORMATO DO ARQUIVO DE ENTRADA

O arquivo de entrada possui linhas no formato CSV com dados de traceroute. A primeira linha é o cabeçalho:

```text
prb_id,probe_src,dst_addr,hop,hop_from,hop_to,rtt
```

As colunas utilizadas pelo programa são:

* `hop_from:` IP de origem do salto;
* `hop_to:` IP de destino do salto.

Cada linha válida representa uma aresta direcionada no grafo:

hop_from -> hop_to

Valores vazios ou `"*"` são ignorados, pois não representam um IP válido para o grafo. Isso acontece porque um `"*"` no traceroute representa um `timeout`. Por isso, não existe um vértice válido para criar a ligação. Também é possível que a mesma ligação apareça mais de uma vez no arquivo. Nesses casos, o grafo não cria arestas duplicadas.

## MENU DO PROGRAMA

Ao executar, o programa mostrará o menu:

```text
1. Exibir Grafo Completo
2. Encontrar Menor Caminho
3. Calcular o Diâmetro do Grafo
4. Identificar Roteadores Críticos
0. Sair
```

Nas opções (1 e 2), o programa permite escolher o formato de saída:

```text
1. Tela
2. Imagem (PNG)
3. Documento (PDF)
```

Essas saídas são geradas usando o Graphviz.

---

## ORGANIZAÇÃO DO CÓDIGO

O projeto foi separado em dois arquivos principais:

* `digraph.cpp`: contém a estrutura do grafo e as funções de análise;
* `graphroute.cpp`: contém a leitura do arquivo, o menu e as chamadas das funções.

A ideia foi deixar a lógica do grafo separada da parte de interação com o usuário. O `digraph.cpp` não sabe nada sobre o formato do arquivo `.log` ou sobre o menu, ele só sabe manipular o grafo.

---

## GRAFO DIRECIONADO

O trabalho usa um grafo direcionado, pois uma ligação de um IP para outro não significa necessariamente que exista o caminho contrário.

Exemplo:

```text
A -> B
```

Isso indica que existe uma ligação de `A` para `B`, mas não necessariamente de `B` para `A`. Isso reflete a realidade de uma rota: o pacote passou de `A` para `B` em um salto, mas o caminho de volta pode passar por roteadores diferentes.

Cada IP é armazenado como um nó do grafo. A estrutura usada foi:

```cpp
struct node {
    std::string ip;
    std::vector<node*> links;
    std::unordered_set<std::string> links_inserted;
};
```

* `ip`: guarda o endereço IP do nó;
* `links`: guarda os vizinhos daquele nó, ou seja, os IPs para onde ele aponta;
* `links_inserted`: usado para evitar arestas duplicadas.

---

## POR QUE USAR VECTOR E UNORDERED_SET?

O `vector<node*> links` foi usado para guardar os vizinhos mantendo a **ordem de inserção**.

Isso é importante no menor caminho, porque se existir mais de um caminho com a mesma quantidade de saltos, o programa deve usar o primeiro caminho encontrado, e essa ordem depende da ordem em que as arestas foram lidas do arquivo.

Já o `unordered_set<std::string> links_inserted` foi usado para evitar que a mesma aresta seja inserida mais de uma vez.

Exemplo:

```text
A -> B
A -> B
```

Mesmo que essa ligação apareça mais de uma vez no arquivo, ela só será inserida uma vez no grafo.

```text
vector         -> mantém a ordem dos vizinhos
unordered_set  -> evita ligações duplicadas
```

---

## UNORDERED_MAP

O grafo é armazenado em um `unordered_map`:

```cpp
std::unordered_map<std::string, node> graph;
```

A chave é o IP, e o valor é o nó correspondente.

Exemplo:

```text
graph["192.168.0.1"] = nó desse IP
```

Isso facilita a busca por um nó do grafo usando o endereço IP.

Também é usado outro unordered_map para armazenar o grau de entrada dos nós:

std::unordered_map<std::string, int> input_degree;

Guardando quantas arestas chegam em cada IP, e é usado depois para identificar os roteadores críticos.
