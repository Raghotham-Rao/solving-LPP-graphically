#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<GL/glut.h>

int n, cost[2], xb[10];
int coeff[10][2], minx = 0, miny = 0, maxx = 0, maxy = 0, max = 0;
char inequality[6];
int right = 10, left = 0, top = 10, bottom = 0, choice = 9;
float feasible_points[25][2], axes_points[25][2], slopes[25][2];
int frptr = 0, aptr = 0;
float Zmax[2], Zmin[2];
int front_pg = 1, check = -1;

void getCoefficient(char* eqn, int* arr){
   int sign = 1, k = 0; //sign to check for negativity of coefficient
   for(int i = 0;i<strlen(eqn);i++){
        if(eqn[i] == '-'){ //if scanned symbol is '-' set sign to -1
            sign = -1;
            continue;
        }
        int num = 0;
        if(isdigit(eqn[i])){
            while(isdigit(eqn[i])){ // converting character string to number
                num = num * 10 + (eqn[i] - '0');
                i++;
            }
            arr[k++] = num * sign;
            sign = 1;
        }
   }
}

void getOptimalSolutions(){
    Zmax[0] = Zmin[0] = (cost[0] * feasible_points[0][0] + cost[1] * feasible_points[0][1]);
    Zmax[1] = Zmin[1] = 0;
    for(int i = 1;i<frptr;i++){
        if((cost[0] * feasible_points[i][0] + cost[1] * feasible_points[i][1]) > Zmax[0]){
            Zmax[0] = (cost[0] * feasible_points[i][0] + cost[1] * feasible_points[i][1]);
            Zmax[1] = i;
        }
        if((cost[0] * feasible_points[i][0] + cost[1] * feasible_points[i][1]) < Zmin[0]){
            Zmin[0] = (cost[0] * feasible_points[i][0] + cost[1] * feasible_points[i][1]);
            Zmin[1] = i;
        }
    }
    //printf("%f\t%f at (%f, %f)\t(%f, %f)\n", Zmax[0], Zmin[0], feasible_points[(int)Zmax[1]][0],feasible_points[(int)Zmax[1]][1], feasible_points[(int)Zmin[1]][0], feasible_points[(int)Zmin[1]][1]);
}

int checkFeasibility(float x, float y){
    for(int i = 0;i<n;i++){
        float value = coeff[i][0] * x + coeff[i][1] * y;
        switch(inequality[i]){
            case 'l': if(value>xb[i])
                            return 0;
                      break;
            case 'g': if(value<xb[i])
                            return 0;
        }
    }
    return 1;
}

float toDegrees(float radians){
    return (180 * radians) / 3.142;
}

void labelPoint(float X, float Y){
    if(Y == 0){
        int ipart = X;
        float fp = X - ipart;
        int fpart = fp*100;
        char str[10];
        itoa(ipart, str, 10);
        glColor3d(0, 0, 0);
        glRasterPos2f(X - max/50.0, 0 - max/20.0);
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '.');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        itoa(fpart, str, 10);
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
    }
    else if(X == 0){
        int ipart = Y;
        float fp = Y - ipart;
        int fpart = fp*100;
        char str[10];
        itoa(ipart, str, 10);
        glColor3d(0, 0, 0);
        glRasterPos2f(0 - max/10.0, Y - max/100.0);
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '.');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        itoa(fpart, str, 10);
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
    }
    else{
        int ipart = X;
        float fp = X - ipart;
        int fpart = fp*100;
        char str[10];
        itoa(ipart, str, 10);
        glColor3d(0, 0, 0);
        glRasterPos2f(X - max/10.0, Y - max/25.0);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '(');
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '.');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        itoa(fpart, str, 10);
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ',');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        ipart = Y;
        fp = Y - ipart;
        fpart = fp*100;
        str[10];
        itoa(ipart, str, 10);
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '.');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ' ');
        itoa(fpart, str, 10);
        for(int i = 0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ')');
    }
}

void drawAxes(){
    glLineWidth(2);
    glBegin(GL_LINES);
       glVertex2d(left, 0);
       glVertex2d(right, 0);
       glVertex2d(0, bottom);
       glVertex2d(0, top);
    glEnd();
    glPointSize(5);
    GLint rastorX;
    glBegin(GL_POINTS);
        for(int i = left/2 + max/10;i<right; i+=max/10){
            glVertex2d(i, 0);
        }
        for(int i = bottom/2 + max/10;i<top; i+=max/10){
            glVertex2d(0, i);
        }
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2d(0, top);
    glVertex2d(0 - max/50.0, top-max/50.0);
    glVertex2d(0 + max/50.0, top - max/50.0);
    glVertex2d(0, bottom/2);
    glVertex2d(0 - max/50.0, bottom/2 + max/50.0);
    glVertex2d(0 + max/50.0, bottom/2 + max/50.0);
    glVertex2d(left / 2, 0);
    glVertex2d(left / 2 + max/50.0, 0 + max/50.0);
    glVertex2d(left / 2 + max/50.0, 0 - max/50.0);
    glVertex2d(right, 0);
    glVertex2d(right - max/50.0, 0 + max/50.0);
    glVertex2d(right - max/50.0, 0 - max/50.0);
    glEnd();
    for(int i = left;i<right; i+=max/10){
        labelPoint(i, 0);
    }
    for(int i = bottom;i<top; i+=max/10){
        labelPoint(0, i);
    }
}

void sortFeasiblePoints(){
    float temp = 0;
    for(int i = 0;i < frptr;i++){
        for(int j = 0;j<frptr-i-1;j++){
            if(feasible_points[j][1]>feasible_points[j+1][1]){
                temp = feasible_points[j][1];
                feasible_points[j][1] = feasible_points[j+1][1];
                feasible_points[j+1][1] = temp;
                temp = feasible_points[j][0];
                feasible_points[j][0] = feasible_points[j+1][0];
                feasible_points[j+1][0] = temp;
            }
            else if(feasible_points[j][1]==feasible_points[j+1][1]){
                if(feasible_points[j][0]>feasible_points[j+1][0]){
                    temp = feasible_points[j][1];
                    feasible_points[j][1] = feasible_points[j+1][1];
                    feasible_points[j+1][1] = temp;
                    temp = feasible_points[j][0];
                    feasible_points[j][0] = feasible_points[j+1][0];
                    feasible_points[j+1][0] = temp;
                }
            }
        }
    }
    for(int i = 1;i<frptr;i++){
		slopes[i][0] = toDegrees(atan((feasible_points[i][1] - feasible_points[0][1]) / (feasible_points[i][0] - feasible_points[0][0])));
		slopes[i][1] = i;
	}
	for(int i = 0;i<frptr;i++){
		for(int j = 0;j<frptr-i-1;j++){
			if(slopes[j][0]>slopes[j+1][0]){
				temp = slopes[j][0];
				slopes[j][0] = slopes[j+1][0];
				slopes[j+1][0] = temp;
				temp = slopes[j][1];
				slopes[j][1] = slopes[j+1][1];
				slopes[j+1][1] = temp;
			}
		}
	}
	/*for(int i = 0;i<frptr;i++)
        printf("%f\t%f\n", slopes[i][0], slopes[i][1]);*/
}

void plotFeasiblePoints(){
    glPointSize(5);
    glBegin(GL_POINTS);
        for(int i = 0;i<frptr;i++){
            glVertex2d(feasible_points[i][0], feasible_points[i][1]);
        }
    glEnd();
    glLineWidth(1);
    for(int i = 0;i<frptr;i++){
        labelPoint(feasible_points[i][0], feasible_points[i][1]);
    }
}

void drawFeasibleRegion(){
    for(int i = 0;i<n;i++){
        for(int j = i+1;j<n;j++){
                solveEquations(coeff[i], xb[i], coeff[j], xb[j]);
        }
    }
    if(checkFeasibility(0, 0) == 1){
        feasible_points[frptr][0] = 0;
        feasible_points[frptr++][1] = 0;
    }
    addAxesPoints();
    sortFeasiblePoints();
    glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
    glBegin(GL_POLYGON);
        for(int i = 0;i<frptr;i++){
            int pno = slopes[i][1];
            glVertex2d(feasible_points[pno][0], feasible_points[pno][1]);
            //printf("%f\t%f\n", feasible_points[i][0], feasible_points[i][1]);
        }
    glEnd();
    glLineWidth(2);
    glColor3d(0, 0, 0);
    glBegin(GL_LINE_LOOP);
        for(int i = 0;i<frptr;i++){
            int pno = slopes[i][1];
            glVertex2d(feasible_points[pno][0], feasible_points[pno][1]);
        }
    glEnd();
    plotFeasiblePoints();
    glLineWidth(1);
}

void addAxesPoints(){
    for(int i = 0;i<aptr;i++){
        if(axes_points[i][0]>=0 && axes_points[i][1]>=0 && checkFeasibility(axes_points[i][0], axes_points[i][1]) == 1){
            feasible_points[frptr][0] = axes_points[i][0];
            feasible_points[frptr++][1] = axes_points[i][1];
        }
        //printf("%f\t%f\n", axes_points[i][0], axes_points[i][1]);
    }
    if(checkFeasibility(0, max)==1){
        feasible_points[frptr][0] = 0;
        feasible_points[frptr++][1] = max;
    }
    if(checkFeasibility(max, 0)==1){
        feasible_points[frptr][0] = max;
        feasible_points[frptr++][1] = 0;
    }
    if(checkFeasibility(max, max)==1){
        feasible_points[frptr][0] = max;
        feasible_points[frptr++][1] = max;
    }
}

void solveEquations(int* coeff1, int cost1, int* coeff2, int cost2){
    int n1 = coeff1[1], n2 = coeff2[1];
    if(coeff1[0] == 0 && coeff2[0]==0){}
    else if(coeff1[1] == 0 && coeff2[1]==0){}
    else{
        float newx = (float)(cost1 * n2 - cost2 * n1) / (coeff1[0] * n2 - coeff2[0] * n1);
        float newy = (float)(cost1 - coeff1[0] * newx) / coeff1[1];
        if(newx>=0 && newy>=0 && checkFeasibility(newx, newy) == 1){
            feasible_points[frptr][0] = newx;
            feasible_points[frptr][1] = newy;
            frptr++;
        }
    }
}

void plotEquations(){
    glBegin(GL_LINES);
        for(int i = 0;i<n;i++){
            if(coeff[i][1] == 0){
                glVertex2d((xb[i] / coeff[i][0]), miny);
                glVertex2d((xb[i] / coeff[i][0]), top);
                axes_points[aptr][0] = (xb[i] / coeff[i][0]);
                axes_points[aptr++][1] = 0;
            }
            else if(coeff[i][0] == 0){
                glVertex2d(minx, (xb[i] / coeff[i][1]));
                glVertex2d(right, (xb[i] / coeff[i][1]));
                axes_points[aptr][1] = (xb[i] / coeff[i][1]);
                axes_points[aptr++][0] = 0;
            }
            else if(xb[i] == 0){
                glVertex2d(0, 0);
                glVertex2d(right, (-1 * coeff[i][0] * right) / coeff[i][1]);
                axes_points[aptr][0] = 0;
                axes_points[aptr++][1] = 0;
            }
            else{
                if(((float)xb[i] / coeff[i][1])<0){
                    glVertex2f(0, ((float)xb[i] / coeff[i][1]));
                    glVertex2d(right, ((float)(xb[i] - coeff[i][0] * right) / coeff[i][1]));
                }
                else if(((float)xb[i] / coeff[i][0])<0){
                    glVertex2f(((float)xb[i] / coeff[i][1]), 0);
                    glVertex2d(((float)(xb[i] - coeff[i][1] * top) / coeff[i][0]), top);
                }
                else{
                    glVertex2f(0, ((float)xb[i] / coeff[i][1]));
                    glVertex2f(((float)xb[i] / coeff[i][0]), 0);
                }
                axes_points[aptr][0] = ((float)xb[i] / coeff[i][0]);
                axes_points[aptr++][1] = 0;
                axes_points[aptr][0] = 0;
                axes_points[aptr++][1] = ((float)xb[i] / coeff[i][1]);
            }
        }
    glEnd();
}

void init(){
    glClearColor(51 / 255.0, 153 / 255.0, 255 / 255.0, 0);
    glMatrixMode(GL_PROJECTION);
    for(int i = 0;i<n;i++){
        if(coeff[i][0] != 0){
            minx = ((xb[i] / coeff[i][0])<minx)?(xb[i] / coeff[i][0]):minx;
            maxx = ((xb[i] / coeff[i][0])>maxx)?(xb[i] / coeff[i][0]):maxx;
        }
        if(coeff[i][1] != 0){
            miny = ((xb[i] / coeff[i][1])<miny)?(xb[i] / coeff[i][1]):miny;
            maxy = ((xb[i] / coeff[i][1])>maxy)?(xb[i] / coeff[i][1]):maxy;
        }
    }
    max = (maxx>maxy)?maxx:maxy;
    left = -1 * max;
    right = max;
    top = max;
    bottom = -1 * max;
    gluOrtho2D(left / 2, right, bottom / 2, top);
}

void frontPage(){
    char* collName = "JSS ACADEMY OF TECHNICAL EDUCATION BANGALORE";
    char* dept = "DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING";
    char* sent = "A MINI-PROJECT ON";
    char* topic = "GRAPHICAL METHOD TO SOLVE AN LPP";
    char* members[] = {"MEMBERS: 1. ARVIND R (1JS16CS018)","            2. D V RAGHOTHAM RAO(1JS16CS037)"};
    glColor3f(1, 1, 1);
    glRasterPos2f(left / 2 + max / 9.5, top - 1.5 * max/10.0);
	for(int i = 0;i<strlen(collName);i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, collName[i]);
	}
	glRasterPos2f(left / 2 + 3 * max / 10, top - 2.5 * max/10.0);
	for(int i = 0;i<strlen(dept);i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, dept[i]);
	}
	glRasterPos2f(left / 2 + 5 * max / 10, top - 4.5 * max/10.0);
	for(int i = 0;i<strlen(sent);i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, sent[i]);
	}
	glRasterPos2f(left / 2 + 3 * max / 10, top - 5.5 * max/10.0);
	for(int i = 0;i<strlen(topic);i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, topic[i]);
	}
	glRasterPos2f(left / 2 + 1.5 * max / 10, top - 8 * max/10.0);
	for(int i = 0;i<strlen(members[0]);i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, members[0][i]);
	}
	glRasterPos2f(left / 2 + 2.5 * max / 10, top - 9 * max/10.0);
	for(int i = 0;i<strlen(members[1]);i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, members[1][i]);
	}
	glRasterPos2f(left / 2 + 1.5 * max / 10, top - 11 * max/10.0);
	for(int i = 0;i<strlen("UNDER THE GUIDANCE OF: RASHMI B N");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "UNDER THE GUIDANCE OF: RASHMI B N"[i]);
	}
}

void key2(unsigned char ch, int x, int y){
    if(ch == 'h')
        front_pg = 2;
    else if(ch == 'g')
        front_pg = 4;
    else if(ch == 't')
        front_pg = 3;
    else if(front_pg  == 1)
        front_pg = 2;
    glutPostRedisplay();
}

void menu(int n){
    choice = n;
    display();
}

void help(){
    glClearColor(255/255.0, 102.0/255, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glRasterPos2f(0 + max / 5.0, top - 3 * max/10.0);
	for(int i = 0;i<strlen("HELP");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "HELP"[i]);
	}
	glRasterPos2f(left / 2 + max / 5.0, top - 4 * max/10.0);
	for(int i = 0;i<strlen("press h for help menu");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "press h for help menu"[i]);
	}
	glRasterPos2f(left / 2 + max / 5.0, top - 5 * max/10.0);
	for(int i = 0;i<strlen("press g to view graph");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "press g to view graph"[i]);
	}
	glRasterPos2f(left / 2 + max / 5.0, top - 6 * max/10.0);
	for(int i = 0;i<strlen("press t to view table");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "press t to view table"[i]);
	}
	glRasterPos2f(left / 2 + max / 5.0, top - 8 * max/10.0);
	for(int i = 0;i<strlen("Use right click to view the graph as a whole or its individual items");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Use right click to view the graph as a whole or its individual items"[i]);
	}
	glRasterPos2f(left / 2 + max / 5.0, top - 9 * max/10.0);
	for(int i = 0;i<strlen(" - You can view only the lines, or");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, " - You can view only the lines, or"[i]);
	}
	glRasterPos2f(left / 2 + max / 5.0, top - 10 * max/10.0);
	for(int i = 0;i<strlen(" - You can view only the feasible region, or");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, " - You can view only the feasible region, or"[i]);
	}
	glRasterPos2f(left / 2 + max / 5.0, top - 11 * max/10.0);
	for(int i = 0;i<strlen(" - You can view all the three");i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, " - You can view all the three"[i]);
	}
}

void table(){
        glClearColor(0/255.0, 153/255.0, 153/255.0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1, 1, 1);
        glRasterPos2f(0 + max / 5.0, top - 3 * max/10.0);
        float w = 4;
        for(int i = 0;i<strlen("TABLE");i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "TABLE"[i]);
        }
        glRasterPos2f(0 - max/7.5, top - w * max/10.0);
        for(int i = 0;i<strlen("Z");i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Z"[i]);
        }
        glRasterPos2f(0 + max/4.5, top - w * max/10.0);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');
        glRasterPos2f(0 + max/1.9, top - w * max/10.0);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');
        w = 5;
        for(int i = 0;i<=frptr;i++){
            if(i == 0 || (feasible_points[i][0]!=feasible_points[i-1][0])){
                int X = (cost[0] * feasible_points[i][0] + cost[1] * feasible_points[i][1]);;
                int ipart = X;
                float fp = X - ipart;
                int fpart = fp*100;
                char str[10];
                itoa(ipart, str, 10);
                glRasterPos2f(0.0 - max/5.0, top - w * max/10.0);
                for(int i = 0;i<strlen(str);i++){
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
                }
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '.');
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
                itoa(fpart, str, 10);
                for(int i = 0;i<strlen(str);i++){
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
                }

                glRasterPos2f(0.0 + max/5.0, top - w * max/10.0);
                X = feasible_points[i][0];
                ipart = X;
                fp = X - ipart;
                fpart = fp*100;
                str[10];
                itoa(ipart, str, 10);
                for(int i = 0;i<strlen(str);i++){
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
                }
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '.');
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
                itoa(fpart, str, 10);
                for(int i = 0;i<strlen(str);i++){
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
                }

                glRasterPos2f(0.0 + max / 2.0, top - w * max/10.0);
                X = feasible_points[i][1];
                ipart = X;
                fp = X - ipart;
                fpart = fp*100;
                str[10];
                itoa(ipart, str, 10);
                for(int i = 0;i<strlen(str);i++){
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
                }
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '.');
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
                itoa(fpart, str, 10);
                for(int i = 0;i<strlen(str);i++){
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
                }
                w++;
            }
        }
}

void drawResult(float Z, float X, float Y, int min){
    int ipart = Z / 1;
    float fp = Z - ipart;
    int fpart = fp * 1000;
    glColor3f(0, 0, 0);
    glRasterPos2f(left / 2 + max / 9.5, 0 - 1.5 * max/10.0);
    char* Zee;
    Zee = (min == 1)?"Zmin = ":"Zmax = ";
    for(int i = 0;i<strlen(Zee);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Zee[i]);
    }
    char str[10];
    itoa(ipart, str, 10);
    for(int i = 0;i<strlen(str);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '.');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    itoa(fpart, str, 10);
    for(int i = 0;i<strlen(str);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glRasterPos2f(left / 2 + max / 9.5, 0 - 2.0 * max/10.0);
    for(int i = 0;i<strlen("When: ");i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "When: "[i]);
    }
    glRasterPos2f(left / 2 + max / 9.5, 0 - 2.5 * max/10.0);
    for(int i = 0;i<strlen("X = ");i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "X = "[i]);
    }
    ipart = X / 1;
    fp = X - ipart;
    fpart = fp*100;
    itoa(ipart, str, 10);
    for(int i = 0;i<strlen(str);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '.');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    itoa(fpart, str, 10);
    for(int i = 0;i<strlen(str);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glRasterPos2f(left / 2 + max / 9.5, 0 - 3.0 * max/10.0);
    for(int i = 0;i<strlen("Y = ");i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Y = "[i]);
    }
    ipart = Y / 1;
    fp = Y - ipart;
    fpart = fp*100;
    itoa(ipart, str, 10);
    for(int i = 0;i<strlen(str);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '.');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    itoa(fpart, str, 10);
    for(int i = 0;i<strlen(str);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glRasterPos2f(left / 2 + max / 9.5, 0 - 2.0 * max/10.0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    if(front_pg == 1)
        frontPage();
    else if(front_pg == 2)
        help();
    else if(front_pg == 3)
        table();
    else{
        glClearColor( 217 / 255.0, 217 / 255.0, 242 / 255.0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3d(0, 0, 0);
        drawAxes();
        glLineWidth(1);
        switch(choice){
        case 0: plotEquations();break;
        case 1: drawFeasibleRegion();
                getOptimalSolutions();
                drawResult(Zmax[0], feasible_points[(int)Zmax[1]][0],feasible_points[(int)Zmax[1]][1], 0);
                break;
        case 2: plotEquations();drawFeasibleRegion();
                getOptimalSolutions();
                drawResult(Zmax[0], feasible_points[(int)Zmax[1]][0],feasible_points[(int)Zmax[1]][1], 0);
                break;
        }
    }
    glFlush();
}

int main(int args, char** argv){
    char obj_fun[15];
    printf("Enter the Objective Function:\n");
    scanf("%s", obj_fun);
    getCoefficient(obj_fun, cost);
    printf("Enter the no of constraints:\n");
    scanf("%d", &n);
    printf("Enter the constraints\n");
    for(int i = 0;i<n;i++){
        char constr[15];
        scanf("%s", constr);
        getCoefficient(constr, coeff[i]);
        int j = 0;
        for(j = strlen(constr) - 1;j>=0;j--){
            if(constr[j] == '-')
                xb[i] *= -1;
            if(!isdigit(constr[j]))
                break;
            xb[i] += ceil(((pow(10, strlen(constr) - 1 - j)) * (constr[j] - '0')));
        }
        if(constr[j-1] == '<')
            inequality[i] = 'l';
        else if(constr[j-1] == '>')
            inequality[i] = 'g';
        else
            inequality[i] = 'e';
    }
    glutInit(&args, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Solution");
    init();
    glutCreateMenu(menu);
    glutAddMenuEntry("Show lines", 0);
    glutAddMenuEntry("Show Feasible Region", 1);
    glutAddMenuEntry("Show all", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutDisplayFunc(display);
    glutKeyboardFunc(key2);
    glutMainLoop();
    getOptimalSolutions();
    return 0;
}
