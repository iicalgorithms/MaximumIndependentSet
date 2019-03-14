//
// Created by Tom Wang on 17/1/24.
//

#include "Queue.h"

Queue::Queue() {
    for (int i = 0;i < 2*MaxNode;i++)
        Data[i] = 0;
    Begin = MaxNode;
    End = MaxNode;
}

Queue::Queue(const Queue &temp) {
    Begin = temp.Begin;
    End = temp.End;
    for (int i = Begin;i < End;i++) {
        Data[i] = temp.Data[i];
    }
}

Queue& Queue::operator=(const Queue &temp) {
    if (this == &temp) return *this;
    Begin = temp.Begin;
    End = temp.End;
    for (int i = Begin;i < End;i++) {
        Data[i] = temp.Data[i];
    }
    return *this;
}

int Queue::PopFromQueue() {
    int Return = 0;
    if (isEmpty()) return -1;
    else {
        Return = Data[Begin];
        Begin = Begin+1;
        return Return;
    }
}

bool Queue::isEmpty() {
    if (Begin == End) return true;
    else return false;
}

void Queue::AddToQueue(int NodeA) {
    Data[End] = NodeA;
    End = End+1;
}

void Queue::Reset() {
    Begin = MaxNode;
    End = MaxNode;
}

void Queue::KeepStatue(int* RB, int* RE) {
    *RB = Begin;
    *RE = End;
}

void Queue::ResetStatue(int RB, int RE) {
    Begin = RB;
    End = RE;
}