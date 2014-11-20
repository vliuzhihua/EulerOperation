#include <iostream>
using namespace std;
#include "OulerBaseDataStructure.h"


Solid* mvfs(Vertex* vertex, Face*& face){

	Solid* newSolid = new Solid();
	Face* newFace = new Face();
	Loop* loop = new Loop();
	//对拓扑结构进行调整。
	newSolid->sFaces = newFace;
	//将新的点加入solid的点集合中去。
	if(newSolid->sVertices == NULL){
		newSolid->sVertices = vertex;
	}else{
		newSolid->sVertices->nxtVertex->preVertex = vertex;
		vertex->nxtVertex = newSolid->sVertices->nxtVertex;
		vertex->preVertex = newSolid->sVertices;
		newSolid->sVertices->nxtVertex = vertex;
	}
	newFace->fSolid = newSolid;
	newFace->fLoops = loop;
	loop->lFace = newFace;
	face = newFace;

	//进行一些数值累加的操作。
	newSolid->faceCount++;
	newSolid->loopCount++;
	newSolid->vertexCount++;

	return newSolid;
}

void mev(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	loop->lFace->fSolid->vertexCount++;
	//初始化的一些操作
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();
	e->halfEdge1 = e1;
	e->halfEdge2 = e2;
	e1->edge = e;
	e2->edge = e;
	e1->heStartVertex = vtx1;
	e2->heStartVertex = vtx2;
	e1->heLoop = loop;
	e2->heLoop = loop;
	//将新的边插入solid的边表里。
	Edge* tmpEdge = loop->lFace->fSolid->sEdges;
	if(tmpEdge == NULL){
		loop->lFace->fSolid->sEdges = e;
	}else{
		tmpEdge->nxtEdge->preEdge = e;
		e->nxtEdge = tmpEdge->nxtEdge;
		e->preEdge = tmpEdge;
		tmpEdge->nxtEdge = e;
	}
	//将新的点加入solid的点集中。
	Vertex* tmpVertex = loop->lFace->fSolid->sVertices;
	if(tmpVertex == NULL){
		loop->lFace->fSolid->sVertices = vtx2;
	}else{
		tmpVertex->nxtVertex->preVertex = vtx2;
		vtx2->nxtVertex = tmpVertex->nxtVertex;
		vtx2->preVertex = tmpVertex;
		tmpVertex->nxtVertex = vtx2;
	}
	//将半边e1，e2进行插入loop中。
	if(loop->lHalfEdges == NULL){
		//将拓扑结构进行设置。
		//e1的前半边指针为e2，后半边指针指向e2
		//e2的前半边指针为e1，后半边指针指向e1
		e1->nxtHalfEdge = e2;
		e1->preHalfEdge = e2;
		e2->nxtHalfEdge = e1;
		e2->preHalfEdge = e1;
		loop->lHalfEdges = e1;
	}else{
		HalfEdge* tHalfEdge = loop->lHalfEdges;
		//找到原来的loo中以vtx1为起点的半边。
		do{
			tHalfEdge = tHalfEdge->nxtHalfEdge;
		}while(tHalfEdge->heStartVertex != vtx1 && tHalfEdge != loop->lHalfEdges);
		if(tHalfEdge->heStartVertex != vtx1){
			cout << "cannot find any halfedge start from vtx1 in mev function" <<endl;
		}else{
			//对拓扑结构进行修改。
			tHalfEdge->preHalfEdge->nxtHalfEdge = e1;
			e1->preHalfEdge = tHalfEdge->preHalfEdge;
			e1->nxtHalfEdge = e2;
			e2->preHalfEdge = e1;
			e2->nxtHalfEdge = tHalfEdge;
			tHalfEdge->preHalfEdge = e2;
		}
	}
}

Loop* mef(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	//边信息简单的初始化。
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();
	//面信息的简单初始化。
	Face* newFace = new Face();
	Loop* newLoop = new Loop();

	e->halfEdge1 = e1;
	e->halfEdge2 = e2;
	e1->edge = e;
	e2->edge = e;
	e1->heStartVertex = vtx1;
	e2->heStartVertex = vtx2;
	e1->heLoop = loop;
	e2->heLoop = newLoop;
	//newFace的拓扑结构填充
	newFace->fLoops = newLoop;
	newFace->fSolid = loop->lFace->fSolid;
	loop->lFace->nxtFace->preFace = newFace;
	newFace->preFace = loop->lFace;
	newFace->nxtFace = loop->lFace->nxtFace;
	loop->lFace->nxtFace = newFace;
	newLoop->lFace = newFace;
	newLoop->lHalfEdges = e2;

	//添加行的edge到solid的edge集合中。
	Edge* tempEdge = loop->lFace->fSolid->sEdges;
	if(tempEdge == NULL){
		loop->lFace->fSolid->sEdges = e;
	}else{
		tempEdge->preEdge->nxtEdge = e;
		e->preEdge = tempEdge->preEdge;
		e->nxtEdge = tempEdge;
		tempEdge->preEdge = e;	
	}
	
	HalfEdge* tEdge = loop->lHalfEdges;
	HalfEdge* vtx1HalfEdge, *vtx2HalfEdge;
	vtx1HalfEdge = vtx2HalfEdge = NULL;
	do{
		if(tEdge->heStartVertex == vtx1){
			vtx1HalfEdge = tEdge;
		}
		if(tEdge->heStartVertex == vtx2){
			vtx2HalfEdge = tEdge;
		}
		tEdge = tEdge->nxtHalfEdge;
	}while(tEdge != loop->lHalfEdges);
	//修改两个loop的半边的数据结构。
	vtx1HalfEdge->preHalfEdge->nxtHalfEdge = e1;
	e1->preHalfEdge = vtx1HalfEdge->preHalfEdge;
	e1->nxtHalfEdge = vtx2HalfEdge;
	vtx2HalfEdge->preHalfEdge->nxtHalfEdge = e2;
	e2->preHalfEdge = vtx2HalfEdge->preHalfEdge;
	e2->nxtHalfEdge = vtx1HalfEdge;
	vtx2HalfEdge->preHalfEdge = e1;
	vtx1HalfEdge->preHalfEdge = e2;
	//修改newLoop中的半边的属性。
	tEdge = newLoop->lHalfEdges;
	do{
		tEdge->heLoop = newLoop;
		tEdge = tEdge->nxtHalfEdge;
	}while(tEdge != newLoop->lHalfEdges);

	//修改loop为新的外边框。
	loop->lHalfEdges = e1;
	//一些数值的累加
	loop->lFace->fSolid->loopCount++;
	loop->lFace->fSolid->faceCount++;
	return newLoop;
}
//默认是从vtx1到vtx2的方向。即以此为潜在要求，not robust
Loop* kemr(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	
	loop->lFace->fSolid->loopCount++;

	HalfEdge* tHalfEdge = loop->lHalfEdges;
	HalfEdge *delHalfEdge1, *delHalfEdge2;
	Loop* newLoop = new Loop();
	newLoop->lFace = loop->lFace;
	//找到以vtx1为起点且下一条为以vtx2的HalfEdge
	while(tHalfEdge->heStartVertex != vtx1 || tHalfEdge->nxtHalfEdge->heStartVertex != vtx2){
		tHalfEdge = tHalfEdge->nxtHalfEdge;
	}
	delHalfEdge1 = tHalfEdge;
	//通过Edge的关系找寻对应的另一条半边。
	delHalfEdge2 = (delHalfEdge1->edge->halfEdge1 == delHalfEdge1)?delHalfEdge1->edge->halfEdge2 : delHalfEdge1->edge->halfEdge1;
	//修改半边的拓扑关系，仅考虑简单情况。复杂情况未考虑。
	
	//修改相应的拓扑结构
	delHalfEdge1->preHalfEdge->nxtHalfEdge = delHalfEdge2->nxtHalfEdge;
	delHalfEdge2->nxtHalfEdge->preHalfEdge = delHalfEdge1->preHalfEdge;
	loop->lHalfEdges = delHalfEdge1->preHalfEdge;
	
	if(delHalfEdge1->nxtHalfEdge != delHalfEdge2){
		delHalfEdge1->nxtHalfEdge->preHalfEdge = delHalfEdge2->preHalfEdge;
		delHalfEdge2->preHalfEdge->nxtHalfEdge = delHalfEdge1->nxtHalfEdge;
		newLoop->lHalfEdges = delHalfEdge1->nxtHalfEdge;
	}
	//删除边在solid的关系
	Edge* delEdge =  delHalfEdge1->edge;
	delEdge->preEdge->nxtEdge = delEdge->nxtEdge;
	delEdge->nxtEdge->preEdge = delEdge->preEdge;

	//删除为半边和边分配的内存。
	delete delHalfEdge1->edge;
	delete delHalfEdge1;
	delete delHalfEdge2;
		
	loop->nxtLoop->preLoop = newLoop;
	newLoop->nxtLoop = loop->nxtLoop;
	newLoop->preLoop = loop;
	loop->nxtLoop = newLoop;

	return newLoop;		
}
//实际是删除一个面，但是这个面只含有一个loop，然后将这个loop放置到另一个面上去，即baseLoop所在的面，
void kfmrh(Loop* baseLoop, Loop* faceLoop){
	baseLoop->lFace->fSolid->faceCount--;
	if(baseLoop->nxtLoop == baseLoop){
		baseLoop->nxtLoop = faceLoop;
		baseLoop->preLoop = faceLoop;
		faceLoop->preLoop = baseLoop;
		faceLoop->nxtLoop = baseLoop;
	}else{
		baseLoop->nxtLoop->preLoop = faceLoop;
		faceLoop->nxtLoop = baseLoop->nxtLoop;
		faceLoop->preLoop = baseLoop;
		baseLoop->nxtLoop = faceLoop;
	}
	

	Face* tmpFace = faceLoop->lFace;
	faceLoop->lFace = baseLoop->lFace;
	if(tmpFace->nxtFace != tmpFace){
		tmpFace->nxtFace->preFace = tmpFace->preFace;
		tmpFace->preFace->nxtFace = tmpFace->nxtFace;
	}
	
	delete tmpFace;
}

class VertexLinker{
public:
	VertexLinker():next(NULL)
	{}
	Vertex* vertex;

	VertexLinker* next;
};
void sweep(Face* face, float dir[], float d){
	if(face == NULL){
		cout << "face is NULL in sweep function which is illegal..." <<endl;
		return;
	}
	//先对dir进行单位化.
	float length = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
	dir[0] /= length;
	dir[1] /= length;
	dir[2] /= length;
	Loop* tmpLoop = face->fLoops;
	do{
		Vertex *vtx1, *vtx2;
		Vertex *addVtx1, *addVtx2, *startAddVtx;
		HalfEdge* tmpHalfEdge;
		if(tmpLoop != NULL){
			tmpHalfEdge = tmpLoop->lHalfEdges;
		}else{
			continue;
		}
		
		//创建暂存点集合的链表
		VertexLinker* vLinker = NULL, *tmpVLinker;
		do{
			tmpVLinker = new VertexLinker();
			tmpVLinker->vertex = tmpHalfEdge->heStartVertex;
			tmpVLinker->next = NULL;
			if(vLinker == NULL){
				vLinker = tmpVLinker;
			}else{
				tmpVLinker->next = vLinker->next;
				vLinker->next = tmpVLinker;
			}
			tmpHalfEdge = tmpHalfEdge->nxtHalfEdge;
		}while(tmpHalfEdge != tmpLoop->lHalfEdges);

		

		tmpVLinker = vLinker;
		vtx1 = tmpVLinker->vertex;
		addVtx1 = new Vertex(vtx1->pos[0] + dir[0] * d, vtx1->pos[1] + dir[1] * d, vtx1->pos[2] + dir[2] * d);
		startAddVtx = addVtx1;
		mev(tmpLoop, vtx1, addVtx1);
		while(tmpVLinker->next != NULL){
			tmpVLinker = tmpVLinker->next;
			vtx2 = tmpVLinker->vertex;
			addVtx2 = new Vertex(vtx2->pos[0] + dir[0] * d, vtx2->pos[1] + dir[1] * d, vtx2->pos[2] + dir[2] * d);
			mev(tmpLoop, vtx2, addVtx2);
			mef(tmpLoop, addVtx2, addVtx1);
			addVtx1 = addVtx2;
		}
		mef(tmpLoop, startAddVtx, addVtx1);
		//删除临时创建的点列表。
		vLinker;
		while(vLinker->next != NULL){
			tmpVLinker = vLinker->next;
			delete vLinker;
			vLinker = tmpVLinker;
		}
		delete vLinker;
		tmpLoop = tmpLoop->nxtLoop;
	}
	while(tmpLoop->nxtLoop != face->fLoops);
}