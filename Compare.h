//
// Created by ramse on 5/17/2023.
//

#ifndef LASTTEST_LOG_COMPARE_H
#define LASTTEST_LOG_COMPARE_H

// the right one is the temporary one 
class RegularCompare{
public:
     const int EQUAL=0;
     const int GO_RIGHT=1;
     const int GO_LEFT=-1;

    RegularCompare()=default;
    int operator()(int key,int tempKey) const{
        if(key>tempKey){
            return GO_LEFT;
        }
        if(key<tempKey){
            return GO_RIGHT;
        }
        return EQUAL;
    }
/*
    int operator()(int id1, int id2, int ratings1, int ratings2, int views1, int views2) const {
        if (ratings1 < ratings2) {
            return GO_RIGHT;
        }
        if (ratings1 > ratings2) {
            return GO_LEFT;
        } else {
            if (views1 < views2) {
                return GO_RIGHT;
            }
            if (views1 > views2) {
                return GO_LEFT;
            } else {
                if (id1 > id2) {
                    return GO_RIGHT;
                }
                if (id1 < id2) {
                    return GO_LEFT;
                } else return EQUAL;
            }
        }
    }
*/
};

// the right one is the temporary one 
template<class Vertex>
class CompareID{

public:
    const int EQUAL=0;
    const int GO_RIGHT=1;
    const int GO_LEFT=-1;


    // in descending order as they required
    int operator()(Vertex*& ver1,Vertex*& tempVertex) const { //check: the *& is okay here or does it make problems?
        if (ver1->data->getID() > tempVertex->data->getID()) {
            return GO_LEFT;
        }
        if (ver1->data->getID() < tempVertex->data->getID()) {
            return GO_RIGHT;
        } else return EQUAL;


    }

    int operator()(Vertex*& ver1,Vertex& tempVertex) const { //check: the *& is okay here or does it make problems?
        if (ver1->data->getID() > tempVertex.data->getID()) {
            return GO_LEFT;
        }
        if (ver1->data->getID() < tempVertex.data->getID()) {
            return GO_RIGHT;
        } else return EQUAL;


    }
}; //check: these



#endif //LASTTEST_LOG_COMPARE_H
