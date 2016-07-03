#include <windows.h>
#include <gl\gl.h>
#include <gl\glut.h>
#include <bits/stdc++.h>
using namespace std;

/**************************************************************************************************/
/**************************************************************************************************/

#define MAX 100
#define CELL 900
#define WALL 1
#define PATH 0

int current_window ;
bool flag = false ;
float ux , uy, uz ;
float vx , vy, vz ;
char gameModeString[40] = "640x480";
char currentMode[80];
int w = 640, h = 512 ;
int mzsize ;
int myMaze[MAX][MAX] ;
int ind ;
static GLubyte image[10][2048][1768][4];
static GLubyte linearImage[10][2048*1768*4];
unsigned int imagewidth=0, imageheight=0;
unsigned long fileSize;
static GLuint texName[10];
bool edge ;
float ang = 0.0;
float ang2 = 0.0;
float ang3 = 0.0;
float X = -10;
float Y = -20;
float Z = 0.0 ;
int r = 1;
int mv = 0 ;
char quote[680];
float spang[2] ;
bool vis[MAX][MAX] ;
bool flrvis ;
int lght = false ;

void reset(){
    memset(vis,0,sizeof(vis)) ;
    flrvis = false ;
    lght = false ;
    ang = 0.0;
    ang2 = 60.0;
    ang3= 0.0;
    spang[0] = 0.0 ;
    spang[1] = 0.0 ;
//    X = -10;
//    Y = -20;
    Z = -57.5 ;
    X = -9 ; Y = 11 ;
    //Z = -5 ;
    mv = 0 ;
    edge = false ;
//    flag = false ;
    ux = 0.0 , uy = 0.0 , uz = 0.0  ;
    vx  = 0.0 , vy  = 0.0 , vz = 0.0  ;
}

void InitGL();
void setOrthographicProjection() ;
void restorePerspectiveProjection() ;

void init_maze(int maze[MAX][MAX]);
void maze_generator(int indeks, int maze[MAX][MAX], int backtrack_x[CELL], int bactrack_y[CELL], int x, int y, int n, int visited);
void print_maze(int maze[MAX][MAX], int maze_size);
int is_closed(int maze[MAX][MAX], int x, int y);

void MyFlor(float x1, float x2, float y1, float z1, float z2) ;
void MyQuad(float x1, float x2, float y1, float y2, float z1, float z2) ;
void renderBitmapString(float x, float y, float z, char *string) ;
void display(void) ;
void keyboard(unsigned char key, int x, int y);
void Resize(GLsizei width, GLsizei height) ;
void convertToLinearArray() ;
int loadImage(char *f_name,int index) ;

void InitGL() ;
void mouseButton(int button, int state, int x, int y) ;
void processSpecialKeys(int key, int x, int y) ;
/*****************************************************************/
/// MAZE CREATE
void MyMazeGen(){
    srand((unsigned)time(NULL));
    int size;
    int indeks = 0;
    printf("input  (3 ~ 35): ");
    if(mzsize){
        size = mzsize ;
    }
    else{
        scanf("%d", &size); mzsize = size;
    }
    printf("\n");
    int maze[MAX][MAX];
    int backtrack_x[CELL];
    int backtrack_y[CELL];

    init_maze(maze);

    backtrack_x[indeks] = 1;
    backtrack_y[indeks] = 1;

    maze_generator(indeks, maze, backtrack_x, backtrack_y, 1, 1, size, 1);
    print_maze(maze, size);

    //Z = -45 - 4.5*(mzsize - 10) ;
//    Z = - 3*(mzsize - 10) - 20 ;
//double arr1[] = {348.5,351,353.5,-5,-2.5,-1.5,0,1.5,2.5,4.5,5,5.5,5.5,6,7.5,7.5,8,8.5,9,9.5,10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,15.5,16,16.5,17} ;
//    ang = arr1[mzsize - 3] ;
//    ang2 = 21 ;
//    cout << mzsize << endl;
}

void init_maze(int maze[MAX][MAX]){
    for(int a = 0; a < MAX; a++){
        for(int b = 0; b < MAX; b++){
            if(a % 2 == 0 || b % 2 == 0)   maze[a][b] = 1 ;
            else   maze[a][b] = 0 ;
        }
    }
}

void maze_generator(int indeks, int maze[MAX][MAX], int backtrack_x[CELL], int backtrack_y[CELL], int x, int y, int n, int visited){
    if(visited < n * n){
        int neighbour_valid = -1;
        int neighbour_x[4];
        int neighbour_y[4];
        int step[4];

        int x_next;
        int y_next;

        if(x - 2 > 0 && is_closed(maze, x - 2, y))  // upside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x - 2;;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=1;
        }

        if(y - 2 > 0 && is_closed(maze, x, y - 2))  // leftside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y - 2;
            step[neighbour_valid]=2;
        }

        if(y + 2 < n * 2 + 1 && is_closed(maze, x, y + 2))  // rightside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y + 2;
            step[neighbour_valid]=3;

        }

        if(x + 2 < n * 2 + 1 && is_closed(maze, x + 2, y))  // downside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x+2;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=4;
        }

        if(neighbour_valid == -1)
        {
            // backtrack
            x_next = backtrack_x[indeks];
            y_next = backtrack_y[indeks];
            indeks--;
        }

        if(neighbour_valid!=-1)
        {
            int randomization = neighbour_valid + 1;
            int random = rand()%randomization;
            x_next = neighbour_x[random];
            y_next = neighbour_y[random];
            indeks++;
            backtrack_x[indeks] = x_next;
            backtrack_y[indeks] = y_next;

            int rstep = step[random];

            if(rstep == 1)
                maze[x_next+1][y_next] = PATH;
            else if(rstep == 2)
                maze[x_next][y_next + 1] = PATH;
            else if(rstep == 3)
                maze[x_next][y_next - 1] = PATH;
            else if(rstep == 4)
                maze[x_next - 1][y_next] = PATH;
            visited++;
        }

        maze_generator(indeks, maze, backtrack_x, backtrack_y, x_next, y_next, n, visited);
    }
}

void print_maze(int maze[MAX][MAX], int maze_size){
//    cout << mzsize << endl;
    vector < pair<int,int> > flr ; flr.clear() ;
    for(int a = 0; a < maze_size * 2 + 1; a++){
        for(int b = 0; b < maze_size * 2 + 1; b++){
            myMaze[a][b] = maze[a][b] ;
            if(maze[a][b] == WALL){
                printf("#");
            }
            else{
                flr.push_back(make_pair(a,b)) ;
                printf(" ");
            }
        }
        printf("\n");
    }
    int sz = flr.size() , k = 0 , dist = -10000 ;
    while(k < sz  ){
        int p = rand()%sz ;
        int q = rand()%sz ;
        int vxx = flr[q].first , vzz = flr[q].second ;
        int uxx = flr[p].first , uzz = flr[p].second ;
        int tmp = abs(vxx-uxx)+abs(vzz-uzz) ;
        if(tmp > dist){
            dist = tmp ;
            vx = flr[q].first ; vz = flr[q].second ;
            ux = flr[p].first ; uz = flr[p].second ;
        }
        k++ ;
    }
//    cout << ux << ' ' << uz << endl ;
//    cout << vx << ' ' << vz << endl ;
}

int is_closed(int maze[MAX][MAX], int x, int y){
    if(maze[x - 1][y]  == WALL
       && maze[x][y - 1] == WALL
       && maze[x][y + 1] == WALL
       && maze[x + 1][y] == WALL
    )
    return 1;
    return 0;
}

/*****************************************************************/
/// MAZE DESIGN
void MyFlor(float x1, float x2, float y1, float z1, float z2){
	glBindTexture(GL_TEXTURE_2D, texName[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    if(flrvis)    glBindTexture(GL_TEXTURE_2D, texName[ind-2]);
glBegin(GL_POLYGON);
	glNormal3f( 0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(x2, y1, z1);
	glTexCoord2f(0.0, 1.0); glVertex3f(x1, y1, z1);
	glTexCoord2f(1.0, 1.0); glVertex3f(x1, y1, z2);
	glTexCoord2f(1.0, 0.0); glVertex3f(x2, y1, z2);
glEnd();
}
void MyQuad(float x1, float x2, float y1, float y2, float z1, float z2){
	glBindTexture(GL_TEXTURE_2D, texName[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBegin(GL_POLYGON);
    glNormal3f( 0.0, 0.0, 1.0);
 	glTexCoord2f(0.0, 0.0); glVertex3f(x1, y1, z1);
	glTexCoord2f(0.0, 1.0); glVertex3f(x2, y1, z1);
	glTexCoord2f(1.0, 1.0); glVertex3f(x2, y2, z1);
	glTexCoord2f(1.0, 0.0); glVertex3f(x1, y2, z1);
glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBegin(GL_POLYGON);
    glNormal3f( 1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(x1, y1, z2);
	glTexCoord2f(0.0, 1.0); glVertex3f(x2, y1, z2);
	glTexCoord2f(1.0, 1.0); glVertex3f(x2, y2, z2);
	glTexCoord2f(1.0, 0.0); glVertex3f(x1, y2, z2);
glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[3]);
    if(edge)    glBindTexture(GL_TEXTURE_2D, texName[ind-1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBegin(GL_POLYGON);
    glNormal3f( 0.0, 1.0, 1.0);

	glTexCoord2f(0.0, 0.0); glVertex3f(x1, y2, z1);
	glTexCoord2f(0.0, 1.0);	glVertex3f(x2, y2, z1);
	glTexCoord2f(1.0, 1.0); glVertex3f(x2, y2, z2);
    glTexCoord2f(1.0, 0.0);	glVertex3f(x1, y2, z2);
glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[4]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBegin(GL_POLYGON);
    glNormal3f( 1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(x1, y1, z1);
	glTexCoord2f(0.0, 1.0); glVertex3f(x1, y2, z1);
	glTexCoord2f(1.0, 1.0); glVertex3f(x1, y2, z2);
	glTexCoord2f(1.0, 0.0); glVertex3f(x1, y1, z2);
glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[5]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBegin(GL_POLYGON);
    glNormal3f( 0.5, 0.5, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(x2, y1, z1);
	glTexCoord2f(0.0, 1.0); glVertex3f(x2, y2, z1);
	glTexCoord2f(1.0, 1.0); glVertex3f(x2, y2, z2);
	glTexCoord2f(1.0, 0.0); glVertex3f(x2, y1, z2);
glEnd();
}

/// TEXT WRITING
void renderBitmapString(float x, float y, float z, int id, char *string) {
	char *c;
    glRasterPos3f(x, y, z);
	for (c=string; *c != '\0'; c++) {
        if(id==1) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
        else if(id==2)  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}


/// DISPLAY FUNCTION
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE ) ;

    GLUquadricObj *qObj = gluNewQuadric();
    gluQuadricNormals(qObj, GLU_SMOOTH);
    gluQuadricTexture(qObj, GL_TRUE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    cout << lght%2 <<' ' ;
    cout << X << ' ' << Y << ' ' << Z <<' ' << ang << ' ' << ang2 << ' ' << ang3 << endl;
    cout << ux << ' ' << uy << ' ' << uz << endl;

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    //glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

    glEnable (GL_BLEND);
//
glPushMatrix();
//    glRotatef(ang,0.0,1.0,0.0);
    //glRotatef(ang2,1.0,0.0,0.0);
//    glRotatef(ang3,0.0,0.0,1.0);

    const GLfloat light_position[] = { mzsize , 0, mzsize, 0.0f };
    const GLfloat light_position1[] = { ux+2 , 0, uz+2, 1.0f };
    GLfloat cyan[] = {0.f, 80.0f, 80.0f, 1.f};
    GLfloat orange[] = {100,69.0*100.0/255.0, 0, 1.f};
    GLfloat colorBlack[]  = {0.0f,0.0f,0.0f,1.0f};
    GLfloat colorWhite[]  = {100.0f,100.0f,100.0f,1.0f};
    GLfloat colorOliveGreen[]  = {176.0*100/255.0,196.0*100.0/255.0, 222.0*100/255.0, 1.f};
    GLfloat colorGreen[]  = {0.0f,100.0f,0.0f,1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorWhite);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colorWhite);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 130.0f);

    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f );
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION , 0.0f );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION , 0.0002 );


    if(lght%5 == 0){
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        //glLightfv( GL_LIGHT0, GL_SPECULAR, orange );
    }
    else if(lght%5 == 1){
        glDisable(GL_LIGHT0);
        glEnable( GL_LIGHT1 );
        glLightfv( GL_LIGHT1, GL_POSITION, light_position1 );
        glLightfv( GL_LIGHT1, GL_SPECULAR, cyan );
    }
    else if(lght%5 == 2){
        glDisable(GL_LIGHT0);
        glEnable( GL_LIGHT1 );
        glLightfv( GL_LIGHT1, GL_POSITION, light_position1 );
        glLightfv( GL_LIGHT1, GL_SPECULAR, colorGreen );
    }
    else if(lght%5 == 3){
        glDisable(GL_LIGHT0);
        glEnable( GL_LIGHT1 );
        glLightfv( GL_LIGHT1, GL_POSITION, light_position1 );
        glLightfv( GL_LIGHT1, GL_SPECULAR, orange );
    }
    else if(lght%5 == 4){
        glDisable(GL_LIGHT0);
        glEnable( GL_LIGHT1 );
        glLightfv( GL_LIGHT1, GL_POSITION, light_position1 );
        glLightfv( GL_LIGHT1, GL_SPECULAR, colorOliveGreen );
    }
glPopMatrix();

    glTranslatef(X , Y  , Z );

    glRotatef(ang,0.0,1.0,0.0);
    glRotatef(ang2,1.0,0.0,0.0);
    glRotatef(ang3,0.0,0.0,1.0);

    //gluLookAt(10,10,10,0,0,0,0,1,0) ;

glEnable(GL_TEXTURE_2D);

vis[(int)ux][(int)uz] = true ;

glPushMatrix();
    for(int i = -1 ; i < mzsize*2+2; i++){
        for(int j = -1 ; j < mzsize*2+2; j++){
            flrvis = false ; MyFlor(i, (i + 1), 0, j, (j + 1));
        }
    }
    for(int i = 0 ; i < mzsize*2+1; i++){
        for(int j = 0 ; j < mzsize*2+1; j++){
            if(i == 0 || j == 0 || i == mzsize*2 || j == mzsize*2) edge = true;
            else    edge = false ;
            if(myMaze[i][j] == 1) MyQuad(i, (i + 1), 0, 1, j, (j + 1));
            else{
                flrvis = false ;
                if(vis[i][j])   flrvis = true ;
                MyFlor(i, (i + 1), 0, j, (j + 1));
            }
        }
    }
glPopMatrix();
glDisable(GL_TEXTURE_2D);


glEnable(GL_TEXTURE_2D);
glPushMatrix();
    glTranslatef(ux+0.5,uy+0.5,uz+0.5);
    vis[(int)ux][(int)uz] = true ;
    glNormal3f( 1.5, 1.5, 1.0);
    glBindTexture(GL_TEXTURE_2D, texName[6]);
    glRotatef(spang[0], 100, 100, 0) ;
    glRotatef(spang[1], 0, 100, 100) ;
    gluSphere(qObj, 0.5f, 100, 100);
    glColor3d(1,1,1);
glPopMatrix();
glDisable(GL_TEXTURE_2D);

glEnable(GL_TEXTURE_2D);
glPushMatrix();
    const double t = glutGet(GLUT_ELAPSED_TIME) / 250.0;
    const double a = t*90.0;
    glTranslatef(vx+0.5,vy+0.5,vz+0.5);
//    glRotated(60,1,0,0);
//    glRotated(a,0,0,1);
    glNormal3f( 1.5, 1.5, 1.0);
    glBindTexture(GL_TEXTURE_2D, texName[7]);
    gluSphere(qObj, 0.5f, 100, 100);
    glutWireCube(1);
    glColor3d(1,1,1);
glPopMatrix();
glDisable(GL_TEXTURE_2D);


//glPushMatrix();
//    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
//    const double a = t*90.0;
//    glTranslatef(mzsize,-5,mzsize);
//    glNormal3f( 1.5, 1.5, 1.0);
//    glRotated(60,1,0,0);
//    glRotated(a,0,0,1);
//    glutWireTorus(0.2,0.8,20,20);
//glPopMatrix();


glDisable(GL_LIGHTING);

        if(ux == vx && uy == vy && uz == vz){
            setOrthographicProjection();
            glPushMatrix();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glLoadIdentity();
                glColor3f(0,1,1) ;
                sprintf(quote, "YOU HAVE TAKEN %d MOVES.", mv) ;
                renderBitmapString(120,60,0,2,(char *)"CONGRATULATION! YOU HAVE SOLVED THE MAZE!");
                renderBitmapString(120,120,0,2, quote);
                renderBitmapString(120,180,0,2,(char *)"PRESS F2 TO NEXT LEVEL... ");
                renderBitmapString(120,240,0,2,(char *)"PRESS F1 TO PREVIOUS LEVEL... ");
                renderBitmapString(120,300,0,2,(char *)"PRESS ESC TO EXIT... ");
            glPopMatrix() ;
            restorePerspectiveProjection();
        }


    glFlush();
    glutSwapBuffers();
}

void Resize(GLsizei width, GLsizei height){
	if (height==0){
		height=1;
	}
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 640.0f / 480.0f, 0.1, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void convertToLinearArray(){
	int len;
	int index;
	for(index=0;index<ind;index++){
		len=0;
        for(unsigned int r=0; r<imageheight; r++){
			for(unsigned int c=0; c<imagewidth; c++){
				for(int k=2; k>=0;k--){
					linearImage[index][len++] = image[index][r][c][k];
				}
				linearImage[index][len++] = (GLubyte)255;
			}
        }
	}
}
int loadImage(char *f_name,int index){
	FILE *fp;
	unsigned char blueValue, redValue, greenValue;
	unsigned int r,c;
	if(!(fp = fopen(f_name, "rb"))){
		printf("File Open Error\n");
		return 0;
	}
	fseek(fp,2,0);
	fread(&fileSize,4,1,fp);
	fseek(fp,18,0);
	fread(&imagewidth,4,1,fp);
	fseek(fp,22,0);
	fread(&imageheight,4,1,fp);
	fseek(fp,54,0);
	for(r=0; r<imageheight; r++){
		for(c=0; c<imagewidth; c++){
			fread(&blueValue, sizeof(char), 1, fp);
			fread(&greenValue, sizeof(char), 1, fp);
			fread(&redValue , sizeof(char), 1, fp);

			image[index][r][c][0] = (GLubyte)blueValue;
			image[index][r][c][1] = (GLubyte)greenValue;
			image[index][r][c][2] = (GLubyte)redValue;
			image[index][r][c][3] = (GLubyte)255;
		}
	}
	fclose(fp);
	return 1;
}
/**********************************************************************/
void keyboard(unsigned char key, int x, int y){
	if(key == 27) exit(0);
    if((mzsize >= 35 || mzsize < 3) && key)    exit(0) ;
	if(key == 'l')  lght++ ;
	if(key == 'a')  X = X - 1 ;
	if(key == 'd')  X = X + 1;
	if(key == 's')  Y = Y - 1;
	if(key == 'w')  Y = Y + 1;
	if(key == 'z')  Z = Z - 2.5;
	if(key == 'c')  Z = Z + 2.5;
    if(key == 'Y')  ang += 2 ;
    if(key == 'y')  ang -= 2;
    if(key == 'X')  ang2 += 2;
    if(key == 'x')  ang2 -= 2;
    if(key == 'V')  ang3 += 2;
    if(key == 'v')  ang3 -= 2;
    glutPostRedisplay();
}
void processSpecialKeys(int key, int x, int y) {
    if((mzsize >= 35 || mzsize < 3) && key)    exit(0) ;
    if(key == GLUT_KEY_RIGHT){
        int kx = ux + 1 ;
        int kz = uz ;
        if(myMaze[kx][kz] == 0){
            mv++;
            spang[0] += 60.0 ;
            ux = kx ; uz = kz ;
        }
    }
    if(key == GLUT_KEY_LEFT){
        int kx = ux - 1 ;
        int kz = uz ;
        if(myMaze[kx][kz] == 0){
            mv++;
            spang[0] -= 60.0 ;
            ux = kx ; uz = kz ;
        }
    }
    if(key == GLUT_KEY_UP){
        int kx = ux ;
        int kz = uz - 1 ;
        if(myMaze[kx][kz] == 0){
            mv++;
            spang[1] -= 60.0 ;
            ux = kx ; uz = kz ;
        }
    }
    if(key == GLUT_KEY_DOWN){
        int kx = ux ;
        int kz = uz + 1 ;
        if(myMaze[kx][kz] == 0){
            mv++;
            spang[1] += 60.0 ;
            ux = kx ; uz = kz ;
        }
    }
    if(key == GLUT_KEY_F1){
        mzsize-- ;
        reset() ;
        MyMazeGen() ;
    }
    if(key == GLUT_KEY_F2){
        mzsize++ ;
        reset() ;
        MyMazeGen() ;
    }
    if(key == GLUT_KEY_F3){
			glutGameModeString("1024x768:32");
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
                glClearColor(49.0/255.0,79.0/255.0,79.0/255.0,1); //102-205-170
				sprintf(gameModeString,"1024x768:32");
				InitGL();
			}
			else{
				glutGameModeString(gameModeString);
			}
    }
    if(key == GLUT_KEY_F4){
			glutGameModeString("1280x1024:32");
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
                glClearColor(49.0/255.0,79.0/255.0,79.0/255.0,1); //102-205-170
				sprintf(gameModeString,"1280x1024:32");
				InitGL();
			}
			else{
				glutGameModeString(gameModeString);
			}
    }
    if(key == GLUT_KEY_F5){
			w = 640; h = 512;
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0) {
				glutLeaveGameMode();
                glutSetWindow(current_window);
			}
    }
    glutPostRedisplay();
}

void Update(int value) {
	glutPostRedisplay();
	glutTimerFunc(25, Update, 0);
}

/**********************************************************************/
int  main(int argc, char *argv[]){
    reset() ;
    MyMazeGen() ;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	char file_name[100];
	strcpy(file_name, "floor_grass.bmp");/// floor
	if(!loadImage(file_name,ind++)){
		printf("\n0File Error\n");
		return 0;
	}
	strcpy(file_name, "pic22 (7).bmp"); /// w
	if(!loadImage(file_name,ind++)){
		printf("\n1File Error\n");
		return 0;
	}
	strcpy(file_name, "pic22 (4).bmp"); /// s
	if(!loadImage(file_name,ind++)){
		printf("\n2File Error\n");
		return 0;
	}
    //strcpy(file_name, "top.bmp"); /// top
	strcpy(file_name, "top2.bmp"); /// top
	if(!loadImage(file_name,ind++)){
		printf("\n3File Error\n");
		return 0;
	}
	strcpy(file_name, "pic22 (5).bmp"); /// d
	if(!loadImage(file_name,ind++)){
		printf("\n4File Error\n");
		return 0;
	}
	strcpy(file_name, "pic22 (2).bmp"); /// a
	if(!loadImage(file_name,ind++)){
		printf("\n5File Error\n");
		return 0;
	}
	strcpy(file_name, "ball.bmp"); /// ball 1
	if(!loadImage(file_name,ind++)){
		printf("\n6File Error\n");
		return 0;
	}
	strcpy(file_name, "ball2.bmp"); /// ball 2
	if(!loadImage(file_name,ind++)){
		printf("\n7File Error\n");
		return 0;
	}
	strcpy(file_name, "floor_sub3.bmp");
	if(!loadImage(file_name,ind++)){
		printf("\n7File Error\n");
		return 0;
	}
	strcpy(file_name, "edge_top.bmp"); /// edge top
	if(!loadImage(file_name,ind++)){
		printf("\n8File Error\n");
		return 0;
	}
	glutInit(&argc,argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(w,h);
	glutCreateWindow("HELLO MAZE WORLD");

    current_window = glutGetWindow();

	InitGL();

    glClearColor(49.0/255.0,79.0/255.0,79.0/255.0,1); //102-205-170

    glutTimerFunc(250, Update, 0);
    glutIdleFunc(NULL);
	glutMainLoop();
	glutFullScreen() ;
    return 0;
}

void InitGL(){
	glutReshapeFunc(Resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(processSpecialKeys);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);

	convertToLinearArray();
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(ind, texName);
	for(int i = 0 ; i < ind ; i++){
        glBindTexture(GL_TEXTURE_2D, texName[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagewidth, imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE,linearImage[i]);
	}
}

void setOrthographicProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
