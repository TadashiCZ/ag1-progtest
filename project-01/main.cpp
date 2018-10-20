#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>


using namespace std;

class CCity {

public:
    long int number = -1;
    long int localFoodType = -1;
    vector<CCity> neighbours;
    long int price = 0;
    set<long int> foodTypes;

    CCity(long int number, long int localFoodType) : number(number), localFoodType(localFoodType) {}

    CCity() = default;

    bool operator<(const CCity &right) const {
        return number < right.number;
    }

};


void printGraph(vector<CCity> mGraph);

int main() {
    long int noOfCities, noOfPaths, noOfFood, noOfFoodMin;

    cin >> noOfCities >> noOfPaths;
    cin >> noOfFood >> noOfFoodMin;

    vector<CCity> mGraph;
    mGraph.resize(noOfCities);

    for (long int i = 0; i < noOfCities; ++i) {
        long int loadedFoodType;
        cin >> loadedFoodType;
        mGraph[i].number = i;
        mGraph[i].localFoodType = loadedFoodType;
    }

    for (int j = 0; j < noOfPaths; ++j) {
        long int from, to;
        cin >> from >> to;

        mGraph[from].neighbours.push_back(mGraph[to]);
        mGraph[to].neighbours.push_back(mGraph[from]);
    }

    for (int i = 0; i < noOfCities; ++i) {
        deque<pair<CCity, int>> nodeQueue;
        set<CCity> searched;
        nodeQueue.emplace_back(make_pair(mGraph[i], 0));
        int currentDepth = 0;
        set<long int> foodTypesInTheCity;

        // start BFS
        while (!nodeQueue.empty()) {
            const CCity currentNode = nodeQueue.front().first;
            searched.insert(currentNode);
            cout << "neighbours: ";
            for (const auto &neighbour : currentNode.neighbours) {
                cout << neighbour.number << " ";
            }
            cout << endl;
            currentDepth = nodeQueue.front().second;
            nodeQueue.pop_front();


            // add neighbours
            for (const CCity &neighbour : currentNode.neighbours) {
                auto found = searched.find(neighbour);
                if (found == searched.end()) {
                    nodeQueue.push_back(make_pair(neighbour, currentDepth + 1));
                }
            }



            // add the food type
            if (foodTypesInTheCity.find(currentNode.localFoodType) == foodTypesInTheCity.end()) {
                foodTypesInTheCity.insert(currentNode.localFoodType);
                mGraph[i].price += currentDepth;
                if (foodTypesInTheCity.size() >= noOfFoodMin) {
                    break;
                }
            }
        }

        if (foodTypesInTheCity.size() < noOfFoodMin) {
            cout << "Oh fuck what the hell happened in city " << i << endl;
            cout << foodTypesInTheCity.size() << ": ";
            for (long int food : foodTypesInTheCity) {
                cout << food << " ";
            }
            cout << endl << endl;
        }
            mGraph[i].foodTypes = foodTypesInTheCity;
    }

    long int totalPrice = 0;
    for (int i = 0; i < noOfCities; ++i) {
        totalPrice += mGraph[i].price;
    }
    cout << totalPrice << endl;

    for (int i = 0; i < noOfCities; ++i) {
        cout << mGraph[i].price << " ";
        bool first = true;
        for (long int food : mGraph[i].foodTypes) {
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
    for (int k = 0; k < mGraph.size(); ++k) {
        cout << k << ", localFoodType: " << mGraph[k].localFoodType << ", neighbours: " << endl;
        for (int i = 0; i < mGraph[k].neighbours.size(); ++i) {
            cout << mGraph[k].neighbours[i].number << "(" << mGraph[k].neighbours[i].localFoodType << "), ";
        }
        cout << endl;
    }
}