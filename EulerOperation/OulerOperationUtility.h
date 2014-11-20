#include <iostream>
using namespace std;
#include "OulerOperation.h"

void output(Solid* solid, char* path){

	if(solid == NULL){
		cout << "solid cannot be NULL in output function." <<endl;
		return;
	}
	int vertexCount = 0, loopCount = 0, faceCount = 0, solidCount = 0;
	Solid* tmpSolid = solid;
	Face* tmpFace;
	Loop* tmpLoop;
	HalfEdge* tmpHF;
	Vertex* tmpVertex = solid->sVertices;
	FILE *fp;
	fp = fopen(path, "w");
	fprintf(fp, "BRP\n");
	//���vertex, loop, face, solid�ĸ�����Ϣ.
	do{
		solidCount++;
		faceCount += tmpSolid->faceCount;
		loopCount += tmpSolid->loopCount;
		vertexCount += tmpSolid->vertexCount;
		tmpSolid = tmpSolid->nxtSolid;
	}while(tmpSolid != solid);
	fprintf(fp, "%d %d %d %d\n", vertexCount, loopCount, faceCount, solidCount);
	//������������Ϣ�����ڹ����и�����б�š�
	for(int i = 0; i < vertexCount; i++){
		tmpVertex->mark = i;
		fprintf(fp, "%f %f %f\n", tmpVertex->pos[0], tmpVertex->pos[1], tmpVertex->pos[2]);
		tmpVertex = tmpVertex->nxtVertex;
	}
	//���loop����Ϣ�����ڹ�����Ϊ���š�
	tmpSolid = solid;
	loopCount = 0;
	for(int i = 0; i < solidCount; i++){
		tmpFace = tmpSolid->sFaces;

		do{
			tmpLoop = tmpFace->fLoops;

			do{
				tmpLoop->mark = loopCount;
				loopCount++;
				
				tmpHF = tmpLoop->lHalfEdges;
				int thisVertexCount = 0;
				do{
					thisVertexCount++;
					tmpHF = tmpHF->nxtHalfEdge;
				}while(tmpHF != tmpLoop->lHalfEdges);
				fprintf(fp, "%d", thisVertexCount);
				do{
					fprintf(fp, " %d", tmpHF->heStartVertex->mark);
					tmpHF = tmpHF->nxtHalfEdge;
				}while(tmpHF != tmpLoop->lHalfEdges);
				fprintf(fp, "\n");	
					
				tmpLoop = tmpLoop->nxtLoop;
			}while(tmpLoop != tmpFace->fLoops);

			tmpFace = tmpFace->nxtFace;//
		}while(tmpFace != tmpSolid->sFaces);

		tmpSolid = tmpSolid->nxtSolid;//
	}


	tmpSolid = solid;
	for(int i = 0; i < solidCount; i++){
		tmpFace = tmpSolid->sFaces;

		do{
			tmpLoop = tmpFace->fLoops;

			fprintf(fp, "%d", tmpLoop->mark);

			int thisLoopCount = 0;
			do{
				thisLoopCount++;
				tmpLoop = tmpLoop->nxtLoop;
			}while(tmpLoop != tmpFace->fLoops);
			fprintf(fp, " %d", thisLoopCount - 1);
			tmpLoop = tmpLoop->nxtLoop;
			while(tmpLoop != tmpFace->fLoops){
				fprintf(fp, " %d", tmpLoop->mark);
				tmpLoop = tmpLoop->nxtLoop;
			}
			fprintf(fp, " %d\n", i);
			tmpFace = tmpFace->nxtFace;//
		}while(tmpFace != tmpSolid->sFaces);

		tmpSolid = tmpSolid->nxtSolid;//
	}


	fclose(fp);
}