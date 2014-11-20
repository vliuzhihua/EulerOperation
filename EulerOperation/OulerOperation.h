#include <iostream>
using namespace std;
#include "OulerBaseDataStructure.h"


Solid* mvfs(Vertex* vertex, Face*& face){

	Solid* newSolid = new Solid();
	Face* newFace = new Face();
	Loop* loop = new Loop();
	//�����˽ṹ���е�����
	newSolid->sFaces = newFace;
	//���µĵ����solid�ĵ㼯����ȥ��
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

	//����һЩ��ֵ�ۼӵĲ�����
	newSolid->faceCount++;
	newSolid->loopCount++;
	newSolid->vertexCount++;

	return newSolid;
}

void mev(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	loop->lFace->fSolid->vertexCount++;
	//��ʼ����һЩ����
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
	//���µı߲���solid�ı߱��
	Edge* tmpEdge = loop->lFace->fSolid->sEdges;
	if(tmpEdge == NULL){
		loop->lFace->fSolid->sEdges = e;
	}else{
		tmpEdge->nxtEdge->preEdge = e;
		e->nxtEdge = tmpEdge->nxtEdge;
		e->preEdge = tmpEdge;
		tmpEdge->nxtEdge = e;
	}
	//���µĵ����solid�ĵ㼯�С�
	Vertex* tmpVertex = loop->lFace->fSolid->sVertices;
	if(tmpVertex == NULL){
		loop->lFace->fSolid->sVertices = vtx2;
	}else{
		tmpVertex->nxtVertex->preVertex = vtx2;
		vtx2->nxtVertex = tmpVertex->nxtVertex;
		vtx2->preVertex = tmpVertex;
		tmpVertex->nxtVertex = vtx2;
	}
	//�����e1��e2���в���loop�С�
	if(loop->lHalfEdges == NULL){
		//�����˽ṹ�������á�
		//e1��ǰ���ָ��Ϊe2������ָ��ָ��e2
		//e2��ǰ���ָ��Ϊe1������ָ��ָ��e1
		e1->nxtHalfEdge = e2;
		e1->preHalfEdge = e2;
		e2->nxtHalfEdge = e1;
		e2->preHalfEdge = e1;
		loop->lHalfEdges = e1;
	}else{
		HalfEdge* tHalfEdge = loop->lHalfEdges;
		//�ҵ�ԭ����loo����vtx1Ϊ���İ�ߡ�
		do{
			tHalfEdge = tHalfEdge->nxtHalfEdge;
		}while(tHalfEdge->heStartVertex != vtx1 && tHalfEdge != loop->lHalfEdges);
		if(tHalfEdge->heStartVertex != vtx1){
			cout << "cannot find any halfedge start from vtx1 in mev function" <<endl;
		}else{
			//�����˽ṹ�����޸ġ�
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
	//����Ϣ�򵥵ĳ�ʼ����
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();
	//����Ϣ�ļ򵥳�ʼ����
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
	//newFace�����˽ṹ���
	newFace->fLoops = newLoop;
	newFace->fSolid = loop->lFace->fSolid;
	loop->lFace->nxtFace->preFace = newFace;
	newFace->preFace = loop->lFace;
	newFace->nxtFace = loop->lFace->nxtFace;
	loop->lFace->nxtFace = newFace;
	newLoop->lFace = newFace;
	newLoop->lHalfEdges = e2;

	//����е�edge��solid��edge�����С�
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
	//�޸�����loop�İ�ߵ����ݽṹ��
	vtx1HalfEdge->preHalfEdge->nxtHalfEdge = e1;
	e1->preHalfEdge = vtx1HalfEdge->preHalfEdge;
	e1->nxtHalfEdge = vtx2HalfEdge;
	vtx2HalfEdge->preHalfEdge->nxtHalfEdge = e2;
	e2->preHalfEdge = vtx2HalfEdge->preHalfEdge;
	e2->nxtHalfEdge = vtx1HalfEdge;
	vtx2HalfEdge->preHalfEdge = e1;
	vtx1HalfEdge->preHalfEdge = e2;
	//�޸�newLoop�еİ�ߵ����ԡ�
	tEdge = newLoop->lHalfEdges;
	do{
		tEdge->heLoop = newLoop;
		tEdge = tEdge->nxtHalfEdge;
	}while(tEdge != newLoop->lHalfEdges);

	//�޸�loopΪ�µ���߿�
	loop->lHalfEdges = e1;
	//һЩ��ֵ���ۼ�
	loop->lFace->fSolid->loopCount++;
	loop->lFace->fSolid->faceCount++;
	return newLoop;
}
//Ĭ���Ǵ�vtx1��vtx2�ķ��򡣼��Դ�ΪǱ��Ҫ��not robust
Loop* kemr(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	
	loop->lFace->fSolid->loopCount++;

	HalfEdge* tHalfEdge = loop->lHalfEdges;
	HalfEdge *delHalfEdge1, *delHalfEdge2;
	Loop* newLoop = new Loop();
	newLoop->lFace = loop->lFace;
	//�ҵ���vtx1Ϊ�������һ��Ϊ��vtx2��HalfEdge
	while(tHalfEdge->heStartVertex != vtx1 || tHalfEdge->nxtHalfEdge->heStartVertex != vtx2){
		tHalfEdge = tHalfEdge->nxtHalfEdge;
	}
	delHalfEdge1 = tHalfEdge;
	//ͨ��Edge�Ĺ�ϵ��Ѱ��Ӧ����һ����ߡ�
	delHalfEdge2 = (delHalfEdge1->edge->halfEdge1 == delHalfEdge1)?delHalfEdge1->edge->halfEdge2 : delHalfEdge1->edge->halfEdge1;
	//�޸İ�ߵ����˹�ϵ�������Ǽ�������������δ���ǡ�
	
	//�޸���Ӧ�����˽ṹ
	delHalfEdge1->preHalfEdge->nxtHalfEdge = delHalfEdge2->nxtHalfEdge;
	delHalfEdge2->nxtHalfEdge->preHalfEdge = delHalfEdge1->preHalfEdge;
	loop->lHalfEdges = delHalfEdge1->preHalfEdge;
	
	if(delHalfEdge1->nxtHalfEdge != delHalfEdge2){
		delHalfEdge1->nxtHalfEdge->preHalfEdge = delHalfEdge2->preHalfEdge;
		delHalfEdge2->preHalfEdge->nxtHalfEdge = delHalfEdge1->nxtHalfEdge;
		newLoop->lHalfEdges = delHalfEdge1->nxtHalfEdge;
	}
	//ɾ������solid�Ĺ�ϵ
	Edge* delEdge =  delHalfEdge1->edge;
	delEdge->preEdge->nxtEdge = delEdge->nxtEdge;
	delEdge->nxtEdge->preEdge = delEdge->preEdge;

	//ɾ��Ϊ��ߺͱ߷�����ڴ档
	delete delHalfEdge1->edge;
	delete delHalfEdge1;
	delete delHalfEdge2;
		
	loop->nxtLoop->preLoop = newLoop;
	newLoop->nxtLoop = loop->nxtLoop;
	newLoop->preLoop = loop;
	loop->nxtLoop = newLoop;

	return newLoop;		
}
//ʵ����ɾ��һ���棬���������ֻ����һ��loop��Ȼ�����loop���õ���һ������ȥ����baseLoop���ڵ��棬
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
	//�ȶ�dir���е�λ��.
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
		
		//�����ݴ�㼯�ϵ�����
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
		//ɾ����ʱ�����ĵ��б�
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