#include <iostream>
#include <vector>
#include <queue>
#include <set>


using namespace std;

class CCity {

public:
    unsigned long int mNumber;
    unsigned long int mLocalFoodType;
    vector<CCity *> mNeighbours;
    unsigned long int mPrice = 0;
    set<unsigned long int> mFoodTypes;

    bool operator<(const CCity &right) const {
        return mNumber < right.mNumber;
    }

};


void printGraph(vector<CCity> mGraph);

int main() {
    unsigned long int noOfPaths, noOfFood, noOfFoodMin;
    unsigned long noOfCities;

    cin >> noOfCities >> noOfPaths;
    cin >> noOfFood >> noOfFoodMin;

    vector<CCity> graph;
    graph.resize(noOfCities);

    for (unsigned long int i = 0; i < noOfCities; ++i) {
        unsigned long int loadedFoodType;
        cin >> loadedFoodType;
        graph[i].mNumber = i;
        graph[i].mLocalFoodType = loadedFoodType;
    }

    for (unsigned long int j = 0; j < noOfPaths; ++j) {
        unsigned long int from, to;
        cin >> from >> to;

        graph[from].mNeighbours.emplace_back(&graph[to]);
        graph[to].mNeighbours.emplace_back(&graph[from]);
    }

    for (unsigned long int i = 0; i < noOfCities; ++i) {
        deque<pair<CCity, int>> nodeQueue;
        set<CCity> searched;
        nodeQueue.emplace_back(make_pair(graph[i], 0));
        int currentDepth = 0;
        set<unsigned long int> foodTypesInTheCity;

        // start BFS
        while (!nodeQueue.empty()) {
            const CCity currentNode = nodeQueue.front().first;
            searched.insert(currentNode);

            /*cout << "mNeighbours of " << currentNode.mNumber << ": ";
            for (const auto &neighbour : currentNode.mNeighbours) {
                cout << neighbour->mNumber << " ";
            }
            cout << endl;*/

            currentDepth = nodeQueue.front().second;
            nodeQueue.pop_front();


            // add mNeighbours
            for (const CCity *neighbour : currentNode.mNeighbours) {
                auto found = searched.find(*neighbour);
                if (found == searched.end()) {
                    pair<CCity, unsigned long int> pair1 = make_pair(*neighbour, currentDepth + 1);
                    nodeQueue.emplace_back(pair1);
                }
            }



            // add the food type
            if (foodTypesInTheCity.find(currentNode.mLocalFoodType) == foodTypesInTheCity.end()) {
                foodTypesInTheCity.insert(currentNode.mLocalFoodType);
                graph[i].mPrice += currentDepth;
                if (foodTypesInTheCity.size() >= noOfFoodMin) {
                    break;
                }
            }
        }

        if (foodTypesInTheCity.size() < noOfFoodMin) {
            cout << "Oh fuck what the hell happened in city " << i << endl;
        }
        graph[i].mFoodTypes = foodTypesInTheCity;
    }

    unsigned long int totalPrice = 0;
    for (unsigned long int i = 0; i < noOfCities; ++i) {
        totalPrice += graph[i].mPrice;
    }
    cout << totalPrice << endl;

    for (unsigned long int i = 0; i < noOfCities; ++i) {
        cout << graph[i].mPrice << " ";
        bool first = true;
        for (unsigned long int food : graph[i].mFoodTypes) {
            if (!first) {
                cout << " ";
            }
            first = false;
            cout << food;
        }
        cout << endl;
    }


    return 0;
}

void printGraph(vector<CCity> mGraph) {
    cout << "Printing the whole graph:" << endl;
    for (unsigned long int k = 0; k < mGraph.size(); ++k) {
        cout << k << ", mLocalFoodType: " << mGraph[k].mLocalFoodType << ", mNeighbours: " << endl;
        for (auto &neighbour : mGraph[k].mNeighbours) {
            cout << neighbour->mNumber << "(" << neighbour->mLocalFoodType << "), ";
        }
        cout << endl;
    }
    cout << endl;
}