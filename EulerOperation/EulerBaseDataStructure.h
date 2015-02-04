#pragma once

#include<iostream>
class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;



//Tuple是三元组类，将被typedef为Vecter和Point两个类，以便见名识意。
class Tuple{
public:
	double pos[3];

	Tuple(){
		pos[0] = 0;
		pos[1] = 0;
		pos[2] = 0;
	}
	Tuple(double x, double y, double z){
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
	}
	Tuple(double p[]){
		pos[0] = p[0];
		pos[1] = p[1];
		pos[2] = p[2];
	}
	double length(){
		return sqrt(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
	}

	Tuple cross(Tuple p){
		return Tuple(pos[1] * p.pos[2] - pos[2] * p.pos[1], 
					pos[2] * p.pos[0] - pos[0] * p.pos[2], 
					pos[0] * p.pos[1] - pos[1] * p.pos[0]);
	}

	friend Tuple operator + (Tuple a, Tuple b){
		return Tuple(a.pos[0] + b.pos[0], a.pos[1] + b.pos[1], a.pos[2] + b.pos[2]);
	}
	friend Tuple operator - (Tuple a, Tuple b){
		return Tuple(a.pos[0] - b.pos[0], a.pos[1] - b.pos[1], a.pos[2] - b.pos[2]);
	}
	friend Tuple operator * (double a, Tuple b){
		return Tuple(a * b.pos[0], a * b.pos[1], a * b.pos[2]);
	}
	friend Tuple operator * (Tuple a, double b){
		return Tuple(a.pos[0] * b, a.pos[1] * b, a.pos[2] * b);
	}
};
typedef Tuple Point;
typedef Tuple Vector;


//所有的循环双向链表均已pre和nxt为自己作为判断只有一个的条件。
class Solid{
public:
	Solid():preSolid(this), nxtSolid(this), sFaces(NULL), sEdges(NULL), sVertices(NULL), vertexCount(0), loopCount(0), faceCount(0)
	{}
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
	{}
	Solid* fSolid;
	Face* preFace;
	Face* nxtFace;
	Loop* fLoops;
	Vector normal;//用于绘制的时候存储法向量。
};
class Loop{
public:
	Loop():lFace(NULL), preLoop(this), nxtLoop(this), lHalfEdges(NULL)
	{}
	Face* lFace;
	Loop* preLoop;
	Loop* nxtLoop;
	HalfEdge* lHalfEdges;
	int mark;//后期输出过程中使用。
};
class HalfEdge{
public:
	HalfEdge():edge(NULL), heLoop(NULL), preHalfEdge(this), nxtHalfEdge(this), heStartVertex(NULL), indexInEdge(0)
	{}
	Edge* edge;
	int indexInEdge;//该条halfEdge在其所属的Edge中的索引。
	Loop* heLoop;
	HalfEdge* preHalfEdge;
	HalfEdge* nxtHalfEdge;
	Vertex* heStartVertex;
};
class Edge{
public:
	Edge():preEdge(this), nxtEdge(this)
	{
		halfEdge[0] = NULL;
		halfEdge[1] = NULL;
	}
	HalfEdge* halfEdge[2];
	Edge* preEdge;
	Edge* nxtEdge;
};
class Vertex{
public:
	Vertex():preVertex(this), nxtVertex(this){}

	Vertex(Vertex& v):preVertex(this), nxtVertex(this)
	{
		this->point = v.point;
	}
	Vertex(double x, double y, double z):preVertex(this), nxtVertex(this)
	{
		point = Point(x, y, z);
	}
	Vertex(Point _point):preVertex(this), nxtVertex(this)
	{
		point = _point;
	}
	Point point;
	Vector normal;//用于绘制的时候存储法向量。
	Vertex* preVertex;
	Vertex* nxtVertex;
	int mark;//后期输出过程中使用的。
};




