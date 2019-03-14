#include <iostream>
#include <cstdio>
#include "Vertex.h"
#include "main.h"
#include <cstring>

using namespace std;

Node* NodeSpace;
long* NodeUsed;
long BeginNode;
long EndNode;
int Mode;
string PATH;
FILE* Result;
Vertex* EdgeList;
int IsCore[MaxNode];
int CoreDegree[MaxNode];

int main(int argc, char* argv[]) {
	NodeSpace = new Node[MaxEdge];
	NodeUsed = new long[MaxEdge];
	EdgeList = new Vertex[MaxNode];
    PATH = string(argv[1]);
    memset(IsCore, 0 ,sizeof(int)*MaxNode);
    memset(CoreDegree,0, sizeof(int)*MaxNode);
    FILE* DEGREE = fopen((PATH + string("/b_degree.bin")).c_str(), "r");
    int n,m;
    int tt;
    int CoreSize;
    BeginNode = 1;
    EndNode = MaxEdge;
    CoreSize = atoi(argv[2]);
    fread(&tt, sizeof(int), 1, DEGREE);
    if(tt != (int)sizeof(int)) {
        printf("sizeof int is different: edge.bin(%d), machine(%d)\n", tt, (int)sizeof(int));
        //return ;
    }
    fread(&n, sizeof(int), 1, DEGREE);
    fread(&m, sizeof(int), 1, DEGREE);
    printf("\tn = %u; m = %u (undirected)\n", n, m);
    int *degree = new int[n];
    fread(degree,sizeof(int),n,DEGREE);
    fclose(DEGREE);

    FILE* GRAPH = fopen((PATH + string("/b_adj.bin")).c_str(), "r");
    int *buf = new int[n];
    for (int i = 0;i < n;i++) {
        if(degree[i] > 0) fread(buf, sizeof(int), degree[i], GRAPH);
	if (degree[i] <= CoreSize) {
		IsCore[i] = -1;
	}
        for (int j = 0;j < degree[i];j++) {
            EdgeList[i].InsertEdge(buf[j]);
	    if (IsCore[i] == -1) CoreDegree[buf[j]]++;
            //if (i == degree[j]) printf("SELF-LOOP!");
        }
    }
    fclose(GRAPH);
    long front;
    int now;
    for (int j = 0;j <= CoreSize;j++) {
    	for (int i = 0; i < n;i++) {
		if (CoreDegree[i] >CoreSize && IsCore[i] != 1) {
			/*if (IsCore[i] != -1) 
				IsCore[i] = 1;
			else*/ 
				IsCore[i] = 1;
			front = EdgeList[i].GetFrontEdge();
			while (front != 0) {
				now = NodeSpace[front].ID;
				CoreDegree[now]--;
				front = NodeSpace[front].next;
			}

		}
    		if (CoreDegree[i] <= j && IsCore[i] != -1) {
			IsCore[i] = -1;
			front = EdgeList[i].GetFrontEdge();
			while (front != 0) {
				now = NodeSpace[front].ID;
				CoreDegree[now]++;
				front = NodeSpace[front].next;
			}
		}	
    	}
    }
    /*for (int i = 0; i < n;i++) {
	if (CoreDegree[i] >3) {
		if (IsCore[i] != -1) 
			IsCore[i] = 1;
		else
			IsCore[i] = 1;
	}
    	if (CoreDegree[i] == 1 && IsCore[i] == 0) {
		IsCore[i] = -1;
		front = EdgeList[i].GetFrontEdge();
		while (front != 0) {
			now = NodeSpace[front].ID;
			CoreDegree[now]++;
			front = NodeSpace[front].next;
		}
	}	
    }
    for (int i = 0; i < n;i++) {
	if (CoreDegree[i] >3) {
		if (IsCore[i] != -1) 
			IsCore[i] = 1;
		else
			IsCore[i] = 1;
	}
    	if (CoreDegree[i] == 2 && IsCore[i] == 0) {
		IsCore[i] = -1;
		front = EdgeList[i].GetFrontEdge();
		while (front != 0) {
			now = NodeSpace[front].ID;
			CoreDegree[now]++;
			front = NodeSpace[front].next;
		}
	}	
    }
    for (int i = 0; i < n;i++) {
	if (CoreDegree[i] >3) {
		if (IsCore[i] != -1) 
			IsCore[i] = 1;
		else
			IsCore[i] = 1;
	}
    	if (CoreDegree[i] == 3 && IsCore[i] == 0) {
		IsCore[i] = -1;
		front = EdgeList[i].GetFrontEdge();
		while (front != 0) {
			now = NodeSpace[front].ID;
			CoreDegree[now]++;
			front = NodeSpace[front].next;
		}
	}	
    }*/
    for (int i = 0; i < n;i++) {
	    if (CoreDegree[i] >CoreSize) {
		    IsCore[i] = 1;
	    }
    }

	FILE *core = fopen((PATH+string("/b_core.bin")).c_str(),"wb");
 	int CoreCount = 0;
	int NonCoreCount = 0;
	for (int j = 0;j < n;j++) {
		if (IsCore[j] == -1) NonCoreCount++;
		if (IsCore[j] == 1) {
			CoreCount++;
			fprintf(core,"%d ",j);
		}
	}
	fprintf(core,"\n");
	fclose(core);
	cout << CoreCount << " " << NonCoreCount << endl;
   return 0;
}


    	
