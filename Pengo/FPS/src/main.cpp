/**
Laboratorio FCG - Pratica 4
Ultima modificacao: 07/05/2014 - Acompanha bibliotecas: 3DObject.cpp(.h), bitmap.c(.h), glm.cpp,
glmimg.cpp, Texture.cpp(Texture.h)

Implementa a movimenta��o simples de uma camera em primeira pessoa para um personagem que anda
sobre um plano. Utiliza a biblioteca bitmap para carregamento simples de texturas, glm + 3DObject
para modelos OBJ e glmimg + Texture para aplicacao de texturas TGA sobre os modelos carregados

Tarefas:

1 - Adicionar neblina � cena ao implementar a fun��o enableFog() com os par�metros de habilita��o
de fog na OpenGL;
2 - Usar uma imagem no formato BMP como mapa pra posicionar objetos 3D na cena. Use a cor de cada
pixel para definir qual o modelo ser� colocado, e a posi��o do pixel para definir a posi��o do
modelo no espa�o. Pixels pretos n�o representam nenhum modelo, a posi��o fica vazia na cena;
3 - Mudar as configura��es da fonte de luz da cena, j� implementada no c�digo base dado, para que
seja uma spotlight;
4 - Adicionar uma segunda fonte de luz que fique girando ao redor do cen�rio.
*/



#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <cmath>
#include <ctime>
#include <cstring>
#include <iostream>
#include <gl/glut.h>
#include <string>
//openal (sound lib)
#include "../include/al/alut.h"

//bitmap class to load bitmaps for textures
#include "../include/bitmap.h"

//handle for the al.obj model
//#include "ModelAl.h"

//handle generic obj models
#include "../include/3DObject.h"

#include "../include/Camera.h"

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

typedef enum { NOTHING, ICECUBE, PENGO, BALL } OBJ_ENUM;

OBJ_ENUM *sceneMatrix;

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
void initSound();
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

/**
Screen dimensions
*/
int windowWidth = 600;
int windowHeight = 480;

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
variavel auxiliar pra dar varia��o na altura do ponto de vista ao andar.
*/
float headPosAux = 0.0f;

float maxSpeed = 0.25f;

float planeSize = 25.0f;

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



// parte de c�digo extra�do de "texture.c" por Michael Sweet (OpenGL SuperBible)
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
Texture chao, iceCube;
//CModelAl modelAL;
Camera pengoCamera, ceilingCamera;
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
Atualiza a posi��o e orienta��o da camera
*/
void updateCam() {
    eyeX = posX;
    eyeY = posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180));
    eyeZ = posZ;
    cntrX = posX + sin(roty*PI/180);
    cntrY = posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180);
    cntrZ = posZ - cos(roty*PI/180);

    pengoCamera.set_eye(posX, posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)), posZ);
    pengoCamera.set_center(posX + sin(roty*PI/180), posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180), posZ - cos(roty*PI/180));

	pengoCamera.callGluLookAt();

	// atualiza a posi��o do listener e da origen do som, s�o as mesmas da camera, j� que os passos vem de onde o personagem est�
	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;
	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;



    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, {});
    /*
    GLfloat light_direction[] = {
        cntrX - eyeX,
        cntrY - eyeY,
        cntrZ - eyeZ
    };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    */



}

void updateLights()
{
    light1Angle = light1Angle > 360.0f ? (light1Angle - 360.0f) + 0.08f : light1Angle + 0.08f;
}

void renderLights()
{
    GLfloat light_position[] = {posX, posY, posZ, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    Point3D direction1 = pengoCamera.get_center() - pengoCamera.get_eye();
    GLfloat direction[] = {direction1.getX(), direction1.getY(), direction1.getZ(), 1.0 };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
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
    glColor3d(1.0, 1.0, 1.0);
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
}

void setViewport(GLint left, GLint right, GLint bottom, GLint top) {
	glViewport(left, bottom, right - left, top - bottom);
}


/**
Initialize
*/
void mainInit() {
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);

    ceilingCamera.set_eye(0.0, 5.0, 0.0);
    ceilingCamera.set_center(0.0, -1.0, 0.0);
    ceilingCamera.set_upvector(1.0, 0.0, 0.0);
	// habilita remocao de faces ocultas

	glFrontFace (GL_CCW);

    glEnable(GL_CULL_FACE);

	// habilita o z-buffer
	glEnable(GL_DEPTH_TEST);

    initSound();

    initTexture();

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
	flower.Load("flowers.obj");
	//modelAL = CModelAl();
	//modelAL.Init();
	printf("Models ok. \n \n \n");
}

/**
Initialize openal and check for errors
*/
void initSound() {

	printf("Initializing OpenAl \n");

	// Init openAL
	alutInit(0, NULL);

	alGetError(); // clear any error messages

    // Generate buffers, or else no sound will happen!
    alGenBuffers(NUM_BUFFERS, buffer);

    if(alGetError() != AL_NO_ERROR)
    {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    else
    {
        printf("init() - No errors yet.\n");
    }

	alutLoadWAVFile("Footsteps.wav",&format,&data,&size,&freq,false);
    alBufferData(buffer[0],format,data,size,freq);

	alGetError(); /* clear error */
    alGenSources(NUM_SOURCES, source);

    if(alGetError() != AL_NO_ERROR)
    {
        printf("- Error creating sources !!\n");
        exit(2);
    }
    else
    {
        printf("init - no errors after alGenSources\n");
    }

	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;

	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;

	alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);

	alSourcef(source[0], AL_PITCH, 1.0f);
    alSourcef(source[0], AL_GAIN, 1.0f);
    alSourcefv(source[0], AL_POSITION, source0Pos);
    alSourcefv(source[0], AL_VELOCITY, source0Vel);
    alSourcei(source[0], AL_BUFFER,buffer[0]);
    alSourcei(source[0], AL_LOOPING, AL_TRUE);

	printf("Sound ok! \n\n");
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
                sceneMatrix[aux] = BALL;
            else if (cor == 0x0000FF)
                sceneMatrix[aux] = PENGO;
            else if (cor == 0x00FF00)
                sceneMatrix[aux] = ICECUBE;
            else sceneMatrix[aux] = NOTHING;
            aux++;
	    }


    free(sceneInfo);
    free(sceneBmp);
    // sceneInfo = NULL;
    /*
	// Set texture parameters
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(type, 0, 4, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );

    */
    printf("Textura %d\n", texture);
    printf("Textura %d\n", iceTexture);
	printf("Textures ok.\n\n", texture);

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
	glTexParameteri(tex.type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(tex.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(tex.type, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(tex.type, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(tex.type, 0, 4, tex.info->bmiHeader.biWidth, tex.info->bmiHeader.biHeight,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, tex.rgba );

}


void enableFog(void)
{
    // glEnable(GL_FOG);
    glFogf(GL_FOG_DENSITY, 0.25f);
    glFogi(GL_FOG_MODE, GL_EXP);
}

void drawCube(float side)
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, iceCube.texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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

void renderScene() {
	glClearColor(backgroundColor[0],backgroundColor[1],backgroundColor[2],backgroundColor[3]);
	  // limpar o depth buffer

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

    renderLights();

    for (int i = 0; i < sceneHeight; ++i)
    {
        for (int j = 0; j < sceneWidth; ++j)
        {
            glPushMatrix();
            glTranslatef(planeSize/2.0f - (i * (planeSize/sceneWidth)), 1.0, planeSize/2.0f - (j * (planeSize/sceneHeight)));
            float cubeSide = 0.8f;
            switch (sceneMatrix[i*sceneWidth + j])
            {
            case ICECUBE:

                glPushMatrix();
                glTranslatef(0.0, -0.6, 0.0);
                drawCube(cubeSide);
                glPopMatrix();
                break;
            case PENGO:
                pengo.Draw(SMOOTH_MATERIAL_TEXTURE);
                break;
            case BALL:
                ball.Draw(SMOOTH_MATERIAL_TEXTURE);
                break;
            case NOTHING:
            default:
                break;
            }
            glPopMatrix();
        }
    }
    glPushMatrix();
    {
        Point3D pengoEye = pengoCamera.get_eye();
        glTranslatef(pengoEye.getX(), 1.0f + posY, pengoEye.getZ());
        glRotatef(180 - roty, 0.0, 0.1, 0.0);
        pengo.Draw(SMOOTH_MATERIAL_TEXTURE);
    }
    glPopMatrix();

    // sets the bmp file already loaded to the OpenGL parameters
    //setTextureToOpengl(chao);
	renderFloor();

	//modelAL.Translate(0.0f,1.0f,0.0f);
	//modelAL.Draw();
}
void updateState() {

	if (upPressed || downPressed || rightPressed || leftPressed) {

		if (running) {
			speedX = 0.05 * sin(roty*PI/180) * 2;
			speedZ = -0.05 * cos(roty*PI/180) * 2;
		} else {
			speedX = 0.05 * sin(roty*PI/180);
			speedZ = -0.05 * cos(roty*PI/180);
		}

		// efeito de "sobe e desce" ao andar
		headPosAux += 8.5f;
		if (headPosAux > 180.0f) {
			headPosAux = 0.0f;
		}

        if (upPressed) {
            posX += speedX;
            posZ += speedZ;
        }
        if (downPressed) {
            posX -= speedX;
            posZ -= speedZ;
        }

        if (rightPressed){
            speedX = 0.05 * sin((roty)*PI/180+(3.14/2));
			speedZ = -0.05 * cos((roty)*PI/180+(3.14/2));
            posX += speedX;
            posZ += speedZ;
        }
        if (leftPressed){
            speedX = 0.05 * sin((roty)*PI/180-(3.14/2));
			speedZ = -0.05 * cos((roty)*PI/180-(3.14/2));
            posX += speedX;
            posZ += speedZ;
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

}

void getTimeString(int minutes, int seconds, char *out)
{
    sprintf(out, "%02d:%02d", minutes, seconds);
}

/**
Render scene
*/
void mainRender() {



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
	renderScene();
	time_t now;
	time(&now);
	int diff = int(difftime(now, startTime));
    glDisable(GL_LIGHTING);
    char buffer[7];
    getTimeString(diff/60, diff % 60, buffer);
    std::string printMe = buffer;
    writeTextAt(0,0,printMe);
    glEnable(GL_LIGHTING);

	glViewport(windowWidth - 100, windowHeight -100, 100, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1, 0.5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_DEPTH_BUFFER_BIT);
	ceilingCamera.set_eye(posX, ceilingCamera.get_eye().getY(), posZ);
    ceilingCamera.set_center(posX, -1, posZ);
    ceilingCamera.callGluLookAt();
	renderScene();

    updateLights();

	glFlush();
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
	//printf("onMouseButton button: %d \n", button);
	if (button == GLUT_MIDDLE_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            changingZoom = true;
        }
        else changingZoom = false;
    }
	glutPostRedisplay();
	mouseLastX = x;
	mouseLastY = y;
}



/**
Mouse move while button pressed event handler
*/
void onMouseMove(int x, int y) {


    if (changingZoom)
    {
        Point3D lastEye = ceilingCamera.get_eye();
        ceilingCamera.set_eye(lastEye.getX(), lastEye.getY() + ((float)x-mouseLastX)/5, lastEye.getZ());
    }
	glutPostRedisplay();
    mouseLastX = x;
	mouseLastY = y;
}

/**
Mouse move with no button pressed event handler
*/
void onMousePassiveMove(int x, int y) {

	roty += (x - mouseLastX);

	rotx -= (y - mouseLastY);

	if (rotx < -128.0) {
		rotx = -128.0;
	}

	if (rotx > -45.0) {
		rotx = -45.0;
	}

	mouseLastX = x;
	mouseLastY = y;

	//glutPostRedisplay();
}

/**
Key press event handler
*/
void onKeyDown(unsigned char key, int x, int y) {
	//printf("%d \n", key);
	switch (key) {
		case 32: //space
			if (!spacePressed && !jumping) {
				jumping = true;
				speedY = jumpSpeed;
			}
			spacePressed = true;
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

/**
Key release event handler
*/
void onKeyUp(unsigned char key, int x, int y) {
	switch (key) {
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
	mainWindowId = glutCreateWindow("FPS");

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
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);

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
