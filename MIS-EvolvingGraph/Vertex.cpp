//
// Created by Tom Wang on 17/1/21.
//

#include "Vertex.h"
#include "Node.h"

#include "main.h"

Vertex::Vertex() {
    front = 0;
    end = 0;
    length = 0;
}

long Vertex::GetDegree() {
    return length;
}

long Vertex::InsertEdge(int NodeB) {
    long i = AddNode(NodeB);
    long j,k;
    j = front;
    if (j == 0) {
        front = i;
        end = i;
        length = 1;
        return i;
    } else {
        length++;
        if (NodeSpace[j].ID > NodeB) {
            NodeSpace[i].next = j;
            front = i;
            return i;
        }
        k = NodeSpace[j].next;
        while (k != 0 && NodeSpace[j].ID < NodeB) {
            j = k;
            k = NodeSpace[j].next;
        }
        if (NodeSpace[j].ID == NodeB) {
            length--;
            return -1;
        }
        NodeSpace[j].next = i;
        NodeSpace[i].next = k;
        if (k == 0) {
            end = i;
        }
        return i;
    }
}

long Vertex::GetFrontEdge() {
    return front;
}

long Vertex::DeleteEdge(int NodeB) {
    long i = front;
    long j = 0;
    while (i != 0 && NodeSpace[i].ID < NodeB) {
        j = i;
        i = NodeSpace[i].next;
    }
    if (i == 0 || NodeSpace[i].ID > NodeB) {
        return -1;
    } else {
        if (NodeSpace[i].ID == NodeB) {
            length--;
            if (j != 0) NodeSpace[j].next = NodeSpace[i].next;
            else front = NodeSpace[i].next;
            if (i == end) end = j;
            RemoveNode(i);
            return 1;
        }
    }
    return -1;
}

bool Vertex::IfExist(int NodeB) {
    long i = front;
    while (i != 0 && NodeSpace[i].ID < NodeB) {
        i = NodeSpace[i].next;
    }
    return NodeSpace[i].ID == NodeB;
}