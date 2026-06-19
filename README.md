# GraphRoute - Trabalho Final de Estrutura de Dados II

#### Desenvolvido por: Wesley Triches (210057), Alberto Luiz Marsaro Neto (188388) e Luis Gustavo Cavalheiro Rossal (161742).

## O QUE É?
É um projeto desenvolvido para a matéria de Estrutura de Dados II do curso de Ciência da Computação.
Este projeto implementa uma ferramenta para análise de rotas de rede a partir de arquivos de log no formato `.log`.

O programa lê dados de rotas, monta um grafo direcionado com os endereços IP e permite realizar análises como exibição do grafo, menor caminho, diâmetro e identificação de roteadores críticos.

---

## COMO USAR?

### LINUX:

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

Se aparecer a versão do Graphviz, a instalação foi concluída corretamente.

#### COMPILAÇÃO

O projeto utiliza o padrão C++17, pois o código usa recursos como `structured bindings`, por exemplo:

```cpp
for (const auto& [key, n] : graph)
```

Para compilar:

```bash
g++ -std=c++17 graphroute.cpp -o graphroute
```

Como o arquivo `graphroute.cpp` já inclui o arquivo `digraph.cpp`, a compilação deve ser feita apenas com o `graphroute.cpp`.

#### EXECUÇÃO

Para executar o programa, informe o arquivo `.log` como argumento:

```bash
./graphroute input_1.log
```

Exemplo com outro arquivo:

```bash
./graphroute input_3.log
```

Caso nenhum arquivo seja informado, o programa exibirá uma mensagem indicando o modo correto de uso.

---

## FORMATO DO ARQUIVO DE ENTRADA

O arquivo de entrada possui linhas em formato CSV.

O formato esperado é:

```text
prb_id,probe_src,dst_addr,hop,hop_from,hop_to,rtt
```

As colunas principais utilizadas pelo programa são:

* `hop_from`: IP de origem do salto;
* `hop_to`: IP de destino do salto.

No código, esses campos são acessados por posição no `vector`:

```cpp
fields[4] = hop_from
fields[5] = hop_to
```

Cada linha válida representa uma aresta direcionada no grafo:

```text
hop_from -> hop_to
```

Valores vazios ou `"*"` são ignorados, pois não representam um IP válido para a construção do grafo.

---

## LEITURA DAS LINHAS DO ARQUIVO

Para separar os campos de cada linha do arquivo `.log`, foi usada a função `split`.

```cpp
vector<string> split(const string& line, char sep)
{
    vector<string> fields;
    stringstream ss(line);
    string campo;

    while (getline(ss, campo, sep))
        fields.push_back(campo);

    return fields;
}
```

A função `split` foi criada no próprio código. Ela usa `stringstream` para tratar uma `string` como se fosse uma entrada de dados.

A chamada da função define que o separador será a vírgula:

```cpp
vector<string> fields = split(line, ',');
```

Assim, cada linha do CSV é dividida em campos separados, que depois são armazenados no `vector`.

---

## VALIDAÇÃO DAS ENTRADAS

A validação dos IPs lidos do arquivo é feita no `graphroute.cpp`, pois é a `main` que lê o arquivo de entrada.

A função utilizada foi:

```cpp
bool valid_ip(const string& ip)
{
    return !ip.empty() && ip != "*";
}
```

Na leitura do arquivo, antes de inserir a ligação no grafo, o programa verifica se os campos são válidos:

```cpp
string from = fields[4];
string to = fields[5];

if (!valid_ip(from) || !valid_ip(to))
    continue;

g.insert_link(from, to);
```

Dessa forma, linhas com IP vazio ou com `"*"` são ignoradas antes de chegar na estrutura do grafo.

---

## MENU DO PROGRAMA

O programa apresenta um menu com as seguintes opções:

```text
1. Exibir Grafo Completo
2. Encontrar Menor Caminho
3. Calcular o Diametro do Grafo
4. Identificar Roteadores Criticos
0. Sair
```

As opções de visualização utilizam o Graphviz e permitem escolher o formato de saída:

```text
1. Tela
2. Imagem (PNG)
3. Documento (PDF)
```

---

## ORGANIZAÇÃO DO CÓDIGO

O projeto foi organizado em dois arquivos principais:

* `digraph.cpp`: contém a estrutura do grafo e as funções de análise;
* `graphroute.cpp`: contém a leitura do arquivo, validação dos dados, menu e chamadas das funções do grafo.

A separação foi feita para manter a lógica do grafo concentrada em um arquivo e a interação com o usuário em outro.

---

## GRAFO DIRECIONADO

O projeto utiliza um grafo direcionado, pois cada linha válida do arquivo representa uma ligação com direção:

```text
hop_from -> hop_to
```

Isso significa que uma ligação de `A` para `B` não significa automaticamente que exista uma ligação de `B` para `A`.

Esse comportamento é importante porque as rotas de rede possuem uma sequência de saltos, e a direção dos saltos precisa ser respeitada.

---
## ESTRUTURA DO NÓ

Cada IP é representado como um nó do grafo.

A estrutura utilizada foi:

```cpp
struct node {
    std::string ip;
    std::unordered_set<node*> links;
    int input_degree = 0;
};
```

O atributo `ip` guarda o endereço IP do nó.

O atributo `links` guarda os vizinhos do nó, ou seja, os nós para os quais ele aponta.

O atributo `input_degree` guarda o grau de entrada do nó, ou seja, quantas arestas chegam nele.

---

## ARMAZENAMENTO DO GRAFO

O grafo é armazenado em um `unordered_map`:

```cpp
std::unordered_map<std::string, node> graph;
```

A chave é o endereço IP em formato de texto, e o valor é o nó correspondente.

Exemplo:

```text
"192.168.0.1" -> node
"10.0.0.1"    -> node
```

Essa estrutura foi usada porque permite encontrar rapidamente um nó a partir do seu IP.

---

## POR QUE USAR UNORDERED_SET NOS LINKS?

Os vizinhos de cada nó são armazenados com:

```cpp
std::unordered_set<node*> links;
```

O `unordered_set` foi utilizado porque ele não permite elementos repetidos. Assim, se a mesma aresta aparecer mais de uma vez no arquivo, ela será inserida apenas uma vez no grafo.

Exemplo:

```text
A -> B
A -> B
```

Mesmo que essa ligação apareça duas vezes no arquivo, o nó `B` será armazenado apenas uma vez nos vizinhos de `A`.

Essa estrutura também é usada nas buscas e na visualização do grafo, pois permite percorrer os vizinhos de cada nó.

### OBSERVAÇÃO SOBRE A ORDEM DOS VIZINHOS

O `unordered_set` não mantém a ordem de inserção dos vizinhos. Em uma versão anterior, em que os vizinhos eram guardados em um `vector`, a ordem em que as arestas eram lidas do arquivo era preservada, o que tornava o resultado do menor caminho determinístico quando havia mais de um caminho com a mesma quantidade de saltos (sempre vencia o primeiro lido). Com o `unordered_set`, a ordem de iteração depende do hash dos ponteiros, então, entre dois ou mais caminhos de mesmo comprimento, qual deles é retornado pode variar. O caminho continua sendo sempre um dos menores, pois a BFS garante o número mínimo de saltos; o que se perde é apenas a garantia de qual dos caminhos empatados aparece.

---

## INSERÇÃO DE NÓS

A função `insert_node` adiciona um IP como vértice do grafo.

```cpp
void insert_node(const std::string& ip)
{
    if (graph.count(ip) != 0) {
        return;
    }

    node aux;
    aux.ip = ip;
    aux.input_degree = 0;

    graph[ip] = aux;
}
```

Ela verifica se o IP já existe no grafo. Se já existir, não insere novamente.

Como a validação de IP vazio ou `"*"` foi colocada na `main`, essa função fica responsável apenas por inserir o nó na estrutura do grafo.

---

## INSERÇÃO DE ARESTAS

A função `insert_link` cria uma aresta direcionada entre dois IPs:

```text
from -> to
```

A função recebe os IPs de origem e destino, garante que os dois nós existam e depois cria a ligação entre eles.

```cpp
void insert_link(const std::string& from, const std::string& to)
{
    insert_node(from);
    insert_node(to);

    node* pfrom = find(from);
    node* pto = find(to);

    if (!pfrom || !pto)
        return;

    if (pfrom->links.count(pto) != 0)
        return;

    pfrom->links.insert(pto);

    pto->input_degree++;
    total_edges++;
}
```

A linha abaixo insere o ponteiro do nó de destino dentro do conjunto de vizinhos do nó de origem:

```cpp
pfrom->links.insert(pto);
```

Depois disso, o grau de entrada do nó de destino é atualizado:

```cpp
pto->input_degree++;
```

E o total de arestas também é incrementado:

```cpp
total_edges++;
```

---
## GRAU DE ENTRADA

O grau de entrada representa quantas arestas chegam em um determinado nó.

Neste projeto, o grau de entrada fica dentro do próprio nó:

```cpp
int input_degree = 0;
```

Antes, essa informação poderia ficar em um `unordered_map` separado, mas foi escolhido deixar o valor dentro do próprio `node`, pois o grau de entrada pertence ao nó.

Assim, quando uma aresta chega em um nó, o programa faz:

```cpp
pto->input_degree++;
```

Isso significa que o nó de destino recebeu mais uma ligação de entrada.

---

## FUNÇÃO RETURN_INPUT_DEGREE

A função `return_input_degree` retorna o grau de entrada de um IP.

```cpp
int return_input_degree(const std::string& ip)
{
    node* p = find(ip);

    if (!p)
        return 0;

    return p->input_degree;
}
```

Ela é usada no `graphroute.cpp` para exibir o grau de entrada dos roteadores críticos.

A `main` recebe apenas os IPs no ranking, então essa função permite buscar o grau de entrada daquele IP sem acessar diretamente a estrutura interna do grafo.

---

## MENOR CAMINHO

Para encontrar o menor caminho entre dois IPs, foi utilizada a busca em largura, conhecida como BFS.

A BFS foi escolhida porque o grafo não possui pesos nas arestas. Nesse caso, o menor caminho é aquele com a menor quantidade de saltos.

A função utiliza:

* `queue`: para controlar a ordem de visita dos nós;
* `unordered_set`: para marcar os nós já visitados;
* `unordered_map`: para guardar de onde cada nó foi alcançado.

Durante a busca, o programa percorre os vizinhos do nó atual:

```cpp
for (node* adj : current->links)
```

O mapa `origin` é usado para reconstruir o caminho depois que o destino é encontrado.

Exemplo:

```text
origin[B] = A
origin[D] = B
```

Isso significa que o caminho até `D` veio por `B`, e o caminho até `B` veio por `A`.

Depois, o caminho é reconstruído do destino até a origem e invertido para ficar na ordem correta.

---

## DIÂMETRO DO GRAFO

O diâmetro do grafo é o maior menor caminho encontrado entre dois nós alcançáveis.

Para calcular o diâmetro, o programa executa uma BFS a partir de cada nó do grafo:

```cpp
for (auto& [key, start_node] : graph) {
    node* start = &start_node;
```

Como o grafo está em um `unordered_map`, cada item possui uma chave e um valor:

```text
key        = IP
start_node = nó
```

No cálculo do diâmetro, a BFS precisa do nó, pois é dentro dele que estão os vizinhos usados na busca. Por isso é usado:

```cpp
node* start = &start_node;
```

Cada BFS calcula as distâncias mínimas daquele nó até os demais nós alcançáveis. Durante esse processo, o programa guarda a maior distância encontrada.

Como o grafo é direcionado, o cálculo respeita a direção das arestas.

---

## ROTEADORES CRÍTICOS

Os roteadores críticos são identificados pelo grau de entrada de cada nó.

Quanto maior o grau de entrada, mais ligações chegam naquele IP. Isso pode indicar que o roteador tem maior importância dentro da rede analisada.

A função `critical_routers` monta um ranking com os IPs do grafo:

```cpp
for (const auto& item : graph) {
    ranking.push_back(item.first);
}
```

Depois, o ranking é ordenado pelo maior grau de entrada:

```cpp
std::sort(ranking.begin(), ranking.end(),
    [this](const std::string& a, const std::string& b) {
        node* pa = find(a);
        node* pb = find(b);

        if (pa->input_degree != pb->input_degree)
            return pa->input_degree > pb->input_degree;

        return a < b;
    });
```

Em caso de empate, o IP em ordem crescente é usado como critério de desempate, apenas para manter a saída organizada.

No `graphroute.cpp`, são exibidos no máximo cinco roteadores:

```cpp
int limit = std::min(5, (int)ranking.size());
```

Assim, se o grafo tiver menos de cinco IPs, o programa mostra apenas a quantidade disponível.

---

## GRAPHVIZ E SAÍDAS VISUAIS

As opções que geram visualização utilizam o Graphviz.

O programa cria dinamicamente arquivos `.dot`, que depois são usados para gerar a visualização em tela, PNG ou PDF.

Exemplo de comando gerado internamente pelo programa:

```bash
dot -Tpng arquivo.dot -o arquivo.png
```

Na visualização do menor caminho, os nós e arestas pertencentes ao caminho encontrado são destacados em vermelho para facilitar a análise.

---

## ARQUIVOS GERADOS

Ao exibir o grafo completo, podem ser gerados arquivos como:

```text
input_1.log.dot
input_1.log.png
input_1.log.pdf
```

Ao exibir o menor caminho, podem ser gerados arquivos como:

```text
input_1.log.dot
input_1.log.png
input_1.log.pdf
```

Esses arquivos são criados a partir do nome do arquivo de entrada informado na execução.

---

## EXEMPLO DE USO 

Compilar:

```bash
g++ -std=c++17 graphroute.cpp -o graphroute
```

Executar:

```bash
./graphroute input_1.log
```

Saída inicial:

```text
Grafo de roteamento inicializado!
Vertices unicos (IPs): 110 | Arestas: 118
```

Exemplo de menor caminho:

```text
Digite o IP de Origem: 82.66.191.65
Digite o IP de Destino: 194.149.162.250

Caminho encontrado (3 saltos):
82.66.191.65 -> 192.168.3.1 -> 194.149.162.248 -> 194.149.162.250
```

Exemplo de roteadores críticos:

```text
Top 5 roteadores criticos:
1. 20.157.222.42 - Grau de entrada: 7
2. 176.52.248.125 - Grau de entrada: 3
3. 213.140.50.187 - Grau de entrada: 3
4. 213.140.36.233 - Grau de entrada: 2
5. 213.140.36.3 - Grau de entrada: 2
```
---

## OBSERVAÇÕES

O projeto busca manter o código próximo ao que foi trabalhado em aula, usando estruturas como `unordered_map`, `unordered_set`, `queue` e BFS.

A implementação representa os IPs como nós de um grafo direcionado, permite analisar caminhos, calcular o diâmetro e identificar roteadores importantes com base no grau de entrada.

As validações dos dados lidos do arquivo foram colocadas na `main`, enquanto o `digraph.cpp` ficou responsável pela estrutura e pelas operações do grafo.