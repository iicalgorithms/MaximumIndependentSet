//
// Created by Tom Wang on 17/1/12.
//


//#define DEBUG

#include "Node.h"
#include <string>
#include <cstdio>
#include <cstdlib>

#ifndef MaxNode
    #define MaxNode 100000000
#endif
#ifndef MaxEdge
    #define MaxEdge 4000000000
#endif
#ifndef MaxVisitTimes
    #define MaxVisitTimes 2
#endif


extern Node* NodeSpace;
extern long* NodeUsed;
extern long BeginNode;
extern long EndNode;
extern std::string PATH;
extern int Mode;
extern FILE* Result;