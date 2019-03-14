//
// Created by Tom Wang on 17/1/11.
//

#ifndef OMIS_NODE_H
#define OMIS_NODE_H


class Node {
public:
    int ID;
    long next;
};

long AddNode(int ID);

long RemoveNode(long ID);

int InitialNode();

#endif //OMIS_NODE_H
