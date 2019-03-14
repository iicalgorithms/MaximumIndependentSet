//
// Created by Tom Wang on 17/1/24.
//

#ifndef OMIS_QUEUE_H
#define OMIS_QUEUE_H

#include "main.h"

class Queue {
public:
    Queue();
    void AddToQueue(int NodeA);
    bool isEmpty();
    int PopFromQueue();
    void Reset();
    Queue(const Queue& temp);
    Queue& operator = (const Queue& temp);
    void KeepStatue(int* RB,int* RE);
    void ResetStatue(int RB, int RE);
private:
    int Data[MaxNode*2];
    int Begin;
    int End;
};


#endif //OMIS_QUEUE_H
