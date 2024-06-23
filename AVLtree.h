//
// Created by ramsey on 5/3/2023.
//

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H
#include "Vertex.h"
#include "exception"
#include "stdexcept"
#include "Compare.h"

using namespace std;


template<class Data,class Key, class Compare>
class AVLtree {
private:
    Vertex<Data,Key>* root;
    Vertex<Data,Key>* mostRightLeaf;
    Vertex<Data,Key>* mostLeftLeaf;
    Compare cp;
    const int EQUAL=0;
    const int GO_RIGHT=1;
    const int GO_LEFT=-1;



public:
    AVLtree():cp(){ root= nullptr; mostRightLeaf= nullptr; mostLeftLeaf= nullptr; }
    // explicit AVLtree(Vertex<Data,Key>* root); //check: review
    ~AVLtree();; // check: review, and should be there a; at the end


    Vertex<Data, Key>* getRoot() const;
    Vertex<Data, Key>* getMostRightLeaf() const;
    Vertex<Data, Key>* getMostLeftLeaf() const;
    void UpdateMostLeftLeaf(); // check: you need to add them in the other functions as the end of insert and remove
    void UpdateMostRightLeaf();
    void setRoot(Vertex<Data, Key>* newRoot);
    void clearTree(Vertex<Data, Key> * t);
    Vertex<Data,Key>* find(Vertex<Data, Key> * vertex,Key key);
    Vertex<Data,Key>* findForGenre(Vertex<Data, Key> * vertex,Key key,Data data); // for genre trees
    int height(Vertex<Data, Key> * vertex);
    int getBalance(Vertex<Data, Key> *vertex);
    Vertex<Data,Key>* LLRotation(Vertex<Data, Key>* p); // single right rotation situation
    Vertex<Data,Key>* RRRotation(Vertex<Data, Key>* q); // single left rotation situation
    Vertex<Data,Key>* LRRotation(Vertex<Data, Key>* C); // double rotation, LL THEN RR
    Vertex<Data,Key>* RLRotation(Vertex<Data, Key>* C); // double rotation, RR THEN LL
    Vertex<Data, Key>* insert(Vertex<Data, Key>* vertex, Key key,Data data); //returns an updated tree
    Vertex<Data, Key> * remove(Vertex<Data, Key> *vertex, Key key);
    Vertex<Data, Key> * removeForGenre(Vertex<Data, Key> *vertex,Key key,Data data); // for genre trees
    Vertex<Data, Key> *balanceTree(Vertex<Data, Key> *vertex);
    void inorder(Vertex<Data, Key> *t);
    void inorderToOutput(Vertex<Data, Key> *t,Data* moviesArray); //check: what is this warning?
};

template<class Data, class Key, class Compare>
Vertex<Data, Key> *AVLtree<Data, Key, Compare>::removeForGenre(Vertex<Data, Key> *vertex, Key key, Data data) {
    // Find the node to be removed
    Vertex<Data, Key>* node = findForGenre(vertex, key,data);
    if (node == nullptr) {
        throw std::runtime_error("Node not found");
    }

    //check: if this part is necessary, also check the == and != operators
    // Find the parent node of the node to be removed
    Vertex<Data, Key>* parent = nullptr;
    Vertex<Data, Key>* current = vertex;
    while (current != nullptr && current != node) {
        parent = current;
        if (cp(current,node)==GO_LEFT) { //  Compare(key, current->key)  // check: this should be replaced with compare key < current->key
            current = current->left; //check: 21.5 is this the right order? or should it be current,node?
        } else {
            current = current->right;
        }
    }

    // Case 1: Node has no children
    if (node->left == nullptr && node->right == nullptr) {
        if (parent == nullptr) {
            // Root node
            delete vertex;
            return nullptr;
        }
        if (parent->left == node) {
            parent->left = nullptr;
        } else {
            parent->right = nullptr;
        }
        delete node; //check: do i need to return something here??!
    }
        // Case 2: Node has only a right child
    else if (node->left == nullptr) {
        if (parent == nullptr) {
            // Root node
            Vertex<Data, Key>* rightChild = node->right;
            rightChild->parent = nullptr;
            delete node; //check: 22.5 should it be delete vertex or node?
            return rightChild;
        }
        if (parent->left == node) {
            parent->left = node->right;
        } else {
            parent->right = node->right;
        }
        node->right->parent = parent;
        delete node; //check: do i need to return something here??!
    }
        // Case 3: Node has only a left child
    else if (node->right == nullptr) {
        if (parent == nullptr) {
            // Root node
            Vertex<Data, Key>* leftChild = node->left;
            leftChild->parent = nullptr;
            delete vertex;
            return leftChild;
        }
        if (parent->left == node) {
            parent->left = node->left;
        } else {
            parent->right = node->left;
        }
        node->left->parent = parent;
        delete node; //check: do i need to return something here??!
    }
        // Case 4: Node has both left and right children
    else {
        // Find the successor node
        Vertex<Data, Key>* successor = node->right;
        while (successor!= nullptr && successor->left != nullptr) {
            successor = successor->left;
        }
        // Replace the node's key and data with the successor's key and data
        node->key = successor->key;
        node->data = successor->data;

        // Remove the successor node recursively
        node->right = removeForGenre(node->right, successor->key,successor->data);
        if(node->right!= nullptr) {
            node->right->parent = node; // check: 22.5 does the below part update this?
        }
        // return remove(node->right, successor->key);
        //check: returns the whole tree, you either add another condition for the while or return this
    }

    // Update the height of the current node
    //vertex->height = 1 + std::max(height(vertex->left), height(vertex->right));
    // Balance the tree
    // vertex = balanceTree(vertex);

    //check: to put the below in a function
    // updates after deletion
    current = parent;
    if(current!= nullptr) parent=current->parent;
    while (current != nullptr && current!=vertex) {
        // balancing the tree if its needed ( in insertion only once happens)
        if (abs(getBalance(current)) == 2) {
            current = balanceTree(current);
            current->parent = parent;
            if (parent != nullptr) { //updating the parent
                if (cp(parent,current)==GO_RIGHT) { // the right child parent->key < current->key
                    // check: 21.5 is this the correct order?
                    parent->right = current;
                } else { // the left child
                    parent->left = current;

                }
            }
        }
        // updating the heights of the ancestors
        current->height = 1 + std::max(height(current->left), height(current->right));
        // check: we can make the insert function to update the tree and not return a vertex
        // by updating the next line to root=current;
        if(parent== nullptr)
        {
            return current; }// it checked all the tree so this is our root now
        else {
            current = current->parent;
            parent = current->parent;
        }
    }
    return vertex;

}

template<class Data, class Key, class Compare>
Vertex<Data, Key> *AVLtree<Data, Key, Compare>::findForGenre(Vertex<Data, Key> *vertex, Key key, Data data) {
    if(vertex == nullptr){
        throw std::runtime_error("Not found");// cuz its null ptr
    }
     Vertex<Data,Key> tempVertex(key,data);
    //Vertex<Data,Key>* tempVertex= new Vertex<Data,Key>(key,data);
     // Vertex<Data,Key>* tempVertexptr=&tempVertex; //check: which one ?
     if(cp(vertex,tempVertex)==EQUAL) {return  vertex;}
     if(cp(vertex,tempVertex)==GO_LEFT) { //if temp>vertex
         return  findForGenre(vertex->left,key,data);
     }
     else{
         return findForGenre(vertex->right,key,data);
     }
}

// implantation
template<class Data, class Key, class Compare>
void AVLtree<Data, Key, Compare>::inorderToOutput(Vertex<Data, Key> *t,Data* moviesArray){
    if(t == nullptr)
        return;
    inorderToOutput(t->left,moviesArray);
    *moviesArray=t->data;
    moviesArray++;
    // cout << t->key << " ";
    inorderToOutput(t->right,moviesArray);
}

template<class Data, class Key, class Compare>
void AVLtree<Data, Key, Compare>::inorder(Vertex<Data, Key> *t) {
    if(t == nullptr)
        return;
    inorder(t->left);
    cout << t->key << " ";
    inorder(t->right);
}

template<class Data, class Key, class Compare>
void AVLtree<Data, Key, Compare>::UpdateMostLeftLeaf() {
    Vertex<Data, Key>* successor = root;
    while (successor!= nullptr && successor->left != nullptr) {
        successor = successor->left;
    }
    // Replace the node's key and data with the successor's key and data
    mostLeftLeaf=successor;
} // check: is this okay  as in pointers matters

template<class Data, class Key, class Compare>
void AVLtree<Data, Key, Compare>::UpdateMostRightLeaf() {
    Vertex<Data, Key>* successor = root;
    while (successor!= nullptr && successor->right != nullptr) {
        successor = successor->right;
    }
    // Replace the node's key and data with the successor's key and data
    mostRightLeaf=successor;
} // check: is this okay  as in pointers matters

template<class Data, class Key, class Compare>
Vertex<Data, Key> *AVLtree<Data, Key, Compare>::getRoot() const {
    return root;
}

template<class Data, class Key, class Compare>
void AVLtree<Data, Key, Compare>::setRoot(Vertex<Data, Key> *newRoot) {
    root = newRoot;
}

template<class Data, class Key, class Compare>
AVLtree<Data, Key, Compare>::~AVLtree() {
    clearTree(root);
   // if(root!= nullptr) delete root;
}

template<class Data, class Key, class Compare>
void AVLtree<Data, Key, Compare>::clearTree(Vertex<Data, Key> *t) {
    if(t == NULL)
        return;
    clearTree(t->left);
    t->left= nullptr;
    clearTree(t->right);
    t->right= nullptr;
    delete t;
    t= nullptr;
}

template<class Data,class Key, class Compare> // check: this could be without recursion
Vertex<Data, Key>* AVLtree<Data, Key, Compare>::find(Vertex<Data, Key> * vertex, Key key) {
    // check: also we could replace it to return bool
    if(vertex == nullptr){
        throw std::runtime_error("Not found");// cuz its null ptr
    }
   /* Vertex<Data,Key> tempVertex(key,vertex->data);
    tempVertex.data->setID(key);
    // Vertex<Data,Key>* tempVertexptr=&tempVertex;
    if(cp(vertex,tempVertex)==EQUAL) {return  vertex;}
    if(cp(vertex,tempVertex)==GO_LEFT) {return  find(vertex->left,key);}
    else{
        return find(vertex->right,key);
    }*/
    RegularCompare lessThan;
    if (lessThan(vertex->key, key) == EQUAL) { return vertex; } // vertex->key==key Compare(vertex->key,key)
    if(lessThan(vertex->key, key) == GO_LEFT){ // check: this should be replaced with compare and i should send vertexes key<vertex->key
        return find(vertex->left,key);
    }
    else{
        return find(vertex->right,key);
    }
}

template<class Data,class Key, class Compare>
int AVLtree<Data,Key,Compare>::height(Vertex<Data, Key> *vertex) {
    return (vertex == nullptr ? -1 : vertex->height);
}

template<class Data,class Key, class Compare>
int AVLtree<Data,Key,Compare>::getBalance(Vertex<Data, Key> *vertex)
{
    if(vertex == NULL)
        return 0;
    else
        return height(vertex->left) - height(vertex->right);
}

template<class Data,class Key, class Compare>
Vertex<Data,Key>* AVLtree<Data,Key,Compare>::LLRotation(Vertex<Data, Key>* p){
    if(p == nullptr) return nullptr;
    Vertex<Data,Key>* q=p->left; // check for dynamic errors and if I need to do something bout q
    Vertex<Data,Key>* B; // do i need to check: if q is null
    if(q!= nullptr) { B=q->right;}
    else{  B= nullptr; }
    p->left=B;
    if(B!= nullptr) {B->parent=p;}
    p->height=max(height(p->left), height(p->right))+1;
    q->right=p;
    p->parent=q;
    q->height=max(height(q->left), height(q->right))+1;
    return q;
}

template<class Data,class Key, class Compare>
Vertex<Data,Key>* AVLtree<Data,Key,Compare>::RRRotation(Vertex<Data, Key>* q){
    if(q == nullptr) return nullptr;
    Vertex<Data,Key>* p=q->right; // try catch check for dynamic errors and if I need to do something bout p
    Vertex<Data,Key>* B;
    if(p!= nullptr) { B=p->left;}
    else{  B= nullptr; }
    q->right=B;
    if(B!= nullptr) {B->parent=q;}
    q->height=max(height(q->left), height(q->right))+1;
    p->left=q;
    p->parent=q->parent;
    q->parent=p;
    p->height=max(height(p->left), height(p->right))+1;
    return p;
}

template<class Data,class Key, class Compare>
Vertex<Data,Key>* AVLtree<Data,Key,Compare>::LRRotation(Vertex<Data, Key>* C){
    if(C == nullptr) return nullptr;
    C->left= RRRotation(C->left);
    C->left->parent=C;
    C->height=max(height(C->left), height(C->right))+1; // is this necessary
    return LLRotation(C);
}

template<class Data,class Key, class Compare>
Vertex<Data,Key>* AVLtree<Data,Key,Compare>::RLRotation(Vertex<Data, Key>* C){
    if(C == nullptr) return nullptr;
    C->right= LLRotation(C->right);
    C->right->parent=C;
    C->height=max(height(C->left), height(C->right))+1; // is this necessary
    return RRRotation(C);
}

template<class Data, class Key, class Compare>
Vertex<Data, Key>* AVLtree<Data, Key, Compare>::insert(Vertex<Data, Key>* vertex, Key key, Data data) {

    /*if(find(getRoot(),key)== true){
            throw std::runtime_error("already inserted in the tree");
        }*/ //if the insert doesn't need to update the node
    Vertex<Data, Key>* newNode = new Vertex<Data, Key>(key, data);
    if (vertex == nullptr) { // vertex=getroot()
        root = newNode;
        return root;
    }
    // Find the appropriate position to insert the new node
    Vertex<Data, Key>* parent = nullptr;
    Vertex<Data, Key>* current = vertex;
    while (current != nullptr) {
        parent = current;
        if (cp(current,newNode)==GO_LEFT) { // check: this should be replaced with compare key < current->key
            // check: 21.5
            current = current->left;
        } else if (cp(current,newNode)==GO_RIGHT) { // check: this should be replaced with compare key > current->key
            current = current->right;
        } else {
            // Key already exists, update the data and return
            current->data = data;
            delete newNode; // Delete the unused new node
            return vertex;
        }
    }
    // Insert the new node as a child of the parent
    if (cp(parent,newNode)==GO_LEFT) { // check: this should be replaced with compare key < parent->key //check: 21.5
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    newNode->parent = parent;
    // updates after insertion
    current = newNode->parent;
    parent=current->parent;
    while (current != nullptr) {
        // balancing the tree if its needed ( in insertion only once happens)
        if (abs(getBalance(current)) == 2) {
            current = balanceTree(current);
            current->parent = parent;
            if (parent != nullptr) { //updating the parent
                if (cp(parent,current)==GO_RIGHT) { // the right child parent->key < current->key
                    // check: this should be replaced with compare ////check: 21.5
                    parent->right = current;
                } else { // the left child
                    parent->left = current;

                }
            }
        }
        // updating the heights of the ancestors
        current->height = 1 + std::max(height(current->left), height(current->right));
        // check: we can make the insert function to update the tree and not return a vertex
        // by updating the next line to root=current;
        if(parent== nullptr) return current; // it checked all the tree so this is our root now
        else {
            current = current->parent;
            parent = current->parent;
        }
    }
    return vertex;
}

template<class Data, class Key, class Compare>
Vertex<Data, Key>* AVLtree<Data, Key, Compare>::remove(Vertex<Data, Key>* vertex, Key key) {
    // Find the node to be removed
    Vertex<Data, Key>* node = find(vertex, key);
    if (node == nullptr) {
        throw std::runtime_error("Node not found");
    }

    //check: if this part is necessary, also check the == and != operators
    // Find the parent node of the node to be removed
    Vertex<Data, Key>* parent = nullptr;
    Vertex<Data, Key>* current = vertex;
    while (current != nullptr && current != node) {
        parent = current;
        if (cp(current,node)==GO_LEFT) { //  Compare(key, current->key)  // check: this should be replaced with compare key < current->key
            current = current->left; //check: 21.5
        } else {
            current = current->right;
        }
    }

    // Case 1: Node has no children
    if (node->left == nullptr && node->right == nullptr) {
        if (parent == nullptr) {
            // Root node
            delete vertex;
            return nullptr;
        }
        if (parent->left == node) {
            parent->left = nullptr;
        } else {
            parent->right = nullptr;
        }
        delete node; //check: do i need to return something here??!
    }
        // Case 2: Node has only a right child
    else if (node->left == nullptr) {
        if (parent == nullptr) {
            // Root node
            Vertex<Data, Key>* rightChild = node->right;
            rightChild->parent = nullptr;
            delete node; //check: 22.5 should it be node?
            return rightChild;
        }
        if (parent->left == node) {
            parent->left = node->right;
        } else {
            parent->right = node->right;
        }
        node->right->parent = parent;
        delete node; //check: do i need to return something here??!
    }
        // Case 3: Node has only a left child
    else if (node->right == nullptr) {
        if (parent == nullptr) {
            // Root node
            Vertex<Data, Key>* leftChild = node->left;
            leftChild->parent = nullptr;
            delete vertex;
            return leftChild;
        }
        if (parent->left == node) {
            parent->left = node->left;
        } else {
            parent->right = node->left;
        }
        node->left->parent = parent;
        delete node; //check: do i need to return something here??!
    }
        // Case 4: Node has both left and right children
    else {
        // Find the successor node
        Vertex<Data, Key>* successor = node->right;
        while (successor!= nullptr && successor->left != nullptr) {
            successor = successor->left;
        }
        // Replace the node's key and data with the successor's key and data
        node->key = successor->key;
        node->data = successor->data;

        // Remove the successor node recursively
        node->right = remove(node->right, successor->key);
        if(node->right!= nullptr) {
            node->right->parent = node; // check: 22.5 does the below part update this?
        }
        // return remove(node->right, successor->key);
        //check: returns the whole tree, you either add another condition for the while or return this
    }

    // Update the height of the current node
    //vertex->height = 1 + std::max(height(vertex->left), height(vertex->right));
    // Balance the tree
   // vertex = balanceTree(vertex);

    //check: to put the below in a function
    // updates after deletion
    current = parent;
    if(current!= nullptr) parent=current->parent;
    while (current != nullptr && current!=vertex) {
        // balancing the tree if its needed ( in insertion only once happens)
        if (abs(getBalance(current)) == 2) {
            current = balanceTree(current);
            current->parent = parent;
            if (parent != nullptr) { //updating the parent
                if (cp(parent,current)==GO_RIGHT) { // the right child parent->key < current->key
                    // //check: 21.5
                    parent->right = current;
                } else { // the left child
                    parent->left = current;

                }
            }
        }
        // updating the heights of the ancestors
        current->height = 1 + std::max(height(current->left), height(current->right));
        // check: we can make the insert function to update the tree and not return a vertex
        // by updating the next line to root=current;
        if(parent== nullptr)
        {
            return current; }// it checked all the tree so this is our root now
        else {
            current = current->parent;
            parent = current->parent;
        }
    }
    return vertex;

}

template<class Data, class Key, class Compare>
Vertex<Data, Key>* AVLtree<Data, Key, Compare>::balanceTree(Vertex<Data, Key>* vertex) {
    // Update the height of the current node
    vertex->height = 1 + std::max(height(vertex->left), height(vertex->right));

    // Check the balance factor
    int balance = getBalance(vertex);

    // Perform rotation if necessary
    if (balance > 1) {
        if (getBalance(vertex->left) >= 0) {
            // Left-Left case
            return LLRotation(vertex);
        } else {
            // Left-Right case
            return LRRotation(vertex);
        }
    } else if (balance < -1) {
        if (getBalance(vertex->right) <= 0) {
            // Right-Right case
            return RRRotation(vertex);
        } else {
            // Right-Left case
            return RLRotation(vertex);
        }
    }

    // No rotation needed, return the current node
    return vertex;
}

template<class Data, class Key, class Compare>
Vertex<Data, Key> *AVLtree<Data, Key, Compare>::getMostRightLeaf() const {
    return mostRightLeaf;
}

template<class Data, class Key, class Compare>
Vertex<Data, Key> *AVLtree<Data, Key, Compare>::getMostLeftLeaf() const {
    return mostLeftLeaf;
}

#endif //WET1_AVLTREE_H
