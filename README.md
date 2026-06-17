# GraphRoute - Trabalho Final de Estrutura de Dados II

#### Desenvolvido por: Wesley Triches (210057), Alberto Luiz Marsaro Neto (188388) e Luis Gustavo Cavalheiro Rossal (161742).

## O QUE É?

É um projeto desenvolvido para a matéria de Estrutura de Dados II do curso de Ciência da Computação.

O problema trabalhado é a análise de rotas de rede a partir de arquivos de log no formato `.log`.

O programa lê dados de traceroute, monta um grafo direcionado usando os endereços IP encontrados no arquivo e permite realizar algumas análises sobre essa rede, como:

* Exibir o grafo completo;
* Encontrar o menor caminho entre dois IPs;
* Calcular o diâmetro do grafo;
* Identificar os 5 roteadores mais críticos.

Cada IP é tratado como um nó do grafo, e cada ligação entre dois IPs representa uma aresta direcionada.

Exemplo:

```text
hop_from -> hop_to
```

Isso quer dizer que se uma linha do arquivo indica que o salto foi de um IP para outro, o programa vai criar uma ligação direcionada entre esses dois nós.

---

## COMO USAR?

LINUX:

Para compilar e executar o projeto, é necessário ter:

* Compilador C++ com suporte ao padrão C++17;
* Graphviz, usado para gerar e visualizar os grafos.

No Linux, o compilador C++ pode ser instalado com:

```bash
sudo apt install build-essential
```

Para verificar se o compilador foi instalado:

```bash
g++ --version
```

O Graphviz pode ser instalado com:

```bash
sudo apt install graphviz
```

Para verificar se a instalação do Graphviz funcionou:

```bash
dot -V
```

Se aparecerem as versões do `g++` e do Graphviz, o ambiente está pronto para compilar e executar o projeto.

---

## COMPILAÇÃO

Para compilar no terminal Linux:

```bash
g++ -std=c++17 graphroute.cpp -o graphroute
```

O `-std=c++17` é usado porque o código utiliza recursos do C++17, como `structured bindings`.

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

* `hop_from`: IP de origem do salto;
* `hop_to`: IP de destino do salto.

No código, essas colunas são acessadas pelos índices `fields[4]` e `fields[5]`, pois o `vector` começa no índice zero:

```cpp
g.insert_link(fields[4], fields[5]);
```

Nesse caso:

```text
fields[4] = hop_from
fields[5] = hop_to
```

Cada linha válida representa uma aresta direcionada no grafo:

```text
hop_from -> hop_to
```

Valores vazios ou `"*"` são ignorados, pois não representam um IP válido para o grafo. Isso acontece porque um `"*"` no traceroute representa um `timeout`. Por isso, não existe um vértice válido para criar a ligação.

Também é possível que a mesma ligação apareça mais de uma vez no arquivo. Nesses casos, o grafo não cria arestas duplicadas.

---

## MENU DO PROGRAMA

Ao executar, o programa mostrará o menu:

```text
1. Exibir Grafo Completo
2. Encontrar Menor Caminho
3. Calcular o Diâmetro do Grafo
4. Identificar Roteadores Críticos
0. Sair
```

Nas opções 1 e 2, o programa permite escolher o formato de saída:

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

Isso indica que existe uma ligação de `A` para `B`, mas não necessariamente de `B` para `A`.

Isso reflete a realidade de uma rota: o pacote passou de `A` para `B` em um salto, mas o caminho de volta pode passar por roteadores diferentes.

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

O `vector<node*> links` foi usado para guardar os vizinhos mantendo a ordem de inserção.

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

Também é usado outro `unordered_map` para armazenar o grau de entrada dos nós:

```cpp
std::unordered_map<std::string, int> input_degree;
```

Ele guarda quantas arestas chegam em cada IP e é usado depois para identificar os roteadores críticos.

---

## INSERÇÃO DE NÓS

A função `insert_node` adiciona um IP como vértice do grafo.

Primeiro verifica se o IP é inválido, como vazio ou `"*"`. Também evita inserir o mesmo IP mais de uma vez. Se o nó já existe, a função simplesmente retorna sem fazer nada.

Quando um novo nó é criado, seu grau de entrada começa em zero:

```cpp
input_degree[ip] = 0;
```

---

## INSERÇÃO DE ARESTAS

A função `insert_link` cria uma ligação direcionada entre dois IPs:

```text
from -> to
```

Antes de criar a ligação, ela garante que os dois IPs existam no grafo:

```cpp
insert_node(from);
insert_node(to);
```

Depois, adiciona o destino na lista de vizinhos da origem:

```cpp
pfrom->links.push_back(pto);
```

E registra essa ligação no `links_inserted`, para que não seja inserida de novo:

```cpp
pfrom->links_inserted.insert(to);
```

Também atualiza o grau de entrada do IP de destino:

```cpp
input_degree[to]++;
```

E incrementa o total de arestas do grafo:

```cpp
total_edges++;
```

No final, o programa exibe o total de vértices e arestas inseridas:

```text
Vertices unicos (IPs): 110 | Arestas: 118
```

---

## VALIDAÇÃO DE IPS

Antes de calcular o menor caminho, o programa verifica se os IPs de origem e destino informados pelo usuário realmente existem no grafo:

```cpp
if (!g.contains(origem) || !g.contains(destino)) {
    cout << "IP de origem ou destino nao existe no grafo.\n";
}
```

Essa validação evita tentar calcular um caminho usando IPs que não foram encontrados no arquivo de entrada.

---

## MENOR CAMINHO (BFS)

Para encontrar o menor caminho entre dois IPs, foi usada a busca em largura, conhecida como BFS.

Todas as ligações representam "um salto". Nesse caso, o menor caminho é aquele com a menor quantidade de saltos, e a BFS é o algoritmo correto para isso.

A BFS percorre o grafo por níveis. Exemplo:

```text
A -> B
A -> C
B -> D
```

Começando em `A`, a BFS visita primeiro:

```text
nível 0: A
nível 1: B, C
nível 2: D
```

Ou seja, primeiro os vizinhos diretos, depois os vizinhos dos vizinhos, e assim por diante. Por isso ela garante o menor número de saltos.

A função `shortest_path` é composta por:

* `queue<node*>`: controla a ordem de visita dos nós;
* `unordered_set<node*>`: marca os nós já visitados (`queued`);
* `unordered_map<node*, node*>`: guarda de onde cada nó foi alcançado (`origin`).

O mapa `origin` guarda o caminho percorrido. Exemplo:

```text
origin[B] = A
origin[D] = B
```

Isso significa que o caminho até `D` veio por `B`, e o caminho até `B` veio por `A`.

Quando o nó de destino é encontrado, o caminho é reconstruído de trás para frente, seguindo o mapa `origin` até chegar na origem, que tem `origin[origem] = nullptr`.

Depois disso, o caminho é invertido com `std::reverse` para ficar na ordem correta.

Se não houver caminho entre os dois IPs, `path` fica vazio, e o programa informa:

```text
Nenhum caminho encontrado entre <origem> e <destino>.
```

---

## DIÂMETRO DO GRAFO

O diâmetro do grafo é o maior menor caminho encontrado entre dois nós alcançáveis.

Para calcular isso, o programa executa uma BFS a partir de cada nó do grafo:

```cpp
for (auto& [key, start_node] : graph) {
    node* start = &start_node;
    // BFS a partir de start
}
```

Em cada BFS, o mapa `dist` guarda a distância mínima do nó inicial até os outros nós alcançáveis.

Exemplo:

```text
dist[A] = 0
dist[B] = 1
dist[C] = 2
```

Sempre que uma distância maior é encontrada, o programa atualiza o `max_dist`.

Ao final de todas as BFS, `max_dist` representa o diâmetro do grafo em saltos. Como o grafo é direcionado, o cálculo respeita a direção das arestas.

