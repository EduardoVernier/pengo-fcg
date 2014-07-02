/// TRABALHO FINAL FUNDAMENTOS DE COMPUTAÇÃO GRÁFICA
/// GRUPO: ÁLISTER MACHADO E EDUARDO VERNIER
///        00220494          00218313


#include <windows.h>
#include "../include/collision.h"
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <cmath>
#include <ctime>
#include <cstring>
#include <iostream>
#include <gl/glut.h>
#include <string>
#include <chrono>
#include <algorithm>
/// STL INCLUSIONS
#include <vector>
#include <map>
#include <list>
using std::vector;
using std::map;
using std::list;

using namespace std::chrono;
//openal (sound lib)
#include "../include/al/alut.h"

//bitmap class to load bitmaps for textures
#include "../include/bitmap.h"

//handle for the al.obj model
//#include "ModelAl.h"

//handle generic obj models
#include "../include/3DObject.h"
#include "../include/Enemy.h"
#include "../include/Camera.h"
#include "../include/MovableBlock.h"

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")

#define PI 3.14159265

// sound stuff
#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

#define SMOOTH 0
#define SMOOTH_MATERIAL 1
#define SMOOTH_MATERIAL_TEXTURE 2
typedef struct
{
    GLubyte r, g, b, a;
} S_COLOR ;

#include "../include/ObjEnum.h"
OBJ_ENUM *sceneMatrix;

int maxTime = 120;

GLfloat cube[6][4][3] =
{
    // x = -0.5
    {
        { -0.5, -0.5, 0.5 }, { -0.5, -0.5, -0.5 }, { -0.5, 0.5, -0.5 }, { -0.5, 0.5, 0.5 }
    },
    // y = -0.5
    {
        { -0.5, -0.5, -0.5 }, { -0.5, -0.5, 0.5 }, { 0.5, -0.5, 0.5 }, { 0.5, -0.5, -0.5 }
    },
    // x = 0.5
    {
        { 0.5, -0.5, -0.5 }, { 0.5, -0.5, 0.5 }, { 0.5, 0.5, 0.5 }, { 0.5, 0.5, -0.5 }
    },
    // y = 0.5
    {
        { 0.5, 0.5, -0.5 }, { 0.5, 0.5, 0.5 }, { -0.5, 0.5, 0.5 }, { -0.5, 0.5, -0.5 }
    },
    // z = 0.5
    {
        { -0.5, -0.5, 0.5 }, { -0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 }, { 0.5, -0.5, 0.5 }
    },
    // z = -0.5
    {
        { -0.5, -0.5, -0.5 }, { 0.5, -0.5, -0.5 }, { 0.5, 0.5, -0.5 }, { -0.5, 0.5, -0.5 }
    }
};
/*
GLfloat cubeNormals[6][3] =
{
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, -1.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f },
    { 0.0f, 0.0f, 1.0f }
};*/
GLfloat cubeNormals[6][3] =
{
    { -1.0f,  0.0f,  0.0f },
    {  0.0f, -1.0f,  0.0f },
    {  1.0f,  0.0f,  0.0f },
    {  0.0f,  1.0f,  0.0f },
    {  0.0f,  0.0f,  1.0f },
    {  0.0f,  0.0f, -1.0f }
};

typedef struct textura {
        BITMAPINFO	*info;           /* Bitmap information */
        GLubyte	    *data;           /* Bitmap RGB pixels */
        GLubyte     *ptr;            /* Pointer into bit buffer */
        GLubyte	    *rgba;           /* RGBA pixel buffer */
        GLubyte	    *rgbaptr;        /* Pointer into RGBA buffer */
        GLubyte     temp;            /* Swapping variable */
        GLuint      texture;         /* Texture object */
        GLenum      type;            /* Texture type */
} Texture;
void mainInit();
void initTexture();
void initModel();
void initLight();
void enableFog();
void createGLUI();
void mainRender();
void mainCreateMenu();
void onMouseButton(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onGLUIEvent(int id);
void onWindowReshape(int x, int y);
void mainIdle();
int main(int argc, char **argv);
void setWindow();
void setViewport(GLint left, GLint right, GLint bottom, GLint top);
void updateState();
void renderFloor();
void updateCam();
void setTextureToOpengl(Texture&);
time_t startTime;
bool collides (float pengoX, float pengoZ);
int mapToMatrixCoordinates (float i);
CAMERA_TYPES nextCamera(CAMERA_TYPES curCamera);
void moveDatBlock();
void gameOver();

int nBlocks = 2;
float speed = 0.05f;

/**
Screen dimensions
*/
int windowWidth = 800;
int windowHeight = 640;

/**
Screen position
*/
int windowXPos = 100;
int windowYPos = 150;

int mainWindowId = 0;

double xOffset = -1.9;
double yOffset = -1.3;
int mouseLastX = 0;
int mouseLastY = 0;

float roty = 0.0f;
float rotx = 90.0f;

bool rightPressed = false;
bool leftPressed = false;
bool upPressed = false;
bool downPressed = false;

bool spacePressed = false;

float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;

float posX = 0.0f;
float posY = 0.0f;
float posZ = 2.0f;

/*
variavel auxiliar pra dar variação na altura do ponto de vista ao andar.
*/
float headPosAux = 0.0f;

float maxSpeed = 0.25f;


// more sound stuff (position, speed and orientation of the listener)
ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat listenerOri[]={0.0,0.0,1.0,
						0.0,1.0,0.0};

// now the position and speed of the sound source
ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

// buffers for openal stuff
ALuint  buffer[NUM_BUFFERS];
ALuint  source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];
ALsizei size,freq;
ALenum  format;
ALvoid  *data;



// parte de código extraído de "texture.c" por Michael Sweet (OpenGL SuperBible)
// texture buffers and stuff
int i;                       /* Looping var */
BITMAPINFO	*info;           /* Bitmap information */
BITMAPINFO  *iceTInfo;
GLubyte	    *bits;           /* Bitmap RGB pixels */
GLubyte     *ptr;            /* Pointer into bit buffer */
GLubyte	    *rgba;           /* RGBA pixel buffer */
GLubyte	    *rgbaptr;        /* Pointer into RGBA buffer */
GLubyte     temp;            /* Swapping variable */
GLenum      type;            /* Texture type */
GLuint      texture;         /* Texture object */
GLenum      iceTType;
GLuint      iceTexture;
GLubyte     *iceTRgba;
GLubyte     *iceBits;

int sceneWidth, sceneHeight;

bool crouched = false;
bool running = false;
bool jumping = false;
float jumpSpeed = 0.15;
float gravity = 0.004;
float heightLimit = 0.2;
float posYOffset = 0.2;

float backgroundColor[4] = {0.0, 0.0, 0.0, 1.0};

float light1Angle = 0.0f;

C3DObject ball, flower, pengo;
Texture chao, iceCube, stoneCube;
//CModelAl modelAL;
Camera pengoCamera, ceilingCamera, fpCamera;
Point3D pengoPosition;
const int planeSize = 24; // mexer nessa constante dá 7 anos de azar
//OBJ_ENUM collisionMatrix [planeSize][planeSize];

list<Enemy*> enemies;
list<MovableBlock*> blocks;
map<pair<int,int>, MovableBlock*> blocksMap;
map<pair<int,int>, Enemy*>        enemiesMap;
list<pair<time_t, MovableBlock*>> blocksToSpawn;

CAMERA_TYPES current_camera = THIRD_PERSON;
bool pengoDead = false;
bool hitFlag = false;
void setWindow() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	pengoCamera.set_eye(posX,posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)),posZ);
	pengoCamera.set_center(posX + sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -cos(roty*PI/180));
	pengoCamera.set_upvector(0.0, 1.0, 0.0);
    pengoCamera.callGluLookAt();
}
GLfloat eyeX, eyeY, eyeZ, cntrX, cntrY, cntrZ;

/**
Atualiza a posição e orientação da camera
*/
void updateCam() {

    pengoPosition.set_coords(posX, posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) , posZ);

    pengoCamera.set_eye(pengoPosition + 5 * Point3D(sin(roty*PI/180), -cos(rotx*PI/180), cos(roty*PI/180)));
    pengoCamera.set_center(pengoPosition);
    pengoCamera.set_upvector(0.0, 1.0, 0.0);
	// pengoCamera.callGluLookAt();

    fpCamera.set_eye(pengoPosition);
    fpCamera.set_center(pengoPosition - 5 * Point3D(sin(roty*PI/180), -cos(rotx*PI/180), cos(roty*PI/180)));


	// atualiza a posição do listener e da origen do som, são as mesmas da camera, já que os passos vem de onde o personagem está
	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;
	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;
}

void updateLights()
{
    light1Angle = light1Angle > 360.0f ? (light1Angle - 360.0f) + 0.08f : light1Angle + 0.08f;
}

void renderLights()
{

    GLfloat light_position[] = {posX, posY, posZ, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, pengoCamera.get_eye().as_opengl_vector());

    Point3D direction1 = pengoCamera.get_center() - pengoCamera.get_eye();

    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction1.as_opengl_vector());
    GLfloat light_position1[] = { -9.0f*cos(light1Angle), 3.0f, 9.0f * sin(light1Angle) , 0.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

}

void writeTextAt(int x, int y, std::string text)
{
    double matrix[16]; // Save the matrices
    double mvmatrix[16];
    glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
    glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
    glLoadIdentity(); // reset PROJECTION matrix to identity matrix
    glOrtho(0, windowWidth, 0, windowHeight, 0.0, 5.0); // orthographic perspective
    glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glLoadIdentity(); // reset it to identity matrix
    glPushMatrix(); // push current state of MODELVIEW matrix to stack
    glLoadIdentity(); // reset it again. (may not be required, but it my convention)
    glRasterPos2i(x, y); // raster position in 2D
    for(int i=0; i<text.size(); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)text.at(i)); // generation of characters in our text with 9 by 15 GLU font
    }
    glPopMatrix(); // get MODELVIEW matrix value from stack
    glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
    glLoadMatrixd(matrix); // reset
    glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW
    glLoadMatrixd(mvmatrix);
}

void initLight() {
    glEnable(GL_LIGHTING );
	glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );
    glEnable( GL_LIGHT2 );
    glEnable( GL_LIGHT3 );
    // PARAMETERS FOR LIGHT0
    GLfloat light_ambient[] = { backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0 };
    GLfloat light_direction[] = { 0.0 , 0.0f, -1.0 };

    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 75.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 25.0f);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

    // PARAMETERS FOR LIGHT1
    GLfloat light_ambient1[] = { backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] };
    GLfloat light_diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular1[] = { 0.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position1[] = { -9.0f, 3.0f, 0.0f, 0.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    GLfloat light_ambient2[] = { backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] };
    GLfloat light_diffuse2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position2[] = { -12.0f, 5.0f, -12.0f, 0.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    GLfloat light_ambient3[] = { backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] };
    GLfloat light_diffuse3[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular3[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position3[] = { 12.0f, 5.0f, 12.0f, 0.0f };
}

void setViewport(GLint left, GLint right, GLint bottom, GLint top) {
	glViewport(left, bottom, right - left, top - bottom);
}

void initEnemies()
{
    for (int i = 0; i < 24; ++i)
    {
        for (int j = 0; j < 24; ++j)
        {
            OBJ_ENUM value = sceneMatrix[i*24 + j];
            if (value == ENEMY)
            {
                Enemy *x = new Enemy(make_pair(i,j), make_pair((float)i-12.0+0.5, (float)j-12.0+0.5));
                enemies.push_back(x);
                enemiesMap.insert(make_pair(make_pair(i,j), x));

            }
            else if (value == ICECUBE)
            {
                MovableBlock *m = new MovableBlock(make_pair(i,j), make_pair((float)i-12.0+0.5, (float)j-12.0+0.5));
                blocksMap.insert(make_pair(make_pair(i,j), m));
                blocks.push_back(m);
            }
            /*
            else if (value == ITEM_BLOCK_CREATION)
            {
                MovableBlock *m = new MovableBlock(make_pair(i,j), make_pair((float)i-12.0+0.5, (float)j-12.0+0.5));
                m->insert_item(NBLOCKS);
                blocksMap.insert(make_pair(make_pair(i,j), m));
                blocks.push_back(m);
                sceneMatrix[i*24+j] = ICECUBE;
            }
            else if (value == ITEM_PLAYER_SPEED)
            {
                MovableBlock *m = new MovableBlock(make_pair(i,j), make_pair((float)i-12.0+0.5, (float)j-12.0+0.5));
                m->insert_item(SPEED);
                blocksMap.insert(make_pair(make_pair(i,j),m));
                blocks.push_back(m);
                sceneMatrix[i*24+j] = ICECUBE;
            }
            */
        }
    }
    vector<MovableBlock*> v(blocks.begin(), blocks.end());
    std::shuffle(v.begin(), v.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    int i = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        if (i/4)
        {
            (*it)->insert_item(SPEED);
        }
        else (*it)->insert_item(NBLOCKS);
        ++i;
        if (i == 8) break;
    }
}

/**
Initialize
*/
void mainInit() {
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);

    ceilingCamera.set_eye(0.0, 32.0, 0.0);
    ceilingCamera.set_center(0.0, -1.0, 0.0);
    ceilingCamera.set_upvector(1.0, 0.0, 0.0);
	// habilita remocao de faces ocultas

	glFrontFace (GL_CCW);

    glEnable(GL_CULL_FACE);

	// habilita o z-buffer
	glEnable(GL_DEPTH_TEST);


    initTexture();

    initEnemies();

	initModel();

	initLight();

	enableFog();

	printf("w - andar \n");
	printf("s - ir pra tras \n");
	printf("mouse - direcao \n");
	printf("r - correr \n");
	printf("c - abaixar \n");
	printf("espaco - pular \n");

}

void initModel() {
	printf("Loading models.. \n");
	ball.Init();
	ball.Load("ball.obj");
	pengo.Init();
	pengo.Load("penguin.obj");
	flower.Init();
	flower.Load("BR_Charizard.obj");
	//modelAL = CModelAl();
	//modelAL.Init();
	printf("Models ok. \n \n \n");
}

/**
Initialize the texture using the library bitmap
*/
void initTexture(void)
{
    printf ("\nLoading texture..\n");
    // Load a texture object (256x256 true color)
    bits = LoadDIBitmap("tiledbronze.bmp", &(chao.info));
    if (bits == (GLubyte *)0) {
		printf ("Error loading texture!\n\n");
		return;
	}
    // Figure out the type of texture
    if (chao.info->bmiHeader.biHeight == 1)
      chao.type = GL_TEXTURE_1D;
    else
      chao.type = GL_TEXTURE_2D;
    // Create and bind a texture object
    glGenTextures(1, &(chao.texture));
	glBindTexture(chao.type, chao.texture);

    // Create an RGBA image
    chao.rgba = (GLubyte *)malloc(chao.info->bmiHeader.biWidth * chao.info->bmiHeader.biHeight * 4);

    i = chao.info->bmiHeader.biWidth * chao.info->bmiHeader.biHeight;
    for( chao.rgbaptr = chao.rgba, chao.ptr = bits;  i > 0; i--, chao.rgbaptr += 4, chao.ptr += 3)
    {
            chao.rgbaptr[0] = chao.ptr[2];     // windows BMP = BGR
            chao.rgbaptr[1] = chao.ptr[1];
            chao.rgbaptr[2] = chao.ptr[0];
            chao.rgbaptr[3] = 255;
    }
    setTextureToOpengl(chao);
    printf("\nLoading ice cube texture...\n");

    iceBits = LoadDIBitmap("icecube.bmp", &(iceCube.info));
    if (iceBits == (GLubyte*) 0) {
        printf("Error loading ice cube texture!\n\n");
        return;
    }
    if (iceCube.info->bmiHeader.biHeight == 1)
        iceCube.type = GL_TEXTURE_1D;
    else iceCube.type = GL_TEXTURE_2D;

    glGenTextures(1, &(iceCube.texture));
    glBindTexture(iceCube.type, iceCube.texture);
    int iceLength = (iceCube.info->bmiHeader.biWidth) * iceCube.info->bmiHeader.biHeight;
    iceCube.rgba = (GLubyte *) malloc(iceLength * 4);

    for (iceCube.rgbaptr = iceCube.rgba, iceCube.ptr = iceBits; iceLength > 0; --iceLength, iceCube.rgbaptr += 4, iceCube.ptr += 3)
    {
        iceCube.rgbaptr[0] = iceCube.ptr[2];
        iceCube.rgbaptr[1] = iceCube.ptr[1];
        iceCube.rgbaptr[2] = iceCube.ptr[0];
        iceCube.rgbaptr[3] = 255;
    }
    setTextureToOpengl(iceCube);

    free(iceBits);
    iceBits = LoadDIBitmap("stones.bmp", &(stoneCube.info));
    if (iceBits == (GLubyte*) 0) {
        printf("Error loading stone cube texture!\n\n");
        return;
    }
    stoneCube.type = GL_TEXTURE_2D;

    glGenTextures(1, &(stoneCube.texture));
    glBindTexture(stoneCube.type, stoneCube.texture);
    int stoneLength = stoneCube.info->bmiHeader.biWidth * stoneCube.info->bmiHeader.biHeight;
    stoneCube.rgba = (GLubyte*) malloc(stoneLength * 4);

    for (stoneCube.rgbaptr = stoneCube.rgba, stoneCube.ptr = iceBits; stoneLength > 0; --stoneLength, stoneCube.rgbaptr += 4, stoneCube.ptr += 3)
    {
        stoneCube.rgbaptr[0] = stoneCube.ptr[2];
        stoneCube.rgbaptr[1] = stoneCube.ptr[1];
        stoneCube.rgbaptr[2] = stoneCube.ptr[0];
        stoneCube.rgbaptr[3] = 255;
    }

    setTextureToOpengl(stoneCube);

    free(iceBits);

    BITMAPINFO *sceneInfo = (BITMAPINFO*) malloc(sizeof(BITMAPINFO));
    GLubyte *sceneBmp = LoadDIBitmap("scene.bmp", &sceneInfo);
    if (sceneBmp == (GLubyte*) NULL) {
        printf("Could not load scene.bmp, please make sure it exists.");
        return;
    }
    printf("Recovering scene information from scene.bmp. The mapping is:\n\
           Red pixels are Balls\n\
           Green pixels are Flowers\n\
           Blue pixels are Dolphins\n\
           Black pixels (or anything else) are empty spaces.\n");
    int k = sceneInfo->bmiHeader.biWidth * sceneInfo->bmiHeader.biHeight;
    // save the scene's height and width for later positioning
    sceneHeight = sceneInfo->bmiHeader.biHeight;
    sceneWidth = sceneInfo->bmiHeader.biWidth;
    // alloc the matrix that contains what goes where
    sceneMatrix = (OBJ_ENUM*) malloc(k * sizeof(OBJ_ENUM));
    int aux = 0;
    int length = (sceneInfo->bmiHeader.biWidth * 3 + 3) & ~3;
    int x;

    for (int y = 0; y < (sceneInfo)->bmiHeader.biHeight; y ++)
        for (GLubyte *ptr = sceneBmp + y * length, x = sceneInfo->bmiHeader.biWidth;
            x > 0;
	     x --, ptr += 3)
        {
            const GLubyte r = ptr[2];
            const GLubyte g = ptr[1];
            const GLubyte b = ptr[0];
            long cor = (r << 16) + (g << 8) + b;
            if (cor == 0x000000)
                sceneMatrix[aux] = NOTHING;
            else if (cor == 0xFF0000)
                sceneMatrix[aux] = STONECUBE;
            else if (cor == 0x0000FF)
                sceneMatrix[aux] = ENEMY;
            else if (cor == 0x00FF00)
                sceneMatrix[aux] = ICECUBE;
            else if (cor == 0xFFFF00)
                sceneMatrix[aux] = ITEM_BLOCK_CREATION;
            else if (cor == 0x00FFFF)
                sceneMatrix[aux] = ITEM_PLAYER_SPEED;
            else sceneMatrix[aux] = NOTHING;
            aux++;
	    }


    free(sceneInfo);
    free(sceneBmp);


}

/**
Recovers the texture already initialized in initTexture(), setting it to opengl
*/
void setTextureToOpengl(Texture &tex)
{
    // Create and bind a texture object
    glGenTextures(1, &(tex.texture));
	glBindTexture(tex.type, tex.texture);

	// Set texture parameters
	glTexParameteri(tex.type, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(tex.type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(tex.type, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(tex.type, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(tex.type, 0, 4, tex.info->bmiHeader.biWidth, tex.info->bmiHeader.biHeight,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, tex.rgba );

    gluBuild2DMipmaps(tex.type, 4, tex.info->bmiHeader.biWidth, tex.info->bmiHeader.biHeight,
                      GL_RGBA, GL_UNSIGNED_BYTE, tex.rgba);
}

void enableFog(void)
{
    glEnable(GL_FOG);
    GLfloat color[] = {0.647, 0.94902, 0.9529, 1.0 };
    glFogfv(GL_FOG_COLOR, color);
    glFogf(GL_FOG_DENSITY, 0.05f);
    glFogi(GL_FOG_MODE, GL_EXP2);
}

void drawCube(float side, OBJ_ENUM cubeType)
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    if (cubeType == ICECUBE)
        glBindTexture(GL_TEXTURE_2D, iceCube.texture);
    else if (cubeType == STONECUBE)
        glBindTexture(GL_TEXTURE_2D, stoneCube.texture);
    else if (cubeType == NOTHING)
    {
        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0,1.0,1.0,0.5);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glPushMatrix();
    glScalef(side, side, side);
    for (int i = 5; i >= 0; --i)
    {
        glBegin(GL_QUADS);
        for (int k = 3; k >= 0; --k)
        {
            glTexCoord2i(k % 2, k / 2);
            glNormal3fv(cubeNormals[i]);
            glVertex3fv(cube[i][k]);
        }
        glEnd();
    }
    glPopMatrix();
    glDisable(iceTType);
}

void renderFloor() {
	// set things up to render the floor with the texture
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, iceCube.texture);
	glPushMatrix();


    glTranslatef(-(float)planeSize/2.0f, 0.0f, -(float)planeSize/2.0f);

	float textureScaleX = 10.0;
	float textureScaleY = 10.0;
    glColor4f(1.0f,1.0f,1.0f,1.0f);
    int xQuads = 40;
    int zQuads = 40;
    for (int i = 0; i < xQuads; i++) {
        for (int j = 0; j < zQuads; j++) {
            glBegin(GL_QUADS);
                glTexCoord2f(1.0f, 0.0f);   // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f(i * (float)planeSize/xQuads, 0.0f, (j+1) * (float)planeSize/zQuads);

                glTexCoord2f(0.0f, 0.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f((i+1) * (float)planeSize/xQuads, 0.0f, (j+1) * (float)planeSize/zQuads);

                glTexCoord2f(0.0f, 1.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f((i+1) * (float)planeSize/xQuads, 0.0f, j * (float)planeSize/zQuads);

                glTexCoord2f(1.0f, 1.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f(i * (float)planeSize/xQuads, 0.0f, j * (float)planeSize/zQuads);

            glEnd();
        }
    }

	glDisable(GL_TEXTURE_2D);


	glPopMatrix();
}
bool hasBeenDrawn(pair<time_t, MovableBlock*> x)
{
    time_t now;
    time(&now);
    return difftime(now, x.first) >= 2.0;
};

bool isNotValid(MovableBlock *b)
{
    return !b->is_valid();
};

bool isNotValidEnemy(Enemy* e)
{
    return !e->isValid();
}
void renderScene() {
	glClearColor(backgroundColor[0],backgroundColor[1],backgroundColor[2],backgroundColor[3]);
	  // limpar o depth buffer
	//glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    renderLights();
    sceneMatrix[((int)std::round(pengoPosition.getX()-0.5)+12)*24 + ((int)std::round(pengoPosition.getZ()-0.5)+12)] = PENGO;


    for (int i = 0; i < sceneHeight; ++i)
    {
        for (int j = 0; j < sceneWidth; ++j)
        {
            GLfloat x = i - (sceneHeight/2) + 0.5f;
            GLfloat y = 0.5f;
            GLfloat z = j - (sceneWidth/2) + 0.5f;


            GLint xAtMatrix = planeSize/2 + (int) floor(x) ;
            GLint zAtMatrix = planeSize/2 + (int) floor(z) ;

            glPushMatrix();
            glTranslatef(x, y ,z);
            float cubeSide =1; // numero magico é 1.8
            switch (sceneMatrix[i*sceneWidth + j])
            {
            //case PENGO:
             //   pengo.Draw(SMOOTH_MATERIAL_TEXTURE);
                //fillCollisionMatrix8 (xAtMatrix,zAtMatrix,PENGO);
                //collisionMatrix[xAtMatrix][zAtMatrix] = PENGO;
                break;
            case STONECUBE:
                drawCube(cubeSide, STONECUBE);
                //fillCollisionMatrix8 (xAtMatrix,zAtMatrix,STONECUBE);
                break;
            /*case ENEMY:
                // Enemies are currently upside down Pengos.
                //glRotatef(180,0.0, 0.0, 1.0);
                //pengo.Draw(SMOOTH_MATERIAL_TEXTURE);
                glTranslatef(0.0,0.4,0.0);
                glRotatef(90,1.0,0.0,0.0);

                flower.Draw(SMOOTH_MATERIAL_TEXTURE);
                fillCollisionMatrix8(xAtMatrix, zAtMatrix, ENEMY);
                break;*/
            case NOTHING:
            default:
                break;
            }
            glPopMatrix();
        }
    }

    for (pair<time_t, MovableBlock*> p : blocksToSpawn)
    {
        time_t now;
        time(&now);
        double diff = difftime(now, p.first);
        if (diff >= 2.0)
        {
            blocks.push_back(p.second);
            nBlocks++;
        }
        else
        {
            glPushMatrix();
            {

                glTranslatef(p.second->get_screen_pos().first, 0.5f, p.second->get_screen_pos().second);
                drawCube(1.0, NOTHING);

            }
            glPopMatrix();
        }
    }
    blocksToSpawn.remove_if(hasBeenDrawn);

    for (MovableBlock* b : blocks)
    {
        sceneMatrix[b->get_matrix_pos().first*24+b->get_matrix_pos().second] = NOTHING;
        blocksMap[b->get_matrix_pos()] = NULL;
        blocksMap.erase(b->get_matrix_pos());
        if (!b->is_valid()) continue;
        if (b->is_moving())
            b->move(sceneMatrix);
        if (b->is_valid())
        {
            sceneMatrix[b->get_matrix_pos().first*24+b->get_matrix_pos().second] = ICECUBE;
            blocksMap.insert(make_pair(b->get_matrix_pos(), b));
            glPushMatrix();
            glTranslatef(b->get_screen_pos().first, 0.5f, b->get_screen_pos().second);
            drawCube(1.0, ICECUBE);
            glPopMatrix();
        }

    }

    blocks.remove_if(isNotValid);

    for (Enemy* e : enemies)
    {
        if (e->isValid()){

            sceneMatrix[e->get_matrix_pos().first*24 + e->get_matrix_pos().second] = NOTHING;
            enemiesMap[e->get_matrix_pos()] = NULL;
            enemiesMap.erase(e->get_matrix_pos());
            e->move_me(sceneMatrix, 24, 24);
            if (e->isValid())
            {
                sceneMatrix[e->get_matrix_pos().first*24 + e->get_matrix_pos().second] = ENEMY;
                enemiesMap.insert(make_pair(e->get_matrix_pos(), e));

                glPushMatrix();
                glTranslatef(e->get_screen_pos().first, 0.6f, e->get_screen_pos().second);
                glRotatef(90,1.0,0.0,0.0);
                glScalef(0.75,0.75,0.75);
                flower.Draw(SMOOTH_MATERIAL_TEXTURE);
                glPopMatrix();
            }
        } else {
            sceneMatrix[e->get_matrix_pos().first*24 + e->get_matrix_pos().second] = NOTHING;
            delete (enemiesMap[e->get_matrix_pos()]);
            enemiesMap.erase(e->get_matrix_pos());
        }
    }

    enemies.remove_if(isNotValidEnemy);
    int pengoMX = (int)std::round(pengoPosition.getX() - 0.5) + 12;
    int pengoMZ = (int)std::round(pengoPosition.getZ() - 0.5) + 12;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = (i != 0 ? 0 : -1); j <= (i != 0 ? 0 : 1); j += (i == 0 ? 2 : 1))
        {
            int l = pengoMX + i;
            int k = pengoMZ + j;
            if (l < 0 || l >= 24 || k < 0 || k >= 24) continue;
            if (sceneMatrix[l*24+k] == ENEMY)
            {
                pengoDead = true;
                break;
            }
        }
        if (pengoDead) break;
    }
    if (!pengoDead){
    glPushMatrix();
    {
        glTranslatef(pengoPosition.getX(), 0.6f + pengoPosition.getY(), pengoPosition.getZ());
        glRotatef(roty - 180, 0.0, 0.1, 0.0);
        pengo.Draw(SMOOTH_MATERIAL_TEXTURE);
    }
    glPopMatrix();
    }
    else {
    glPushMatrix();
    {
        glTranslatef(pengoPosition.getX(), 0.0f + pengoPosition.getY(), pengoPosition.getZ());
        glRotatef(90, 1.0, 0.0, 0.0);
        pengo.Draw(SMOOTH_MATERIAL_TEXTURE);
    }
    glPopMatrix();
    }

    // sets the bmp file already loaded to the OpenGL parameters
    //setTextureToOpengl(chao);
	renderFloor();



	//modelAL.Translate(0.0f,1.0f,0.0f);
	//modelAL.Draw();
}

void gameOver(){
    pengoDead = true;
    std::string printMe ("GAME OVER");
    writeTextAt(10,10,printMe);//nao funciona essa bagaça
}

void updateState() {
    sceneMatrix[((int)std::round(pengoPosition.getX() -0.5)+12)*24 + (int)std::round(pengoPosition.getZ()-0.5)+12] = NOTHING;
if (!pengoDead)
	if (upPressed || downPressed || rightPressed || leftPressed ) {
		if (running) {
			speedX = speed * sin((roty-180)*PI/180) * 2;
            speedZ = -speed * cos((roty)*PI/180) * 2;
		} else {
			speedX = speed * sin((roty-180)*PI/180);
			speedZ = -speed * cos((roty)*PI/180);
		}

		// efeito de "sobe e desce" ao andar
		headPosAux += 8.5f;
		if (headPosAux > 180.0f) {
			headPosAux = 0.0f;
		}

        if (upPressed) {

            if (collides(posX + speedX, posZ + speedZ)==false){
                posX += speedX;
                posZ += speedZ;
            }
            else {
                if (collides(posX + speedX, posZ)==false){
                    posX += speedX;
                }
                else{
                    if (collides(posX, posZ + speedZ)==false){
                        posZ += speedZ;
                    }
                }
            }

        }
        if (downPressed) {

            if (collides(posX - speedX, posZ - speedZ)==false){
                posX -= speedX;
                posZ -= speedZ;
            }
            else {
                if (collides(posX - speedX, posZ)==false){
                    posX -= speedX;
                }
                else{
                    if (collides(posX, posZ - speedZ)==false){
                        posZ -= speedZ;
                    }
                }
            }
        }

        if (rightPressed){
            speedX = -speed * sin((roty-180)*PI/180+(3.14/2));
			speedZ = speed * cos((roty)*PI/180+(3.14/2));

            if (collides(posX + speedX, posZ + speedZ)==false){
                posX += speedX;
                posZ += speedZ;
            }
            else {
                if (collides(posX + speedX, posZ)==false){
                    posX += speedX;
                }
                else{
                    if (collides(posX, posZ + speedZ)==false){
                        posZ += speedZ;
                    }
                }
            }
        }
        if (leftPressed){
            speedX = -speed * sin((roty-180)*PI/180-(3.14/2));
			speedZ = speed * cos((roty)*PI/180-(3.14/2));

            if (collides(posX + speedX, posZ + speedZ)==false){
                posX += speedX;
                posZ += speedZ;
            }
            else {
                if (collides(posX + speedX, posZ)==false){
                    posX += speedX;
                }
                else{
                    if (collides(posX, posZ + speedZ)==false){
                        posZ += speedZ;
                    }
                }
            }

        }



	} else {
		// parou de andar, para com o efeito de "sobe e desce"
		headPosAux = fmod(headPosAux, 90) - 1 * headPosAux / 90;
		headPosAux -= 4.0f;
		if (headPosAux < 0.0f) {
			headPosAux = 0.0f;
		}
	}



	posY += speedY;
	if (posY < heightLimit) {
		posY = heightLimit;
		speedY = 0.0f;
		jumping = false;
	} else {
		speedY -= gravity;
	}

	if (crouched) {
		posYOffset -= 0.01;
		if (posYOffset < 0.1) {
			posYOffset = 0.1;
		}
	} else {
		posYOffset += 0.01;
		if (posYOffset > 0.2) {
			posYOffset = 0.2;
		}
	}

    int myRow = (int) (std::round(pengoPosition.getX()-0.5) + 12);
    int myCol = (int) (std::round(pengoPosition.getZ()-0.5) + 12);




}

void moveDatBlock(){
    int direction = round(std::abs(int(roty) % 360)/90.0);
    if (direction == 4)
        direction = 0;

    int x = (int) (std::round(pengoPosition.getX()-0.5) + 12);
    int z = (int) (std::round(pengoPosition.getZ()-0.5) + 12);

    int speedX, speedZ;
    switch (direction) {

    case 0:
        speedZ = -1;
        speedX = 0;
        break;
    case 1:
        speedX = 1;
        speedZ = 0;
        break;
    case 2:
        speedZ = 1;
        speedX = 0;
        break;
    case 3:
        speedX = -1;
        speedZ = 0;
        break;
    }
    if (sceneMatrix[(x+speedX)*24 + (z+speedZ)  ] == ICECUBE)
    {
        MovableBlock *b = blocksMap[make_pair(x+speedX,z+speedZ)];
        b->start_moving(speedX, speedZ);
    }
    else if (sceneMatrix[(x+2*speedX)*24+(z+2*speedZ)] == ICECUBE)
    {
        MovableBlock *b = blocksMap[make_pair(x+2*speedX,z+2*speedZ)];
        b->start_moving(speedX, speedZ);
    }

}

bool collides (float pengoX, float pengoZ){
    int direction = round(std::abs(int(roty) % 360)/45.0);
    if (direction == 8)
        direction = 0;

    if (downPressed)
        direction = (4+direction) % 8;
    if (leftPressed)
        direction = (6+direction) % 8;
    if (rightPressed)
        direction = (2+direction) % 8;
    int z = (int)std::round(pengoPosition.getZ() - 0.5) + 12;
    int x = (int)std::round(pengoPosition.getX() - 0.5) + 12;
    z = (int)(std::round(pengoZ - 0.5)) + 12;
    x = + ((int) std::round(pengoX - 0.5)+12);
    if (z <= 0 || z >= planeSize - 1 || x <= 0 || x >= planeSize - 1) return true;

    bool helper = false;
    /*for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            int l = i + x;
            int k = j + z;
            if (l < 0 || l >= 24 || k < 0 || k >= 0 || (i == 0 && j == 0)) continue;
            if (sceneMatrix[l*24+k] == ENEMY)
            {
                pengoDead = true;
                return false;
            }
        }
    }
    */
    switch (direction)
    {
    case 0:
        z--;
        return  sceneMatrix[(x-1)*24+z] != NOTHING ||
                sceneMatrix[x*24+z] != NOTHING ||
                sceneMatrix[(x+1)*24+z] != NOTHING;
    case 1:
        return  sceneMatrix[(x+1)*24+z-1] != NOTHING ||
                sceneMatrix[x*24+z-1] != NOTHING ||
                sceneMatrix[(x+1)*24+z] != NOTHING;
        break;
    case 2:
        x++;
        return  sceneMatrix[x*24+z-1] != NOTHING ||
                sceneMatrix[x*24+z] != NOTHING ||
                sceneMatrix[x*24+z+1] != NOTHING;
        break;
    case 3:
        return  sceneMatrix[(x+1)*24+z] != NOTHING ||
                sceneMatrix[(x+1)*24+z+1] != NOTHING ||
                sceneMatrix[x*24+z+1] != NOTHING;
        break;
    case 4:
        z++;
        return sceneMatrix[(x+1)*24+z] != NOTHING ||
                sceneMatrix[x*24+z] != NOTHING ||
                sceneMatrix[(x-1)*24+z] != NOTHING;
        break;
    case 5:
        return sceneMatrix[x*24+z+1] != NOTHING ||
                sceneMatrix[(x-1)*24+z+1] != NOTHING ||
                sceneMatrix[(x-1)*24+z] != NOTHING;
        break;
    case 6:
        x--;
        return  sceneMatrix[x*24+z-1] != NOTHING ||
                sceneMatrix[x*24+z] != NOTHING ||
                sceneMatrix[x*24+z+1] != NOTHING;
        break;
    case 7:
        return sceneMatrix[(x-1)*24 + z] != NOTHING ||
                sceneMatrix[(x-1)*24+z-1] != NOTHING ||
                sceneMatrix[x*24 + z-1] != NOTHING;
        break;

    }
}

int mapToMatrixCoordinates (float i){
    return planeSize/2 + (int) floor(i);
}




void getTimeString(int minutes, int seconds, char *out)
{
    sprintf(out, "%02d:%02d", minutes, seconds);
}
bool userWon = false;
/**
Render scene
*/
void mainRender() {

    sceneMatrix[(int)std::round(pengoPosition.getX()-0.5) + 12 + ((int)std::round(pengoPosition.getZ()-0.5)+12)*24] = NOTHING;
	updateState();
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_DEPTH_BUFFER_BIT);
	updateCam();
	switch (current_camera)
	{
    case THIRD_PERSON:
        pengoCamera.callGluLookAt();
        break;
    case FIRST_PERSON:
        fpCamera.callGluLookAt();
        break;
    case CEILING_VISION:
        ceilingCamera.callGluLookAt();
        glDisable(GL_FOG);
        break;
	}
	renderScene();

	time_t now;
	time(&now);

	int diff = maxTime - int(difftime(now, startTime));
    glDisable(GL_LIGHTING);
    char buffer[7];
    getTimeString(diff/60, diff % 60, buffer);
    std::string printMe = buffer;
    if (diff <= 10)
    {
        glColor3d(1.0,0.0,0.0);
    }
    else if (diff <= 0)
    {
        pengoDead = true;
    }
    else
    {
        glColor3d(1.0,1.0,1.0);
    }
    writeTextAt(0,0,printMe);
    glColor3d(1.0,0.0,0.0);
    if (userWon)
    {
        glColor3d(0.0,1.0,0.0);
        std::string youwon = "YOU WON!!!!";
        writeTextAt(10, 10, youwon);
    }
    else if (pengoDead) gameOver();
    glColor3d(1.0,1.0,1.0);
    char buffer2[256];
    sprintf(buffer2, "N blocos simultaneos: %d", nBlocks);
    printMe = buffer2;
    writeTextAt(0,windowHeight-20, printMe);
    sprintf(buffer2, "Velocidade: %d", (int)(speed/0.05));
    printMe = buffer2;
    writeTextAt(windowWidth/3, windowHeight-20, printMe);
    glEnable(GL_LIGHTING);

    glDisable(GL_FOG);
    setViewport((4*windowWidth)/5, windowWidth, (4*windowHeight)/5, windowHeight);

	//glViewport(windowWidth - 100, windowHeight -100, 100, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.5, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_DEPTH_BUFFER_BIT);

	ceilingCamera.set_eye(pengoPosition.getX(), ceilingCamera.get_eye().getY(), pengoPosition.getZ());
    ceilingCamera.set_center(pengoPosition.getX(), -1, pengoPosition.getZ());
    ceilingCamera.callGluLookAt();

    renderScene();
    updateLights();

    if (enemies.empty())
    {
        userWon = true;
    }

	glFlush();
    glEnable(GL_FOG);
	glutPostRedisplay();
	Sleep(30);
}

/**
Handles events from the mouse right button menu
*/
void mainHandleMouseRightButtonMenuEvent(int option) {
	switch (option) {
		case 1 :
			exit(0);
			break;
		default:
			break;
	}
}

/**
Create mouse button menu
*/
void mainCreateMenu() {
	glutCreateMenu(mainHandleMouseRightButtonMenuEvent);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

bool changingZoom = false;

/**
Mouse button event handler
*/
void onMouseButton(int button, int state, int x, int y) {
	printf("onMouseButton button: %d \n", button);
	if (button == GLUT_MIDDLE_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            changingZoom = true;
        }
        else changingZoom = false;
    }

    mouseLastX = x;
    mouseLastY = y;

	glutPostRedisplay();

}



/**
Mouse move while button pressed event handler
*/
void onMouseMove(int x, int y) {

    if (changingZoom)
    {
        Point3D lastEye = ceilingCamera.get_eye();
        const float dy = float(y - mouseLastY) / 5.0;
        float newY;
        newY = lastEye.getY() + dy <= 3.0 ? 3.0 : lastEye.getY() + dy;
        ceilingCamera.set_eye(lastEye.getX(), newY, lastEye.getZ());
    }
    mouseLastX = x;
	mouseLastY = y;
	glutPostRedisplay();

}

/**
Mouse move with no button pressed event handler
*/
void onMousePassiveMove(int x, int y) {

	roty -= (x - mouseLastX);

	rotx -= (y - mouseLastY);

	if (rotx < -128.0) {
		rotx = -128.0;
	}

	if (rotx > -90.0) {
		rotx = -90.0;
	}

	mouseLastX = x;
	mouseLastY = y;

	glutPostRedisplay();
}

/**
Key press event handler
*/
void onKeyDown(unsigned char key, int x, int y) {
	//printf("%d \n", key);
	switch (tolower(key)) {
		case 32: //space
            moveDatBlock();
			break;
		case 119: //w
			if (!upPressed) {
				alSourcePlay(source[0]);
			}
			upPressed = true;
			break;
		case 115: //s
			downPressed = true;
			break;
		case 97: //a
			leftPressed = true;
			break;
		case 100: //d
			rightPressed = true;
			break;
		case 99: //c
			crouched = true;
			break;
		case 114: //r
			running = true;
			break;
		default:
			break;
	}

	//glutPostRedisplay();
}

void spawnBlock()
{
    int x = (int)std::round(pengoPosition.getX()-0.5) + 12;
    int z = (int)std::round(pengoPosition.getZ()-0.5) + 12;
    int direction = round(std::abs(int(roty) % 360)/90.0);
    if (direction == 4)
        direction = 0;

    switch (direction)
    {
    case 0:
        z--;
        break;
    case 1:
        x++;
        break;
    case 2:
        z++;
        break;
    case 3:
        x--;
        break;
    }

    if (sceneMatrix[x*24+z] != NOTHING)
        return;
    else if(nBlocks > 0)
    {
        time_t t;
        time(&t);
        blocksToSpawn.push_front(make_pair(t,
                                           new MovableBlock(
                                            make_pair(x,z),
                                            make_pair((float)x-12.0+0.5, (float)z-12.0+0.5))));
        nBlocks--;
    }

}

/**
Key release event handler
*/
void onKeyUp(unsigned char key, int x, int y) {
	switch (tolower(key)) {
		case 32: //space
			spacePressed = false;
			break;
		case 119: //w
			if (upPressed) {
				alSourceStop(source[0]);
			}
			upPressed = false;
			break;
		case 115: //s
			downPressed = false;
			break;
		case 97: //a
			leftPressed = false;
			break;
		case 100: //d
			rightPressed = false;
			break;
		case 99: //c
			crouched = false;
			break;
		case 114: //r
			running = false;
			break;
		case 27:
			exit(0);
			break;
        case 'v':
            current_camera = nextCamera(current_camera);
            break;
        case 'e':
            spawnBlock();
            break;
		default:
			break;
	}

	//glutPostRedisplay();
}

void onWindowReshape(int x, int y) {
	windowWidth = x;
	windowHeight = y;
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);
}

/**
Glut idle funtion
*/
void mainIdle() {
	/**
	Set the active window before send an glutPostRedisplay call
	so it wont be accidently sent to the glui window
	*/

	glutSetWindow(mainWindowId);
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowXPos,windowYPos);

	/**
	Store main window id so that glui can send it redisplay events
	*/
	mainWindowId = glutCreateWindow("Pato Simulator 2014");

	glutDisplayFunc(mainRender);

	glutReshapeFunc(onWindowReshape);

	/**
	Register mouse events handlers
	*/
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMove);
	glutPassiveMotionFunc(onMousePassiveMove);

	/**
	Register keyboard events handlers
	*/

    glutGetModifiers();
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
    speed = 0.05;
	mainInit();
    time(&startTime);

    #ifdef FULLSCREEN
    glutFullScreen();
    windowWidth = 1920;
    windowHeight = 1080;
    #endif
	glutMainLoop();

    free(sceneMatrix);
    return 0;
}


CAMERA_TYPES nextCamera(CAMERA_TYPES curCamera)
{
    switch (curCamera)
    {
    case THIRD_PERSON:
        return FIRST_PERSON;
    case FIRST_PERSON:
        return CEILING_VISION;
    case CEILING_VISION:
        return THIRD_PERSON;
    }
}

