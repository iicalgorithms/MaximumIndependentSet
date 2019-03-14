//
// Created by Tom Wang on 17/1/11.
//

#ifndef OMIS_GRAPH_H
#define OMIS_GRAPH_H


#include "Vertex.h"
#include "main.h"
#include "Queue.h"

class Graph {
private:
    Vertex* EdgeList;
    int* MIS;
    char* Exist;
    Queue Q;
    Queue P;
    int* Visit;
    int* Count;
    char* Core;
    void cleanData();
    void SetMIS();
    int* Able;
    friend int RemovePointsFromQueue(Graph& G);
    int Level;
    int NaiveAddCross(int NodeA, int NodeB);
    int NaiveRemove(int NodeA);
    int NaiveAdd(int NodeA);
    bool change;

public:
    int AddEdge(int NodeA, int NodeB);
    int RemoveEdge(int NodeA, int NodeB);
    Graph();
    int setExist(int NodeA, bool exist);
    int Latest;
    int AddNode(int NodeA);
    int RemoveNode(int NodeA);
    int PrintMIS();
    int InsertGraph();
    long getDegree(int NodeA);
    int RenewCount();
    int countR;
};

int RemovePointsFromQueue(Graph& G);

#endif //OMIS_GRAPH_H
