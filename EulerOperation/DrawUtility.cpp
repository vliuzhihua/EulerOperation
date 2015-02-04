#include "EulerBaseDataStructure.h"
#include <GL/glut.h> 



void CALLBACK PolyLine3DBegin(GLenum type)  
{  
    glBegin(type);  
}  
  
void CALLBACK PolyLine3DVertex ( GLdouble * vertex)  
{  
    const GLdouble *pointer = (GLdouble *) vertex;  
	glVertex3dv(pointer);  
}  
  
void CALLBACK PolyLine3DEnd()  
{  
    glEnd();  
}  
GLUtesselator* tesser()  
{  
    GLUtesselator * tess;  
    tess=gluNewTess();  
    gluTessCallback(tess,GLU_TESS_BEGIN,(void (CALLBACK*)())&PolyLine3DBegin);   
    gluTessCallback(tess,GLU_TESS_VERTEX,(void (CALLBACK*)())&PolyLine3DVertex);
	
    gluTessCallback(tess,GLU_TESS_END,(void (CALLBACK*)())&PolyLine3DEnd); 

	gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    return tess;  
} 

void drawFace(Face* face){

	//glClear(GL_STENCIL_BUFFER_BIT);  
    GLUtesselator* tess = tesser();  
    if (!tess) return; 

    gluTessBeginPolygon(tess,NULL);
	Loop* firLp = face->fLoops;
	Loop* tmpLp = firLp;
	do{
		HalfEdge* firHe = tmpLp->lHalfEdges;
		HalfEdge* tmpHe = firHe;
		gluTessBeginContour(tess);
		do{
			gluTessVertex(tess, tmpHe->heStartVertex->point.pos, tmpHe->heStartVertex->point.pos);
			glNormal3dv(face->normal.pos);
			//以下注释的函数不知道为什么是错的，总之传进去的法向量有问题。再显示的时候不对。使用上面的函数传就可以了。
			/*gluTessNormal(tess, tmpHe->heStartVertex->normal.pos[0], 
				tmpHe->heStartVertex->normal.pos[1], 
				tmpHe->heStartVertex->normal.pos[2]);*/
			
			tmpHe = tmpHe->nxtHalfEdge;
		}while(tmpHe != firHe);		  
		gluTessEndContour(tess); 

		tmpLp = tmpLp->nxtLoop;
	}while(tmpLp != firLp);

    gluTessEndPolygon(tess);   
}

void drawWireSolid(Solid* solid){
	glBegin(GL_LINES);
	Face* tmpFace = solid->sFaces;
	do{
		Loop* tmpLoop = tmpFace->fLoops;
		do{
			HalfEdge* tmpHe = tmpLoop->lHalfEdges;
			do{
				glVertex3dv(tmpHe->preHalfEdge->heStartVertex->point.pos);
				glVertex3dv(tmpHe->heStartVertex->point.pos);
				tmpHe = tmpHe->nxtHalfEdge;

			}while(tmpHe != tmpLoop->lHalfEdges);
			tmpLoop = tmpLoop->nxtLoop;
		}while(tmpLoop != tmpFace->fLoops);
		tmpFace = tmpFace->nxtFace;
	}while(tmpFace != solid->sFaces);
	glEnd();
}

void calNormal(Solid* solid){
	Face* tmpFace = solid->sFaces;
	do{
		Loop* tmpLoop = tmpFace->fLoops;
		
		Point p[3];
		p[0] = tmpLoop->lHalfEdges->heStartVertex->point;
		p[1] = tmpLoop->lHalfEdges->nxtHalfEdge->heStartVertex->point;
		p[2] = tmpLoop->lHalfEdges->nxtHalfEdge->nxtHalfEdge->heStartVertex->point;
		Vector normal = (p[1] - p[0]).cross(p[2] - p[1]);
		normal = normal * (1 / normal.length());
		tmpFace->normal = normal;

		do{
			HalfEdge* tmpHe = tmpLoop->lHalfEdges;
			do{
				tmpHe->heStartVertex->normal =  tmpHe->heStartVertex->normal + normal;

				tmpHe = tmpHe->nxtHalfEdge;

			}while(tmpHe != tmpLoop->lHalfEdges);
			tmpLoop = tmpLoop->nxtLoop;
		}while(tmpLoop != tmpFace->fLoops);
		

		tmpFace = tmpFace->nxtFace;
	}while(tmpFace != solid->sFaces);
}

//删除某个实体中的所有的边，半边，点，环等。
void deleteSolid(Solid* solid){

	//首先循环删除所有的半边，环， 面。
	Face* tmpFace = solid->sFaces;
	Face* nxtFace;

	for(int i = 0; i < solid->faceCount; i++){
		Loop* tmpLoop = tmpFace->fLoops;
		Loop* nxtLoop;
		//先对环的数目进行统计。
		int lpCount = 0;
		do{
			tmpLoop = tmpLoop->nxtLoop;
			lpCount++;
		}while(tmpLoop != tmpFace->fLoops);
		while(lpCount--){
			HalfEdge* tmpHe = tmpLoop->lHalfEdges;
			HalfEdge* nxtHe;
			//先对半边的数目进行统计
			int heCount = 0;
			do{
				tmpHe = tmpHe->nxtHalfEdge;
				heCount++;
			}while(tmpHe != tmpLoop->lHalfEdges);
			while(heCount--){
				nxtHe = tmpHe->nxtHalfEdge;
				delete tmpHe;
				tmpHe = nxtHe;
			}
			nxtLoop = tmpLoop->nxtLoop;
			delete tmpLoop;
			tmpLoop = nxtLoop;
		}
		nxtFace = tmpFace->nxtFace;
		delete tmpFace;
		tmpFace = nxtFace;
	};

	//接着删除所有的顶点
	Vertex * tmpV = solid->sVertices, *nxtV;
	for(int i = 0; i < solid->vertexCount; i++){
		nxtV = tmpV->nxtVertex;
		delete tmpV;
		tmpV = nxtV;
	}
	//删除所有的边。
	Edge * tmpE = solid->sEdges, *nxtE;
	int edgeCount = 0;
	do{
		edgeCount++;
		tmpE = tmpE->nxtEdge;
	}while(tmpE != solid->sEdges);
	while(edgeCount--){
		nxtE = tmpE->nxtEdge;
		delete tmpE;
		tmpE = nxtE;
	}

}