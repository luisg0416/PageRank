#include <iostream>
#include <iomanip>
#include<string>
#include<sstream>
#include<vector>
#include <map>
#include <algorithm>
using namespace std;

class AdjacencyList {
private:
    map<string, pair<float, vector<string>>> graph; // String is the website name, float is outdegree, vector contains websites pointing to website stored in string
    map<string, float> r; // r maps acts as the r matrix
    vector<float> final; // Stores the final rankings of each site

public:
    void insertEdge(string from, string to);
    void pageRank(int currentPower, int n);
    void setRankMatrix();
    void printPageRank();
    
};
void AdjacencyList::insertEdge(string from, string to)
{
    if (graph.find(to) == graph.end()) {
        pair<float, vector<string>> v; // Pair needs to be made as website is not initially found in graph
        graph[to] = v;
        auto itr = graph.find(to);
        itr->second.second.push_back(from); // We want from to be put into vector b/c we want indegrees to be stored
    }

    else if (graph.find(to) != graph.end()) {
        auto itr = graph.find(to); // No pair made because website is already in graph
        itr->second.second.push_back(from);
    }

    if (graph.find(from) == graph.end()) {
        pair<float, vector<string>> v; // We need to make a vertex for our from string as well even if nothing points to it
        graph[from] = v;
    }
    auto itr = graph.find(from); // O(log(v))
    itr->second.first++; // Allows us to increment the outdegree of a website without searching the whole graph
}

void AdjacencyList::setRankMatrix() {
    float N = graph.size();
    for (auto itr = graph.begin(); itr != graph.end(); itr++) {
        float startingValue = 1.0 / N; 
        r[itr->first] = startingValue; // Sets the values in the r matrix
    }
}

void AdjacencyList::pageRank(int currentPower, int n) {
    float M = 0.0;
    if (currentPower < n) {
        for (auto itr = graph.begin(); itr != graph.end(); itr++) {
            float rank = 0.0;
            if (itr->second.second.size() == 0) {  // If statement for when a page exists but nothing points to it; Did just in case, technically this should not occur in test cases
                final.push_back(rank);
            }
            else {
                for (int i = 0; i < itr->second.second.size(); i++) {
                    auto outDegree = itr->second.second[i]; // Lets us know what website's outdegree we are looking for
                    auto temp = graph.find(outDegree); // Initilaizes temp to the website we found
                    float M = 1.0 / temp->second.first; // Uses websites outdegree for equation
                    auto rValue = r.find(outDegree); // Selects proper value in r map to calculate rank of website
                    rank = M * rValue->second + rank; // Makes sure to use previous values in rank calculation; takes into account multiple indegrees
                    if (i == itr->second.second.size() - 1) {
                        final.push_back(rank); // Final vector stores ranks already in order due to use of ordered map
                    }
                }
            }
        }
    }
    currentPower++;
    if (currentPower < n) {
        int i = 0;
        for (auto itr = r.begin(); itr != r.end(); itr++) {
            itr->second = final[i]; //Updates values in r for future calculations
            i++;
        }
        final.clear(); // Clears vector as we need no longer need values stored in this vector
        pageRank(currentPower, n);
    }
} 
void AdjacencyList::printPageRank() {
    int i = 0;
    for (auto itr = graph.begin(); itr != graph.end(); itr++) {
        cout << itr->first << " ";
        cout << fixed << showpoint;
        cout << setprecision(2);
        if (final.size() > 0) { // Prints r(0) if p is 1 
            cout << final[i] << endl;
            i++;
        }
        else {
            auto newItr = r.find(itr->first);
            cout << newItr->second << endl;
        }
    }
}



int main()
{
    // Format taken from PDF
    AdjacencyList graph;
    int no_of_lines, power_iterations;
    std::string from, to;
    std::cin >> no_of_lines;
    std::cin >> power_iterations;
    for (int i = 0; i < no_of_lines; i++)
    {
        std::cin >> from;
        std::cin >> to;
        graph.insertEdge(from, to); 
    } 

    graph.setRankMatrix();
    graph.pageRank(1, power_iterations);
    graph.printPageRank();
}