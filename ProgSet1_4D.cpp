#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <stdlib.h>
#include <random>
#include <chrono>
using namespace std;


class DSU {
	int* parent;
	int* rank;

public:
	DSU(int n) {
		parent = new int[n];
		rank = new int[n];

		for (int i = 0; i < n; i++) {
			parent[i] = -1;
			rank[i] = 1;
		}
	}

	int find(int i) {
		if (parent[i] == -1) {
			return i;
        }
		return parent[i] = find(parent[i]);
	}

	void link(int x, int y) {
		if (x != y) { // Union by rank logic
			if (rank[x] < rank[y]) {
				parent[x] = y;
			}
			else if (rank[x] > rank[y]) {
				parent[y] = x;
			}
			else {
				parent[y] = x;
				rank[x] += 1;
			}
		}
	}

    void union_(int x, int y) {
        link(find(x), find(y));
    }

};

class Graph {
    public:
        vector< vector <double> > edges;
        vector< vector <double> > nodes;
        int V;

    //Graph class constructor
	Graph(int V) { 
        this->V = V; 
    } 

	void addEdge(double w, double v1, double v2) {
        vector <double> values;

        values.push_back(w);
        values.push_back(v1);
        values.push_back(v2);

		edges.push_back(values);
	}

    void addNode(double x_coord, double y_coord, double z_coord, double a_coord) {
        vector <double> values;

        values.push_back(x_coord);
        values.push_back(y_coord);
        values.push_back(z_coord);
        values.push_back(a_coord);

		nodes.push_back(values);
	}

    double getWeight(double x1, double y1, double z1, double a1, double x2, double y2, double z2, double a2) {
        double weight = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2) + pow(a1 - a2, 2));
        return weight;
    }

	double kruskal(int n) {
		// sort the edges first
		sort(edges.begin(), edges.end());

        // Create the DSU
        DSU my_DSU(V);

		double answer = 0;
        int count = 0;
        double max_weight = -1;

		for (vector <double> edge : edges) {
            double w = edge[0];

            // efficiency: stop when you have n-1 edges added
            if (count == n-1) {
                cout << "Minimum Cost Spanning Tree: " << answer << endl;
                cout << "Max Weight: " << max_weight << endl << endl;
                return answer;
            }

			double v1 = edge[1];
			double v2 = edge[2];

            // checking for the highest weight value
            if (w > max_weight) {
                max_weight = w;
            }

			// if the edge doesn't create a cycle
			if (my_DSU.find(v1) != my_DSU.find(v2)) {
				my_DSU.union_(v1, v2); // union the edge into the MST
                count ++;
				answer += w;
			}
		}

		cout << "Minimum Cost Spanning Tree: " << answer << endl;
        cout << "Max Weight: " << max_weight << endl << endl;
        return answer;
	}
};

// testing code
int main() {
    // using a better random number generator (more random) using a distribution
    std::random_device rd;
    std::mt19937 mt_generator(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    int nVals []= {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144};

    for (auto nVal : nVals) {

        cout << "_____________________________" << endl << endl;
        cout << "Value of N: " << nVal << endl << endl;

        vector<double> array_to_average;

        for (int i = 0; i < 5; i++) {
            Graph g(nVal);
            // Generating random values of node coordinates
            for (int i = 0; i < nVal; i++) {
                double x_coord = dist(mt_generator);
                double y_coord = dist(mt_generator);
                double z_coord = dist(mt_generator);
                double a_coord = dist(mt_generator);

                g.addNode(x_coord, y_coord, z_coord, a_coord);
            }
            for (int x = 0; x < nVal-1; x++) {
                for (int y = x+1; y < nVal; y++) {
                    double weight = g.getWeight(g.nodes[x][0], g.nodes[x][1], g.nodes[x][2], g.nodes[x][3], g.nodes[y][0], g.nodes[y][1], g.nodes[y][2], g.nodes[y][3]);
                    if (1.1/pow(nVal, 0.18) > weight) {              
                    g.addEdge(weight, x, y);
                    } 
                }
            }                       
            
            // Calling Kruskal's to find MST then and the min weight to array
            array_to_average.push_back(g.kruskal(nVal));
        }
    

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    cout << "Time taken by code: " << (double)duration.count()/1000000 << " seconds" << endl;

    // Getting Average
    double sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += array_to_average[i];
    }
    double answer = sum / 5;
    cout << "Average Answer: " << answer << endl;

    }

	return 0;
}
