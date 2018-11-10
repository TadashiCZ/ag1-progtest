#include <iostream>
#include <iomanip>

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

    if (current->mParent == nullptr) {
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
    } else {
        if (node->mRight == nullptr && node->mLeft == nullptr) {
            delete node;
            node = nullptr;
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
            node->mKey = succ->mKey;
            node->mRight = deleteNode(node->mRight, succ->mKey);
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

Node * rotateTreeLeft(Node * node) {
    if (node->mRight == nullptr) { return nullptr; }

    Node * second = node->mRight;
    Node * third = second->mLeft;

    second->mLeft = node;
    node->mRight = third;

    second->mParent = node->mParent;
    node->mParent = second;
    if (third != nullptr) third->mParent = node;

    if (second->mParent != nullptr) {
        if (second->mParent->mLeft == node) {
            second->mParent->mLeft = second;
        } else {
            second->mParent->mRight = second;
        }
    }

    return second;
}

Node * rotateTreeRight(Node * node) {
    if (node->mLeft == nullptr) { return nullptr; }

    Node * second = node->mLeft;
    Node * third = second->mRight;

    second->mRight = node;
    node->mLeft = third;

    second->mParent = node->mParent;
    node->mParent = second;
    if (third != nullptr) third->mParent = node;

    if (second->mParent != nullptr) {
        if (second->mParent->mLeft == node) {
            second->mParent->mLeft = second;
        } else {
            second->mParent->mRight = second;
        }
    }

    return second;
}

Node * rotateTree(Node * root, int val1, int val2) {
    bool headRotate = false;
    Node * found = search(root, val1);
    if (found == nullptr) {
        cout << "notfound" << endl;
        return root;
    }
    if (found == root) headRotate = true;

    Node * tmp = nullptr;
    if (val2 == 1) {
        tmp = rotateTreeLeft(found);
    } else {
        tmp = rotateTreeRight(found);
    }

    if (tmp == nullptr) {
        cout << "norotate" << endl;
        return root;
    }

    if (headRotate) {
        return tmp;
    } else {
        return root;
    }
}

void postorder(Node* p, int indent = 3)
{
    if(p != nullptr) {
        if(p->mRight) {
            postorder(p->mRight, indent+4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->mRight) std::cout<<" /\n" << std::setw(indent) << ' ';
        std::cout<< p->mKey << "(" << ((p->mParent) ? to_string(p->mParent->mKey) : "root") << ")\n ";
        if(p->mLeft) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder(p->mLeft, indent+4);
        }
    }
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
        //postorder(treeRoot);
        //cout << "---------------" << endl << endl;

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
           // cout << "rotation (" << inputVal1 << ", " << inputVal2 << ")" <<endl;
            treeRoot = rotateTree(treeRoot, inputVal1, inputVal2);
        }

    }

    return 0;
}

