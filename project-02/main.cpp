#include <iostream>

using namespace std;

class Node {
public:
    int mKey;
    Node * mParent;
    Node * mLeft;
    Node * mRight;

    explicit Node(int key) : mKey(key), mParent(nullptr), mLeft(nullptr), mRight(nullptr) {}

};

Node * search(Node * node, int key) {
    if (node == nullptr || node->mKey == key) {
        return node;
    }
    if (node->mKey < key) {
        return search(node->mRight, key);
    } else {
        return search(node->mLeft, key);
    }
}

Node * insertNode(Node * node, int key) {
    if (node == nullptr) {
        return new Node(key);
    }

    if (key < node->mKey) {
        Node * leftChild = insertNode(node->mLeft, key);
        node->mLeft = leftChild;
        leftChild->mParent = node;
    } else if (key > node->mKey) {
        Node * rightChild = insertNode(node->mRight, key);
        node->mRight = rightChild;
        rightChild->mParent = node;
    }
    return node;
}

Node * mostLeftChildOfRightChild(Node * node) {
    Node * current = node->mRight;
    while ( current->mLeft != nullptr ) {
        current = current->mLeft;
    }
    return current;
}

Node * mostRightChildOfLeftChild(Node * node) {
    Node * current = node->mLeft;
    while ( node->mRight != nullptr ) {
        current = current->mRight;
    }
    return current;
}

Node * firstRightParent(Node * node) {
    if (node->mParent == nullptr) {
        return node;
    }

    Node * current = node;
    while ( current->mParent != nullptr && current->mParent->mRight == current ) {
        current = current->mParent;
    }

    if (current->mParent == nullptr){
        return nullptr;
    } else {
        return current->mParent;
    }
}

Node * firstLeftParent(Node * node) {
    if (node->mParent == nullptr) {
        return node;
    }

    Node * current = node;
    while ( current->mParent != nullptr && current->mParent->mLeft == current ) {
        current = current->mParent;
    }

    return current;
}

Node * successor(Node * node) {
    if (node->mRight != nullptr) {
        return mostLeftChildOfRightChild(node);
    } else {
        return firstRightParent(node);
    }
}

Node * predecessor(Node * node) {
    if (node->mLeft != nullptr) {
        return mostRightChildOfLeftChild(node);
    } else {
        return firstLeftParent(node);
    }
}

Node * deleteNode(Node * node, int key) {
    if (node == nullptr) return node;

    // search
    if (key < node->mKey) {
        node->mLeft = deleteNode(node->mLeft, key);
        return node;
    } else if (node->mKey < key) {
        node->mRight = deleteNode(node->mRight, key);
        return node;
    }

    if (node->mRight == nullptr && node->mLeft == nullptr) {
        delete node;
        return nullptr;
    } else if (node->mRight == nullptr) {
        // one child
        Node * tmp = node->mLeft;
        tmp->mParent = node->mParent;
        delete node;
        return tmp;
    } else if (node->mLeft == nullptr) {
        // one child
        Node * tmp = node->mRight;
        tmp->mParent = node->mParent;
        delete node;
        return tmp;
    } else {
        // two children

        // find node to insert instead of the deleted node
        Node * succ = successor(node);

        if (succ->mRight == nullptr) {
            node->mKey = succ->mKey;
            if (succ->mParent->mLeft == succ){
                succ->mParent->mLeft = nullptr;
            } else {
                succ->mParent->mRight = nullptr;
            }
            delete succ;
            return node;
        } else {
            Node * lastSuccChild = succ->mRight;
            while ( lastSuccChild->mRight != nullptr ) {
                lastSuccChild = lastSuccChild->mRight;
            }


            // change key, disconnect 19 and 23
            node->mKey = succ->mKey;
            if (succ->mParent->mLeft == succ){
                succ->mParent->mLeft = nullptr;
            } else {
                succ->mParent->mRight = nullptr;
            }

            // connect 21 and 23
            lastSuccChild->mRight = node->mRight;
            lastSuccChild->mRight->mParent = lastSuccChild; // todo segfault on pyramid

            // connect 12 and 20, disconnect 19 and 20
            succ->mRight->mParent = node;
            node->mRight = succ->mRight;


            delete succ;
            return node;

        }
    }
    return node;
}

void printParent(Node * node, int key) {
    Node * tmp = search(node, key);
    if (tmp == nullptr) {
        cout << "notfound" << endl;
        return;
    }
    if (tmp->mParent != nullptr) {
        cout << tmp->mParent->mKey << endl;
    } else {
        cout << "noparent" << endl;
    }
}

void printSuccessor(Node * node, int key) {
    Node * succ = search(node, key);
    if (succ == nullptr) {
        cout << "notfound" << endl;
        return;
    }

    succ = successor(succ);
    if (succ == nullptr || succ->mKey == key) {
        cout << "nosuccessor" << endl;
        return;
    } else {
        cout << succ->mKey << endl;
    }


}

void deleteTree(Node * node) {
    if (node == nullptr) return;

    deleteTree(node->mLeft);
    deleteTree(node->mRight);
    delete node;
}

int main() {
    int inputType, inputVal1, inputVal2;
    Node * treeRoot = nullptr;
    while ( true ) {
        cin >> inputType;
        if (inputType == 6) {
            deleteTree(treeRoot);
            break;
        } else if (inputType == 5) {
            cin >> inputVal1 >> inputVal2;
        } else {
            cin >> inputVal1;
        }

        if (inputType == 1) {
            // cout << 1 << " " << inputVal1 << endl;
            treeRoot = insertNode(treeRoot, inputVal1);
        } else if (inputType == 2) {
            //  cout << 2 << " " << inputVal1 << endl;
            treeRoot = deleteNode(treeRoot, inputVal1);
        } else if (inputType == 3) {
            // cout << 3 << " " << inputVal1 << endl;
            printParent(treeRoot, inputVal1);
        } else if (inputType == 4) {
            //   cout << 4 << " " << inputVal1 << endl;
            printSuccessor(treeRoot, inputVal1);
        } else if (inputType == 5) {
            cout << 5 << " " << inputVal1 << endl;
            // rotateTree(inputVal1, inputVal2);
        }

    }

    return 0;
}

