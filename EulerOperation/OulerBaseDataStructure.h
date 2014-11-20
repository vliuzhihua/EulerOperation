#pragma once
#include<iostream>
class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;
//所有的循环双向链表均已pre和nxt为自己作为判断只有一个的条件。
class Solid{
public:
	Solid():preSolid(this), nxtSolid(this), sFaces(NULL), sEdges(NULL), sVertices(NULL), vertexCount(0), loopCount(0), faceCount(0)
	{};
	Solid* preSolid;
	Solid* nxtSolid;
	Face* sFaces;
	Edge* sEdges;//为了便于生成线框模型,暂时没用到.
	Vertex* sVertices;//为了便于生成点模型。
	int vertexCount;//统计这个solid中的顶点的个数
	int loopCount;//统计这个solid中的loop的个数。
	int faceCount;//统计这个solid中的face的个数。
};
class Face{
public:
	Face():fSolid(NULL), preFace(this), nxtFace(this), fLoops(NULL)
	{};
	Solid* fSolid;
	Face* preFace;
	Face* nxtFace;
	Loop* fLoops;
};
class Loop{
public:
	Loop():lFace(NULL), preLoop(this), nxtLoop(this), lHalfEdges(NULL)
	{};
	Face* lFace;
	Loop* preLoop;
	Loop* nxtLoop;
	HalfEdge* lHalfEdges;
	int mark;//后期输出过程中使用。
};
class HalfEdge{
public:
	HalfEdge():edge(NULL), heLoop(NULL), preHalfEdge(this), nxtHalfEdge(this), heStartVertex(NULL)
	{};
	Edge* edge;
	Loop* heLoop;
	HalfEdge* preHalfEdge;
	HalfEdge* nxtHalfEdge;
	Vertex* heStartVertex;
};
class Edge{
public:
	Edge():halfEdge1(NULL), halfEdge2(NULL), preEdge(this), nxtEdge(this)
	{};
	HalfEdge* halfEdge1;
	HalfEdge* halfEdge2;
	Edge* preEdge;
	Edge* nxtEdge;
};
class Vertex{
public:
	Vertex():preVertex(this), nxtVertex(this)
	{
		pos[0] = pos[1] = pos[2] = 0;
	};
	Vertex(Vertex& v):preVertex(this), nxtVertex(this)
	{
		this->pos[0] = v.pos[0];
		this->pos[1] = v.pos[1];
		this->pos[2] = v.pos[2];
	}
	Vertex(float x, float y, float z):preVertex(this), nxtVertex(this)
	{
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
	}
	double pos[3];
	Vertex* preVertex;
	Vertex* nxtVertex;
	int mark;//后期输出过程中使用的。
};
