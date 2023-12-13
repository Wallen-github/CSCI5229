/*
 *  Project: CSCI4229/5229 Fall 2023
 *  Author: Hai-Shuo Wang
 *
*/

#include "CSCIx229.h"
#define GL_NORMAL(a,b,c,p,q,r,x,y,z)  glNormal3d(((q-b)*(z-c))-((y-b)*(r-c)),-((p-a)*(z-c))-((x-a)*(r-c)),((p-a)*(y-b))-((x-a)*(q-b)))
//#define PI 3.14159265

int move=1;       //  Move light
int colli_ball = 0; // Show collision ball
// time
double timer = 0; //timer
double time_speed = 0.5; // time speed
int daynight = 0; // day and night index, 0 day, 1 night
float Zlight=-2;   //  Light elevation
// Perspective
int proj=0;       //  Projection type
int th=20;         //  Azimuth of view angle
int ph=20;         //  Elevation of view angle
int fov=50;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5;   //  Size of world
double zh=90;        //  Light azimuth
// first-person mode
double fp_x = -2.0; //x position
double fp_y = 0.5; // y position
double fp_z = 2.0; // z position
int fp_th = 0; // Azimuth
int fp_ph = 60; // Elevation
//  Object
int obj_car = 0;
int obj_cop_car = 0;
int obj_tree = 0;
int obj_street_lamp = 0;
int obj_alien = 0;
int obj_rocket = 0;
int obj_loopcar = 0;
// user car
double pos_user_car[] = {1,0,0,0.2};
double ang_user_car = 90;
double speed_user_car = 0.2; // speed of car
double speed_user_car2 = 0.2; // temp variable
double angle_wiper = 1;
int wiper = 0;
int speedup = 1;
double radiu_user_car = 0.1;
// loop car
double pos_loop_car[] = {-3.,0.01,0,0.2};
double ang_loop_car[] = {0,0,0};
// coop car
double pos_cop_car[] = {-2,0,2,0.2};
double ang_cop_car = 0;
double speed_cop_car = 0.01;
float ProjectionMatrix[16]; //  Transformation matrixes
float ViewMatrix[16]; //  Transformation matrixes
// wave flag
double ang_flag[] = {180.,0.,0.};
double siz_flag = 0.1;
double rgb_flag[] = {1.,1.,1.};
int boolInit = 1;
double flag[64][64][3];
unsigned int tex_flag = 0;
// UFO
unsigned int UFOvbo=0;
double pos_ufo[] = {0.,3.,0.,0.9};
// Ground
double Ground_width = 6.;
// static objects position
double pos_tree[] = {-0.5,0,0.5,0.15};
double pos_lamp[] = {0.5,0,0.5,0.1};
double pos_lamp1[] = {2,0,0.5,0.1};
double pos_tree0[] = {-0.5, 0.01,-2.5, 0.04};
double pos_tree1[] = {-0.5, 0.01,-1.0, 0.04};
double pos_tree2[] = {-2.0, 0.01,0.5, 0.04};
double pos_tree3[] = {-3.0, 0.01,0.5, 0.04};
double pos_tree4[] = {-2.5, 0.01,-0.5, 0.04};
double pos_tree5[] = {-2.0, 0.01,-2.0, 0.1};
double pos_flag[] = {3.,0.7,-1.,0.15};
double pos_rocket[] = {1.5,0.6,1.5,0.6};
// texture values
unsigned int texture_roof;
unsigned int texture_door;
unsigned int texture_wall;
unsigned int texture_tree;
unsigned int texture_leaf;
unsigned int texture_road;
unsigned int texture_grass;
unsigned int texture_car;
unsigned int texture_car_user;
unsigned int texture_daybox;
unsigned int texture_nightbox;
unsigned int texture_ufo;
unsigned int texture_cockpit;
//  Light colors
int ambient = 50;
int diffuse = 100;
int emission = 50;
int specular = 50;
float shiny   =  16;    // Shininess (value)



//  2D vector
typedef struct {float x,y;} vec2;
//  UFO outline
#define Np 53
vec2 P[Np] = {
    {2.000,-2.600},
    {2.010,-2.500},
    {2.020,-2.400},
    {2.030,-2.300},
    {2.020,-2.200},
    {2.010,-2.100},
    {2.000,-2.000},
    {2.200,-1.900},
    {2.400,-1.800},
    {2.600,-1.700},
    {2.800,-1.600},
    {3.000,-1.500},
    {3.200,-1.400},
    {3.400,-1.300},
    {3.600,-1.200},
    {3.800,-1.100},
    {4.000,-1.000},
    {4.200,-0.900},
    {4.400,-0.800},
    {4.600,-0.700},
    {4.800,-0.600},
    {5.000,-0.500},
    {5.200,-0.400},
    {5.400,-0.300},
    {5.600,-0.200},
    {5.800,-0.100},
    {6.000,-0.000},
    {5.900,+0.100},
    {5.800,+0.200},
    {5.600,+0.300},
    {5.400,+0.400},
    {5.200,+0.500},
    {5.000,+0.600},
    {4.900,+0.700},
    {4.700,+0.800},
    {4.600,+0.900},
    {4.500,+1.000},
    {4.300,+1.100},
    {4.200,+1.200},
    {4.000,+1.300},
    {3.900,+1.400},
    {3.700,+1.500},
    {3.600,+1.600},
    {3.400,+1.700},
    {3.300,+1.800},
    {3.100,+1.900},
    {3.000,+2.000},
    {3.100,+2.200},
    {3.200,+2.400},
    {3.300,+2.600},
    {3.200,+2.800},
    {3.100,+3.000},
  };

//  Make vec2 unit length
vec2 normalize(float x,float y)
{
   float len = sqrt(x*x+y*y);
   if (len>0)
   {
      x /= len;
      y /= len;
   }
   return (vec2){x,y};
}

//  Calculate normals for Pawn
vec2 N[Np];
void CalcNorm(void)
{
   //  Calculate normals for each facet
   //  y increases with point index so dy>0
   for (int i=0;i<Np-2;i++)
   {
      // Vector in the plane of the facet
      float dx = P[i+1].x - P[i].x;
      float dy = P[i+1].y - P[i].y;
      // Normal is perpendicular
      // dy>0 so normal faces out
      N[i] = normalize(dy,-dx);
   }
   //  The normal at the last point on the pawn is straight up
   //  This is NOT true in general
   N[Np-1] = (vec2){0,1};

   //  Average normals of adjacent facets
   //  First and last normal unchanged
   vec2 N2 = N[0];
   for (int i=1;i<Np-2;i++)
   {
      vec2 N1 = N2;
      N2 = N[i];
      N[i] = normalize(N1.x+N2.x , N1.y+N2.y);
   }
}

/*
 *  2-3-2 Euler rotation
 *  Pram:
 *      angle1 -- first angle around y axis
 *      angle2 -- second angle around z axis
 *      angle3 -- third angle around y axis
 */
void EulerRotation(double angle1, double angle2, double angle3){
    glRotatef(angle3, 0, 1, 0);
    glRotatef(angle2, 0, 0, 1);
    glRotatef(angle1, 0, 1, 0);
}

/*
 *  Vector Calculation
 */
double VectorDot(double vec1[3], double vec2[3]){
    return sqrt(vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2]);
}
double VectorNorm(double vec[3]){
    return VectorDot(vec,vec);
}
double VectorAngle(double vec1[3], double vec2[3]){
    double cos_phi = (vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2])/VectorNorm(vec1)/VectorNorm(vec2);
    return acos(cos_phi) *180/3.14159265;
}

/*
 * Set color
 */
void SetColor(float R,float G,float B)
{
   float color[] = {R,G,B,1.0};
   glColor3f(R,G,B);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
}

/*
 * Draw cylinder
 */
void Cylinder(double*pos, double radius, double height, double* rgb) {
    const int d=5;
    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glTranslatef(pos[0], pos[1], pos[2]);
    glScalef(radius, height, radius);
//    SetColor(94./245, 52./245, 3./245);
    SetColor(rgb[0],rgb[1],rgb[2]);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int th=0; th<=360; th+=d) {
//        glNormal3d(Sin(th), 0, Cos(th));
        glNormal3d(0, -1, 0);
        glVertex3f(Sin(th), 0, Cos(th));
    }
    glEnd();

    // //  Latitude bands
    for (int th=0; th<=360; th+=d) {
        glBegin(GL_QUADS);
        glNormal3d(Sin(th), 0, Cos(th));glVertex3f(Sin(th), 0, Cos(th));
        glNormal3d(Sin(th), 1, Cos(th));glVertex3f(Sin(th), 1, Cos(th));
        glNormal3d(Sin(th+d), 1, Cos(th+d));glVertex3f(Sin(th+d), 1, Cos(th+d));
        glNormal3d(Sin(th+d), 0, Cos(th+d));glVertex3f(Sin(th+d), 0, Cos(th+d));
        glEnd();
    }

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 1, 0);
    for (int th=0; th<=360; th+=d) {
//        glColor3f(0.4*Cos(th), 0.4 , 0.4*Sin(th));
        glNormal3d(0, 1, 0);
        glVertex3f(Sin(th), 1, Cos(th));
    }
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

/*
 * Draw Cone
 */
void Cone (double* pos_cone , double radius, double height, double* RGB) {
    const int d=5;
    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glTranslatef(pos_cone[0], pos_cone[1], pos_cone[2]);
    glScalef(radius, height, radius);
    SetColor( RGB[0], RGB[1], RGB[2]);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int th=0; th<=360; th+=d) {
    glNormal3d(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 0, Cos(th));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 1, 0);
    for (int th=0; th<=360; th+=d) {
    glNormal3d(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 0, Cos(th));
    }
    glEnd();

    //  Undo transformations
    glPopMatrix();
}

/*
 *  Draw a sphere at (x,y,z) with radius (r)
 */
void Sphere (double*pos, double radius, double* RGB) {
    const int d=5;
    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glTranslatef(pos[0], pos[1], pos[2]);
    glScalef(radius, radius, radius);
    glColor3f(RGB[0],RGB[1],RGB[2]);

    for (int ph1=-90; ph1<90; ph1+=d) {
    glBegin(GL_QUAD_STRIP);
    for (int th1=0; th1<=360; th1+=d) {
        double x_1 = Sin(th1)*Cos(ph1);
        double y_1 = Cos(th1)*Cos(ph1);
        double z_1 = Sin(ph1);
        glNormal3d(x_1, y_1, z_1);
        glVertex3d(x_1, y_1, z_1);

        double x_2 = Sin(th1)*Cos(ph1+d);
        double y_2 = Cos(th1)*Cos(ph1+d);
        double z_2 = Sin(ph1+d);
        glNormal3d(x_2, y_2, z_2);
        glVertex3d(x_2, y_2, z_2);
    }
    glEnd();
    }
    //  Undo transformations
    glPopMatrix();
}

/*
 *  Draw a surface
 */
void Surface(double mul, int num){
    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    for (int i=0;i<num;i++)
       for (int j=0;j<num;j++)
       {
           glTexCoord2d(mul*(i+0),mul*(j+0)); glVertex3d( i , 0, j );
           glTexCoord2d(mul*(i+1),mul*(j+0)); glVertex3d(i, 0, j+1 );
           glTexCoord2d(mul*(i+1),mul*(j+1)); glVertex3d(i+1, 0, j+1);
           glTexCoord2d(mul*(i+0),mul*(j+1)); glVertex3d( i+1 , 0, j);
       }
    glEnd();
}

/*
 *  Draw a Cube (working on updating to multiple block on surface)
 */
void Cube(double x, double y, double z, double width, double height, double depth, double angle1, double angle2, double angle3) {
    //  Save transformation
    glPushMatrix();
    //  shift, scale
    glTranslatef(x,y,z);
    EulerRotation(angle1,angle2,angle3);
    glScalef(width, height, depth);
    
    glBegin(GL_QUADS);
    // Front
    glNormal3f( 0, 0,+1);
    glVertex3f(-1, 0,+1);
    glVertex3f(+1, 0,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(-1,+1,+1);
    
    //  Back
    glNormal3f( 0, 0,-1);
    glVertex3f(+1, 0, -1);
    glVertex3f(-1, 0, -1);
    glVertex3f(-1, +1, -1);
    glVertex3f(+1, +1, -1);
    //  Right
    glNormal3f(+1, 0, 0);
    glVertex3f(+1, 0, +1);
    glVertex3f(+1, 0, -1);
    glVertex3f(+1, +1, -1);
    glVertex3f(+1, +1, +1);
    //  Left
    glNormal3f(-1, 0, 0);
    glVertex3f(-1, 0, -1);
    glVertex3f(-1, 0, +1);
    glVertex3f(-1, +1, +1);
    glVertex3f(-1, +1, -1);
    //  Top
    glNormal3f( 0,+1, 0);
    glVertex3f(-1, +1, +1);
    glVertex3f(+1, +1, +1);
    glVertex3f(+1, +1, -1);
    glVertex3f(-1, +1, -1);
    //  Bottom
    glNormal3f( 0,-1, 0);
    glVertex3f(-1, 0, -1);
    glVertex3f(+1, 0, -1);
    glVertex3f(+1, 0, +1);
    glVertex3f(-1, 0, +1);
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

/*
 *  Draw sky box
 */
static void Skybox(double D)
{
   //  Textured white box dimension (-D,+D)
   glPushMatrix();
   glScaled(D,D,D);
   glEnable(GL_TEXTURE_2D);
    glTranslatef(0,0,0);
   glColor3f(1,1,1);

   //  Sides
    if (Sin(zh)>0) {
        glBindTexture(GL_TEXTURE_2D,texture_daybox);
    } else {
        glBindTexture(GL_TEXTURE_2D,texture_nightbox);
    }
   glBegin(GL_QUADS);
   glTexCoord2f(0.25,0.34); glVertex3f(-1,-1,-1);
   glTexCoord2f(0.50,0.34); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.50,0.66); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.25,0.66); glVertex3f(-1,+1,-1);

   glTexCoord2f(0.50,0.34); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.75,0.34); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.75,0.66); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.50,0.66); glVertex3f(+1,+1,-1);

   glTexCoord2f(0.75,0.34); glVertex3f(+1,-1,+1);
   glTexCoord2f(1.00,0.34); glVertex3f(-1,-1,+1);
   glTexCoord2f(1.00,0.66); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.75,0.66); glVertex3f(+1,+1,+1);

   glTexCoord2f(0.00,0.34); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.25,0.34); glVertex3f(-1,-1,-1);
   glTexCoord2f(0.25,0.66); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.00,0.66); glVertex3f(-1,+1,+1);
   glEnd();

   //  Top and bottom
   glBegin(GL_QUADS);
   glTexCoord2f(0.50,0.66); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.50,1.00); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.25,1.00); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.25,0.66); glVertex3f(-1,+1,-1);

   glTexCoord2f(0.25,0.00); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.50,0.00); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.50,0.34); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.25,0.34); glVertex3f(-1,-1,-1);
   glEnd();

   //  Undo
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

/*
 *  Initialize flag
 */
void init_flag(void)
{
    for (int x=0; x<64; x++) {
        for (int y=0; y<64; y++) {
            flag[x][y][0] = (double) (x/3.0);
            flag[x][y][1] = (double) (y/5.0);
            flag[x][y][2] = (double) (sin((((x*45)/8.0)/360.0)*2.0*3.14159265));
        }
    }
}

/*
 *  Initialize screen
 */
static void init_screen(unsigned int *tex_list, const char * path, unsigned int count)
{
  unsigned int current = 0;
  unsigned int path_length = strlen(path);
  char full_path[path_length+10];

  for(current = 0; current < count; current++)
  {
    sprintf(full_path, "%s-%u.bmp", path, current + 1);
    *(tex_list + current) = LoadTexBMP(full_path);
  }
}

/*
 *  Draw flag
 */
void Flag(double* pos, double* angle, double size, double*rgb, const char * path, unsigned int count)
{
    unsigned int x, y;
    double state;
    static unsigned int *tex_pb_screen;
    static unsigned int current = 0;

    if(boolInit == 1)
    {
        init_flag();
        tex_pb_screen = (unsigned int *) malloc(sizeof(unsigned int) * count);
        init_screen(tex_pb_screen, path, count);
        boolInit += 1;
    }
    
    double pos_pole[] = {pos[0]-2.1,pos[1]-0.7,pos[2]};
    double rgb_pole[] = {0.800000, 0.764745, 0.456507};
    Cylinder(pos_pole, 0.05, 2.0, rgb_pole);
    Cylinder(pos_pole, 0.1, 0.1, rgb_pole);

    glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]);
    EulerRotation(angle[0], angle[1], angle[2]);
    glScalef(size, size, size);
    SetColor( rgb[0], rgb[1], rgb[2]);

//    glBindTexture(GL_TEXTURE_2D, texture_wall);
    glBindTexture(GL_TEXTURE_2D, *(tex_pb_screen + current));
    glShadeModel(GL_SMOOTH);

    for (x=0; x<63; x++)
    {
      for (y=0; y<63; y++)
      {
          glBegin(GL_QUADS);
          GL_NORMAL(flag[x][y][0], flag[x][y][1], flag[x][y][2], flag[x+1][y][0], flag[x+1][y][1], flag[x+1][y][2], flag[x][y+1][0], flag[x][y+1][1], flag[x][y+1][2]);
          glTexCoord2f(x/63.0, y/63.0);     glVertex3f(flag[x][y][0], flag[x][y][1], flag[x][y][2]);
          glTexCoord2f(x/63.0, (y+1)/63.0);   glVertex3f(flag[x][y+1][0], flag[x][y+1][1], flag[x][y+1][2]);
          glTexCoord2f((x+1)/63.0, (y+1)/63.0); glVertex3f(flag[x+1][y+1][0], flag[x+1][y+1][1], flag[x+1][y+1][2]);
          glTexCoord2f((x+1)/63.0, y/63.0);   glVertex3f(flag[x+1][y][0], flag[x+1][y][1], flag[x+1][y][2]);
          glEnd();
      }
    }
    glPopMatrix();
    
    if (move){
        for (y=0; y<64; y++)
        {
            state = flag[0][y][2];
            for (x=0; x<63; x++)
                flag[x][y][2] = flag[x+1][y][2];
            flag[63][y][2] = state;
        }
    }
    
    if ((int) timer%10 ==0 && move)
        current++;
    if(current == count)
        current = 0;
    
    double rgb_sphere[] = {242./245,239./245,19./245};
    if (colli_ball) Sphere (pos_pole, pos[3],rgb_sphere);
}


/*
 *  Draw Tree leaf
 */
void Tree_leaf (double x, double y, double z, double radius, double height) {
    const int d=5;
    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glTranslatef(x, y, z);
    glScalef(radius, height, radius);

    /*
     *  Draw first layer
     */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int th=0; th<=360; th+=d) {
    SetColor(0.256861,0.440506,0.110769);
    glNormal3d(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 0, Cos(th));
    }
    glEnd();

    /*
     *  Draw second layer
     */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 1, 0);
    for (int th=0; th<=360; th+=d) {
    glNormal3d(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 0, Cos(th));
    }
    glEnd();

    //  Undo transformations
    glPopMatrix();
}

/*
 *  Draw Tree
 */
void Tree1 (double x, double y, double z, double radius, double height) {
    double pos[] = {x,y,z};
    double rgb_treebody[] = {94./245, 52./245, 3./245};
    Cylinder(pos, radius, height, rgb_treebody);
    Tree_leaf(x, y+height/2, z, radius*5, height);
    Tree_leaf(x, y+height, z, radius*5/1.5, height/1.5);
    
    double pos_sphere[] = {x,0.0,z,radius};
    double rgb_sphere[] = {242./245,239./245,19./245};
    if (colli_ball) Sphere (pos_sphere, pos_sphere[3],rgb_sphere);
}

/*
 *  Draw Road
 */
void Road(double x, double y, double z, double length, double width, double angle1, double angle2, double angle3){

    int num = 20;
    double mul = 2.0/num;
    //  Save transformation
    glPushMatrix();
    //  shift, scale
    glTranslatef(x,y,z);
    EulerRotation(angle1,angle2,angle3);
    glTranslated(-length/2,0,-width/2);
    glScaled(length/num,1,width/num);
    glBindTexture(GL_TEXTURE_2D, texture_road);
    glColor3f(149./245, 160./245, 163./245);
    
    Surface(mul,num);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPopMatrix();
}

/*
 *  Draw Car
 */
void Car(double x, double y, double z, double width, double height, double depth, double angle, int obj){
    double pos_sphere[] = {x,y,z};
    double rgb_sphere[] = {242./245,239./245,19./245};
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(angle, 0, 1, 0);
    glScalef(width, height, depth);
    glCallList(obj);
    glPopMatrix();
    
    // when the user car pass through ufo beam, speed changes randomly
    double dis_vec[] = {pos_ufo[0]-pos_user_car[0],0,pos_ufo[2]-pos_user_car[2]};
    double dis = VectorNorm(dis_vec);
    if (dis<(pos_user_car[3]+pos_ufo[3])){
        pos_user_car[1] += 0.01;
        if (speedup==1 && speed_user_car>0 && speed_user_car<0.5) {
            // Generate a random number (0 or 1)
            int randomValue = rand() % 2;
            // Adjust the random value to be either +1 or -1
            int finalValue = (randomValue == 0) ? -1 : 1;
            speed_user_car2 += (double)finalValue * 0.05;
            speedup = 0;
        }
        else if (speedup==1 && speed_user_car>0.5) speed_user_car2 -= 0.05;
        else if (speedup==1 && speed_user_car<0.0) speed_user_car2 += 0.05;
    }
    else if(dis>=(pos_user_car[3]+pos_ufo[3]) && pos_user_car[1]>0.01){
        pos_user_car[1] -= 0.01;
        speedup = 1;
    }
    
    // car will loop in the map
    double signx;
    double signz;
    if (pos_user_car[0] >= 0) signx=1; else signx=-1;
    if (pos_user_car[2] >= 0) signz=1; else signz=-1;
    pos_user_car[0] = fmod(pos_user_car[0]+signx*(Ground_width/2),signx*Ground_width) - signx*(Ground_width/2);
    pos_user_car[2] = fmod(pos_user_car[2]+signz*(Ground_width/2),signz*Ground_width) - signz*(Ground_width/2);
    
    // draw collision ball
    if (colli_ball) Sphere (pos_sphere, width*2.5, rgb_sphere);
}

/*
 *  Draw Loop Car
 */
void Loop_Car(double x, double y, double z, double width, double height, double depth, double* angle, int obj){
    double pos_sphere[] = {x,y,z};
    double rgb_sphere[] = {242./245,239./245,19./245};
    glPushMatrix();
    glTranslatef(x,y,z);
    EulerRotation(angle[0],angle[1],angle[2]);
    EulerRotation(90,0,0);
    glScalef(width, height, depth);
    glCallList(obj);
    glPopMatrix();
    
    //when the loop car pass through ufo beam, loop car flies
    double dis_vec1[] = {pos_ufo[0]-pos_loop_car[0],0,pos_ufo[2]-pos_loop_car[2]};
    double dis1 = VectorNorm(dis_vec1);
    if (dis1<(pos_loop_car[3]+pos_ufo[3]))
        pos_loop_car[1] += 0.01;
    else if(dis1>=(pos_loop_car[3]+pos_ufo[3]) && pos_loop_car[1]>0.01)
        pos_loop_car[1] -= 0.01;
    
    // car loops in map
    if (pos_loop_car[0] < 0 && move){
        pos_loop_car[0] += 0.05;
    }
    if (pos_loop_car[0] > 0 && pos_loop_car[2] < 3 && move){
        ang_loop_car[0] = -90;
        pos_loop_car[2] += 0.05;
    }
    if (pos_loop_car[0]<-3 || pos_loop_car[0]>3 || pos_loop_car[2]<-3 || pos_loop_car[2]>3){
        pos_loop_car[0] = -3;
        pos_loop_car[2] = 0;
        ang_loop_car[0] = 0;
    }
    
    // draw collision ball
    if (colli_ball) Sphere (pos_sphere, pos_loop_car[3], rgb_sphere);
}

/*
 *  Draw Alarm light for Cop Car
 */
void Cop_Alarm(double x, double y, double z, double width, double height, double depth, double angle1, double angle2, double angle3) {
    SetColor(1,0,0);
    Cube(x+0.2,y,z, width,height,depth, angle1,angle2,angle3);
    SetColor(0,0,1);
    Cube(x-0.2,y,z, width,height,depth, angle1,angle2,angle3);
    
}

/*
 *  Draw Cop Car
 */
void Cop_Car(double x, double y, double z, double width, double height, double depth, double x_user, double y_user, double z_user, int obj){
    
    double pos_sphere[] = {x,y,z};
    double rgb_sphere[] = {242./245,239./245,19./245};
    
    double dx = x_user - x;
    double dy = y_user - y;
    double dz = z_user - z;
    //  Unit vector in direction of flght
    double D0 = sqrt(dx*dx+dy*dy+dz*dz);
    double X0 = dx/D0;
    double Y0 = dy/D0;
    double Z0 = dz/D0;
    //  Unit vector in "up" direction
    double X1 = 0.;
    double Y1 = 1.;
    double Z1 = 0.;
    //  Cross product gives the third vector
    double X2 = Y0*Z1-Y1*Z0;
    double Y2 = Z0*X1-Z1*X0;
    double Z2 = X0*Y1-X1*Y0;
    double M[16] = {X0,Y0,Z0,0 , X1,Y1,Z1,0 , X2,Y2,Z2,0 , 0,0,0,1};
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(90, 0, 1, 0);
    glMultMatrixd(M);
    glScalef(width, height, depth);
    glCallList(obj);
    Cop_Alarm(0, 1.6, 0, 0.2, 0.1, 0.1, 0, 0, 0);
    glPopMatrix();
    
    //when the cop car pass through ufo beam, cop car flies
    double dis_vec[] = {pos_ufo[0]-pos_cop_car[0],0,pos_ufo[2]-pos_cop_car[2]};
    double dis = VectorNorm(dis_vec);
    if (dis<(pos_cop_car[3]+pos_ufo[3]))
        pos_cop_car[1] += 0.01;
    else if(dis>=(pos_cop_car[3]+pos_ufo[3]) && pos_cop_car[1]>0.01)
        pos_cop_car[1] -= 0.01;
    
    // set the cop car's attitude, which always points to the user car
    double signx;
    double signz;
    if (pos_cop_car[0] >= 0) signx=1; else signx=-1;
    if (pos_cop_car[2] >= 0) signz=1; else signz=-1;
    pos_cop_car[0] = fmod(pos_cop_car[0]+signx*(Ground_width/2),signx*Ground_width) - signx*(Ground_width/2);
    pos_cop_car[2] = fmod(pos_cop_car[2]+signz*(Ground_width/2),signz*Ground_width) - signz*(Ground_width/2);
    
    // if (colli_ball) Sphere (x,y,z, width*2.5);
    if (colli_ball) Sphere (pos_sphere, width*2.5, rgb_sphere);
}

/*
 *  Draw street lamp
 */
void Street_Lamp(double pos[], double width, double height, double depth, double angle){
    double rgb_sphere[] = {242./245,239./245,19./245};
    glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]);
    glRotatef(angle, 0, 1, 0);
    glScalef(width, height, depth);
    glCallList(obj_street_lamp);
    glPopMatrix();
    
    if (colli_ball) Sphere (pos, pos[3], rgb_sphere);
}

/*
 *  Draw low-poly tree
 */
void Tree(double pos[], double width, double height, double depth, double angle){
    double rgb_sphere[] = {242./245,239./245,19./245};
    
    glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]);
    glRotatef(angle, 0, 1, 0);
    glScalef(width, height, depth);
    glCallList(obj_tree);
    glPopMatrix();
    
    if (colli_ball) Sphere (pos, pos[3],rgb_sphere);
}

/*
 *  Draw low-poly rocket
 */
void Rocket(double pos[], double width, double height, double depth, double angle){
    
    double rgb_sphere[] = {242./245,239./245,19./245};
    glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]);
    glRotatef(angle, 0, 1, 0);
    glScalef(width, height, depth);
    glCallList(obj_rocket);
    glPopMatrix();
    
    // rocket pad
    double pos_pad[] = {pos[0],0,pos[2]};
    double rgb_pad[] = {1.0,1.0,1.0};
    Cylinder(pos_pad, 0.5, 0.2, rgb_pad);
    
    // fly rocket
    if (timer>130 && pos_rocket[1]<5 && move)
        pos_rocket[1] += 0.01;
    else if (timer>30 && pos_rocket[1]>=5)
        pos_rocket[1] = 6;
    
    // collision ball
    if (colli_ball) Sphere (pos_pad, pos[3],rgb_sphere);
}

/*
 *  Draw grass ground
 */
void Ground(double length, double width, int num){
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);
    double mul = 2.0/num;
//    printf("length = %f, width = %f \n", length, width);
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture_grass);
    glTranslated(-length/2,0,-width/2);
    glScaled(length/2*mul,1,width/2*mul);
    Surface(mul,num);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPopMatrix();
    
    Cube(0,-2.05,0, width/2,2,length/2, 0,0,0);
}

/*
 *  Draw sun light
 */
void DrawLight(){
    //  Draw light position as sphere (still no lighting here)
    float Position[]  = {5*Cos(zh), 5*Sin(zh),Zlight,1.0};
    double pos_sphere[] = {5*Cos(zh), 5*Sin(zh),Zlight};
    double rgb_sphere[] = {242./245,239./245,19./245};
     //  Draw light position as ball (still no lighting here)
     Sphere(pos_sphere , 0.1, rgb_sphere);
    float white[] = {1,1,1,1};
//    float Direction[] = {Cos(th)*Sin(ph),0,Sin(th)*Sin(ph),-Cos(ph)};
    float Direction[] = {-5*Cos(zh), -5*Sin(zh),-2*Zlight,1.0};
    int at0=100;      //  Constant  attenuation %
    int at1=0;        //  Linear    attenuation %
    int at2=0;        //  Quadratic attenuation %
    float Exp=0;      //  Spot exponent
    float shiny = 0;
    float sco=180;    //  Spot cutoff angle
    float Emission[]  = {0.0,0.0,0.0,1.0};
    float Ambient[] = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[] = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[] = {0.01*specular,0.01*specular,0.01*specular,1.0};
    float Shinyness[] = {shiny};
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Location of viewer for specular calculations
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,0);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    //  Two sided mode
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    //  Set specular colors
    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
    glMaterialf(GL_FRONT,GL_SHININESS,shiny);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position);
    //  Set spotlight parameters
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);
    //  Set attenuation
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION ,at0/100.0);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION   ,at1/100.0);
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,at2/100.0);
    
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    
}

/*
 *  Draw lamp light
 */
void DrawLampLight0(double x, double y, double z){
    //  Draw light position as sphere (still no lighting here)
    float Position[]  = {x,y,z,1.0};
    float Direction[] = {0,-1,0};
    int at0=100;      //  Constant  attenuation %
    int at1=0;        //  Linear    attenuation %
    int at2=0;        //  Quadratic attenuation %
    float Exp=0;      //  Spot exponent
    float sco=50;    //  Spot cutoff angle
    float Ambient[] = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[] = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[] = {0.01*specular,0.01*specular,0.01*specular,1.0};
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Enable light 0
    glEnable(GL_LIGHT3);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT3,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT3,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT3,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT3,GL_POSITION,Position);
    //  Set spotlight parameters
    glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,Direction);
    glLightf(GL_LIGHT3,GL_SPOT_CUTOFF,sco);
    glLightf(GL_LIGHT3,GL_SPOT_EXPONENT,Exp);
    //  Set attenuation
    glLightf(GL_LIGHT3,GL_CONSTANT_ATTENUATION ,at0/100.0);
    glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION   ,at1/100.0);
    glLightf(GL_LIGHT3,GL_QUADRATIC_ATTENUATION,at2/100.0);
}

/*
 *  Draw lamp light
 */
void DrawLampLight1(double x, double y, double z){
    //  Draw light position as sphere (still no lighting here)
    float Position[]  = {x,y,z,1.0};
    float Direction[] = {0,-1,0};
    int at0=100;      //  Constant  attenuation %
    int at1=0;        //  Linear    attenuation %
    int at2=0;        //  Quadratic attenuation %
    float Exp=0;      //  Spot exponent
    float sco=50;    //  Spot cutoff angle
    float Ambient[] = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[] = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[] = {0.01*specular,0.01*specular,0.01*specular,1.0};
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Enable light 1
    glEnable(GL_LIGHT1);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT1,GL_POSITION,Position);
    //  Set spotlight parameters
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,Direction);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,sco);
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,Exp);
    //  Set attenuation
    glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,at0/100.0);
    glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,at1/100.0);
    glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,at2/100.0);
}

/*
 *  Draw UFO light
 */
void DrawUFOLight(double x, double y, double z){
    //  Draw light position as sphere (still no lighting here)
    float Position[]  = {x,y,z,1.0};
    float Direction[] = {0,-1,0};
    int at0=100;      //  Constant  attenuation %
    int at1=0;        //  Linear    attenuation %
    int at2=0;        //  Quadratic attenuation %
    float Exp=0;      //  Spot exponent
    float sco=50;    //  Spot cutoff angle
    float Ambient[] = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[] = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[] = {0.01*specular,0.01*specular,0.01*specular,1.0};
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Enable light 1
    glEnable(GL_LIGHT2);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT2,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT2,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT2,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT2,GL_POSITION,Position);
    //  Set spotlight parameters
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,Direction);
    glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,sco);
    glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,Exp);
    //  Set attenuation
    glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION ,at0/100.0);
    glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION   ,at1/100.0);
    glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,at2/100.0);
}

/*
 *  Draw Alien
 */
void Alien(double pos[], double size, double angle){
    glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]);
    glRotatef(angle, 0, 1, 0);
    glScalef(size, size, size);
//    SetColor(1.,0.,0.);
    glCallList(obj_alien);
    glPopMatrix();
}

/*
 *  Draw the UFO
 *     at (x,y,z) size s
 */
static void UFO(double* pos,  double size)
{
    int inc=15;
    const int d=5;
    double rgb_cone[] = {242./245,239./245,19./245};
    double pos_cone[] = {pos[0],  0,  pos[2]};
    double pos_alien[] = {pos[0],  3.25,  pos[2]};
    double pos_balllight0[] = {pos[0], 3.1, pos[2]+0.4};
    double pos_balllight1[] = {pos[0], 3.1, pos[2]-0.4};
    double pos_balllight2[] = {pos[0]+0.4, 3.1, pos[2]};
    double pos_balllight3[] = {pos[0]-0.4, 3.1, pos[2]};
    double rgb_balllight[] = {179./245,206./245,234./245,1.};
    
    Sphere (pos_balllight0, 0.05, rgb_balllight);
    Sphere (pos_balllight1, 0.05, rgb_balllight);
    Sphere (pos_balllight2, 0.05, rgb_balllight);
    Sphere (pos_balllight3, 0.05, rgb_balllight);
    Alien(pos_alien, size*2.5, 0);
    
    //  Save transformation
    glPushMatrix();
    glTranslated(pos[0],pos[1],pos[2]);
    glScaled(size,size,size);
    SetColor(157./250,157./250,157./250);
    
    //  Draw bottom
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,-1,0);
    glVertex3f(0,0,0);
    for (int th1=0;th1<=360;th1+=inc)
        glVertex3f(Cos(th1)*P[0].x , 0 , Sin(th1)*P[0].x);
    glEnd();
    //  Draw cylindrical base
    glBegin(GL_QUAD_STRIP);
    for (int th1=0;th1<=360;th1+=inc)
    {
        float c = Cos(th1);
        float s = Sin(th1);
        glNormal3f(c,0,s);
        glTexCoord2f(th1/360.0 ,   0   ); glVertex3f(c*P[0].x ,   0    , s*P[0].x);
        glTexCoord2f(th1/360.0 , P[0].y); glVertex3f(c*P[0].x , P[0].y , s*P[0].x);
    }
    glEnd();
    //  Draw the curved part of the UFO
    for (int i=0;i<Np-1;i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th1=0;th1<=360;th1+=inc)
        {
            float c = Cos(th1);
            float s = Sin(th1);
            glTexCoord2f(th1/360.0 ,P[i].y);
            glNormal3f(c*N[i].x,N[i].y,s*N[i].x);
            glVertex3f(c*P[i].x,P[i].y,s*P[i].x);
            
            glTexCoord2f(th1/360.0   ,P[i+1].y);
            glNormal3f(c*N[i+1].x,N[i+1].y,s*N[i+1].x);
            glVertex3f(c*P[i+1].x,P[i+1].y,s*P[i+1].x);
        }
        glEnd();
    }
    glPopMatrix();
    
    // draw collision ball
    double pos_sphere[] = {pos[0],0.0,pos[2],pos[3]};
    double rgb_sphere[] = {242./245,239./245,19./245};
    if (colli_ball) Sphere (pos_sphere, pos_sphere[3],rgb_sphere);
    
    // Draw UFO light
    if (daynight==1) DrawUFOLight(pos[0], pos[1]-2, pos[2]);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glDepthMask(0);
    
    //  Draw canopy
    glPushMatrix();
    //  Offset and scale
    glTranslatef(pos[0], pos[1]+0.2, pos[2]);
    glScalef(0.3, 0.3, 0.3);
    SetColor(179./245,206./245,234./245);
    
    float blue[] = {179./245,206./245,234./245,0.3};
    glColor4f(1,1,1,blue[3]);
//    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,blue);

    for (int ph1=-90; ph1<90; ph1+=d) {
      glBegin(GL_QUAD_STRIP);
      for (int th1=0; th1<=360; th1+=d) {
        double x_1 = Sin(th1)*Cos(ph1);
        double y_1 = Cos(th1)*Cos(ph1);
        double z_1 = Sin(ph1);
        glNormal3d(x_1, y_1, z_1);
        glVertex3d(x_1, y_1, z_1);

        double x_2 = Sin(th1)*Cos(ph1+d);
        double y_2 = Cos(th1)*Cos(ph1+d);
        double z_2 = Sin(ph1+d);
        glNormal3d(x_2, y_2, z_2);
        glVertex3d(x_2, y_2, z_2);
      }
      glEnd();
    }
    //  Undo transformations
    glPopMatrix();
    
    Cone (pos_cone, 1,  pos[1],  rgb_cone);
    
    glDepthMask(1);
    glDisable(GL_BLEND);
    
    
    glDisable(GL_POLYGON_OFFSET_FILL);
}

/*
 *  Draw Axes
 */
void DrawAxes(){
    //  Draw axes - no lighting from here on
    glColor3f(1,1,1);
    const double len=2.0;  //  Length of axes
    glBegin(GL_LINES);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(len,0.0,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,len,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,0.0,len);
    glEnd();
    //  Label axes
    glRasterPos3d(len,0.0,0.0);
    Print("X");
    glRasterPos3d(0.0,len,0.0);
    Print("Y");
    glRasterPos3d(0.0,0.0,len);
    Print("Z");
}

/*
 *  Draw Cockpit in first-person view
 */
void Cockpit(double* pos, double angle,double size){
    //  Save transformation
    glPushMatrix();
    //  shift, scale
    glTranslatef(pos[0],pos[1],pos[2]);
    EulerRotation(angle+180,0,0);
    glScalef(size, size, size);
    
    //  Draw instrument panel with texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_cockpit);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0);glVertex3f(-2,0,0);
    glTexCoord2d(1,0);glVertex3f(+2,0,0);
    glTexCoord2d(1,1);glVertex3f(+2, 0.8,0);
    glTexCoord2d(0,1);glVertex3f(-2, 0.8,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    // draw engin cover
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
    glVertex3f(-2,0.8,0);
    glVertex3f(+2,0.8,0);
    glVertex3f(+0.7, 0.9,0);
    glVertex3f(-0.7, 0.9,0);
    glEnd();
    
    // draw side rod
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex3f(-1.9,0.8,0.01);
    glVertex3f(-1.65,0.8,0.01);
    glVertex3f(-1.9, 3.0,0.01);
    glVertex3f(-2.1, 4.0,0.01);
    glEnd();
    
    // draw side rod
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex3f(1.7,0.8,0);
    glVertex3f(1.9,0.8,0);
    glVertex3f(2.1, 4.0,0);
    glVertex3f(1.9, 3.0,0);
    glEnd();
    
    // draw head cover
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex3f(-2,2.5,0);
    glVertex3f(+2,2.5,0);
    glVertex3f(+2, 4.0,0);
    glVertex3f(-2, 4.0,0);
    glEnd();
    
    if (wiper){
        angle_wiper += 1;
    }
    double r = 1.5;
    glColor3f(0,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.05,0.8,0);
    glVertex3f(+0.05,0.8,0);
    glVertex3f(r*Cos(angle_wiper), 0.8+abs(r*Sin(angle_wiper)),0);
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

/*
 *  Draw total county
 */
void CrimeCounty () {
    
    // draw objects with texture
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    Flag(pos_flag, ang_flag, siz_flag, rgb_flag, "bmp/playback/playback", 81);
    glEnable(GL_LIGHTING);
    Ground(Ground_width, Ground_width, 40);
    Road(0,0.01,0, 0.5,Ground_width, 0,0,0);
    Road(0,0.01,0, 0.5,Ground_width, 90,0,0);
    glDisable(GL_TEXTURE_2D);
    
    // draw objects without texture
    Tree1 (pos_tree0[0],pos_tree0[1],pos_tree0[2],pos_tree0[3], 1);
    Tree1 (pos_tree1[0],pos_tree1[1],pos_tree1[2],pos_tree1[3], 1);
    Tree1 (pos_tree2[0],pos_tree2[1],pos_tree2[2],pos_tree2[3], 0.5);
    Tree1 (pos_tree3[0],pos_tree3[1],pos_tree3[2],pos_tree3[3], 1);
    Tree1 (pos_tree4[0],pos_tree4[1],pos_tree4[2],pos_tree4[3], 0.75);
    Tree1 (pos_tree5[0],pos_tree5[1],pos_tree5[2],pos_tree5[3], 1.5);
    
    // draw low-poly objects
    Car(pos_user_car[0],pos_user_car[1],pos_user_car[2], 0.1,0.1,0.1, ang_user_car,obj_car);
    Cop_Car(pos_cop_car[0],pos_cop_car[1],pos_cop_car[2], 0.1,0.1,0.1, pos_user_car[0],pos_user_car[1],pos_user_car[2],obj_cop_car);
    Loop_Car(pos_loop_car[0],pos_loop_car[1],pos_loop_car[2], 0.1,0.1,0.1, ang_loop_car,obj_loopcar);
    Tree(pos_tree, 0.05,0.05,0.05, 0);
    Street_Lamp(pos_lamp, 0.1,0.1,0.1, 90);
    Street_Lamp(pos_lamp1, 0.1,0.1,0.1, 90);
    Rocket(pos_rocket, 0.2,0.2,0.2, 0);

    // draw UFO
    CalcNorm();
    UFO(pos_ufo, 0.1);
}

/*
 *  Set Orthogonal/Perpective/First-Person/Third-Person Mode
 */
void SetProject(int proj){
    
    // Orthogonal Mode (Never enter in)
    if (proj == 4){
        glRotatef(ph, 1, 0, 0);
        glRotatef(th, 0, 1, 0);
        //  Display parameters
       glWindowPos2i(5,5);
       Print("Time=%f,  Angle=%d,%d   Projection=%s",timer,th,ph,"Orthogonal");
    }
    // Perpective Mode
    else if (proj == 0){
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);
    }
    // Third person Mode
    else if (proj == 1){
        // Recalculate where the camera is looking
        double AX = Sin(fp_th);
        double AY = Sin(fp_ph);
        double AZ = Cos(fp_th);
        fp_x = pos_user_car[0] + 2*AX;
        fp_y = pos_user_car[1] + AY;
        fp_z = pos_user_car[2] + 2*AZ;
        gluLookAt(fp_x, fp_y, fp_z, pos_user_car[0], pos_user_car[1], pos_user_car[2], 0, 1, 0);
        
    }
    // First person Mode
    else if (proj == 2){
        // Recalculate where the camera is looking
        double AX = Sin(ang_user_car);
        double AY = Sin(0);
        double AZ = Cos(ang_user_car);
        fp_x = pos_user_car[0];
        fp_y = pos_user_car[1] + 0.3;
        fp_z = pos_user_car[2];
        // Orient the scene so it imitates first person movement
        gluLookAt(fp_x, fp_y, fp_z, AX + fp_x, AY + fp_y, AZ + fp_z, 0, 1, 0);
        
        // draw the cockpit only in first-person view
        double cockpit_pos[] = {pos_user_car[0] + 0.65*Sin(ang_user_car), pos_user_car[1], pos_user_car[2] + 0.65*Cos(ang_user_car)};
        double cockpit_size = 0.2;
        Cockpit(cockpit_pos, ang_user_car,cockpit_size);
    }
    
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //  Undo previous transformations
    glLoadIdentity();

    // Set prospective
    SetProject(proj);

    // Draw Box
    Skybox(5.0);

    // Draw Light
    if (daynight==0){
        DrawLight();
    }
    else{
        double pos_sphere1[] = {pos_lamp[0],pos_lamp[1]+1.3,pos_lamp[2]-0.7};
        double pos_sphere2[] = {pos_lamp1[0],pos_lamp1[1]+1.3,pos_lamp1[2]-0.7};
        double rgb_sphere[] = {242./245,239./245,19./245};
        
        Sphere(pos_sphere1 , 0.05, rgb_sphere);
        Sphere(pos_sphere2 , 0.05, rgb_sphere);
        DrawLampLight0(pos_sphere1[0], pos_sphere1[1], pos_sphere1[2]);
        DrawLampLight1(pos_sphere2[0], pos_sphere2[1], pos_sphere2[2]);
    }
    
    //  Draw the objects
    CrimeCounty();

    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    DrawAxes();

    //  Display parameters
    glWindowPos2i(5, 25);
    Print("Police Car Speed = %f, User Car Speed = %f",speed_cop_car,speed_user_car);
    glWindowPos2i(5,5);
    if (proj==0)
        Print("Time=%d  Angle=%d,%d FOV=%d   Projection=%s",(int)timer, th,ph,fov,"Perpective");
    else if (proj==1)
        Print("Time=%d  Angle=%d,%d FOV=%d   Projection=%s",(int)timer, fp_th,fp_ph,fov,"Third-Person");
    else if (proj==2)
        Print("Time=%d  Angle=%f,%d FOV=%d   Projection=%s",(int)timer, ang_user_car,0,fov,"First-Person");
    //  Display parameters
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
    
    // Police car points to the user car
    double dis_vec[] = {pos_user_car[0]-pos_cop_car[0],pos_user_car[1]-pos_cop_car[1],pos_user_car[2]-pos_cop_car[2]};
    double dis_cop =VectorNorm(dis_vec);
    if (dis_cop> (pos_user_car[3]+pos_cop_car[3]) && move) {
        double dis_norm =VectorNorm(dis_vec);
        pos_cop_car[0] += speed_cop_car*dis_vec[0]/dis_norm;
        pos_cop_car[2] += speed_cop_car*dis_vec[2]/dis_norm;
        zh = timer*time_speed;
    } else {
        move = 0;
    }

    // fly UFO
    pos_ufo[0] = 2.5*Cos(zh);
    pos_ufo[2] = 2.5*Sin(zh);

    // change day and night
    if (Sin(zh)<0)
        daynight = 1;
    else
        daynight = 0;

    // timer
    if(move) timer = timer + 1;

    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  Collision Detecttion
 */
int CollisionDetect(int front){
    
    
    double dir_user_car[] = {Sin(ang_user_car),0,Cos(ang_user_car)};
    int flag;
    // Set objects' position
    int obj_static_num = 12;
    double pos_static[12][4] = {{pos_loop_car[0],pos_loop_car[1],pos_loop_car[2],pos_loop_car[3]},
        {pos_tree[0],pos_tree[1],pos_tree[2],pos_tree[3]},
        {pos_lamp[0],pos_lamp[1],pos_lamp[2],pos_lamp[3]},
        {pos_lamp1[0],pos_lamp1[1],pos_lamp1[2],pos_lamp1[3]},
        {pos_tree0[0],pos_tree0[1],pos_tree0[2],pos_tree0[3]},
        {pos_tree1[0],pos_tree1[1],pos_tree1[2],pos_tree1[3]},
        {pos_tree2[0],pos_tree2[1],pos_tree2[2],pos_tree2[3]},
        {pos_tree3[0],pos_tree3[1],pos_tree3[2],pos_tree3[3]},
        {pos_tree4[0],pos_tree4[1],pos_tree4[2],pos_tree4[3]},
        {pos_tree5[0],pos_tree5[1],pos_tree5[2],pos_tree5[3]},
        {pos_flag[0]-2.1,pos_flag[1]-0.7,pos_flag[2],pos_flag[3]},
        {pos_rocket[0],0,pos_rocket[2],pos_rocket[3]}};
    
    // Detect Collision
    for(int i=0;i<obj_static_num;i++)
    {
        double dis_vec[] = {pos_static[i][0]-pos_user_car[0],pos_static[i][1]-pos_user_car[1],pos_static[i][2]-pos_user_car[2]};
        double dis = VectorNorm(dis_vec);
        double angle =VectorAngle(dir_user_car,dis_vec);
        
        if (front==1) {
            if (dis<=(pos_user_car[3]+pos_static[i][3]) && angle<=90){flag = 1; break;} else {flag = 0;}
        } else {
            if (dis<=(pos_user_car[3]+pos_static[i][3]) && angle>90){flag = 1; break;} else {flag = 0;}
        }
    }

    return flag;
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    if (proj == 0)
    {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
        th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
        th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
        ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
      ph -= 5;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
    }
    else if (proj == 1)
    {
      if (key == GLUT_KEY_RIGHT)
        fp_th -= 5;
      else if (key == GLUT_KEY_LEFT)
        fp_th += 5;
      else if (key == GLUT_KEY_UP)
        fp_ph += 5;
      else if (key == GLUT_KEY_DOWN)
        fp_ph -= 5;

      // constrain azimuth to [0,360]
      fp_th %= 360;
      // constrain elevation to [-90,90]
      if (fp_ph > 90) fp_ph = 90;
      if (fp_ph < -90) fp_ph = -90;
    }
    //  Tell GLUT it is necessary to redisplay the scene
    Project(proj,fov,asp,dim);
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
    //  Exit on ESC
    if (ch == 27)
      exit(0);
    //  Reset view angle
    else if (ch == '0'){
       th = ph = 20;
       pos_cop_car[0]= -2;
       pos_cop_car[2]= 2;
       pos_user_car[0] = 1;
       pos_user_car[2] = 0;
       speed_user_car = 0.2;
    }
    //  Toggle projection type
    else if (ch == 'p' || ch == 'P')
       proj = (proj + 1) % 3;
    //  Toggle light movement
    else if (ch == 'l' || ch == 'L')
      move = 1-move;
    else if (ch == 'r' || ch == 'R')
        wiper = 1-wiper;
    else if (ch == 'c' || ch == 'C')
       colli_ball = 1-colli_ball;
    else if (ch == 'v'){
       if (speed_cop_car>0) speed_cop_car -= 0.01;}
    else if (ch == 'V')
       speed_cop_car += 0.01;
    else if (ch == '+' && ch<179)
       fov++;
    else if (ch == '-' && ch>1)
       fov--;
    else if (ch == 'w' || ch == 'W'){
       int flag = CollisionDetect(1);
       if (flag==1) {speed_user_car=0;} else speed_user_car=speed_user_car2;
       pos_user_car[0] += speed_user_car*Sin(ang_user_car);
       pos_user_car[2] += speed_user_car*Cos(ang_user_car);
    }
    else if (ch == 's' || ch == 'S'){
       int flag = CollisionDetect(0);
       if (flag==1) {speed_user_car=0;} else speed_user_car=speed_user_car2;
       pos_user_car[0] -= speed_user_car*Sin(ang_user_car);
       pos_user_car[2] -= speed_user_car*Cos(ang_user_car);
    }
    else if (ch == 'a' || ch == 'A'){
       ang_user_car += 5;
    }
    else if (ch == 'd' || ch == 'D'){
       ang_user_car -= 5;
    }
    //  Reproject
    Project(proj,fov,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, RES*width,RES*height);
    //  Set projection
    Project(proj,fov,asp,dim);
}

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Projects");
    #ifdef USEGLEW
    //  Initialize GLEW
    if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
    #endif
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    //  Load object
    obj_car = LoadOBJ("./obj/Car.obj");
    obj_tree = LoadOBJ("./obj/Lowpoly_tree_sample.obj");
    obj_cop_car = LoadOBJ("./obj/cop_car.obj");
    obj_street_lamp = LoadOBJ("./obj/StreetLamp.obj");
    obj_alien = LoadOBJ("./obj/character_skeleton_archer_head.obj");
    obj_rocket = LoadOBJ("./obj/rocket.obj");
    obj_loopcar = LoadOBJ("./obj/Loop_Car.obj");
   //  Load textures
    texture_wall = LoadTexBMP("bmp/wall.bmp");
    texture_tree = LoadTexBMP("bmp/tree.bmp");
    texture_leaf = LoadTexBMP("bmp/leaf.bmp");
    texture_door = LoadTexBMP("bmp/door.bmp");
    texture_roof = LoadTexBMP("bmp/roof.bmp");
    texture_road = LoadTexBMP("bmp/road.bmp");
    texture_grass = LoadTexBMP("bmp/grass.bmp");
    texture_car_user = LoadTexBMP("bmp/car_police.bmp");
    texture_daybox = LoadTexBMP("bmp/grasscube.bmp");
    texture_nightbox = LoadTexBMP("bmp/nightcube.bmp");
    texture_ufo = LoadTexBMP("bmp/metalgray.bmp");
    texture_cockpit = LoadTexBMP("bmp/cockpit.bmp");

    ErrCheck("init");
    glutMainLoop();
    return 0;
}
