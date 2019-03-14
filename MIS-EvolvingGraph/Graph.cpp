//
// Created by Tom Wang on 17/1/11.
//

#include "Graph.h"

#include <iostream>
#include <cstring>

using namespace std;

void Graph::cleanData() {
    memset(Visit,0,sizeof(int)*Latest);
    if (Mode != 7 && Mode != 8) memset(Able,0,sizeof(int)*Latest);
    else {
        if (change == true && Mode == 7) {
            memset(Able,0,sizeof(int)*Latest);
        }
        change = false;
    }
    memset(Count,0,sizeof(int)*Latest);
    Level = 3;
    Q.Reset();
    P.Reset();
}

void Graph::SetMIS() {
    for (int i = 0;i < Latest;i++) {
        if (Exist[i] == 1 && Visit[i] < 0) {
            if (MIS[i] == 1) MIS[i] = 0;
            else MIS[i] = 1;
        }
    }
    change = true;
}

int RemovePointsFromQueue(Graph& G) {
    if (G.Q.isEmpty()) return 0;
    else {
        int NodeA = G.Q.PopFromQueue(); // 从尚未找到匹配的节点中拿出一个节点
        int LastNode = G.P.PopFromQueue();
        /*
         * 下面的程序用于保存当前节点需要的一些状态
         * */
        int RB,RE;
        int l = G.Level;
        G.Level++;
        G.Q.KeepStatue(&RB,&RE);

        //EdgeNow及EdgeID主要用于遍历节点的邻居
        long EdgeNow = G.EdgeList[NodeA].GetFrontEdge();
        long EdgeID = NodeSpace[EdgeNow].ID;
        while (EdgeNow != 0) {
            if (G.Exist[EdgeID] == 1 && G.Able[EdgeID] == 0 && G.Visit[EdgeID] == 0 && G.Core[EdgeID] == 0
                    //G.Count[EdgeID] > MaxVisitTimes * G.EdgeList[EdgeID].GetDegree() ) {//如果找到一个节点不在MIS中并且可以交换
                    //G.Count[EdgeID] < 3){
                    ){
                    G.Visit[EdgeID] = -l;//将这个节点交换进MIS中
                    //G.Count[EdgeID]++;
                    /*
                     * 下面的程序主要用于更新队列和横向边
                     */

                    long AddQueue = G.EdgeList[EdgeID].GetFrontEdge();
                    int AddID = NodeSpace[AddQueue].ID;

                    while (AddQueue != 0) {
                        if (G.Visit[AddID] == 0) {
                            if (G.MIS[AddID] == 1) {//如果EdgeID的一个尚未访问过的邻居是MIS中的点,则其需要被加入到队列中
                                G.Q.AddToQueue(AddID);
                                G.P.AddToQueue(EdgeID);
                                G.Visit[AddID] = -l;
                            } else { //否则是横向边,标记节点不可使用
                                G.Visit[AddID] =2+l;
                            }
                        }

                        AddQueue = NodeSpace[AddQueue].next;
                        AddID = NodeSpace[AddQueue].ID;
                    }
                int ReturnID = RemovePointsFromQueue(G);
                if (ReturnID == 0) return 0; //如果成功找到了一个结果,则直接退出
                else { //否则恢复状态
                    for (int i = 0;i < G.Latest;i++) {
                        if (G.Visit[i] <= -l || G.Visit[i] >= 2 + l) G.Visit[i] = 0;
                        if (Mode == 4) {
                            if (G.Able[i] < -l) G.Able[i] = 0;
                        }
                    }
                    G.Q.ResetStatue(RB,RE);
                    G.P.ResetStatue(RB,RE);
                    if (Mode == 6 || Mode == 7 || Mode == 8) {
                        if (ReturnID != EdgeID) return ReturnID;
                        else {
                            G.Able[EdgeID] = -l;
                        }
                    } else {
                        G.Able[EdgeID] = -l; //将该节点标记成无需访问,没有办法通过该节点进行交换。
                    }
                }
            }
            EdgeNow = NodeSpace[EdgeNow].next;
            EdgeID = NodeSpace[EdgeNow].ID;
        }
        if (Mode == 4) {
            for (int i = 0; i < G.Latest; i++) {
                if (G.Able[i] <= -l) G.Able[i] = 0;
            }
        }
        return LastNode;//对于没找到的情况,返回该节点无法找到需要的结果
    }
}

int Graph::NaiveRemove(int NodeA) {
    if (Mode == 2 || Mode == 5) return 0;
    countR++;
    long NextNode = EdgeList[NodeA].GetFrontEdge();
    int NextID = NodeSpace[NextNode].ID;
    long NeightNode;
    int NeightID;
    while (NextNode != 0) {
        NeightNode = EdgeList[NextID].GetFrontEdge();
        NeightID = NodeSpace[NeightNode].ID;
        bool find = true;
        while (NeightNode != 0) {
            if (MIS[NeightID] == 1 && NeightID != NodeA){
                find = false;
                break;
            }
            NeightNode = NodeSpace[NeightNode].next;
            NeightID = NodeSpace[NeightNode].ID;
        }
        if (find) {
            MIS[NodeA] = 0;
            MIS[NextID] = 1;
            return 1;
        }
        NextNode = NodeSpace[NextNode].next;
        NextID = NodeSpace[NextNode].ID;
    }
    return 0;
}

int Graph::RenewCount() {
   for (int i = 0;i < Latest;i++) {
       if (Exist[i] == 0) continue;
       if (MIS[i] == 1) {
           Count[i] = 0;
           continue;
       }
       Count[i] = 0;
       long NextNode = EdgeList[i].GetFrontEdge();
       int NextID = NodeSpace[NextNode].ID;
       while (NextNode != 0) {
           if (MIS[NextID] == 1) Count[i]++;
           NextNode = NodeSpace[NextNode].next;
           NextID = NodeSpace[NextNode].ID;
       }
       if (Count[i] == 0) MIS[i] = 1;
   }
    return 1;
}

long Graph::getDegree(int NodeA) {
    return EdgeList[NodeA].GetDegree();
}

int Graph::AddEdge(int NodeA, int NodeB) {
    if (NodeA == NodeB) return 2;
    if (!EdgeList[NodeA].IfExist(NodeB)) {
        if (MIS[NodeA] != 1 || MIS[NodeB] != 1) {
            EdgeList[NodeB].InsertEdge(NodeA);
            EdgeList[NodeA].InsertEdge(NodeB);
            return 0;
        } else {//需要将A点或者B点移除到MIS外面,首先尝试移除A点
            if (NaiveRemove(NodeA) == 1) {
                EdgeList[NodeB].InsertEdge(NodeA);
                EdgeList[NodeA].InsertEdge(NodeB);
                return 1;
            } else if (NaiveRemove(NodeB) == 1) {
                EdgeList[NodeB].InsertEdge(NodeA);
                EdgeList[NodeA].InsertEdge(NodeB);
                return 1;
            } else {
                if (Mode == 1) {
                    EdgeList[NodeB].InsertEdge(NodeA);
                    EdgeList[NodeA].InsertEdge(NodeB);
                    if (EdgeList[NodeB].GetDegree() > EdgeList[NodeA].GetDegree()) {
                        MIS[NodeB] = 0;
                    } else {
                        MIS[NodeA] = 0;
                    }
                    return 0;
                }
            }
            cleanData();
            Q.AddToQueue(NodeA);
            P.AddToQueue(-1);
            Visit[NodeA] = -1;
            if (RemovePointsFromQueue(*this) == 0) {
                SetMIS();
                EdgeList[NodeB].InsertEdge(NodeA);
                EdgeList[NodeA].InsertEdge(NodeB);
                return 1;
            } else { //再尝试移除B点
                cleanData();
                Q.AddToQueue(NodeB);
                P.AddToQueue(-1);
                Visit[NodeB] = -1;
                Able[NodeA] = -1;
                if (RemovePointsFromQueue(*this) == 0) {
                    SetMIS();
                    EdgeList[NodeB].InsertEdge(NodeA);
                    EdgeList[NodeA].InsertEdge(NodeB);
                    return 1;
                } else { //如果不能移除则移除度数较大的点
                    EdgeList[NodeB].InsertEdge(NodeA);
                    EdgeList[NodeA].InsertEdge(NodeB);
                    if (EdgeList[NodeB].GetDegree() > EdgeList[NodeA].GetDegree()) {
                        MIS[NodeB] = 0;
                    } else {
                        MIS[NodeA] = 0;
                    }
                    //RenewCount();
                    return 1;
                }
            }
        }
    } else {
        return -1;
    }
}

Graph::Graph() {
    MIS = new int[MaxNode];
    Exist = new char[MaxNode];
    Visit = new int[MaxNode];
    Count = new int[MaxNode];
    Able = new int[MaxNode];
    Core = new char[MaxNode];
    EdgeList = new Vertex[MaxNode];
    change = false;
    memset(MIS,0,MaxNode*sizeof(int));
    memset(Exist,0,MaxNode*sizeof(char));
    memset(Able,0,MaxNode*sizeof(int));
    memset(Count,0,MaxNode*sizeof(int));
    memset(Visit,0,MaxNode*sizeof(int));
    memset(Core,0,MaxNode*sizeof(char));
    Latest = 0;
}

int Graph::NaiveAdd(int NodeA) {
    if (Mode == 2 || Mode == 5) return 0;
    countR++;
    long NextNode = EdgeList[NodeA].GetFrontEdge();
    int NodeID = NodeSpace[NextNode].ID;
    while (NextNode != 0) {
        if (MIS[NodeID] == 1) return 0;
        NextNode = NodeSpace[NextNode].next;
        NodeID = NodeSpace[NextNode].ID;
    }
    MIS[NodeA] = 1;
    return 1;
}

int Graph::NaiveAddCross(int NodeA, int NodeB) {
    if (Mode == 2 || Mode == 5) return 0;
    int u,v;
    u = -1;v = -1;
    long NextNode = EdgeList[NodeA].GetFrontEdge();
    int NodeID = NodeSpace[NextNode].ID;
    while (NextNode != 0) {
        if (MIS[NodeID] == 1) {
            if (u != -1) return 0;
            u = NodeID;
        }
        NextNode = NodeSpace[NextNode].next;
        NodeID = NodeSpace[NextNode].ID;
    }
    NextNode = EdgeList[NodeB].GetFrontEdge();
    NodeID = NodeSpace[NextNode].ID;
    while (NextNode != 0) {
        if (MIS[NodeID] == 1) {
            if (v != -1) return 0;
            v = NodeID;
        }
        NextNode = NodeSpace[NextNode].next;
        NodeID = NodeSpace[NextNode].ID;
    }
    if (u != v) return 0;
    else {
        MIS[u] = 0;
        MIS[NodeA] = 1;
        MIS[NodeB] = 1;
        return 1;
    }
}

int Graph::RemoveEdge(int NodeA, int NodeB) {
    if (!EdgeList[NodeA].IfExist(NodeB)) {
        return -1;
    } else {
        EdgeList[NodeA].DeleteEdge(NodeB);
        EdgeList[NodeB].DeleteEdge(NodeA);
        if (MIS[NodeA] == 0 && MIS[NodeB] == 0)
            if (NaiveAddCross(NodeA,NodeB) == 1) return 1;
        if (MIS[NodeA] == 0) {
            if (NaiveAdd(NodeA) == 1) return 1;
        } else {
            if (NaiveAdd(NodeB) == 1) return 1;
        }
        if (Mode == 1) return 0;
        if (MIS[NodeA] == 0 && Core[NodeA] == 0) {
            cleanData();
            long k = EdgeList[NodeA].GetFrontEdge();
            int ID = NodeSpace[k].ID;
            while (k != 0) {
                if (Exist[ID] == 1) {
                    if (MIS[ID] == 1) {
                        Q.AddToQueue(ID);
                        P.AddToQueue(-1);
                        Visit[ID] = -1;
                    } else {
                        Visit[ID] = 3;
                    }
                }
                k = NodeSpace[k].next;
                ID = NodeSpace[k].ID;
            }
            Visit[NodeA] = -1;
            if (RemovePointsFromQueue(*this) == 0) {
                SetMIS();
                return 1;
            }
	    } else {
		    cleanData();
		    if (MIS[NodeB] == 0 && Core[NodeB] == 0) {
			    long k = EdgeList[NodeB].GetFrontEdge();
			    int ID = NodeSpace[k].ID;
			    while (k != 0) {
				    if (Exist[ID] == 1) {
					    if (MIS[ID] == 1) {
						    Q.AddToQueue(ID);
                            P.AddToQueue(-1);
						    Visit[ID] = -1;
					    } else {
						    Visit[ID] = 3;
					    }
				    }
				    k = NodeSpace[k].next;
				    ID = NodeSpace[k].ID;
			    }
			    Visit[NodeB] = -1;
			    if (RemovePointsFromQueue(*this) == 0) {
				    SetMIS();
				    return 1;
			    }
		    }
	    }
        return 1;
    }
}

int Graph::setExist(int NodeA, bool exist) {
    if (exist) {
        Exist[NodeA] = 1;
        if (Latest <= NodeA) Latest = NodeA+1;
    } else {
        Exist[NodeA] = 0;
        while (Latest > 0 && Exist[Latest-1] == 0) Latest--;
    }
    return 0;
}


int Graph::AddNode(int NodeA) {
    if (Exist[NodeA] == 1) return -1;
    else {
        setExist(NodeA, 1);
        MIS[NodeA] = 1;
        return 1;
    }
}

int Graph::PrintMIS() {
    int n = 0;
    for (int i = 0;i < Latest;i++) {
        if (MIS[i] == 1 && Exist[i] == 1) {
            cout << i << " ";
            n++;
        }
    }
    fprintf(Result,"%d\t",n);
    cout << endl << n << endl;
    return 0;
}

int Graph::RemoveNode(int NodeA) {
    if (Exist[NodeA] == 0) return -1;
    else {
        if (MIS[NodeA] == 1) {
            cleanData();
            Q.AddToQueue(NodeA);
            P.AddToQueue(-1);
            Visit[NodeA] = -1;
            if (RemovePointsFromQueue(*this) == 0) {
                SetMIS();
            }
        }
        setExist(NodeA,0);
        long k = EdgeList[NodeA].GetFrontEdge();
        int j = NodeSpace[k].ID;
        while (k != 0) {
            k = NodeSpace[k].next;
            EdgeList[NodeA].DeleteEdge(j);
            EdgeList[j].DeleteEdge(NodeA);
            j = NodeSpace[k].ID;
        }
        return 0;
    }
}

int Graph::InsertGraph() {
    FILE* DEGREE = fopen((PATH + string("/b_degree.bin")).c_str(), "r");
    int n,m;
    int tt;
    fread(&tt, sizeof(int), 1, DEGREE);
    if(tt != (int)sizeof(int)) {
        printf("sizeof int is different: edge.bin(%d), machine(%d)\n", tt, (int)sizeof(int));
        //return ;
    }
    fread(&n, sizeof(int), 1, DEGREE);
    fread(&m, sizeof(int), 1, DEGREE);
    printf("\tn = %u; m = %u (undirected)\n", n, m);
    Latest = n;
    int *degree = new int[n];
    fread(degree,sizeof(int),n,DEGREE);
    fclose(DEGREE);

    FILE* GRAPH = fopen((PATH + string("/b_adj.bin")).c_str(), "r");
    int *buf = new int[n];
    for (int i = 0;i < n;i++) {
        if(degree[i] > 0) fread(buf, sizeof(int), degree[i], GRAPH);

        for (int j = 0;j < degree[i];j++) {
            EdgeList[i].InsertEdge(buf[j]);
        }
	Exist[i] = 1;
    }
    fclose(GRAPH);

    FILE* MISF = fopen((PATH + string("/b_mis.bin")).c_str(), "r");
    int l;

    int test = 0;
    while (fscanf(MISF,"%d",&l) != EOF) {
	MIS[l] = 1;
    	test++;
    }
    cout << test << endl;
    fprintf(Result,"%d\t",test);
    test = 0;
    if (Mode == 0 || Mode == 5 || Mode == 6 || Mode == 7 || Mode == 8) {
        FILE* core = fopen((PATH+string("/b_core.bin")).c_str(),"r");
        while (fscanf(core,"%d",&l) != EOF) {
            Core[l] = 1;
            test++;
        }
        fclose(core);
    }
    fprintf(Result,"%d\t",test);
    cout << BeginNode << " " << EndNode << endl;
    fclose(MISF);
    return 1;
}
