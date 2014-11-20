#include<iostream>
#include "OulerOperationUtility.h"
using namespace std;

int main(){
	Vertex* vtx1 = new Vertex(0, 0, 0);
	Vertex* vtx2 = new Vertex(0, 4, 0);
	Vertex* vtx3 = new Vertex(4, 4, 0);
	Vertex* vtx4 = new Vertex(4, 0, 0);
	Vertex* vtx5 = new Vertex(0, 0, 4);
	Vertex* vtx6 = new Vertex(0, 4, 4);
	Vertex* vtx7 = new Vertex(4, 4, 4);
	Vertex* vtx8 = new Vertex(4, 0, 4);
	Vertex* vtx9 = new Vertex(1, 1, 0);
	Vertex* vtx10 = new Vertex(1, 3, 0);
	Vertex* vtx11 = new Vertex(3, 3, 0);
	Vertex* vtx12 = new Vertex(3, 1, 0);
	Face* face;
	Loop* loop, *loop2, *loop3;
	Solid* solid = mvfs(vtx1, face);
	mev(face->fLoops, vtx1, vtx2);
	mev(face->fLoops, vtx2, vtx3);
	mev(face->fLoops, vtx3, vtx4);
	loop = mef(face->fLoops, vtx4, vtx1);
	
	mev(loop, vtx1, vtx9);
	loop2 = kemr(loop, vtx1, vtx9);
	mev(loop2, vtx9, vtx10);
	mev(loop2, vtx10, vtx11);
	mev(loop2, vtx11, vtx12);
	loop3 = mef(loop2, vtx12, vtx9);
	kfmrh(face->fLoops, loop3);

	float dir[] = {1, 1, 1};
	sweep(loop->lFace, dir, 1);
	/*mev(face->fLoops, vtx1, vtx5);
	mev(face->fLoops, vtx2, vtx6);
	mev(face->fLoops, vtx3, vtx7);
	mev(face->fLoops, vtx4, vtx8);
	//mef
	mef(face->fLoops, vtx5, vtx6);
	mef(face->fLoops, vtx6, vtx7);
	mef(face->fLoops, vtx7, vtx8);
	mef(face->fLoops, vtx8, vtx5);
	*/


	output(solid, "d:\\ouler.brp");
}