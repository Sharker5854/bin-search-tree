#include <iostream>
#include <vector>
#include <algorithm>


struct Node {
        bool is_fantom;
        int value;
        Node* leftSon;
        Node* rightBro;

        Node(int value): value(value), leftSon(nullptr), rightBro(nullptr), is_fantom(false) {}
        Node(int value, Node* rightBro): value(value), leftSon(nullptr), rightBro(rightBro), is_fantom(false) {}
        Node(int value, bool is_fantom): value(value), leftSon(nullptr), rightBro(nullptr), is_fantom(is_fantom) {
            if (is_fantom) {
                value = value;
            }
        }
};


struct BinSearchTree
{
public:

    Node* root;
    BinSearchTree(): root(nullptr) {}
    BinSearchTree(Node* rootNode): root(rootNode) {
        AddFantomForNewNode(rootNode);
    }
    
    Node* Insert(int value) {
        if (!root) {
            std::cout << "Вставляю корень " << value << "\n";
            Node* new_root = new Node(value);
            root = new_root;
            AddFantomForNewNode(root); // при создании корня дерева, сразу же добавляем фантомного левого сына
            return root;
        }
        else {
            InsertNode(root, value);
        }
    };

    class ForwardTraversalIterator {
        private:
            Node* currentNode;
            std::vector<Node*> traversalVec;
        
        public:
            ForwardTraversalIterator(Node* root) : currentNode(root) {
                if (root != nullptr) {
                    traversalVec.push_back(root);
                }
            }

            ForwardTraversalIterator& operator++() {
                if (!currentNode->leftSon->is_fantom) {
                    currentNode = currentNode->leftSon;
                    traversalVec.push_back(currentNode);
                }
                else {
                    if (currentNode->leftSon->rightBro != nullptr) {
                        currentNode = currentNode->leftSon->rightBro;
                        traversalVec.push_back(currentNode);
                    }
                    else if (currentNode->rightBro != nullptr) {
                        currentNode = currentNode->rightBro;
                        traversalVec.push_back(currentNode);
                    }
                    else {
                        Node* prevNode;
                        for (int i = traversalVec.size()-1; i >= 0; i--) {
                            prevNode = traversalVec[i];
                            if ((prevNode->rightBro != nullptr) && ((std::find(traversalVec.begin(), traversalVec.end(), prevNode->rightBro) == traversalVec.end()))) {
                                currentNode = prevNode->rightBro;
                                traversalVec.push_back(currentNode);
                                break;
                            }
                            else if (prevNode == traversalVec[0]) {
                                currentNode = nullptr;
                            }
                        }
                    }
                }
                return *this;
            };

            bool operator!=(const ForwardTraversalIterator& other) const {
                return currentNode != other.currentNode;
            }

            int operator*() const {
                return currentNode->value;
            }
    };

    ForwardTraversalIterator frw_begin() {
        return ForwardTraversalIterator(root);
    }
    
    ForwardTraversalIterator frw_end() {
        return ForwardTraversalIterator(nullptr);
    }


    class SymmetricTraversalIterator {
        private:
            int currentNodeIndex = 1;
            Node* currentNode;
            std::vector<Node*> traversalVec;
        
        public:
            SymmetricTraversalIterator(Node* root) : currentNode(root) {
                if (root != nullptr) {
                    makeSymmetricTraversal(currentNode);
                }
            }

            SymmetricTraversalIterator& operator++() {
                currentNode = traversalVec[currentNodeIndex];
                currentNodeIndex += 1;
                return *this;
            };

            bool operator!=(const SymmetricTraversalIterator& other) const {
                return currentNode != other.currentNode;
            }

            int operator*() const {
                return currentNode->value;
            }

        private:
            void makeSymmetricTraversal(Node* curRoot) {
                if (curRoot->leftSon->is_fantom == true) {
                    traversalVec.push_back(curRoot);
                    if (curRoot->leftSon->rightBro != nullptr) {
                        curRoot = curRoot->leftSon->rightBro;
                        makeSymmetricTraversal(curRoot);
                    }
                }
                else {
                    makeSymmetricTraversal(curRoot->leftSon);
                    traversalVec.push_back(curRoot);
                    if (curRoot->leftSon->rightBro != nullptr) {
                        curRoot = curRoot->leftSon->rightBro;
                        makeSymmetricTraversal(curRoot);
                    }
                }
                currentNode = traversalVec[0];
            }
    };

    SymmetricTraversalIterator sym_begin() {
        return SymmetricTraversalIterator(root);
    }
    
    SymmetricTraversalIterator sym_end() {
        return SymmetricTraversalIterator(nullptr);
    }


    class ReversedTraversalIterator {
        private:
            int currentNodeIndex = 1;
            Node* currentNode;
            std::vector<Node*> traversalVec;
        
        public:
            ReversedTraversalIterator(Node* root) : currentNode(root) {
                if (root != nullptr) {
                    makeReversedTraversal(currentNode);
                }
            }

            ReversedTraversalIterator& operator++() {
                currentNode = traversalVec[currentNodeIndex];
                currentNodeIndex += 1;
                return *this;
            };

            bool operator!=(const ReversedTraversalIterator& other) const {
                return currentNode != other.currentNode;
            }

            int operator*() const {
                return currentNode->value;
            }

        private:
            void makeReversedTraversal(Node* curRoot) {
                if (curRoot->leftSon->is_fantom == true) {
                    if (curRoot->leftSon->rightBro != nullptr) {
                        makeReversedTraversal(curRoot->leftSon->rightBro);
                    }
                    traversalVec.push_back(curRoot);
                }
                else {
                    makeReversedTraversal(curRoot->leftSon);
                    if (curRoot->leftSon->rightBro != nullptr) {
                        makeReversedTraversal(curRoot->leftSon->rightBro);
                    }
                    traversalVec.push_back(curRoot);
                }
                currentNode = traversalVec[0];
            }
    };

    ReversedTraversalIterator rev_begin() {
        return ReversedTraversalIterator(root);
    }
    
    ReversedTraversalIterator rev_end() {
        return ReversedTraversalIterator(nullptr);
    }

private:
    
    Node* InsertNode(Node* cur_root, int value) {
        if ((cur_root->leftSon->is_fantom == true) && (cur_root->leftSon->rightBro == nullptr)) {
            if (value >= cur_root->value) { 
                Node* new_node = new Node(value); 
                std::cout << "Добавляю сына " << value << " справа от " << cur_root->value << "\n";
                cur_root->leftSon->rightBro = new_node;
                AddFantomForNewNode(cur_root->leftSon->rightBro);
                return cur_root->leftSon->rightBro;
            }
            else {
                Node* new_node = new Node(value);
                std::cout << "Добавляю сына " << value << " слева от " << cur_root->value << "\n";
                cur_root->leftSon = new_node;
                AddFantomForNewNode(cur_root->leftSon);
                return cur_root->leftSon;
            }
        }
        else {
            if ((cur_root->leftSon->is_fantom == true) && (cur_root->leftSon->rightBro != nullptr)) {
                if (value < cur_root->value) {
                    Node* oldRightBro = cur_root->leftSon->rightBro;  // если у фантома был rightBro, то при замене фантома на нормальную ноду, нужно также перенсти указатель на этого rightBro в новую ноду!!! Очень тонкий момент!!!
                    Node* new_node = new Node(value, oldRightBro);
                    std::cout << "Добавляю сына " << value << " слева от " << cur_root->value << "\n";
                    cur_root->leftSon = new_node;
                    AddFantomForNewNode(cur_root->leftSon);
                    return cur_root->leftSon;
                }
                else {
                    return InsertNode(cur_root->leftSon->rightBro, value); 
                }
            }
            else {
                if ((value >= cur_root->value)) {
                    if (cur_root->leftSon->rightBro == nullptr) {
                        Node* new_node = new Node(value);
                        std::cout << "Добавляю сына " << value << " справа от " << cur_root->value << "\n";
                        cur_root->leftSon->rightBro = new_node;
                        AddFantomForNewNode(cur_root->leftSon->rightBro);
                        return cur_root->leftSon->rightBro;
                    }
                    else {
                        return InsertNode(cur_root->leftSon->rightBro, value);
                    }
                }
                else {
                    return InsertNode(cur_root->leftSon, value);
                }
            }
        }
    }

    void AddFantomForNewNode(Node* newNode) {
        Node* new_fantom_node = new Node(NULL, true);
        newNode->leftSon = new_fantom_node;
    }
};



int main() {
    BinSearchTree B = BinSearchTree();
    B.Insert(8);
    B.Insert(4);
    B.Insert(15);
    B.Insert(2);
    B.Insert(6);
    B.Insert(1);
    B.Insert(3);
    B.Insert(7);
    B.Insert(5);
    B.Insert(20);
    B.Insert(25);
    B.Insert(12);
    B.Insert(11);
    B.Insert(18);
    B.Insert(14);
    std::cout << "\n";


    std::cout << "Обход B в прямом порядке:   ";
    for (auto forwardIter = B.frw_begin(); forwardIter != B.frw_end(); ++forwardIter) {
        std::cout << *forwardIter << " ";
    }
    std::cout << "\n";

    std::cout << "Обход B в симметричном порядке:   ";
    for (auto symmetricIter = B.sym_begin(); symmetricIter != B.sym_end(); ++symmetricIter) {
        std::cout << *symmetricIter << " ";
    }
    std::cout << "\n";

    std::cout << "Обход B в обратном порядке:   ";
    for (auto reversedIter = B.rev_begin(); reversedIter != B.rev_end(); ++reversedIter) {
        std::cout << *reversedIter << " ";
    }
    std::cout << "\n\n\n";



    BinSearchTree A = BinSearchTree();

    std::cout << "--- Выполняем операцию   А = A ⋃ сим B ---\n\n";
    // Вставляем в А значения из B, обходя B симметричным способом
    for (auto symIter = B.sym_begin(); symIter != B.sym_end(); ++symIter) {  
        A.Insert(*symIter);
    }
    std::cout << "\n";

    std::cout << "Обходим  A  в прямом порядке:   ";
    for (auto symIter = A.frw_begin(); symIter != A.frw_end(); ++symIter) {
        std::cout << *symIter << " ";
    }
    std::cout << "\n";

    std::cout << "Обходим  A  в симметричном порядке:   ";
    for (auto symIter = A.sym_begin(); symIter != A.sym_end(); ++symIter) {
        std::cout << *symIter << " ";
    }
    std::cout << "\n";

    std::cout << "Обходим  A  в обратном порядке:   ";
    for (auto symIter = A.rev_begin(); symIter != A.rev_end(); ++symIter) {
        std::cout << *symIter << " ";
    }
    std::cout << "\n";
}