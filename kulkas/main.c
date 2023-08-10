/*
********************************************************
          Tugas Akhir Grafika Komputer

          Pemodelan Objek 3 Dimensi Dan Animasi Dari 
		  Representasi Alat Elektronik Kulkas Mini
			Arrasyid Muhammad Alfath-2100018207
            Informatika - UAD
            Ahmad Azhari,S.Kom.,M.Eng
********************************************************
*/
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define GERAK    5

#define NUM_OBJEK 2
#define CENA        0
#define GELADEIRA   1
#define PORTA   2


#define NUM_TEX   21
#define TEKSTUR1  1000
#define TEKSTUR2  1001
#define TEKSTUR3  1002
#define TEKSTUR4  1003
#define TEKSTUR5  1004 
#define TEKSTUR6  1005 
#define TEKSTUR7  1006 
#define TEKSTUR8  1007 
#define TEKSTUR9  1008 
#define TEKSTUR10  1009
#define TEKSTUR11  1010 
#define TEKSTUR12  1011 
#define TEKSTUR13  1012 
#define TEKSTUR14  1013 
#define TEKSTUR15  1014 
#define TEKSTUR16  1015 
#define TEKSTUR17  1016
#define TEKSTUR18  1017
#define TEKSTUR19  1018 
#define TEKSTUR20  1019 
#define TEKSTUR21  1020

struct tipo_camera {
   GLfloat posx;               
   GLfloat posy;               
   GLfloat posz;              
   GLfloat targetx;              
   GLfloat targety;              
   GLfloat targetz;              
   GLfloat awal;             
   GLfloat akhir;                
   GLfloat ang;               
};

struct tipo_light {
   GLfloat posisi[ 4 ];
   GLfloat ambient[ 4 ];
   GLfloat diffuse[ 4  ];
   GLfloat specular[ 4 ];
   GLfloat specularidade[ 4 ];
};

GLfloat qaAmbientLight[] = {0.1 ,0.1, 0.1, 1.0};
GLfloat qaDiffuseLight[] = {1, 1, 1, 1.0};
GLfloat qaSpecularLight[] = {1.0, 1.0, 1.0, 1.0};
GLfloat emitLight[] = {0.1, 0.1, 0.1, 0.01};
GLfloat Noemit[] = {0.0, 0.0, 0.0, 1.0};

GLfloat qaLightPosition[] = {0,1,0,0};
GLfloat qaLightDirection[] = {0,1,0,0};
GLfloat dirVector[] = {0.0,1.0,0.0,0.0};

typedef struct type_transformation_{
    GLfloat dx, dy, dz;
    GLfloat sx, sy, sz;
    GLfloat angx, angy, angz, ang;
} type_transformation;

type_transformation transf[ NUM_OBJEK ];


struct tipo_camera camera;

struct tipo_light light;

GLfloat aspecto;
GLfloat rotasi = 0;
GLuint  texture_id[ NUM_TEX ];

enum boolean {
    true = 1, false = 0
};
typedef enum boolean bool;

bool Dapur;
char transformacao, eixo;
GLint  objeto;
GLfloat cab;

int  LoadBMP(char *filename){
    #define SAIR        {fclose(fp_arquivo); return -1;}
    #define CTOI(C)     (*(int*)&C)

    GLubyte     *image;
    GLubyte     Header[0x54];
    GLuint      DataPos, imageSize;
    GLsizei     Width,Height;

    int nb = 0;


    FILE * fp_arquivo = fopen(filename,"rb");
    if (!fp_arquivo)
        return -1;
    if (fread(Header,1,0x36,fp_arquivo)!=0x36)
        SAIR;
    if (Header[0]!='B' || Header[1]!='M')
        SAIR;
    if (CTOI(Header[0x1E])!=0)
        SAIR;
    if (CTOI(Header[0x1C])!=24)
        SAIR;



    Width   = CTOI(Header[0x12]);
    Height  = CTOI(Header[0x16]);
    (CTOI(Header[0x0A]) == 0 ) ? ( DataPos=0x36 ) : ( DataPos = CTOI(Header[0x0A]));

    imageSize=Width*Height*3;

    image = (GLubyte *) malloc ( imageSize );
    int retorno;
    retorno = fread(image,1,imageSize,fp_arquivo);

    if (retorno !=imageSize){
        free (image);
        SAIR;
    }

    int t, i;

    for ( i = 0; i < imageSize; i += 3 ){
        t = image[i];
        image[i] = image[i+2];
        image[i+2] = t;
    }

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S    ,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T    ,GL_REPEAT);

    glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    fclose (fp_arquivo);
    free (image);
    return 1;
}

void Teksturisasi(){
   glEnable(GL_TEXTURE_2D);
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
   glGenTextures ( NUM_TEX , texture_id );

   texture_id[ 0 ] = TEKSTUR1;
   glBindTexture ( GL_TEXTURE_2D, texture_id[0] );
   LoadBMP ( "textures/logo-uad.bmp" ); 

   texture_id[ 1 ] = TEKSTUR2; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[1] );
   LoadBMP ( "textures/jeruk.bmp" ); 

   texture_id[ 2 ] = TEKSTUR3; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[2] );
   LoadBMP ( "textures/apel.bmp" ); 

   texture_id[ 3 ] = TEKSTUR4; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[3] );
   LoadBMP ( "textures/cocacola.bmp" ); 

   texture_id[ 4 ] = TEKSTUR5; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[4] );
   LoadBMP ( "textures/dapur.bmp" ); 

   texture_id[ 5 ] = TEKSTUR6; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[5] );
   LoadBMP ( "textures/pintukrem.bmp" ); 

   texture_id[ 6 ] = TEKSTUR7; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[6] );
   LoadBMP ( "textures/krem.bmp" ); 

   texture_id[ 7 ] = TEKSTUR8;
   glBindTexture ( GL_TEXTURE_2D, texture_id[7] );
   LoadBMP ( "textures/laci.bmp" ); 

   texture_id[ 8 ] = TEKSTUR9; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[8] );
   LoadBMP ( "textures/lacitutup.bmp" );

   texture_id[ 9 ] = TEKSTUR10; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[9] );
   LoadBMP ( "textures/pintuijo.bmp" ); 

   texture_id[ 10 ] = TEKSTUR11;
   glBindTexture ( GL_TEXTURE_2D, texture_id[10] );
   LoadBMP ( "textures/ijo.bmp" ); 

   texture_id[ 11 ] = TEKSTUR12; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[11] );
   LoadBMP ( "textures/oven.bmp" ); 

   texture_id[ 12 ] = TEKSTUR13; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[12] );
   LoadBMP ( "textures/merah.bmp" ); 

   texture_id[ 13 ] = TEKSTUR14;
   glBindTexture ( GL_TEXTURE_2D, texture_id[13] );
   LoadBMP ( "textures/latar.bmp" );

   texture_id[ 14 ] = TEKSTUR15; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[14] );
   LoadBMP ( "textures/isirak1.bmp" ); 

   texture_id[ 15 ] = TEKSTUR16; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[15] );
   LoadBMP ( "textures/rak.bmp" );

   texture_id[ 16 ] = TEKSTUR17; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[16] );
   LoadBMP ( "textures/isirak2.bmp" ); 

   texture_id[ 17 ] = TEKSTUR18;
   glBindTexture ( GL_TEXTURE_2D, texture_id[17] );
   LoadBMP ( "textures/fridge_open_door.bmp" );

   texture_id[ 18 ] = TEKSTUR19; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[18] );
   LoadBMP ( "textures/support1.bmp" ); 

   texture_id[ 19 ] = TEKSTUR20; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[19] );
   LoadBMP ( "textures/support2.bmp" ); 

    texture_id[ 20 ] = TEKSTUR21; 
   glBindTexture ( GL_TEXTURE_2D, texture_id[20] );
   LoadBMP ( "textures/fundo4.bmp" ); // l? a textura

   glTexGeni( GL_S , GL_TEXTURE_GEN_MODE , GL_SPHERE_MAP );
   glTexGeni( GL_T , GL_TEXTURE_GEN_MODE , GL_SPHERE_MAP );
}


void Define_Iluminacao( void ){

    glEnable ( GL_LIGHT2 );
    glLightfv(GL_LIGHT2, GL_AMBIENT, qaAmbientLight );
    glLightfv(GL_LIGHT2, GL_DIFFUSE, qaDiffuseLight );
    glLightfv(GL_LIGHT2, GL_POSITION, qaLightPosition );
    glLightfv(GL_LIGHT2, GL_SPECULAR, qaSpecularLight );
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 95.0);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dirVector);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1);

    glShadeModel( GL_SMOOTH );


    glEnable( GL_LIGHTING );
    glEnable ( GL_LIGHT0 );

    glLightModelfv( GL_LIGHT_MODEL_AMBIENT , light.ambient );

    glLightfv( GL_LIGHT0 , GL_POSITION , light.posisi );

    glLightfv( GL_LIGHT0 , GL_AMBIENT  , light.ambient  );

    glLightfv( GL_LIGHT0 , GL_DIFFUSE  , light.diffuse );

    glLightfv( GL_LIGHT0 , GL_SPECULAR , light.specular );





    glMaterialfv( GL_FRONT , GL_SPECULAR  , light.specularidade );

    glMateriali ( GL_FRONT , GL_SHININESS , 20 );

    glEnable(GL_COLOR_MATERIAL);

}

void Inicializa (void){
    glClearColor( 0.0 , 0.0 , 0.0 , 1.0 );
    int i;
    for( i = 0 ; i < NUM_OBJEK ; i++ ){
        transf[ i ].dx  = 0.0;
        transf[ i ].dy  = 0.0;
        transf[ i ].dz  = 0.0;
        transf[ i ].sx  = 1.0;
        transf[ i ].sy  = 1.0;
        transf[ i ].sz  = 1.0;
        transf[i].angx  = 0.0;
        transf[i].angy  = 0.0;
        transf[i].angz  = 0.0;
        transf[i].ang   = 0.0;
    }

    //inicializa??o
    objeto = CENA;
    transformacao = 'R';
    eixo = 'Y';
    Dapur = false;
    camera.posx   = 0;
    camera.posy   = 0;
    camera.posz   = 400;
    camera.targetx  = 0;
    camera.targety  = 0;
    camera.targetz  = 0;
    camera.awal = 0.1;
    camera.akhir    = 5000.0;
    camera.ang    = 45;
    light.posisi[ 0 ] = 200.0;
    light.posisi[ 1 ] = -200.0;
    light.posisi[ 2 ] = 200.0;
    light.posisi[ 3 ] = 1.0;
    light.ambient[ 0 ] = 0.3;
    light.ambient[ 1 ] = 0.3;
    light.ambient[ 2 ] = 0.3;
    light.ambient[ 3 ] = 0.3;
    light.diffuse[ 0 ] = 0.5;
    light.diffuse[ 1 ] = 0.5;
    light.diffuse[ 2 ] = 0.5;
    light.diffuse[ 3 ] = 0.5;
    light.specular[ 0 ] = 1.0;
    light.specular[ 1 ] = 1.0;
    light.specular[ 2 ] = 1.0;
    light.specular[ 3 ] = 1.0;
    light.specularidade[ 0 ] = 0.5;
    light.specularidade[ 1 ] = 0.5;
    light.specularidade[ 2 ] = 0.5;
    light.specularidade[ 3 ] = 0.5;
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );
    Define_Iluminacao();
    Teksturisasi();
}
void drawCube(int x, int y, int z){

   glPushMatrix(); // pintu
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 1.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-4);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glVertex3d(-x,-y,-z);
            glVertex3d(x,-y,-z);
            glVertex3d(x,y,-z);
            glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-100,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-100,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube2(int x, int y, int z){ 

   glPushMatrix(); // face frontal
        glColor3ub(255,255,255);

        glBegin(GL_QUADS);
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,y,z);
            glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-8);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
            glVertex3d(-x,-y,-z);
            glVertex3d(x,-y,-z);
            glVertex3d(x,y,-z);
            glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glColor3ub(255,255,255);

        glBegin(GL_QUADS);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(x,y,-z);
            glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-12,0,0); // SE AUMENTAR AQUI, AUMENTA TODOS
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
            glVertex3d(-x,-y,z);
            glVertex3d(-x,-y,-z);
            glVertex3d(-x,y,-z);
            glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glColor3ub(255,255,255);

        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
            glVertex3d(-x,y,z);
            glVertex3d(x,y,z);
            glVertex3d(x,y,-z);
            glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-8,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube3(int x, int y, int z){ // prateleira dentro

   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(0.0 , 0.0 ,1.0 );	// normal da face
           glVertex3d(-x,-y,z);
           glVertex3d(x,-y,z);
           glVertex3d(x,y,z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face traseira
        glTranslated(0,0,-2);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
           glVertex3d(-x,-y,-z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glTranslatef(-0,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glTranslatef(0,-0,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}


void drawCube4(int x, int y, int z){ //prateleira porta

   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 1.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-2);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glVertex3d(-x,-y,-z);
            glVertex3d(x,-y,-z);
            glVertex3d(x,y,-z);
            glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-100,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-30,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube5(int x, int y, int z){ //prateleira porta

   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 1.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-2);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glVertex3d(-x,-y,-z);
            glVertex3d(x,-y,-z);
            glVertex3d(x,y,-z);
            glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-20,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-30,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}


void drawCube6(int x, int y, int z){ //prateleira porta

   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 1.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-2);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glVertex3d(-x,-y,-z);
            glVertex3d(x,-y,-z);
            glVertex3d(x,y,-z);
            glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-100,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-20,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube7(int x, int y, int z){ //cena

   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-4);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glVertex3d(-x,-y,-z);
            glVertex3d(x,-y,-z);
            glVertex3d(x,y,-z);
            glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-300,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-180,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube8(int x, int y, int z){ //a?reo

   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture ( GL_TEXTURE_2D, TEKSTUR9 );
    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-40);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face direita
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-100,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face superior
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-40,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glVertex3d(-x,-y,z);
            glVertex3d(x,-y,z);
            glVertex3d(x,-y,-z);
            glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube9(int x, int y, int z){ //a?reo prateleira

   glEnable(GL_TEXTURE_2D);
   glBindTexture ( GL_TEXTURE_2D, TEKSTUR15 );
   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR17 );
    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-40);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR16 );
    glPushMatrix(); // face direita
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();


    glPushMatrix(); // face esquerda
        glColor3ub(255,255,255);
        glTranslatef(-100,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPushMatrix(); // face superior
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
   glBindTexture ( GL_TEXTURE_2D, TEKSTUR13 );
    glPushMatrix(); // face inferior
        glColor3ub(255,255,255);
        glTranslatef(0,-40,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,-z);
             glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube10(int x, int y, int z){ //suporte de cima

   glEnable(GL_TEXTURE_2D);
   glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
   glPushMatrix(); // face frontal
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix(); // face traseira
        glColor3ub(255,255,255);
        glTranslated(0,0,-40);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix();
        glColor3ub(255,255,255);
        glTranslatef(-180,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(-x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(-x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,y,z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix();
        glColor3ub(255,255,255);
        glTranslatef(0,-4,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube11(int x, int y, int z){ 

   glEnable(GL_TEXTURE_2D);
   glBindTexture ( GL_TEXTURE_2D, TEKSTUR12 );
   glPushMatrix(); 
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR11 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslated(0,0,-40);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR11 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture ( GL_TEXTURE_2D, TEKSTUR11 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(-40,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glPushMatrix();
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(0,-50,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,-z);
             glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}


void drawCube12(int x, int y, int z){ 

   glEnable(GL_TEXTURE_2D);
   glBindTexture ( GL_TEXTURE_2D, TEKSTUR6 );
   glPushMatrix();
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR7 );
    glPushMatrix();
        glColor3ub(255,255,255);
        glTranslated(0,0,-40);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();


    glPushMatrix();
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR7 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(-40,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(0,-50,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,-z);
             glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}

void drawCube13(int x, int y, int z){

   glEnable(GL_TEXTURE_2D);
   glBindTexture ( GL_TEXTURE_2D, TEKSTUR10 );
   glPushMatrix(); 
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR11 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslated(0,0,-40);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR11 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR11 );
    glPushMatrix();
        glColor3ub(255,255,255);
        glTranslatef(-40,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPushMatrix(); 
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR11 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(0,-50,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,-z);
             glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}


void drawCube14(int x, int y, int z){ 

   glEnable(GL_TEXTURE_2D);
   glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
   glPushMatrix(); 
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	// normal da face
           glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 1.0 ); glVertex3d(x,y,z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslated(0,0,-40);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();


    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix();
        glColor3ub(255,255,255);
        glTranslatef(-150,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(-x,-y,-z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(-x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix();
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,y,z);
           glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,y,z);
           glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
           glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(0,-6,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}


void drawCube15(int x, int y, int z){

   glPushMatrix(); 
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  1.0 );	
           glVertex3d(-x,-y,z);
           glVertex3d(x,-y,z);
           glVertex3d(x,y,z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslated(0,0,-4);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   0.0 ,  -1.0 );
            glTexCoord2f(  1.0 , 0.0 );glVertex3d(-x,-y,-z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,y,-z);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glEnd();
    glPopMatrix();

    glPushMatrix(); 
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   1.0 ,   0.0 ,  0.0 );
           glVertex3d(x,-y,z);
           glVertex3d(x,-y,-z);
           glVertex3d(x,y,-z);
           glVertex3d(x,y,z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(-780,0,0);
        glRotated(180,0,1,0);
        glBegin(GL_QUADS);
        glNormal3f(   -1.0 ,   0.0 ,  0.0 );
           glVertex3d(-x,-y,z);
           glVertex3d(-x,-y,-z);
           glVertex3d(-x,y,-z);
           glVertex3d(-x,y,z);
        glEnd();
    glPopMatrix();


    glPushMatrix();
        glColor3ub(255,255,255);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   1.0 ,  0.0 );
           glVertex3d(-x,y,z);
           glVertex3d(x,y,z);
           glVertex3d(x,y,-z);
           glVertex3d(-x,y,-z);
        glEnd();
    glPopMatrix();

    glPushMatrix(); 
        glColor3ub(255,255,255);
        glTranslatef(0,-180,0);
        glRotated(180,1,0,0);
        glBegin(GL_QUADS);
        glNormal3f(   0.0 ,   -1.0 ,  0.0 );
            glTexCoord2f(  1.0 , 0.0 ); glVertex3d(-x,-y,z);
            glTexCoord2f(  0.0 , 0.0 );glVertex3d(x,-y,z);
            glTexCoord2f(  0.0 , 1.0 );glVertex3d(x,-y,-z);
            glTexCoord2f(  1.0 , 1.0 );glVertex3d(-x,-y,-z);
        glEnd();
    glPopMatrix();
}



// Fun??o callback chamada para fazer o desenho
void Kulkas(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Define_Iluminacao();

    GLUquadricObj *quadObj; 
    quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GL_TRUE);
    gluQuadricDrawStyle(quadObj, GLU_FILL);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLineWidth( 1 );

    GLint raio, segmentos;
    GLfloat ang;

    if(Dapur){
            glTranslatef( 0 , 0 , 100 );

            glTranslatef( transf[ CENA ].dx , 39+transf[ CENA ].dy , transf[ CENA ].dz );
            glRotatef( transf[CENA].angx, 1,0,0);
            glRotatef( transf[CENA].angy, 0,1,0);
            glRotatef( transf[CENA].angz, 0,0,1);


            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glBindTexture ( GL_TEXTURE_2D, TEKSTUR14 );
                glPushMatrix();
                    glTranslatef( 0 , -25 , -25 );
                    drawCube7(150,90,2);
                glPopMatrix();

            glPopMatrix();

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_2D);
            glBindTexture ( GL_TEXTURE_2D, TEKSTUR8 );
            glPushMatrix(); 
                glTranslatef( -20 , 0 , 0 );
                drawCube8(50,20,20);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);

            glPushMatrix();
                glTranslatef( 50 , 0 ,0 );
            
                drawCube9(20,20,20);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_2D);
            glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 ); 
            glPushMatrix(); //suporte de cima
                glTranslatef( 0 , 23 ,0 );
           
                drawCube10(90,2,20);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_2D);
            glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 );
                glTranslatef( 0 , -60 ,0 );
            
                drawCube10(90,2,20);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_2D);
            glBindTexture ( GL_TEXTURE_2D, TEKSTUR12 ); 
            glPushMatrix();
                glTranslatef( -60 , -27 ,0 );
        
                drawCube11(20,25,20);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D); //balcao amarelo
            glPushMatrix();
                glTranslatef( -20 , -27 ,0 );
    
                drawCube12(20,25,20);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D); //balcao verde
            glPushMatrix();
                glTranslatef( 60 , -27 ,0 );
     
                drawCube13(20,25,20);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_2D);
            glBindTexture ( GL_TEXTURE_2D, TEKSTUR20 ); //suporte do meio
                glTranslatef( 0 , -55 ,0 );
             
                drawCube14(75,3,20);
            glPopMatrix();

            glScalef(0.4,0.4,0.4);
            glTranslatef(50,70,0);

             glPushMatrix(); 
                glEnable(GL_TEXTURE_2D);
                glBindTexture ( GL_TEXTURE_2D, TEKSTUR21 );
                glPushMatrix();

                    glTranslatef( -50 , -77 , 80 );
                    glRotatef(90,1,0,0);
                    drawCube15(390,180,2);
                glPopMatrix();
            glPopMatrix();
    }

        glPushMatrix(); //fundo

        glTranslatef( 0 , 0 , 0 );

        if(Dapur==false){
        glTranslatef( transf[ CENA ].dx , transf[ CENA ].dy , transf[ CENA ].dz );
        glRotatef( transf[CENA].angx, 1,0,0);
        glRotatef( transf[CENA].angy, 0,1,0);
        glRotatef( transf[CENA].angz, 0,0,1);

        }
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, TEKSTUR1 );

        glPushMatrix(); //CUBO FRONTAL

            glTranslatef(0,0,50);
            glTranslatef(50, 0, 0);
            glRotatef(180,0,1,0);
            glRotatef( transf[GELADEIRA].angx, 1,0,0);
            glRotatef( transf[GELADEIRA].angy, 0,1,0);
            glRotatef( transf[GELADEIRA].angz, 0,0,1);
            glTranslatef(50, 0, 0);
            glRotatef(180,1,0,0);
            drawCube(50,60,2);
            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glColor3ub(255,255,255);;
                glTranslatef(0,40,-20);
                drawCube4(50,15,1);
            glPopMatrix();

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glColor3ub(255,255,255);;
                glTranslatef(50,40,-10);
                glRotatef(90,0,1,0);
                drawCube5(10,15,1);
            glPopMatrix();

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glColor3ub(255,255,255);;
                glTranslatef(-50,40,-10);
                glRotatef(90,0,1,0);
                drawCube5(10,15,1);
            glPopMatrix();

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glColor3ub(255,255,255);
                glTranslatef(0,55,-10);
                glRotatef(90,1,0,0);
                drawCube6(50,10,1);
            glPopMatrix();

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glColor3ub(255,255,255);
                glTranslatef(0,-20,-10);
                glRotatef(90,1,0,0);
                drawCube6(50,10,1);
            glPopMatrix();

        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        glPushMatrix(); 
            glTranslatef( 0 , 0 , -50 );
            glRotatef(180,1,0,0);
            drawCube(50,60,2);
        glPopMatrix();

        glPushMatrix(); 
            glTranslatef( -50 , 0 , 0 );
            glRotated(90, 0, 1, 0);
            glRotatef(180,1,0,0);
            drawCube(50,60,2);

        glPopMatrix();

        glPushMatrix(); 
            glTranslatef( 50 , 0 , 0 );
            glRotatef( 90, 0, 1, 0);
            drawCube(50,60,2);
        glPopMatrix();

        glPushMatrix();
            glTranslatef( 0 , 60 , 0 );
            glRotatef( 90, 1, 0, 0);
            glRotatef(180,1,0,0);
            drawCube(50,50,2);
        glPopMatrix();

        glPushMatrix(); 
            glTranslatef( 0 , -60 , 0 );
            glRotatef( 90, 1, 0, 0);
            drawCube(50,50,2);
        glPopMatrix();

        glPushMatrix(); 
            glTranslatef(30,-64,30);
            drawCube2(6,4,4);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(30,-64,-30);
            drawCube2(6,4,4);
        glPopMatrix();

        glPushMatrix(); 
            glTranslatef(-30,-64,30);
            drawCube2(6,4,4);
        glPopMatrix();

        glPushMatrix(); 
            glTranslatef(-30,-64,-30);
            drawCube2(6,4,4);
        glPopMatrix();

        glPushMatrix();
            glRotatef(90,1,0,0);
            glTranslatef(0,-20,-20);
            drawCube3(50,30,1);
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, TEKSTUR2 );

        glPushMatrix();
            glTranslatef(-10,-10,-10);
            glRotatef(90,0,1,0);
            gluSphere(quadObj,10,20,20);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, TEKSTUR3 );

        glPushMatrix(); 
            glTranslatef(-30,-48,-10);
            glRotatef(90,0,1,0);
            gluSphere(quadObj,10,20,20);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        glPushMatrix(); 
            glRotatef(90,1,0,0);
            glTranslatef(0,-20,20);
            drawCube3(50,30,1);
        glPopMatrix();

        glEnable(GL_TEXTURE_2D); 
        glBindTexture ( GL_TEXTURE_2D, TEKSTUR4 );
        glPushMatrix(); 
           
		   glTranslatef(30,-20,-30);
            glRotatef(270,1,0,0);
            gluCylinder(quadObj,10,10,30,30,30);
        glPopMatrix();


    glPopMatrix();
    glPopMatrix();


    glPopMatrix();

    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_T );

    glutSwapBuffers();

    gluDeleteQuadric( quadObj );
 }

void maxTrans(){
    if(transf[GELADEIRA].angy > 110) transf[GELADEIRA].angy = 110;
    if(transf[GELADEIRA].angy < 0) transf[GELADEIRA].angy = 0;
}

void parametervisual( void ){
    glMatrixMode( GL_PROJECTION );


    glLoadIdentity();


    gluPerspective( camera.ang , aspecto , camera.awal , camera.akhir );

    glMatrixMode( GL_MODELVIEW );

 
    glLoadIdentity();


    glRotatef( rotasi , 0 , 1 , 0 );

  
    gluLookAt( camera.posx , camera.posy , camera.posz , camera.targetx , camera.targety , camera.targetz , 0 , 1 , 0 );
}

void ChangeSizeWindow( GLsizei largura , GLsizei altura ){

    if ( altura == 0 ) altura = 1;


    glViewport( 0 , 0 , largura , altura );

 
    aspecto = ( GLfloat )largura / ( GLfloat )altura;

    printf("\nAspek (rasio antara tinggi dan lebar) : %f", aspecto );

    parametervisual();
}


void Mouse( GLint button , GLint state , GLint x , GLint y ){
  
    if ( button == GLUT_LEFT_BUTTON )
        if ( state == GLUT_DOWN )
            if ( camera.ang >= 6 ) camera.ang -= 5;

    if ( button == GLUT_RIGHT_BUTTON )
        if ( state == GLUT_DOWN ) 
            if ( camera.ang <= 174 ) camera.ang += 5;

    parametervisual();

    glutPostRedisplay();
}
void Keyboard( GLubyte key , GLint x , GLint y ){
    if ( key-48 >= 0 && key-48 <= NUM_OBJEK-1 )
        objeto = key - 48;

    if ( key == 27 )
        exit( 0 );

    if ( key == 'v' || key == 'V' )
        Dapur =! Dapur;

    if ( key == 'o' || key == 'O' ){
        transf[ GELADEIRA ].angy += GERAK;
        maxTrans();
    }

    if ( key == 'c' || key == 'C' ){
        transf[ GELADEIRA ].angy -= GERAK;
        maxTrans();
    }

    if ( toupper( key ) == 'R' || toupper( key ) == 'R' )
        transformacao = toupper( key );


    if ( toupper( key ) == 'T' || toupper( key ) == 'T' )
        transformacao = toupper( key );

    if ( toupper( key ) == 'X' || toupper( key ) == 'Y' || toupper(key)=='Z')
        eixo = toupper( key );

    if ( key == '+' || key == '=' ){
        switch( transformacao ){
            case 'R':
                if(objeto != 1){
                    if ( eixo == 'X'){
                        transf[ objeto ].angx += GERAK;
                        maxTrans();
                    }
                    else if ( eixo == 'Y'){
                        transf[ objeto ].angy += GERAK;
                        maxTrans();
                    }
                    else{
                        transf[ objeto ].angz += GERAK;
                        maxTrans();
                    }
                }
                break;

            case 'T':
                if ( eixo == 'X'){
                    transf[ objeto ].dx += GERAK;
                    maxTrans();
                }
                else if ( eixo == 'Y'){
                    transf[ objeto ].dy += GERAK;
                    maxTrans();
                }
                else{
                    transf[ objeto ].dz += GERAK;
                    maxTrans();
                }
                break;

        }
    }
    if ( key == '-' || key == '_' ){
        switch( transformacao ){
            case 'R':
                if(objeto != 1){
                    if ( eixo == 'X'){
                        transf[ objeto ].angx -= GERAK;
                        maxTrans();
                    }

                    else if ( eixo == 'Y'){
                        transf[ objeto ].angy -= GERAK;
                        maxTrans();
                    }
                    else{
                        transf[ objeto ].angz -= GERAK;
                        maxTrans();
                    }
                }
                break;

            case 'T':
                if ( eixo == 'X'){
                    transf[ objeto ].dx -= GERAK;
                    maxTrans();
                }
                else if(eixo=='Y'){
                    transf[ objeto ].dy -= GERAK;
                    maxTrans();
                }
                else{
                    transf[ objeto ].dz -= GERAK;
                    maxTrans();
                }

                break;
        }
    }

   if ( key == 'G' )
       rotasi += GERAK;

   if ( key == 'g' )
       rotasi -= GERAK;

   if ( key == 'D' ){
       camera.posx += GERAK;
       camera.targetx += GERAK;
   }

   if ( key == 'd' ){
       camera.posx -= GERAK;
       camera.targetx -= GERAK;
   }

   if ( key == 'H' ){
       camera.posy += GERAK;
       camera.targety += GERAK;
   }

 
   if ( key == 'h' ){
       camera.posy -= GERAK;
       camera.targety -= GERAK;
   }

   if ( key == 'P' ){
       camera.posz += GERAK;
       camera.targetz += GERAK;
   }

   if ( key == 'p' ){
       camera.posz -= GERAK;
       camera.targetz -= GERAK;
   }

   if ( key == 'E' )
       camera.targetx += GERAK;

   if ( key == 'e' )
       camera.targetx -= GERAK;

   if ( key == 'B' )
       camera.targety += GERAK;

   if ( key == 'b' )
       camera.targety -= GERAK;

   if ( key == 'f' )
       camera.targetz += GERAK;

   if ( key == 'F' )
       camera.targetz -= GERAK;

   if ( key == 'K' && camera.awal + GERAK < camera.akhir )
       camera.awal += GERAK;

   if ( key == 'k' && camera.awal - GERAK > 0 )
       camera.awal -= GERAK;

   if ( key == 'L' )
       camera.akhir += GERAK;

   if ( key == 'l' && camera.akhir - GERAK > camera.awal )
       camera.akhir -= GERAK;

   if ( key == 'A' && camera.ang + GERAK < 180 )
       camera.ang += GERAK;

   if ( key == 'a' && camera.ang - GERAK > 0 )
       camera.ang -= GERAK;

   if ( key == 'I'  || key == 'i' )
      Inicializa();

   parametervisual();

   glutPostRedisplay();

}
void SpecialKey( GLint key , GLint x , GLint y ){
}

int main( int argc , char *argv[] ){
   setlocale(LC_ALL, "Indonesian");

   glutInit( &argc , argv );
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH  );
   glutInitWindowSize( 700 , 600 );
   glutInitWindowPosition( 0 , 0 );
   glutCreateWindow( "Kulkas Mini Arrasyid Muhammad Alfath" );
   glutDisplayFunc( Kulkas );
   glutReshapeFunc( ChangeSizeWindow );
   glutMouseFunc( Mouse );
   glutKeyboardFunc( Keyboard );
   glutSpecialFunc( SpecialKey );
   printf("\n----------------------UI----------------------");
   printf("\ni => inisialisasi");
   printf("\nv => tampilkan dapur");
   printf("\nr => ROTASI");
   printf("\nt => TRANSLASI");
   printf("\nx => pilih sumbu x");
   printf("\ny => pilih sumbu y");
   printf("\nz => pilih sumbu z");
   printf("\no => buka pintu");
   printf("\nc => tutup pintu");
   printf("\n+ - => arah transformasi");
   printf("\ng => putar kamera");
   printf("\nd => ganti posisi kamera di sumbu x");
   printf("\nh => ganti posisi kamera di sumbu y");
   printf("\np => ganti posisi kamera di sumbu z");
   printf("\ne => ganti target x");
   printf("\nb => ganti target y");
   printf("\nf => ganti target z");
   printf("\na => ganti sudut kamera");
   printf("\nklik kiri => zoom +");
   printf("\nklik kanan=> zoom -");
   printf("\nESC => exit");
   printf("\n\n--------------------------WINDOW----------------------------");
   Inicializa();
   glutMainLoop();
}

