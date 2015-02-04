#pragma once
#include <iostream>
using namespace std;
#include "EulerBaseDataStructure.h"

//�½�һ��solid���������д���һ��face��һ�����㣬����face��ֻ����һ��û��HalfEdge��Loop��
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
//����һ���㣬��һ���ߡ�loop�ǽ��д˲������Ǹ�Loop����vtx1��vtx2�Ǿ���һ������ġ�
//Ĭ����Ϊvtx1����loop���˵Ķ��㣬��vtx2���´����Ķ��㣬
void mev(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	loop->lFace->fSolid->vertexCount++;
	//��ʼ����һЩ����
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
//����һ���ߣ�����һ���µ��档����vtx1��vtx2���Ǿ��з����Եģ�������������ʹ�á�
//����������Ὣ�����loop�޸�Ϊָ��vtx1��vtx2�ķ�����Ǹ�Loop����������򷵻ء�
//���ʹ�õ�ʱ��Ҫע�ⷽ��
Loop* mef(Loop* loop, Vertex* vtx1, Vertex* vtx2){
	//����Ϣ�򵥵ĳ�ʼ����
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();
	//����Ϣ�ļ򵥳�ʼ����
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

	//�޸�loopΪ�µ���߿򡣼���ԭ���ķ������һ��loop�� vtx1 --> vtx2
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
	//ͨ��Edge�Ĺ�ϵ��Ѱ��Ӧ����һ����ߡ� 1 - delHalfEdge1->indexInEdge��Ϊ��Ӧ��
	delHalfEdge2 = delHalfEdge1->edge->halfEdge[1 - delHalfEdge1->indexInEdge];
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
//edge �ǽ�Ҫ����µ�ıߣ�vtx����ӵ��µ㡣ÿ�η�������ӵıߣ�����ӵı�������ǰ��edge��halfEdge[0]����㿪ʼ��.
Edge* semv(Edge* edge, Vertex* vtx){
	//����Ϣ�򵥵ĳ�ʼ����
	HalfEdge* e1 = new HalfEdge();
	HalfEdge* e2 = new HalfEdge();
	Edge* e = new Edge();

	//solid����ֵ��¼����
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
	//�����������
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
	
	//����µ�edge��solid��edge�����С�
	Edge* tempEdge = edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges;
	if(tempEdge == NULL){
		edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges = e;
	}else{
		tempEdge->preEdge->nxtEdge = e;
		e->preEdge = tempEdge->preEdge;
		e->nxtEdge = tempEdge;
		tempEdge->preEdge = e;	
	}
	//����µ�vertex��solid��vertex�����С�
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
//edge��Ҫɾ���ıߣ����������ɾ�����������е�һ����
//��ʱ���ǵ��������������Լ�����������μ��ŵ�һ���߱�ɾ����
void kef(Edge* edge){
	HalfEdge *e1, *e2, *tmpHe;
	Edge *tmpE,*firE;
	e1 = edge->halfEdge[0];
	e2 = edge->halfEdge[1];
	//solid����ֵ�ĸ���
	edge->halfEdge[0]->heLoop->lFace->fSolid->faceCount--;
	edge->halfEdge[0]->heLoop->lFace->fSolid->loopCount--;

	//�޸�solid��sEdgesʹ��ָ����Ч������solid��sEdges��ɾ����ʵ��ߡ�
	//���ǵļ򵥵������������ֻ��һ�����ڵ������
	edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges = edge->nxtEdge;
	edge->preEdge->nxtEdge = edge->nxtEdge;
	edge->nxtEdge->preEdge = edge->preEdge;
	
	
	//����ȡe1�е�face��Ϊ��Ҫ��ȥ���ģ���face��˫��������ɾ���������µ�loopҲһ��ɾ����
	//�˴��м���Ĭ��ɾ����ֻ���ǲ������ڻ��ġ�
	Face* f = e1->heLoop->lFace;
	f->fSolid->sFaces = f->nxtFace;
	f->preFace->nxtFace = f->nxtFace;
	f->nxtFace->preFace = f->preFace;
	Loop* l = e1->heLoop;
	//��e1���еİ�ߵ�����loop�޸�Ϊe2��ָ��ġ�
	tmpHe = e1;
	do{
		tmpHe->heLoop = e2->heLoop;
		tmpHe = tmpHe->nxtHalfEdge;
	}while(tmpHe != e1);
	//�޸�һ��e2�� heLoop����֤������ȷָ��
	e2->heLoop->lHalfEdges = e2->nxtHalfEdge;

	//���˹�ϵ���޸�
	e1->nxtHalfEdge->preHalfEdge = e2->preHalfEdge;
	e2->preHalfEdge->nxtHalfEdge = e1->nxtHalfEdge;
	e1->preHalfEdge->nxtHalfEdge = e2->nxtHalfEdge;
	e2->nxtHalfEdge->preHalfEdge = e1->preHalfEdge;

	//����ɾ����Щ��ߺͱ�,face, loop��
	delete l;
	delete f;
	delete e1;
	delete e2;
	delete edge;

}
//edge��Ҫɾ���ıߣ�vtx��Ҫɾ���Ķ��㡣
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
	
	//solid�е���ֵ���и���.
	edge->halfEdge[0]->heLoop->lFace->fSolid->vertexCount--;

	//ʹsolid�е�sVerticesָ����Ч����solid�ĵ�������ɾ���õ㡣
	edge->halfEdge[0]->heLoop->lFace->fSolid->sVertices = vtx->nxtVertex;
	vtx->preVertex->nxtVertex = vtx->nxtVertex;
	vtx->nxtVertex->preVertex = vtx->preVertex;
		
	//ʹsolid�е�sEdgesָ����Ч������solid�ı߱���ɾ���ñߡ�
	//���ǵļ򵥵������������ֻ��һ�����ڵ������
	//�ӱ߱���ȡ���˸���edge��
	edge->halfEdge[0]->heLoop->lFace->fSolid->sEdges = edge->nxtEdge;
	edge->preEdge->nxtEdge = edge->nxtEdge;
	edge->nxtEdge->preEdge = edge->preEdge;

	//�޸����˹�ϵ��
	e1->nxtHalfEdge->preHalfEdge = e2->preHalfEdge;
	e2->preHalfEdge->nxtHalfEdge = e1->nxtHalfEdge;

	//����ɾ��edge�еİ�ߣ��ߣ��Լ�vtx�ȡ�
	delete e1;
	delete e2;
	delete edge;
	delete vtx;

}
//�����������������ݽṹ��
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
//ɨ�ɲ�����face��Ҫ������ɨ�ɵ��棬dir��ɨ�ɵķ���d���ڸ÷�����ɨ�ɵľ��롣
void sweep(Face* face, double dir[], float d){
	if(face == NULL){
		cout << "face is NULL in sweep function which is illegal..." <<endl;
		return;
	}
	//�ȶ�dir���е�λ��.
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
		
		
		//ɾ����ʱ�����ĵ��б�
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
//vtx��Ҫ����ƽ�ĽǶ��㣬 d���ڸ������´����ĵ㵽�Ƕ���ľ��롣reV���Է����´�������Щ���㡣
void chamfer(Solid* solid, Vertex* vtx, float d, Vertex* reV[] = NULL){
	int edgeCount = 0;
	int mark;
	Edge* tmpE = solid->sEdges;
	Edge* firE;
	HalfEdge* tmpHe, *firHe;
	Vertex *tmpV, *preV;
	//����solid�е�����edge���ҵ�һ����vtxΪ�����ص��edgeȻ��ֹͣ��
	do{
		mark = 0;
		if(tmpE->halfEdge[0]->heStartVertex == vtx || tmpE->halfEdge[1]->heStartVertex == vtx){
			mark = 1;
			firE = tmpE;
			break;
		}
		tmpE = tmpE->nxtEdge;
	}while(tmpE != solid->sEdges);
	//ȷ��edge������ߵ������vtx��
	if(tmpE->halfEdge[0]->heStartVertex == vtx){
		firHe = tmpE->halfEdge[0];
	}
	if(tmpE->halfEdge[1]->heStartVertex == vtx){
		firHe = tmpE->halfEdge[1];
	}
	tmpHe = firHe;

	//������¼��vtx��Χ�ж������ߡ�
	EdgeLinker *elinker = NULL, *tmpeLinker;

	do{
		if(elinker == NULL){
			//��elinkerΪ�յ�ʱ��ĳ�ʼ��.
			tmpeLinker = new EdgeLinker();
			tmpeLinker->edge = tmpHe->edge;
			tmpeLinker->next = NULL;
			elinker = tmpeLinker;
		}else{
			//��elinker�в����ϵ������ͷ��������ķ�ʽ��
			tmpeLinker = new EdgeLinker();
			tmpeLinker->edge = tmpHe->edge;
			tmpeLinker->next = elinker;
			elinker = tmpeLinker;
		}
		edgeCount++;
		tmpHe = tmpHe->preHalfEdge->edge->halfEdge[1 - tmpHe->preHalfEdge->indexInEdge];
	}while(tmpHe != firHe);

	//��˳�������б��Ͻ���semv,����Ϊd.
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
		//reC��reV�����´����ĵ�����ʱʹ�õġ�
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
	//��elinker˳�����ν���mef������
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
	//���Ϲ����ٴ��������һ�������ӵ���һ���ߵ���һ��mef���ʶ����⴦�����¡�
	if(elinker->edge->halfEdge[0]->heStartVertex != vtx){
		Loop* lp = elinker->edge->halfEdge[1]->heLoop;
		tmpV = elinker->edge->halfEdge[0]->heStartVertex;
		mef(lp, preV, tmpV);
	}else{
		Loop* lp = elinker->edge->halfEdge[0]->heLoop;
		tmpV = elinker->edge->halfEdge[1]->heStartVertex;
		mef(lp, preV, tmpV);
	}



	//����tmpeLinkerʹ��kef���ɾ���ߺ��棬�������һ��Ҫʹ��kev.
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
//ɨ�ɲ�������ʹ�õĽӿ�
void sweep(Face* face, Vector dir, float d){
	double nd[3] = {dir.pos[0], dir.pos[1], dir.pos[2]};
	sweep(face, nd, d);
}