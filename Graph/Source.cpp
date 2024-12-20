#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

struct Edge 
{
    std::string vertex1, vertex2;
    int weight;

    bool operator < (const Edge& edge) const 
    {
        return weight < edge.weight;
    }
};

class DisjointSet {
public:
    DisjointSet(int size) : parent(size), rank(size, 0) 
    {
        for (int i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }

    int find(int u) 
    {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    void unite(int u, int v) 
    {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV])
                parent[rootV] = rootU;
            else if (rank[rootU] < rank[rootV])
                parent[rootU] = rootV;
            else 
            {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

void readGraph(const std::string& filename, std::vector<Edge>& edges, std::vector<std::string>& vertices) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::string line;

    // Read vertices
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string vertex;
        while (iss >> vertex) {
            vertices.push_back(vertex);
        }
    }

    int n = vertices.size();
    for (int i = 0; i < n && std::getline(file, line); ++i) {
        std::istringstream iss(line);
        for (int j = 0; j < n; ++j) {
            int weight;
            iss >> weight;
            if (weight > 0) {
                edges.push_back({ vertices[i], vertices[j], weight });
            }
        }
    }

    file.close();
}

void kruskalMST(const std::vector<Edge>& edges, const std::vector<std::string>& vertices) {
    std::vector<Edge> mst;
    int totalWeight = 0;

    std::vector<Edge> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end());

    DisjointSet ds(vertices.size());

    for (const auto& edge : sortedEdges) {
        int u = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), edge.vertex1));
        int v = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), edge.vertex2));

        if (ds.find(u) != ds.find(v)) {
            ds.unite(u, v);
            mst.push_back(edge);
            totalWeight += edge.weight;
        }
    }

    for (const auto& edge : mst) {
        std::cout << edge.vertex1 << " " << edge.vertex2 << std::endl;
    }
    std::cout << totalWeight << std::endl;
}

int main() {
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;

    try {
        std::vector<Edge> edges;
        std::vector<std::string> vertices;

        readGraph(filename, edges, vertices);
        kruskalMST(edges, vertices);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}