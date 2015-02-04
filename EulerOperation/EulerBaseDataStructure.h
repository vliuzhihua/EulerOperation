#pragma once

#include<iostream>
class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;



//Tuple����Ԫ���࣬����typedefΪVecter��Point�����࣬�Ա����ʶ�⡣
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


//���е�ѭ��˫���������pre��nxtΪ�Լ���Ϊ�ж�ֻ��һ����������
class Solid{
public:
	Solid():preSolid(this), nxtSolid(this), sFaces(NULL), sEdges(NULL), sVertices(NULL), vertexCount(0), loopCount(0), faceCount(0)
	{}
	Solid* preSolid;
	Solid* nxtSolid;
	Face* sFaces;
	Edge* sEdges;//Ϊ�˱��������߿�ģ��,��ʱû�õ�.
	Vertex* sVertices;//Ϊ�˱������ɵ�ģ�͡�
	int vertexCount;//ͳ�����solid�еĶ���ĸ���
	int loopCount;//ͳ�����solid�е�loop�ĸ�����
	int faceCount;//ͳ�����solid�е�face�ĸ�����
};
class Face{
public:
	Face():fSolid(NULL), preFace(this), nxtFace(this), fLoops(NULL)
	{}
	Solid* fSolid;
	Face* preFace;
	Face* nxtFace;
	Loop* fLoops;
	Vector normal;//���ڻ��Ƶ�ʱ��洢��������
};
class Loop{
public:
	Loop():lFace(NULL), preLoop(this), nxtLoop(this), lHalfEdges(NULL)
	{}
	Face* lFace;
	Loop* preLoop;
	Loop* nxtLoop;
	HalfEdge* lHalfEdges;
	int mark;//�������������ʹ�á�
};
class HalfEdge{
public:
	HalfEdge():edge(NULL), heLoop(NULL), preHalfEdge(this), nxtHalfEdge(this), heStartVertex(NULL), indexInEdge(0)
	{}
	Edge* edge;
	int indexInEdge;//����halfEdge����������Edge�е�������
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
	Vector normal;//���ڻ��Ƶ�ʱ��洢��������
	Vertex* preVertex;
	Vertex* nxtVertex;
	int mark;//�������������ʹ�õġ�
};




