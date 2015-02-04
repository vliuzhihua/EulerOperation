#pragma once
#include <iostream>
using namespace std;
#include "EulerBaseDataStructure.h"

//新建一个solid，并在其中穿件一个face和一个顶点，但是face中只含有一个没有HalfEdge的Loop。
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
//创建一个点，和一条边。loop是进行此操作的那个Loop，而vtx1和vtx2是具有一定次序的。
//默认认为vtx1是在loop中了的顶点，而vtx2是新创建的顶点，
void mev(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	loop->lFace->fSolid->vertexCount++;
	//初始化的一些操作
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();
	e->halfEdge[0] = e1;
	e->halfEdge[1] = e2;
	e1->indexInEdge = 0;
	e2->indexInEdge = 1;
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
//创建一条边，创建一个新的面。其中vtx1和vtx2的是具有方向性的，不可以乱序了使用。
//函数结束后会将传入的loop修改为指向vtx1向vtx2的方向的那个Loop，而方向的则返回。
//因此使用的时候要注意方向。
Loop* mef(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	//边信息简单的初始化。
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();
	//面信息的简单初始化。
	Face* newFace = new Face();
	Loop* newLoop = new Loop();

	e->halfEdge[0] = e1;
	e->halfEdge[1] = e2;
	e1->indexInEdge = 0;
	e2->indexInEdge = 1;
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

	//修改loop为新的外边框。即按原来的方向的那一个loop。 vtx1 --> vtx2
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
	//通过Edge的关系找寻对应的另一条半边。 1 - delHalfEdge1->indexInEdge即为对应的
	delHalfEdge2 = delHalfEdge1->edge->halfEdge[1 - delHalfEdge1->indexInEdge];
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
//edge 是将要添加新点的边，vtx是添加的新点。每次返回新添加的边，新添加的边是以以前的edge的halfEdge[0]的起点开始的.
Edge* semv(Edge* edge, Vertex* vtx){
	//边信息简单的初始化。
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();

	//solid的数值记录更新
	edge->halfEdge[0]->heLoop->lFace->fSolid->vertexCount++;

	e->halfEdge[0] = e1;
	e->halfEdge[1] = e2;
	e1->indexInEdge = 0;
	e2->indexInEdge = 1;
	e1->edge = e;
	e2->edge = e;

	e1->heStartVertex = vtx;
	e2->heStartVertex = edge->halfEdge[0]->heStartVertex;
	edge->halfEdge[0]->heStartVertex = vtx;
	
	e1->heLoop = edge->halfEdge[1]->heLoop;
	e2->heLoop = edge->halfEdge[0]->heLoop;
	//考虑特殊情况
	if(edge->halfEdge[1]->nxtHalfEdge == edge->halfEdge[0]){
		e1->preHalfEdge = edge->halfEdge[1];
		e1->nxtHalfEdge = e2;
		edge->halfEdge[1]->nxtHalfEdge = e1;

		e2->preHalfEdge = e1;
		e2->nxtHalfEdge = edge->halfEdge[0];
		edge->halfEdge[0]->preHalfEdge = e2;
	}else{
		e1->preHalfEdge = edge->halfEdge[1];
		e1->nxtHalfEdge = edge->halfEdge[1]->nxtHalfEdge;
		edge->halfEdge[1]->nxtHalfEdge->preHalfEdge = e1;
		edge->halfEdge[1]->nxtHalfEdge = e1;
		
		e2->preHalfEdge = edge->halfEdge[0]->preHalfEdge;
		e2->nxtHalfEdge = edge->halfEdge[0];
		edge->halfEdge[0]->preHalfEdge->nxtHalfEdge = e2;
		edge->halfEdge[0]->preHalfEdge = e2;
	}
	
	//添加新的edge到solid的edge集合中。
	Edge* tempEdge = edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges;
	if(tempEdge == NULL){
		edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges = e;
	}else{
		tempEdge->preEdge->nxtEdge = e;
		e->preEdge = tempEdge->preEdge;
		e->nxtEdge = tempEdge;
		tempEdge->preEdge = e;	
	}
	//添加新的vertex到solid的vertex集合中。
	Vertex* tempVertex = edge->halfEdge[0]->heLoop->lFace->fSolid->sVertices;
	if(tempVertex == NULL){
		edge->halfEdge[0]->heLoop->lFace->fSolid->sVertices = vtx;
	}else{
		tempVertex->preVertex->nxtVertex = vtx;
		vtx->preVertex = tempVertex->preVertex;
		vtx->nxtVertex = tempVertex;
		tempVertex->preVertex = vtx;
	}
	
	return e;
}
//edge是要删除的边，而面则随便删除两个相邻中的一个。
//暂时考虑的是理想情况。大约是两个三角形夹着的一条边被删除。
void kef(Edge* edge){
	HalfEdge *e1, *e2, *tmpHe;
	Edge *tmpE,*firE;
	e1 = edge->halfEdge[0];
	e2 = edge->halfEdge[1];
	//solid中数值的更新
	edge->halfEdge[0]->heLoop->lFace->fSolid->faceCount--;
	edge->halfEdge[0]->heLoop->lFace->fSolid->loopCount--;

	//修改solid的sEdges使其指向有效，并从solid的sEdges中删除该实体边。
	//考虑的简单的情况，不考虑只有一条边在的情况。
	edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges = edge->nxtEdge;
	edge->preEdge->nxtEdge = edge->nxtEdge;
	edge->nxtEdge->preEdge = edge->preEdge;
	
	
	//现在取e1中的face认为是要被去掉的，从face的双向链表中删除，并将下的loop也一并删除。
	//此处有假设默认删除的只能是不包含内环的。
	Face* f = e1->heLoop->lFace;
	f->fSolid->sFaces = f->nxtFace;
	f->preFace->nxtFace = f->nxtFace;
	f->nxtFace->preFace = f->preFace;
	Loop* l = e1->heLoop;
	//将e1所有的半边的所属loop修改为e2所指向的。
	tmpHe = e1;
	do{
		tmpHe->heLoop = e2->heLoop;
		tmpHe = tmpHe->nxtHalfEdge;
	}while(tmpHe != e1);
	//修改一下e2的 heLoop，保证其能正确指向。
	e2->heLoop->lHalfEdges = e2->nxtHalfEdge;

	//拓扑关系的修改
	e1->nxtHalfEdge->preHalfEdge = e2->preHalfEdge;
	e2->preHalfEdge->nxtHalfEdge = e1->nxtHalfEdge;
	e1->preHalfEdge->nxtHalfEdge = e2->nxtHalfEdge;
	e2->nxtHalfEdge->preHalfEdge = e1->preHalfEdge;

	//真正删除这些半边和边,face, loop。
	delete l;
	delete f;
	delete e1;
	delete e2;
	delete edge;

}
//edge是要删除的边，vtx是要删除的顶点。
void kev(Edge* edge, Vertex* vtx){
	HalfEdge *e1, *e2;
	Edge *tmpE,*firE;
	Vertex *tmpV, *firV;
	if(edge->halfEdge[0]->heStartVertex == vtx){
		e1 = edge->halfEdge[0];
		e2 = edge->halfEdge[1];
	}else{
		e1 = edge->halfEdge[1];
		e2 = edge->halfEdge[0];
	}
	
	//solid中的数值进行更新.
	edge->halfEdge[0]->heLoop->lFace->fSolid->vertexCount--;

	//使solid中的sVertices指向有效，从solid的点链表中删除该点。
	edge->halfEdge[0]->heLoop->lFace->fSolid->sVertices = vtx->nxtVertex;
	vtx->preVertex->nxtVertex = vtx->nxtVertex;
	vtx->nxtVertex->preVertex = vtx->preVertex;
		
	//使solid中的sEdges指向有效，并从solid的边表中删除该边。
	//考虑的简单的情况，不考虑只有一条边在的情况。
	//从边表中取出了该条edge。
	edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges = edge->nxtEdge;
	edge->preEdge->nxtEdge = edge->nxtEdge;
	edge->nxtEdge->preEdge = edge->preEdge;

	//修改拓扑关系。
	e1->nxtHalfEdge->preHalfEdge = e2->preHalfEdge;
	e2->preHalfEdge->nxtHalfEdge = e1->nxtHalfEdge;

	//真正删除edge中的半边，边，以及vtx等。
	delete e1;
	delete e2;
	delete edge;
	delete vtx;

}
//后续操作辅助的数据结构。
class VertexLinker{
public:
	VertexLinker():next(NULL)
	{}
	Vertex* vertex;

	VertexLinker* next;
};
class EdgeLinker{
public:
	EdgeLinker():next(NULL){}
	Edge* edge;
	EdgeLinker* next;
};
//扫成操作，face是要被用来扫成的面，dir是扫成的方向，d是在该方向上扫成的距离。
void sweep(Face* face, double dir[], float d){
	if(face == NULL){
		cout << "face is NULL in sweep function which is illegal..." <<endl;
		return;
	}
	//先对dir进行单位化.
	float length = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
	dir[0] /= length;
	dir[1] /= length;
	dir[2] /= length;
	Loop* tmpoutLoop = face->fLoops;
	Loop* firLoop = NULL;
	do{
		Vertex *vtx1, *vtx2;

		Vertex *addVtx1, *addVtx2, *startAddVtx;
		HalfEdge* tmpHalfEdge;
		if(tmpoutLoop != NULL){
			tmpHalfEdge = tmpoutLoop->lHalfEdges;
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
		}while(tmpHalfEdge != tmpoutLoop->lHalfEdges);

		
		Loop* tmpLoop = tmpoutLoop;
		tmpVLinker = vLinker;
		vtx1 = tmpVLinker->vertex;
		addVtx1 = new Vertex(vtx1->point + Point(dir) * d);
		startAddVtx = addVtx1;
		mev(tmpLoop, vtx1, addVtx1);
		while(tmpVLinker->next != NULL){
			tmpVLinker = tmpVLinker->next;
			vtx2 = tmpVLinker->vertex;
			addVtx2 = new Vertex(vtx2->point + Point(dir) * d);
			mev(tmpLoop, vtx2, addVtx2);
			mef(tmpLoop, addVtx2, addVtx1);
			//mef(tmpLoop, addVtx1, addVtx2);
			addVtx1 = addVtx2;
		}
		mef(tmpLoop, startAddVtx, addVtx1);
		//mef(tmpLoop, addVtx1, startAddVtx);
		/*if(tmpoutLoop == face->fLoops){
			
			firLoop =  tmpLoop;
		}else{
			kfmrh(firLoop, tmpLoop);
		}*/
		
		
		//删除临时创建的点列表。
		vLinker;
		while(vLinker->next != NULL){
			tmpVLinker = vLinker->next;
			delete vLinker;
			vLinker = tmpVLinker;
		}
		delete vLinker;
		tmpoutLoop = tmpoutLoop->nxtLoop;
	}while(tmpoutLoop != face->fLoops);
}
//vtx是要被削平的角顶点， d是在各边上新创建的点到角顶点的距离。reV可以返回新创建的那些顶点。
void chamfer(Solid* solid, Vertex* vtx, float d, Vertex* reV[] = NULL){
	int edgeCount = 0;
	int mark;
	Edge* tmpE = solid->sEdges;
	Edge* firE;
	HalfEdge* tmpHe, *firHe;
	Vertex *tmpV, *preV;
	//遍历solid中的所有edge，找到一条以vtx为起点或重点的edge然后停止。
	do{
		mark = 0;
		if(tmpE->halfEdge[0]->heStartVertex == vtx || tmpE->halfEdge[1]->heStartVertex == vtx){
			mark = 1;
			firE = tmpE;
			break;
		}
		tmpE = tmpE->nxtEdge;
	}while(tmpE != solid->sEdges);
	//确定edge哪条半边的起点是vtx。
	if(tmpE->halfEdge[0]->heStartVertex == vtx){
		firHe = tmpE->halfEdge[0];
	}
	if(tmpE->halfEdge[1]->heStartVertex == vtx){
		firHe = tmpE->halfEdge[1];
	}
	tmpHe = firHe;

	//遍历记录在vtx周围有多少条边。
	EdgeLinker *elinker = NULL, *tmpeLinker;

	do{
		if(elinker == NULL){
			//在elinker为空的时候的初始化.
			tmpeLinker = new EdgeLinker();
			tmpeLinker->edge = tmpHe->edge;
			tmpeLinker->next = NULL;
			elinker = tmpeLinker;
		}else{
			//在elinker中插入关系，采用头插入链表的方式。
			tmpeLinker = new EdgeLinker();
			tmpeLinker->edge = tmpHe->edge;
			tmpeLinker->next = elinker;
			elinker = tmpeLinker;
		}
		edgeCount++;
		tmpHe = tmpHe->preHalfEdge->edge->halfEdge[1 - tmpHe->preHalfEdge->indexInEdge];
	}while(tmpHe != firHe);

	//按顺序在所有边上进行semv,距离为d.
	tmpeLinker = elinker;
	int reC = 0;
	while(tmpeLinker != NULL){
		Point endP;
		Point startP = vtx->point;
		if(tmpeLinker->edge->halfEdge[0]->heStartVertex != vtx){
			endP = tmpeLinker->edge->halfEdge[0]->heStartVertex->point;
		}else{
			endP = tmpeLinker->edge->halfEdge[1]->heStartVertex->point;
		}
		Vector dir = endP + (-1.0 * startP);
		Point newP = startP + dir * (d / dir.length());
		tmpV = new Vertex(newP);
		//reC和reV返回新创建的点所临时使用的。
		if(reV != NULL){
			reV[reC] = tmpV;
			reC++;
		}
		


		tmpE = semv(tmpeLinker->edge, tmpV);
		if(tmpE->halfEdge[0]->heStartVertex == vtx || tmpE->halfEdge[1]->heStartVertex == vtx){
			tmpeLinker->edge = tmpE;
		}

		tmpeLinker = tmpeLinker->next;
	}
	//按elinker顺序依次进行mef操作。
	tmpeLinker = elinker;
	if(tmpeLinker->edge->halfEdge[0]->heStartVertex != vtx){
		preV = tmpeLinker->edge->halfEdge[0]->heStartVertex;
	}else{
		preV = tmpeLinker->edge->halfEdge[1]->heStartVertex;
	}
	tmpeLinker = tmpeLinker->next;
	while(tmpeLinker != NULL){
		if(tmpeLinker->edge->halfEdge[0]->heStartVertex != vtx){
			Loop* lp = tmpeLinker->edge->halfEdge[1]->heLoop;
			tmpV = tmpeLinker->edge->halfEdge[0]->heStartVertex;
			mef(lp, preV, tmpV);
		}else{
			Loop* lp = tmpeLinker->edge->halfEdge[0]->heLoop;
			tmpV = tmpeLinker->edge->halfEdge[1]->heStartVertex;
			mef(lp, preV, tmpV);
		}
		preV = tmpV;
		tmpeLinker = tmpeLinker->next;
	}
	//以上过程少处理了最后一条边链接到第一条边的那一次mef，故而特殊处理如下。
	if(elinker->edge->halfEdge[0]->heStartVertex != vtx){
		Loop* lp = elinker->edge->halfEdge[1]->heLoop;
		tmpV = elinker->edge->halfEdge[0]->heStartVertex;
		mef(lp, preV, tmpV);
	}else{
		Loop* lp = elinker->edge->halfEdge[0]->heLoop;
		tmpV = elinker->edge->halfEdge[1]->heStartVertex;
		mef(lp, preV, tmpV);
	}



	//按照tmpeLinker使用kef逐个删除边和面，但是最后一个要使用kev.
	tmpeLinker = elinker;
	while(tmpeLinker != NULL){
		if(tmpeLinker->next != NULL){
			kef(tmpeLinker->edge);
		}else{
			kev(tmpeLinker->edge, vtx);
		}
		tmpeLinker = tmpeLinker->next;
	}

}


//operation has the override version
//扫成操作方便使用的接口
void sweep(Face* face, Vector dir, float d){
	double nd[3] = {dir.pos[0], dir.pos[1], dir.pos[2]};
	sweep(face, nd, d);
}