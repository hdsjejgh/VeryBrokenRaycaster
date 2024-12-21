#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*P2


float px,py,pdx,pdy,pa;
int moveAmount=10;





void drawPlayer(){
	glColor3f(1,1,0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(px,py);
	glEnd();
	
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(px,py);
	glVertex2i(px+pdx*moveAmount, py+pdy*moveAmount);
	glEnd();
}

int mapX = 8; int mapY = 8; int mapS = mapX*mapY;
int map[]=
{
	1,1,1,1,1,1,1,1,
	1,0,1,0,1,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,1,1,1,0,1,
	1,0,0,0,0,0,0,1,
	1,0,1,0,1,1,0,1,
	1,0,0,0,1,1,0,1,
	1,1,1,1,1,1,1,1
};

void drawMap2d(){
	int xo,yo;
	for (int y=0;y<mapY;y++){
		for (int x=0;x<mapY;x++){
			if (map[y*mapX+x]==1) {glColor3f(1,1,1);} else {glColor3f(0,0,0);}
			xo = x*mapS; yo = y*mapS;
			glBegin(GL_QUADS);
			glVertex2i(xo+1,yo+1);
			glVertex2i(xo+1,yo+mapS-1);
			glVertex2i(xo+mapS-1,yo+mapS-1);
			glVertex2i(xo+mapS-1,yo+1);
			glEnd();
		}
	}
}

float dist(float ax. float ay, float bx, float by, float ang){
	return (sqrt((bx-ax)*(bx-ax)+(bx-ay)*(bx-ay)));
}

void drawRays3D(){
	int r,mx,my,dof,mp; float rx,ry,ra,xo,yo;
	ra = pa;
	
	for (r=0;r<1;r++){
		dof = 0;
		float disH=10000000;hx=px,hy=py;
		float aTan = -1/tan(ra);
		if (ra>PI){ry=(((int)py>>6)<<6)-0.0001; rx=(py-ry)*aTan+px;yo=-64;xo=-yo*aTan;}
		if (ra<PI){ry=(((int)py>>6)<<6)+64; rx=(py-ry)*aTan+px;yo=64;xo=-yo*aTan;}
		if (ra==0||ra==PI){rx=px; ry=py;dof=8;}
		while(dof<8){
			mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
			if(mp>0 && mp<mapX*mapY && map[mp]==1){hx=rx; hy=ry; disH=dist(px,py,hx,hy,ra);dof=8;}
			else{rx+=xo;ry+=yo;dof+=1;}
			
		}

		dof = 0;
		float disV=10000000;vx=px,vy=py;
		float nTan = -tan(ra);
		if (ra>P2 && ra<P3){rx=(((int)px>>6)<<6)-0.0001; ry=(px-rx)*nTan+py;xo=-64;yo=-xo*nTan;}
		if (ra<P2 || ra>P3){rx=(((int)px>>6)<<6)+64; ry=(px-rx)*nTan+py;xo=64;yo=-xo*nTan;}
		if (ra==0||ra==PI){rx=px; ry=py;dof=8;}
		while(dof<8){
			mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
			if(mp>0 && mp<mapX*mapY && map[mp]==1){vx=rx; vy=ry; disV=dist(px,py,vx,vy,ra);dof=8;}
			else{rx+=xo;ry+=yo;dof+=1;}
			
		}
		if (disV<disH){rx=vx;ry=vy;} else if (disV>disH){rx=hx; ry=hy;}
		glColor3f(1,0,0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px,py);
		glVertex2i(rx,ry);
		glEnd();
	}
	
}

void buttons(unsigned char key, int x, int y){
	
	if (key=='a') {pa-=0.1; if (pa<0){pa+=2*PI;} pdx=cos(pa)*moveAmount; pdy=sin(pa)*moveAmount;}
	if (key=='d') {pa+=0.1; if (pa>2*PI){pa-=2*PI;} pdx=cos(pa)*moveAmount; pdy=sin(pa)*moveAmount;}
	if (key=='s') {py-=pdy; px-=pdx;}
	if (key=='w') {py+=pdy;px+=pdx;}
	glutPostRedisplay();
}

void display()
{
 glClear(GL_COLOR_BUFFER_BIT);
 drawMap2d();
 drawPlayer();
 drawRays3D();
 glutSwapBuffers();
}

void init(){
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,1024,512,0);
	px=300; py=300; pdx=cos(pa)*moveAmount; pdy=sin(pa)*moveAmount;
}


int main(int argc, char** argv)
{ 
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowSize(1024,512);
 glutCreateWindow("OpenGL");
 init();
 glutDisplayFunc(display);
 glutKeyboardFunc(buttons);
 glutMainLoop();
 return 0;
}
