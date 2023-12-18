#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "include/glaux.h"

#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>


using namespace std;
using namespace glm;

static int Day = 0, Time = 0;
vec2 preMouse, currentMouse;
int windowHeight, windowWidth;
float plusangle = 0;

GLuint g_textureID[2];
void loadTexture() {
    //��η� ���� �̹��� ���� �ҷ�����
    AUX_RGBImageRec* pEarthImage = auxDIBImageLoad("history/Earth.bmp");
    AUX_RGBImageRec* pSunImage = auxDIBImageLoad("history/Sun.bmp");
    if (pEarthImage != NULL && pSunImage != NULL) {
        //�ؽ��� ���� (�ؽ��ļ�, �ؽ��� ID �迭 �ּ�)
        glGenTextures(2, &g_textureID[0]);
        //�ؽ��� �Ķ���� ������ ���� �κ� (0�� �ؽ��� ID ����)
        glBindTexture(GL_TEXTURE_2D, g_textureID[0]);
        //�ؽ��� �Ķ���� ����: �ؽ��� Ȯ�� ��ҽ� ���� ó�� ��� ����
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //�ҷ��� �̹��� ������ ���� ������ �ؽ���ID�� ����
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pEarthImage->sizeX, pEarthImage->sizeY, 0,
            GL_RGB, GL_UNSIGNED_BYTE, pEarthImage->data);
        //�ؽ��� �Ķ���� ������ ���� �κ� (1�� �ؽ��� ID ����)
        glBindTexture(GL_TEXTURE_2D, g_textureID[1]);
        //�ؽ��� �Ķ���� ����: �ؽ��� Ȯ�� ��ҽ� ���� ó�� ��� ����
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //�ҷ��� �̹��� ������ ���� ������ �ؽ���ID�� ����
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pSunImage->sizeX, pSunImage->sizeY, 0,
            GL_RGB, GL_UNSIGNED_BYTE, pSunImage->data);
        //�ҷ��� �ؽ��� ���� ������ ����
        if (pEarthImage->data)
            free(pEarthImage->data);
        if (pSunImage->data)
            free(pSunImage->data);
        free(pEarthImage);
        free(pSunImage);
    }
}

//define camera class
class camera {
public:
    vec3 eye; //center position
    vec3 at; //point where this camera is pointing out
    vec3 up; // up direction (normalized)
};

camera myCamera;

void InitLight() {
    GLfloat sun_light_amb[] = { 0.5, 0, 0, 1.0 };
    GLfloat sun_light_diffuse[] = { 1, 0.5, 0.5, 1.0 };
    GLfloat sun_light_specular[] = { 1, 1, 1, 1.0 };

    GLfloat moon_light_amb[] = { 0.5, 0.5, 0, 1.0 };
    GLfloat moon_light_diffuse[] = { 1, 1, 0.5, 1.0 };
    GLfloat moon_light_specular[] = { 1, 1, 1, 1.0 };

    glShadeModel(GL_SMOOTH); //���� ���̵�
    glEnable(GL_DEPTH_TEST); //���� ���� Ȱ��ȭ
    glEnable(GL_LIGHTING); //���� Ȱ��ȭ

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_amb); //�ֺ��� ����
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse); //Ȯ�걤 ����
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular); //���ݻ籤 ����
    //glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, moon_light_amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, moon_light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, moon_light_specular);
}

void DrawSolarSystem() {
    //���� ����
    //�¾�
    GLfloat sun_mat_amb[] = { 0.2, 0 , 0, 1.0 };
    GLfloat sun_mat_diffuse[] = { 1, 0.5, 0.5, 1.0 };
    GLfloat sun_mat_specular[] = { 0, 0, 0, 1 };
    GLfloat sun_mat_emission[] = { 0.3, 0.1, 0.1, 0.0 };
    //��
    GLfloat moon_mat_amb[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat moon_mat_diff[] = { 0.5, 0.5, 0.1, 1.0 };
    GLfloat moon_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat moon_shininess[] = { 100.0 };
    GLfloat moon_mat_emission[] = { 0.3, 0.3, 0.1, 0.0 };
    //����
    GLfloat earth_mat_amb[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat earth_mat_diff[] = { 0.1, 0.1, 0.8, 1.0 };
    GLfloat earth_mat_specular[] = { 0.5, 0.5, 1.0, 1.0 };
    GLfloat earth_shininess[] = { 80.0 };

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat no_shininess[] = { 0.0 };
    GLfloat no_emission[] = { 0.3, 0.2, 0.2, 0.0 };

    //get camera variables from camera class
    vec3 eye = myCamera.eye;
    vec3 at = myCamera.at;
    vec3 up = myCamera.up;

    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat LightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat moonPosition[4];

    //draw solar system
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glColor3f(1.0, 0.3, 0.3);

    glBindTexture(GL_TEXTURE_2D, g_textureID[1]);
    GLUquadric* obj = gluNewQuadric();
    gluQuadricTexture(obj, GL_TRUE);
    gluQuadricNormals(obj, GLU_SMOOTH);
    gluSphere(obj, 0.2, 200, 160);
    //gluDeleteQuadric(obj);
    
    

    //glutSolidSphere(0.2, 200, 160);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    //glPushMatrix();
    glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);
    glTranslatef(0.7, 0.0, 0.0);
    glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
    //glColor3f(0.5, 0.6, 0.7);
    glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, earth_shininess);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_emission);
    glBindTexture(GL_TEXTURE_2D, g_textureID[0]);

    //GLUquadric* obj = gluNewQuadric();
    gluQuadricTexture(obj, GL_TRUE);
    gluQuadricNormals(obj, GLU_SMOOTH);
    gluSphere(obj, 0.1, 100, 80);
    gluDeleteQuadric(obj);

    //glutSolidSphere(0.1, 100, 80);
    glPushMatrix();
    glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
    glTranslatef(0.2, 0.0, 0.0);
    GLfloat modelviewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    moonPosition[0] = modelviewMatrix[3];
    moonPosition[1] = modelviewMatrix[7];
    moonPosition[2] = modelviewMatrix[11];
    moonPosition[3] = modelviewMatrix[15];
    glLightfv(GL_LIGHT1, GL_POSITION, moonPosition);
    glColor3f(0.9, 0.8, 0.2);
    glutSolidSphere(0.04, 100, 80);
    glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
    glMaterialfv(GL_FRONT, GL_SHININESS, moon_shininess);
    glPopMatrix();
    glPopMatrix();
}

void MyDisplay() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //set Identity
    gluPerspective(20, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1, 100);
    glMatrixMode(GL_MODELVIEW); //set matrix mode
    glLoadIdentity(); //set Identity
    glEnable(GL_TEXTURE_2D);
    DrawSolarSystem();
    glutSwapBuffers();
}


void MyReshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;

    //Set viewport
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void MyTimer(int Value) {
    Day = (Day + 2) % 360;
    Time = (Time + 1) % 360;
    plusangle = 1.0f;

    // ȸ���� ���� ������ �������� ��ȯ
    float angle = plusangle * glm::pi<float>() / 180.0f;

    // ȸ�� ��� ����
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 up = myCamera.up;
    up = glm::vec3(rotation * glm::vec4(up, 0.0f));
    myCamera.up = glm::normalize(up);

    // at�� �߽����� eye�� ȸ����Ŵ
    glm::vec4 eyePosition = glm::vec4(myCamera.eye - myCamera.at, 1.0f);
    eyePosition = rotation * eyePosition;
    myCamera.eye = glm::vec3(eyePosition) + myCamera.at;


    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); //�ʱ� ����
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //â ���
    glutInitWindowSize(500, 500); //â ũ�� ����
    glutInitWindowPosition(0, 0); //â �ʱ� ��ġ ����
    glutCreateWindow("195848"); //â �̸� ����

    //glClearColor(0.0, 0.0, 0.0, 1.0); //�� �ʱ�ȭ �� �޹�� ����
    windowWidth = glutGet(GLUT_WINDOW_WIDTH); //â ���� ũ��
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT); //â ���� ũ��

    loadTexture();

    //Init camera
    vec3 center(0, 2, 4);
    vec3 at(0.2, 0, 0);
    vec3 up = cross(vec3(1, 0, 0), at - center);
    up = normalize(up);
    myCamera.eye = center;
    myCamera.at = at;
    myCamera.up = up;

    InitLight();
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutTimerFunc(40, MyTimer, 1);
    glutMainLoop();
    return 0;
}