#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>

namespace graph
{
    class digraph{
    private:
        struct node {
            std::string ip;
            std::vector<node*> links;
            std::unordered_set<std::string> links_inserted;
        };

        std::unordered_map<std::string, node> graph;
        std::unordered_map<std::string, int> input_degree;
        int total_edges = 0; // total arestas

        node *find(const std::string& ip){
            auto it = graph.find(ip);

            if (it == graph.end()){
                return nullptr;
            }
            return &it->second;
        }

        void visit_node(node* current, std::unordered_set<std::string>& visited, int level)
        {
            std::cout << std::string(level, '\t') << current->ip << "\n";
            visited.insert(current->ip);

            for (node* p : current->links) {
                if (visited.count(p->ip) == 0) {
                    visit_node(p, visited, level + 1);
                }
            }
        }
        

    public:
        void insert_node(const std::string& ip){
            if (ip.empty() || ip == "*") {
                return;
            }

            if (graph.count(ip) != 0) {
                return;
            }

            node aux;
            aux.ip = ip;

            graph[ip] = aux;
            input_degree[ip] = 0;
        }


        void insert_link(const std::string& from, const std::string& to){
            if (from.empty() || to.empty())
                return;

            if (from == "*" || to == "*")
                return;

            insert_node(from);
            insert_node(to);

            auto pfrom = find(from);
            auto pto = find(to);
            
            if (!pfrom || !pto)
                return;

            if (pfrom->links_inserted.count(to) != 0)
                return;

            pfrom->links.push_back(pto);
            pfrom->links_inserted.insert(to);

            input_degree[to]++;
            total_edges++;
        }


        int node_count() {
            return graph.size();
        }

        int edge_count() {
            return total_edges;
        }

        
        void show(const std::string& input_filepath, char format){
            std::string base = input_filepath;
            std::string dot_file = base + ".dot";

            std::ofstream dot(dot_file);

            dot << "digraph {\n";

            for (const auto& [key, n] : graph) {
                for (const node* link : n.links) {
                    dot << "\t\"" << key << "\" -> \"" << link->ip << "\";\n";
                }
            }

            dot << "}\n";
            dot.close();

            std::string command;

            if (format == '1') {
                command = "dot -Tx11 " + dot_file + " &";
            }
            else if (format == '2') {
                command = "dot -Tpng " + dot_file + " -o " + base + ".png";
            }
            else if (format == '3') {
                command = "dot -Tpdf " + dot_file + " -o " + base + ".pdf";
            }

            system(command.c_str()); // converte uma std::string do C++ para texto no formato C
        }

        
        std::vector<std::pair<std::string, int>> critical_routers() {
            std::vector<std::pair<std::string, int>> ranking;

            for (const auto& item : input_degree)
                ranking.push_back(item);

            std::sort(ranking.begin(), ranking.end(),
                [](const auto& a, const auto& b) {
                    if (a.second != b.second)
                        return a.second > b.second;   // grau decrescente
                    return a.first < b.first;          // desempate por IP
                });

            return ranking;
        }
   
};
}
