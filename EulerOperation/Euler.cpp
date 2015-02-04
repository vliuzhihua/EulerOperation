#include "GL/glew.h"
#include "GL/glut.h"
#include <iostream>
#include "EulerOperationUtility.h"
#include "SceneDisplay.h"
using namespace std;

//2014/12/14����ΪCADʵ�����ҵ�ϴ��ˡ�
//�ֽ������ܽ᣺��������SweepDisplay��ChamferDisplay�Ⱥ����д����ڴ�й¶��
//����deleteSolidδ����ȫɾ������ʹ�ù���ջ�ռ䡣Ҳ������ŷ������������ʵ�ֹ����д������⡣
//SceneDisplay��Ⱦ���δ��ʵ���ӵ��Ǩ�ƺͺ��ˣ���Ҫʹ��glFrustum�ȣ�����ʹ��ʱ�������⣬��ʱȡ�����Ժ�ɸġ�
//�������SceneDisplay��������˺ܶࡣ

Solid* solid1, *solid2;
int wireOrNot = 0;
int rotateOrNot = 1;
//�ڻ�������ɨ�ɵ�ʱ��ʹ�õ�bezier���ߵ��ĸ����ƶ��㡣
GLfloat cp[4][3] = {
		-5, -5, -1,
		-1, 6, 1,
		5, -5, 1,
		5, 4, 6};
SceneDisplay sceneDis;

#define NORMAL_MODE 1
#define SWEEP_MODE 2

extern void drawFace(Face* face);
extern void drawWireSolid(Solid* solid);
extern void calNormal(Solid* solid);
extern void deleteSolid(Solid* solid);


void drawModel(Solid* s){
	if(wireOrNot == 0){
		Face* firFace = s->sFaces;
		Face* tmpFace = firFace;
		int t = 0;
		do{
			drawFace(tmpFace);
			tmpFace = tmpFace->nxtFace;
			t++;
		}while(tmpFace != firFace);
	}else{
		drawWireSolid(s);
	}
}
//���ǲ���ʾ����ʹ�ô���
void ChamferDisplay(){
	//����������������Ĺ���
	Vertex* vtx[] = {new Vertex(-4, -4, -4), new Vertex(-4, 4, -4), new Vertex(4, 4, -4), new Vertex(4, -4, -4),
		new Vertex(-4, -4, 4), new Vertex(-4, 4, 4), new Vertex(4, 4, 4), new Vertex(4, -4, 4)};
	Face* face;
	Loop* loop, *loop2, *loop3;
	solid1 = mvfs(vtx[0], face);
	mev(face->fLoops, vtx[0], vtx[1]);
	mev(face->fLoops, vtx[1], vtx[2]);
	mev(face->fLoops, vtx[2], vtx[3]);
	loop = mef(face->fLoops, vtx[3], vtx[0]);
	mev(loop, vtx[0], vtx[4]);
	mev(loop, vtx[1], vtx[5]);
	mev(loop, vtx[2], vtx[6]);
	mev(loop, vtx[3], vtx[7]);
	mef(loop, vtx[7], vtx[6]);
	mef(loop, vtx[6], vtx[5]);
	mef(loop, vtx[5], vtx[4]);
	mef(loop, vtx[4], vtx[7]);
	//������������̽�����
	//��ʼ���ǹ��̡�
	//�ݹ�Ľ��е��ǣ��������ɵĵ㣬�ٵ���һ�Ρ�
	Vertex* reV[3];
	for(int i = 0; i < 8; i++){
		chamfer(solid1, vtx[i], 3, reV);
		for(int j = 0; j < 3; j++){
			chamfer(solid1, reV[j], 0.9);
		}
	}
	int a = 0;
	cout << "" << endl;

	//���㷨������
	calNormal(solid1);
	//����ģ�͡�
	drawModel(solid1);
	//���´���ʹ���������ֻ��ִ��һ�Ρ�
	static int mark = 0;
	if(mark == 0){
		output(solid1, ".\\euler1.brp");
		mark ++;
	}
	//ɾ��ɨβ��
	deleteSolid(solid1);
}
//ɨ�ɲ���ʾ����ʹ�ô���
void SweepDisplay(){

	Vertex* vtx1, * vtx2, *vtx3, *vtx4;
	
	GLfloat length;
	Point oldV, newV;
	Face* face;
	Loop* loop;

	for(GLfloat i = 0, k = 0; i < 1; i += 0.01, k++){
		for(int j = 0; j < 3; j++){
			newV.pos[j] = cp[0][j] * pow((1 - i), 3) 
			+ cp[1][j] * 3 * i * pow((1 - i), 2)
			+ cp[2][j] * 3 * i * i * (1 - i)
			+ cp[3][j] * i * i * i;
		}
		Vector normal = newV + ( -1.0 * oldV);
		if(k == 1){
			GLfloat t = -(normal.pos[0] + normal.pos[1]) / normal.pos[2];
			length = sqrt(1 + 1 + t * t);
			Point p1(4 / length, 4 / length, t * 4 / length);
			Point p2 = -1.0f * p1;
			Point p3 = p1.cross(normal);
			p1 = p1 + oldV;
			p2 = oldV + p2;
			p3 = p3 * (4 / p3.length());
			p3 = oldV + p3;

			vtx1 = new Vertex(p1);
			vtx2 = new Vertex(p2);
			vtx3 = new Vertex(p3);
			
			solid2 = mvfs(vtx1, face);
			mev(face->fLoops, vtx1, vtx2);
			mev(face->fLoops, vtx2, vtx3);
			loop = mef(face->fLoops, vtx3, vtx1);
			sweep(face, normal, normal.length());

		}else if (k > 1){
			sweep(face, normal, normal.length());
		}
		if(k >= 1){
			glBegin(GL_LINES);
			glVertex3f(oldV.pos[0], oldV.pos[1], oldV.pos[2]);
			glVertex3f(newV.pos[0], newV.pos[1], newV.pos[2]);
			glEnd();
		}
		
		oldV = newV;
		
	}
	//���㷨������������ʾ��
	calNormal(solid2);
	//��ʾ����
	drawModel(solid2);
	//���´���ʹ���������ֻ��ִ��һ�Ρ�
	static int mark = 0;
	if(mark == 0){
		output(solid2, ".\\euler2.brp");
		mark ++;
	}
	//���ɨβ��
	deleteSolid(solid2);
}

void myKey(unsigned char key, int x, int y){
	switch(key){
		case 'c':
			wireOrNot = ! wireOrNot;
			//printf("faceC ++\n");
			break;
		case 'v':
			rotateOrNot = !rotateOrNot;
			sceneDis.setViewRotate(rotateOrNot);
			break;
	}
}
//���Ҽ��Ĳ˵��Ĵ�������
void processMenu(int bt){
	switch(bt){
	case NORMAL_MODE:
		sceneDis.setUsrDisplayFunc(ChamferDisplay);
		printf("ChamferDisplay..................\n");
		break;
	case SWEEP_MODE:
		sceneDis.setUsrDisplayFunc(SweepDisplay);
		printf("SweepDisplay..................\n");
		break;
	}
}
//ϲ���ּ���main������
int main(){
    int fillMenu = glutCreateMenu(processMenu);
    glutAddMenuEntry("����չʾ",NORMAL_MODE);
    glutAddMenuEntry("ɨ��չʾ",SWEEP_MODE);
    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	sceneDis.setXYZSpan(-10, 10, -10, 10, 1, 30);
	sceneDis.setUsrDisplayFunc(ChamferDisplay);
	//����ʹ��shader�Ĺ���
	sceneDis.setShaderState(1);
	sceneDis.setUsrKeyboardFunc(myKey);
	sceneDis.startTimer();
	sceneDis.start();
}