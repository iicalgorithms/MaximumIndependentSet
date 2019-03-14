//
// Created by Tom Wang on 17/1/21.
//

#ifndef OMIS_VERTEX_H
#define OMIS_VERTEX_H



class Vertex{
private:
    long front;
    long end;
    long length;

public:
    Vertex();
    long InsertEdge(int NodeB);
    long DeleteEdge(int NodeB);
    long GetFrontEdge();
    long GetDegree();
    bool IfExist(int NodeB);
};

#endif //OMIS_VERTEX_H
