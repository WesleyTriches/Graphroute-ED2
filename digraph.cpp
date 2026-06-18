#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>

namespace graph
{
    class digraph {
    private:
        struct node {
            std::string ip;
            std::unordered_set<node*> links;
            int input_degree = 0;
        };

        std::unordered_map<std::string, node> graph;
        int total_edges = 0;

        node* find(const std::string& ip)
        {
            auto it = graph.find(ip);

            if (it == graph.end()) {
                return nullptr;
            }

            return &it->second;
        }

    public:
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

        int node_count()
        {
            return graph.size();
        }

        int edge_count()
        {
            return total_edges;
        }

        bool contains(const std::string& ip)
        {
            return find(ip) != nullptr;
        }

        void show(const std::string& input_filepath, char format)
        {
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

            system(command.c_str());

            if (format == '2') {
                std::cout << "Arquivo " << base << ".png gerado com sucesso.\n";
            }
            else if (format == '3') {
                std::cout << "Arquivo " << base << ".pdf gerado com sucesso.\n";
            }
        }

        void show_path(const std::string& input_filepath, const std::vector<node*>& path, char format)
        {
            std::unordered_set<std::string> path_edges;
            std::unordered_set<std::string> path_nodes;

            for (int i = 0; i + 1 < (int)path.size(); i++) {
                path_edges.insert(path[i]->ip + "->" + path[i + 1]->ip);
            }

            for (node* p : path) {
                path_nodes.insert(p->ip);
            }

            std::string base = input_filepath + "_path";
            std::string dot_file = base + ".dot";

            std::ofstream dot(dot_file);

            dot << "digraph {\n";

            for (const auto& ip : path_nodes) {
                dot << "\t\"" << ip << "\" [style=filled, fillcolor=red, fontcolor=white];\n";
            }

            for (const auto& [key, n] : graph) {
                for (const node* link : n.links) {
                    if (path_edges.count(key + "->" + link->ip) != 0) {
                        dot << "\t\"" << key << "\" -> \"" << link->ip
                            << "\" [color=red, penwidth=3];\n";
                    }
                    else {
                        dot << "\t\"" << key << "\" -> \"" << link->ip << "\";\n";
                    }
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

            system(command.c_str());

            if (format == '2') {
                std::cout << "Arquivo " << base << ".png gerado com sucesso.\n";
            }
            else if (format == '3') {
                std::cout << "Arquivo " << base << ".pdf gerado com sucesso.\n";
            }
        }

        std::vector<node*> shortest_path(const std::string& start, const std::string& end)
        {
            std::vector<node*> path;

            node* pstart = find(start);
            if (!pstart)
                return path;

            node* pend = find(end);
            if (!pend)
                return path;

            std::queue<node*> q;
            std::unordered_set<node*> queued;
            std::unordered_map<node*, node*> origin;

            q.push(pstart);
            queued.insert(pstart);
            origin[pstart] = nullptr;

            bool found = false;

            while (!q.empty()) {
                node* current = q.front();
                q.pop();

                if (current == pend) {
                    found = true;
                    break;
                }

                for (node* adj : current->links) {
                    if (queued.count(adj) == 0) {
                        q.push(adj);
                        queued.insert(adj);
                        origin[adj] = current;
                    }
                }
            }

            if (found) {
                node* p = pend;

                while (p) {
                    path.push_back(p);
                    p = origin[p];
                }
            }

            std::reverse(path.begin(), path.end());

            return path;
        }

        int diameter()
        {
            int max_dist = 0;

            for (auto& [key, start_node] : graph) {
                node* start = &start_node;

                std::queue<node*> q;
                std::unordered_map<node*, int> dist;

                q.push(start);
                dist[start] = 0;

                while (!q.empty()) {
                    node* current = q.front();
                    q.pop();

                    for (node* adj : current->links) {
                        if (dist.count(adj) == 0) {
                            dist[adj] = dist[current] + 1;
                            q.push(adj);

                            if (dist[adj] > max_dist)
                                max_dist = dist[adj];
                        }
                    }
                }
            }

            return max_dist;
        }

        std::vector<std::string> critical_routers()
        {
            std::vector<std::string> ranking;

            for (const auto& item : graph) {
                ranking.push_back(item.first);
            }

            std::sort(ranking.begin(), ranking.end(),
                [this](const std::string& a, const std::string& b) {
                    node* pa = find(a);
                    node* pb = find(b);

                    if (pa->input_degree != pb->input_degree)
                        return pa->input_degree > pb->input_degree;

                    return a < b;
                });

            return ranking;
        }

        int return_input_degree(const std::string& ip)
        {
            node* p = find(ip);

            if (!p)
                return 0;

            return p->input_degree;
        }
    };
}
