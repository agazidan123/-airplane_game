#include<windows.h>
#include<stdlib.h>
#include<gl/glut.h>
#include<time.h>
#include<stdio.h>
#include<math.h>
#define BLOCKSPEED 0.009
#define BOOSTER_MAX 50
int SCREENH=700,SCREENW=1000;
//----------------Obstacles declaration----------
typedef struct building
{
    float block_x,block_y;
    bool state;
    int no_floors;
} building;

typedef struct Cloud
{
    float block_x,block_y;
    bool state;
} Cloud;
//-------------------declarations---------------
float bspd=BLOCKSPEED;  // block speed
bool pause=false,lflag = true,wflag = true,gameEndStatus=false,instflag=false,abtflag=false,start=false;  //flags
float plane_mvmt=0.0;//jet movement up or down
float score=1;
char score_Str[20],slevel[20];   //score string and levelstring
int level=1,buildColor;     // initial level=1
building b;  // building struct
Cloud s;     // cloud struct
float booster=BOOSTER_MAX,boost=0;

void keyPressed(unsigned char,int,int);
void mouse(int button, int state, int x, int y);
void printString(float x,float y,float z,void *font,char *string);
void buildingBlock();
void CloudBlock();
void init();
void drawJet();
void gameEnd();
void drawBg();
void welcome();
void drawBuilding();
void drawCloud();
bool cloudHit();
bool buildingHit();
void printScore();
void display();
void moveJetU();
void moveJetD();

void buildingBlock()
{
	b.block_x=50.0;
	b.no_floors = rand()%3+3;
	buildColor = rand()%3;
	b.block_y=b.no_floors*10 +15;
	b.state=true;
	s.state=false;
}
void CloudBlock()
{
	s.block_x=50.0;
	srand(time(0));
	s.block_y=(rand()%30)+50;
	s.state=true;
	b.state=false;
}
void drawJet()
{
	//left tail wing
	glColor3f(1.0,1.0,0.6);
	glBegin(GL_POLYGON);
	glVertex2f(5.5,47.0);
	glVertex2f(8.5,47.0);
	glVertex2f(5.5,48.0);
	glVertex2f(4.5,48.0);
	glEnd();


	//left front wing
	glColor3f(0.6,0.6,0.6);
	glBegin(GL_POLYGON);
	glVertex2f(13.0,47.0);
	glVertex2f(20.0,47.0);
	glVertex2f(13.0,50.0);
	glVertex2f(11.0,50.0);
	glEnd();

	//tail
	glColor3f(1.0,0.5,0.5);
	glBegin(GL_POLYGON);
	glVertex2f(4.7,45.0);
	glVertex2f(5.5,51.0);
	glVertex2f(7.0,51.0);
	glVertex2f(9.0,45.0);
	glEnd();


	//body
	glColor3f(0.5,1.0,0.5);
	glBegin(GL_POLYGON);
	glVertex2f(5.0,48.0);
	glVertex2f(11.0,48.0);
	glVertex2f(22.0,46.5);
	glVertex2f(22.0,45.0);
	glVertex2f(5.0,45.0);
	glEnd();


	//right front wing
	glColor3f(1.0,0.6,0.6);
	glBegin(GL_POLYGON);
	glVertex2f(13.0,46.0);
	glVertex2f(18.0,46.0);
	glVertex2f(13.0,41.0);
	glVertex2f(11.0,41.0);
	glEnd();


	//dome
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POLYGON);
	glVertex2f(13.0,47.0);
	glVertex2f(15.0,48.5);
	glVertex2f(17.0,49.0);
	glVertex2f(19.0,48.0);
	glVertex2f(21.0,46.0);
	glVertex2f(17.0,46.0);
	glVertex2f(15.0,47.5);
	glVertex2f(13.0,47.0);
	glEnd();


	//right tail wing
	glColor3f(0.6,0.6,0.6);
	glBegin(GL_POLYGON);
	glVertex2f(5.5,47.0);
	glVertex2f(8.5,47.0);
	glVertex2f(5.5,43.0);
	glVertex2f(4.5,43.0);
	glEnd();


	// front tip
	glColor3f(0.4,0.4,0.4);
	glBegin(GL_POLYGON);
	glVertex2f(22.0,45.0);
	glVertex2f(22.3,45.375);
	glVertex2f(22.6,45.75);
	glVertex2f(22.3,46.125);
	glVertex2f(22.0,46.5);
	glEnd();
}




void drawString(float x,float y,float z,void *font,char *string)
{
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}


void gameEnd()
{
	gameEndStatus = true;

	glColor3f(0,0,1);
	glRectf(20.0,20.0,80.0,80.0);

	glColor3f(0.8,0.8,0.8);
	glRectf(21.0,21.0,79.0,79.0);


	glColor3f(0,0,1);
	glRectf(40,5,60,10);

	glColor3f(0.8,0.8,0.8);
	glRectf(40.5,5.5,59.5,9.5);

	glColor3f(0,0,1);

	drawString(43,6,0,GLUT_BITMAP_TIMES_ROMAN_24,"RESTART");
	drawString(41,71,0,GLUT_BITMAP_TIMES_ROMAN_24,"GAME OVER!!!");
	drawString(23,61,0,GLUT_BITMAP_HELVETICA_18,"DISTANCE :");
	drawString(40,61,0,GLUT_BITMAP_TIMES_ROMAN_24,score_Str);
    printf("m\n");
	printf("\n");
	drawString(23,56,0,GLUT_BITMAP_HELVETICA_18,"LEVEL         :");
	drawString(40,56,0,GLUT_BITMAP_TIMES_ROMAN_24,slevel);
	drawString(33,30,0,GLUT_BITMAP_HELVETICA_18," ENJOY PLAYING THE GAME");

	glutPostRedisplay();
}


void drawBg()
{
	glPushMatrix();

	glColor3f(0,1,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 9.0, 0.0);
    glVertex3f(100.0, 9.0, 0.0);
    glVertex3f(100.0, 10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glEnd();


	glColor3f(0.4,0.2,0.1);
	glBegin(GL_POLYGON);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(100.0,0.0,0.0);
	glVertex3f(100.0,9.0,0.0);
	glVertex3f(0.0,9.0,0.0);
	glEnd();



    glColor3f(0.5, 0.6, 0.79);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 10.0, 0.0);
    glVertex3f(100.0, 10.0, 0.0);
    glVertex3f(100.0, 100.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);
    glEnd();

	glPopMatrix();
}




void welcome()
{
	glColor3f(0.3,0.56,0.84);
	glBegin(GL_POLYGON);
	glVertex3f(0.0,0.0,0.0);
	glColor3f(0.137,0.137,0.556);
	glVertex3f(100.0,0.0,0.0);
	glColor3f(0.196,0.196,0.8);
	glVertex3f(100.0,100.0,0.0);
	glVertex3f(0.0,100.0,0.0);
	glEnd();
	drawJet();

	// button 1 .. PLAY
    glColor3f(0.196,0.196,0.8);
    glRectf(39.5,39.5,60.5,45.5);

    glColor3f(0.8,0.8,0.8);
    glRectf(40,40,60,45);
    glColor3f(0.137,0.137,0.556);
    drawString(47,42,0,GLUT_BITMAP_HELVETICA_18,"PLAY");

    // button 2 .. instructions
    glColor3f(0.196,0.196,0.8);
    glRectf(39.5,29.5,60.5,35.5);

    glColor3f(0.8,0.8,0.8);
    glRectf(40,30,60,35);
    glColor3f(0.137,0.137,0.556);
    drawString(41,31,0,GLUT_BITMAP_HELVETICA_18,"INSTRUCTIONS");

    // button 3 .. ABOUT
    glColor3f(0.196,0.196,0.8);
    glRectf(39.5,19.5,60.5,25.5);

    glColor3f(0.8,0.8,0.8);
    glRectf(40,20,60,25);
    glColor3f(0.137,0.137,0.556);
    drawString(46,21,0,GLUT_BITMAP_HELVETICA_18,"ABOUT");

    // button 4 .. exit
    glColor3f(0.196,0.196,0.8);
    glRectf(39.5,9.5,60.5,15.5);

    glColor3f(0.8,0.8,0.8);
    glRectf(40,10,60,15);
    glColor3f(0.137,0.137,0.556);
    drawString(47,11,0,GLUT_BITMAP_HELVETICA_18,"EXIT");
	glPushMatrix();

	glColor3f(0.8,0.8,0.8);
	drawString(35.5,80,0,GLUT_BITMAP_TIMES_ROMAN_24,"AIRPLANE GAME");
	glPopMatrix();
	glColor3f(0.137,0.137,0.556);

}


void drawBuilding()
{

	for(int i=1;i<=b.no_floors;i++)
	{
		glPushMatrix();

		if(buildColor==0)
			glColor3f(0.8,0.0,0.0);
		else if (buildColor ==1)
			glColor3f(0.8,0.8,0.0);
		else
			glColor3f(0.0,0.8,0.8);

		glTranslatef(b.block_x,10.0*i,0.0);   //base
		glBegin(GL_POLYGON);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(15.0,0.0,0.0);
		glVertex3f(15.0,10.0,0.0);
		glVertex3f(0.0,10.0,0.0);
		glEnd();
		glColor3f(1.0,1.0,1.0);     // left window
		glBegin(GL_POLYGON);
		glVertex3f(2.5,5.0,0.0);
		glVertex3f(5.5,5.0,0.0);
		glVertex3f(5.5,8.0,0.0);
		glVertex3f(2.5,8.0,0.0);

		glEnd();
		glColor3f(1.0,1.0,1.0);     // right window
		glBegin(GL_POLYGON);
		glVertex3f(12.5,5.0,0.0);
		glVertex3f(9.5,5.0,0.0);
		glVertex3f(9.5,8.0,0.0);
		glVertex3f(12.5,8.0,0.0);
		glEnd();

		glPopMatrix();
	}
	glPushMatrix();

	if(buildColor==0)
		glColor3f(0.8,0.0,0.0);
	else if (buildColor ==1)
		glColor3f(0.8,0.8,0.0);
	else
		glColor3f(0.0,0.8,0.8);

	glTranslatef(b.block_x,10.0,0.0);   //base
	glBegin(GL_POLYGON);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(15.0,0.0,0.0);
	glVertex3f(15.0,10.0,0.0);
	glVertex3f(0.0,10.0,0.0);
	glEnd();
	glColor3f(1.0,1.0,1.0);     // door
	glBegin(GL_POLYGON);
	glVertex3f(5.5,0.0,0.0);
	glVertex3f(9.5,0.0,0.0);
	glVertex3f(9.5,6.0,0.0);
	glVertex3f(5.5,6.0,0.0);
	glEnd();
	glPopMatrix();
}

void drawCloud()
{
	glColor3f(1.0,1.0,1.0);
	glTranslatef(s.block_x,s.block_y,0.0);
	glutSolidSphere(5,100,10);
	glTranslatef(6,-3.0,0.0);
	glutSolidSphere(5,100,10);
	glTranslatef(0,6.0,0.0);
	glutSolidSphere(5,100,10);
	glTranslatef(6,-3.0,0.0);
	glutSolidSphere(5,100,10);

}



bool cloudHit()
{
    if(s.block_x < 13 && s.block_x> -5)
        if(plane_mvmt-3+50 >s.block_y-3 && plane_mvmt-3+50 < s.block_y+3)   // plane front to cloud mid box1
            return true;

    if(s.block_x < 12 && s.block_x> -4)
        if(plane_mvmt-3+50 >s.block_y-5 && plane_mvmt-3+50 < s.block_y+5)   // plane front to cloud mib box2
            return true;

    if(s.block_x < 10 && s.block_x> -1)
        if(plane_mvmt-3+50 >s.block_y-6 && plane_mvmt-3+50 < s.block_y-2)
            return true;

    //for top wing and bottom wing
    if(s.block_x < 5 && s.block_x> -3)
        if(plane_mvmt-3+50 >s.block_y-11 && plane_mvmt-3+50 < s.block_y+13)
            return true;

    return false;
}

bool buildingHit()
{
	if (((int)b.block_x<=8 &&(int)b.block_x>=-7 && ((int)plane_mvmt+50)-b.block_y<=3))   //buildin back  body to tail
		return true;
	else if (((int)b.block_x<=10 &&(int)b.block_x>=-5 && ((int)plane_mvmt+50)-b.block_y<=0))   //body to tail
		return true;
	else if(((int)b.block_x<=6 &&(int)b.block_x>=-3 && ((int)plane_mvmt+47)-b.block_y<=0))  //right wing
		return true;
	else if(((int)b.block_x<=4 &&(int)b.block_x>=-4 && ((int)plane_mvmt+47)-b.block_y<=3))  //   building back right wing
		return true;
	else
		return false;
}

bool boundHit()
{
	if(plane_mvmt+50>=100||plane_mvmt+50 <=18)   // top and bottom boundary
		return true;
	else
		return false;
}




void printScore()
{
	glColor3f(1.0,1.0,0.0);//score

	sprintf(slevel, "%d", (int)level );
	drawString(58,1.8,0,GLUT_BITMAP_TIMES_ROMAN_10,"Level");
	drawString(58,3.5,0,GLUT_BITMAP_TIMES_ROMAN_24,slevel);

	if(booster >0 && boost)
		score+=0.03;//SCORE with booster
	else
		score+=0.005;//SCORE without booster

	drawString(38,1.5,0,GLUT_BITMAP_TIMES_ROMAN_10,"Distance");
	sprintf(score_Str, "%d m",(int)score );
	drawString(38,3,0,GLUT_BITMAP_TIMES_ROMAN_24, score_Str);

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GameOver Checking
	if(gameEndStatus == true)
	{
		gameEnd();
	}
	else if(wflag==true)//Welcome Screen
	{
		welcome();
	}
		else if (instflag == true)
		{
			glColor3f(0.3,0.56,0.84);   // background
			glBegin(GL_POLYGON);
			glVertex3f(0.0,0.0,0.0);
			glColor3f(0.137,0.137,0.556);
			glVertex3f(100.0,0.0,0.0);
			glColor3f(0.196,0.196,0.8);
			glVertex3f(100.0,100.0,0.0);
			glVertex3f(0.0,100.0,0.0);
			glEnd();
			glPushMatrix();
			glScalef(0.8,0.8,0);
			drawJet();
			glPopMatrix();
			glColor3f(0.137,0.137,0.556);
			glRectf(20.0,20.0,80.0,80.0);
			glColor3f(0.8,0.8,0.8);
			glRectf(21.0,21.0,79.0,79.0);


			glColor3f(0.196,0.196,0.8);
			glRectf(40,5,60,10);
			glColor3f(0.8,0.8,0.8);
			glRectf(40.5,5.5,59.5,9.5);

			glColor3f(0.137,0.137,0.556);
			drawString(46,6,0,GLUT_BITMAP_TIMES_ROMAN_24,"BACK");

			glColor3f(0.137,0.137,0.556);
			drawString(37,75,0,GLUT_BITMAP_TIMES_ROMAN_24,"HOW TO PLAY");
			drawString(23,69,0,GLUT_BITMAP_HELVETICA_18,"- Click and hold mouse left key to gain altitude of ");
			drawString(23,65,0,GLUT_BITMAP_HELVETICA_18,"    the plane.");
			drawString(23,61,0,GLUT_BITMAP_HELVETICA_18,"- Release the mouse left key to reduce the altitude.");
			drawString(23,57,0,GLUT_BITMAP_HELVETICA_18,"- Use the Right mouse key to speed up the plane(NOS)");
			drawString(23,53,0,GLUT_BITMAP_HELVETICA_18,"- Main aim of the game is to avoid the obstacles ");
			drawString(23,49,0,GLUT_BITMAP_HELVETICA_18,"    such as buildings and clouds.");
			drawString(23,45,0,GLUT_BITMAP_HELVETICA_18,"- Also the meter at the bottom shows the distance ");
			drawString(23,41,0,GLUT_BITMAP_HELVETICA_18,"    travelled,NITROS left,Atitude and the LEVEL.");
			drawString(23,33,0,GLUT_BITMAP_HELVETICA_18,"    increases as well as the speed of the plane.");
			drawString(33,27,0,GLUT_BITMAP_HELVETICA_18," ENJOY PLAYING THE GAME");

			glutPostRedisplay();

		}
	else if (abtflag == true)
	{
		glColor3f(0.3,0.56,0.84);   // background
		glBegin(GL_POLYGON);
		glVertex3f(0.0,0.0,0.0);
		glColor3f(0.137,0.137,0.556);
		glVertex3f(100.0,0.0,0.0);
		glColor3f(0.196,0.196,0.8);
		glVertex3f(100.0,100.0,0.0);
		glVertex3f(0.0,100.0,0.0);
		glEnd();
		glPushMatrix();
		glScalef(0.8,0.8,0);
		drawJet();
		glPopMatrix();
		glColor3f(0.137,0.137,0.556);
		glRectf(20.0,20.0,80.0,80.0);
		glColor3f(0.8,0.8,0.8);
		glRectf(21.0,21.0,79.0,79.0);


		glColor3f(0.196,0.196,0.8);
		glRectf(40,5,60,10);
		glColor3f(0.8,0.8,0.8);
		glRectf(40.5,5.5,59.5,9.5);
		glColor3f(0.137,0.137,0.556);
		drawString(46,6,0,GLUT_BITMAP_TIMES_ROMAN_24,"BACK");

		glColor3f(0.137,0.137,0.556);
		drawString(44,75,0,GLUT_BITMAP_TIMES_ROMAN_24,"ABOUT");
		drawString(21,61,0,GLUT_BITMAP_HELVETICA_18,"            Published by: ");
		drawString(23,53,0,GLUT_BITMAP_HELVETICA_18,"       Abanoub ,Abdo ,Ziad, Mustafa, Ahmed, Amal");

		drawString(33,40,0,GLUT_BITMAP_HELVETICA_18," ENJOY PLAYING THE GAME");
		glutPostRedisplay();

	}
	else if( pause == true)
	{
		drawBg();
		glPushMatrix();
		glScalef(0.8,0.8,0);
		drawJet();
		glPopMatrix();
		glPushMatrix();
		glColor3f(0.196,0.196,0.8);
		glRectf(35.0,40.0,65.0,60.0);
		glColor3f(0.8,0.8,0.8);
		glRectf(36.0,41.0,64.0,59.0);
		glPopMatrix();
		glColor3f(0.137,0.137,0.556);
  		glutPostRedisplay();

	}
	else if((b.state == true && buildingHit() == true)|| boundHit()== true)
	{
		gameEndStatus = true;
		gameEnd();
	}
	else if(s.state == true && cloudHit() == true)
	{

		gameEndStatus = true;
		gameEnd();
	}
	else
	{

		if((int)score%50==0 && lflag==true)// l-level
		{
			lflag=false;
			level++;
			bspd+=0.01;
		}
		else if((int)score%50!=0 && lflag==false)
		{
			lflag=true;
		}

		glPushMatrix();
		drawBg();


		glPushMatrix();
		glTranslatef(0.0,plane_mvmt,0.0);
		drawJet();			//code for jet
		glPopMatrix();


		if( booster <= BOOSTER_MAX && !boost)    // booster charging
			booster+=0.005;



		if( (b.state == true && b.block_x < -10) || (s.state == true && s.block_x < -10))            //for new building //building has gone outside the screen- state=true
		{
			srand(time(NULL));
			int random = rand()%2;//for random building or cloud
			if( random == 0)
			{
				buildingBlock();
			}
			else
			{
				CloudBlock();
			}
		}

		else if(b.state == true)
		{
			if(booster >0 && boost)
			{
				b.block_x-=bspd+boost;
				booster = booster-0.02;//reduce to normal speed after leaving boost key
			}
			else
				b.block_x-=bspd;
		}
		else if( s.state == true)
		{
			if(booster >0 && boost)
			{
				s.block_x-=bspd+boost;
				booster = booster-0.02;

			}
			else
			{
				s.block_x-=bspd;
			}
		}
		if(b.state == true)
		{
			glTranslatef(b.block_x,0.0,0.0);
			drawBuilding();
		}
		else if( s.state == true)
		{
			glTranslatef(s.block_x,0.0,0.0);
			drawCloud();
		}
		glPopMatrix();

		printScore();
	}
	glFlush();
	glutSwapBuffers();
}


void moveJetU()
{
	if(start == false)
		glutPostRedisplay();
	else if(pause == false)
	{
		plane_mvmt+=0.05;
		glutPostRedisplay();
	}
}


void moveJetD()
{
	if(start == false)
		glutPostRedisplay();
	else if(pause == false )
	{
		plane_mvmt-=0.05;
		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y)
{
	int mx=x*100/SCREENW,my=(SCREENH-y)*100/SCREENH;		// m = mouse cordinate to graphics

	if(instflag || abtflag || gameEndStatus)
	{
		if(mx>40 && mx<60)
		{
			if(my>5 && my<10)
			{
				wflag = true;
				if(instflag)
					instflag = false;
				else if (abtflag)
					abtflag = false;
				if(gameEndStatus)
				{
					wflag = true;
					gameEndStatus = false;
					plane_mvmt = 0;
					start = false;
					init();
					bspd = BLOCKSPEED;//restarting the game
					booster=BOOSTER_MAX;
					score=1;
					level=1;
					glutPostRedisplay();
				}
			}
		}
	}
	if(wflag == true)
	{
		if(mx>40 && mx<60)
		{
			if(my>40 && my<45)
			{
				start = true;
				wflag=false;
			}
			else if(my>30 && my<35)
			{
				instflag = true;
				wflag = false;
			}
			else if(my>20 && my<25)
			{
				abtflag = true;
				wflag = false;
			}
			else if(my>10 && my<15)
			{
				exit(0);
			}

		}
	}
	else
	{

		if(button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN )
				glutIdleFunc(moveJetU);

			else if (state == GLUT_UP )
				glutIdleFunc(moveJetD);
		}
		if(button == GLUT_RIGHT_BUTTON)
		{

			if(state == GLUT_DOWN)
			{
				if(booster>0)
				{
					boost = 0.05;
				}
			}
			if(state == GLUT_UP)
			{
				boost = 0;
			}
		}
	}
}
void keyPressed(unsigned char key,int x, int y)
{
	if(key == 'e')
	{
		exit(0);
	}
	glutPostRedisplay();
}

void myReshape(int w, int h)
{
	SCREENH=h,SCREENW=w;
	printf("width = %d\theight= %d",w,h);
	glViewport(0,0,w,h);
	glOrtho(0.0, 100.0, 0.0, 100.0,	-5.0 , 10.0);
}

void init()
{
	int random = rand()%2;
	if( random == 0)
	{
		buildingBlock();
	}
	else
	{
		CloudBlock();
	}
}

int main(int argc, char** argv)
{
        printf("\nHow To Play");
		printf("Click and hold mouse left key to gain altitude of the plane\n");
		printf("Release the mouse left key to reduce the altitude\n");
		printf("\n");
		printf("-->The main aim of the game is to avoid the obstacles: buildings and clouds\n");
		printf("-->Also the meter at the bottom shows the distance travelled,NITROS left,Atitude and LEVEL.\n");
		printf("\n");
		printf("-->As you reach distance multples of 50 tour level increases as well as the speed of the plane.\n");
		printf("==>ENJOY PLAYING THE GAME\n");
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (SCREENW,SCREENH);
	glutInitWindowPosition (50,50);
	glutCreateWindow ("Airplane Simulation Game");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyPressed);
	glutMainLoop();
	return 0;
}
