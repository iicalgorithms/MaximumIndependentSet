//
// Created by Tom Wang on 17/1/11.
//

#include "Node.h"
#include "main.h"
#include <cstring>

long AddNode(int ID) {
    if (BeginNode == EndNode) return -1;
    long i = NodeUsed[BeginNode];
    if (i == 0) {
	i = BeginNode;
        NodeUsed[BeginNode] = BeginNode;
    }
    BeginNode = BeginNode +1;
    NodeSpace[i].ID = ID;
    NodeSpace[i].next = 0;
    return i;
}

long RemoveNode(long ID) {
    if (ID == 0) return -1;
    BeginNode = BeginNode-1;
    NodeUsed[BeginNode] = ID;
    return ID;
}

int InitialNode() {
    /*for (int i = 0;i < MaxEdge;i++) {
        NodeUsed[i] = i;
    }*/
    memset(NodeUsed,0,sizeof(long)*MaxEdge);
    BeginNode = 1;
    EndNode = MaxEdge;
    return 0;
}
