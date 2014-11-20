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
	//输出vertex, loop, face, solid的个数信息.
	do{
		solidCount++;
		faceCount += tmpSolid->faceCount;
		loopCount += tmpSolid->loopCount;
		vertexCount += tmpSolid->vertexCount;
		tmpSolid = tmpSolid->nxtSolid;
	}while(tmpSolid != solid);
	fprintf(fp, "%d %d %d %d\n", vertexCount, loopCount, faceCount, solidCount);
	//输出点的坐标信息，并在过程中给其进行标号。
	for(int i = 0; i < vertexCount; i++){
		tmpVertex->mark = i;
		fprintf(fp, "%f %f %f\n", tmpVertex->pos[0], tmpVertex->pos[1], tmpVertex->pos[2]);
		tmpVertex = tmpVertex->nxtVertex;
	}
	//输出loop的信息，并在过程中为其标号。
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