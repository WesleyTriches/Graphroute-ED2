#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include <string>
#include <queue>
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
        int total_edges = 0;

        node *find(const std::string& ip){
            auto it = graph.find(ip);

            if (it == graph.end()){
                return nullptr;

                return &it->second;
            }
        }



    public:
        void insert_node(const std::string &ip){ // implementar
    }
};
}