#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;


//Macierz sasiedztwa
class Matrix {
public:
    vector<vector<int>> matrix;
    int size;

    Matrix(int size) : size(size) {
        matrix.resize(size, vector<int>(size, 0));
    }

    void addEdge(int src, int dest) {
        matrix[src][dest] = 1;
        matrix[dest][src] = 1;
    }
};

//Lista sasiedztwa

class List {
public:
    vector<vector<int>> list;

    List(int size) {
        list.resize(size);
    }

    void addEdge(int src, int dest) {
        list[src].push_back(dest);
        list[dest].push_back(src);
    }
};

//Lista Krawedzi
class EdgeList {
public:
    vector<pair<int, int>> edges;

    void addEdge(int src, int dest) {
        edges.push_back(make_pair(src, dest));
        edges.push_back(make_pair(dest, src));
    }
};


void dfsMatrix(const Matrix& graph, int root, vector<bool>& checked) {
    checked[root] = true;

    for (int i = 0; i < graph.size; i++) {
        if (graph.matrix[root][i] == 1 && !checked[i]) {
            dfsMatrix(graph, i, checked);
        }
    }
}

void dfsList(const List& graph, int root, vector<bool>& checked) {
    checked[root] = true;

    for (int i = 0; i < graph.list[root].size(); i++) {
        int adjacentVertex = graph.list[root][i];
        if (!checked[adjacentVertex]) {
            dfsList(graph, adjacentVertex, checked);
        }
    }
}

void dfsEdgeList(const EdgeList& graph, int root, vector<bool>& checked) {
    checked[root] = true;

    for (int i = 0; i < graph.edges.size(); i++) {
        int src = graph.edges[i].first;
        int dest = graph.edges[i].second;

        if (src == root && !checked[dest]) {
            dfsEdgeList(graph, dest, checked);
        }
    }
}


void testGraph(int size, ofstream& file) {
    List list(size);
    Matrix matrix(size);
    EdgeList edge;
 
    srand(time(nullptr));
    for (int i = 0; i < size * 2; i++) {
        int src = rand() % size;
        int dest = rand() % size;
        list.addEdge(src, dest);
        matrix.addEdge(src, dest);
        edge.addEdge(src, dest);
    }

    vector<bool> visitedMatrix(size, false);
    vector<bool> visitedList(size, false);
    vector<bool> visitedEdge(size, false);
    

    file << size << endl;
    //Macierz sasiedstwa
    auto start = high_resolution_clock::now();
    for (int i = 0; i < size; i++) {
        if (!visitedMatrix[i]) {
            dfsMatrix(matrix, i, visitedMatrix);
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    file << "Macierz Sasiedstwa: " << duration.count() / 1000.0 << endl;

    //Lista sasiedstwa

    start = high_resolution_clock::now();
    for (int i = 0; i < size; i++) {
        if (!visitedMatrix[i]) {
            dfsList(list, i, visitedMatrix);
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    file << "Lista Sasiedstwa: " << duration.count()  / 1000.0<< endl;

    //Edge

    start = high_resolution_clock::now();
    for (int i = 0; i < size; i++) {
        if (!visitedMatrix[i]) {
            dfsEdgeList(edge, i, visitedMatrix);
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    file << "Edge: " << duration.count() / 1000.0 << endl;

    file << endl;

}

int main() {

    ofstream file("tests.txt");
    
    testGraph(10, file);
    testGraph(50, file);
    testGraph(100, file);
    testGraph(500, file);
    testGraph(1000, file);
    testGraph(1500, file);

    file.close();

    return 0;
}