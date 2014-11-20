#pragma once
#include<iostream>
class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;
//���е�ѭ��˫���������pre��nxtΪ�Լ���Ϊ�ж�ֻ��һ����������
class Solid{
public:
	Solid():preSolid(this), nxtSolid(this), sFaces(NULL), sEdges(NULL), sVertices(NULL), vertexCount(0), loopCount(0), faceCount(0)
	{};
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
	int mark;//�������������ʹ�á�
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
	int mark;//�������������ʹ�õġ�
};
