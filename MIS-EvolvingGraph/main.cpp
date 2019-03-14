#include <iostream>
#include "main.h"
#include "Graph.h"
#include <sys/time.h>

using namespace std;

Node* NodeSpace;
long* NodeUsed;
long BeginNode = 1;
long EndNode = MaxEdge;
int Mode = 0;

Graph G;
string PATH;
FILE* Result;
string log;

struct timeval t1,t2;
double timeuse;


int main(int argc, char* argv[]) {
    char Symbol;
    long n;
    NodeSpace = new Node[MaxEdge];
    NodeUsed = new long[MaxEdge];
    if (argc < 3) {
        //TODO Print Useage
        //Mode :    0 Core优化及Naive优化 LSTwo + LazySearch+
        //          1 Naive程序 LSTwo
        //          2 无Core及Naive优化 LazySearch
        //          3 Naive优化 LSTwo+ LazySearch
        //          4 准确算法 ExactUpdate
        //          5 Core优化 LazySearch+
        //          6 新标记算法
        //          7 新标记算法+LazyLabel
        //          8 新标记算法+不修改Label

    }
    PATH = string(argv[1]); // Path_to_input_graph
    log = string(argv[2]); // Path_to_running_log
    Result = fopen(log.c_str(),"a+");
    fprintf(Result,"%s\t",argv[1]);
    if (argc > 3) Mode = atoi(argv[3]);
    FILE* SEQ = fopen((PATH+string("/b_seq.bin")).c_str(),"r"); // Path_to_update_seq
    InitialNode();
    clock_t T1 = clock();
    /*
     * Exist[]代表节点的状态,0表示节点不存在,1表示节点可以被搜索,2表示节点被选中将要交换
     * -ID表示节点由于ID的节点被加入MIS中导致该节点无法被使用。
     */
    long count = 0;
	long EdgeCount = 1;

    while (fscanf(SEQ,"%c ",&Symbol) != EOF) {
        int NodeA,NodeB;
        switch (Symbol) {
            case '\n':
		        count++;
                break;
            case 'q':
                cout << "Quitting..." << endl;
                return 0;
            case 'i'://增加一条边
                fscanf(SEQ,"%d %d",&NodeA,&NodeB);

                gettimeofday(&t1,NULL);
		        EdgeCount++;
		        G.AddEdge(NodeA,NodeB);
                gettimeofday(&t2,NULL);
                timeuse = timeuse+ t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
                break;
            case 'I':
                fscanf(SEQ,"%d",&NodeA);
                G.AddNode(NodeA);
                break;
            case 'r':
                fscanf(SEQ,"%d %d",&NodeA,&NodeB);

                gettimeofday(&t1,NULL);
                EdgeCount--;
                G.RemoveEdge(NodeA,NodeB);
                gettimeofday(&t2,NULL);
                timeuse = timeuse + t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
                break;
            case 'R':
                fscanf(SEQ,"%d",&NodeA);

                gettimeofday(&t1,NULL);
                EdgeCount = EdgeCount-G.getDegree(NodeA);
                G.RemoveNode(NodeA);
                gettimeofday(&t2,NULL);
                timeuse = timeuse + t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
                break;
            case 'P':
                G.PrintMIS();
                break;
            case 'G':
                G.InsertGraph();
                break;
            default:
                cout << "Wrong Input!" << endl;
                return 1;
        }
    }
    clock_t T2 = clock();
    cout << endl << "TotalTime: " << (double)(T2-T1)/CLOCKS_PER_SEC << endl;
    cout << "RunningTime: " << timeuse << endl;
    fprintf(Result,"%f\n",timeuse);
    cout << G.countR << endl;
    fclose(SEQ);
}
