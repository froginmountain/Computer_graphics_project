#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>

#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib> // for random

float g_fDistance = -50.0f;
//float g_fDistance = -80.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;


static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

std::vector < glm::vec3 > vertices;
std::vector < glm::ivec3 > faces;
std::vector < glm::ivec3 > f_normals;
std::vector < glm::vec3 > normals;

std::vector < glm::vec3 > fire_vertices;
std::vector < glm::ivec3 > fire_faces;
std::vector < glm::ivec3 > fire_f_normals;
std::vector < glm::vec3 > fire_normals;

// 조명
GLfloat moon_light_amb[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat moon_light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat moon_light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat fire_light_amb[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat fire_light_diffuse[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat fire_light_specular[] = { 0.1, 0.1, 0.1, 1.0 };

float treemap[10][10] = { 0, };
float treemap_move[10][10] = { 0, };

bool LoadObj(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::ivec3>& out_faces,
    std::vector<glm::ivec3>& out_f_normals,
    std::vector<glm::vec3>& out_normals)
{
    // Initialize variables
    out_vertices.clear();
    out_faces.clear();
    out_f_normals.clear();
    out_normals.clear();

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file !\n");
        return false;
    }

    while (1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == -1)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            out_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            out_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::vector<unsigned int> vertexIndices;
            std::vector<unsigned int> uvIndices;
            std::vector<unsigned int> normalIndices;

            while (1) {
                int vertexIndex, textureIndex, normalIndex;
                int matches = fscanf(file, "%u//%u", &vertexIndex, &normalIndex);
                if (matches != 2) {
                    break;
                }
                //printf("%d//%d\n",vertexIndex, normalIndex);
                vertexIndices.push_back(vertexIndex);
                normalIndices.push_back(normalIndex);
            }

            if (vertexIndices.size() < 3) {
                std::cerr << "Error: Face what less than 3 vertices. something wrong!" << std::endl;
                fclose(file);
                return false;
            }
            //printf("vertexIndices size : %d\n", vertexIndices.size());
            
            for (size_t i = 0; i < (vertexIndices.size() - 3) + 1; i++) {
                out_faces.push_back(glm::ivec3(vertexIndices[0], vertexIndices[i+1], vertexIndices[i+2]));
                out_f_normals.push_back(glm::ivec3(normalIndices[0], normalIndices[i+1], normalIndices[i+2]));
                //out_faces.push_back(glm::ivec3(vertexIndices[i], vertexIndices[i + 1], vertexIndices[i+2]));
                //printf("%d %d %d\n", vertexIndices[0], vertexIndices[i + 1], vertexIndices[i + 2]);
            }
        }
    }

    fclose(file);
    return true;
}

void MyMouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
        }
        else
            bMousing = false;
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void MyMotion(int x, int y) {
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if (bMousing)
    {
        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
    }

    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;

    glutPostRedisplay();
}

void InitLight()
{
    glEnable(GL_LIGHTING);      //조명 활성화

    glEnable(GL_LIGHT0); //달 광원
    {
        GLfloat constant_attenuation = 1.0f;
        GLfloat linear_attenuation = 0.01f;
        GLfloat quadratic_attenuation = 0.001f;

        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constant_attenuation);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear_attenuation);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadratic_attenuation);
        glLightfv(GL_LIGHT0, GL_AMBIENT, moon_light_amb); //주변광 설정
        glLightfv(GL_LIGHT0, GL_DIFFUSE, moon_light_diffuse); //확산광 설정
        glLightfv(GL_LIGHT0, GL_SPECULAR, moon_light_specular); //반사광 설정
    }
    //glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1); // 모닥불 광원
    {
        GLfloat constant_attenuation = 0.1f;
        GLfloat linear_attenuation = 0.01f;
        GLfloat quadratic_attenuation = 0.0001f;

        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, constant_attenuation);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, linear_attenuation);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, quadratic_attenuation);
        glLightfv(GL_LIGHT1, GL_AMBIENT, fire_light_amb); //주변광 설정
        glLightfv(GL_LIGHT1, GL_DIFFUSE, fire_light_diffuse); //확산광 설정
        glLightfv(GL_LIGHT1, GL_SPECULAR, fire_light_specular); //반사광 설정
    }
    //glDisable(GL_LIGHT1);

    //glEnable(GL_LIGHT0);
    //glLightfv(GL_LIGHT0, GL_AMBIENT, fire_light_amb); //주변광 설정
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, fire_light_diffuse); //확산광 설정
    //glLightfv(GL_LIGHT0, GL_SPECULAR, fire_light_specular); //반사광 설정
    
}

void PrintNormals(const std::vector<glm::vec3>& normals) {
    std::cout << "Normals:" << std::endl;

    for (size_t i = 0; i < normals.size(); ++i) {
        const glm::vec3& normal = normals[i];
        std::cout << "Normal " << i << ": (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
    }
}

void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);    //구로 셰이딩
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST); // 깊이버퍼
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glFrontFace(GL_FRONT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 2000.0f);
    InitLight();
}

void DrawSurface(std::vector < glm::vec3 >& vectices,
    std::vector < glm::vec3 >& normals, std::vector<glm::ivec3>& f_normals,
    std::vector < glm::ivec3 >& faces)
{
    GLfloat tree_mat_amb[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat tree_mat_diff[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat tree_mat_specular[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat tree_shininess[] = { 80.0 };
    GLfloat no_emission[] = { 0.1, 0.1, 0.1, 0.0 };

    GLfloat tree_mat_amb2[] = { 0.545, 0.271, 0.075, 1.0 };
    GLfloat tree_mat_diff2[] = { 0.545, 0.271, 0.075, 1.0 };
    GLfloat tree_mat_specular2[] = { 0.545, 0.271, 0.075, 1.0 };
    GLfloat tree_shininess2[] = { 80.0 };

    glBegin(GL_TRIANGLES);

    glMaterialfv(GL_FRONT, GL_AMBIENT, tree_mat_amb2);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tree_mat_diff2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, tree_mat_specular2);
    glMaterialfv(GL_FRONT, GL_SHININESS, tree_shininess2);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_emission);
    for (int i = 1; i < faces.size(); i++) {
        if (i >= (faces.size() / 4)) {
            glMaterialfv(GL_FRONT, GL_AMBIENT, tree_mat_amb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, tree_mat_diff);
            glMaterialfv(GL_FRONT, GL_SPECULAR, tree_mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, tree_shininess);
        }
        glm::vec3 vtemp = vertices[faces[i][0] - 1];
        glm::vec3 ntemp = normals[f_normals[i][0] - 1];
        glVertex3f(vtemp.x, vtemp.y, vtemp.z);
        glNormal3f(ntemp.x, ntemp.y, ntemp.z);
        //printf("%d : %f %f %f\n", i, vtemp.x, vtemp.y, vtemp.z);
        //printf("%d : %f %f %f\n", i, ntemp.x, ntemp.y, ntemp.z);
        for (int j = 1; j < 3; j++) {
            glm::vec3 p = vertices[faces[i][j] - 1];
            glm::vec3 n = normals[f_normals[i][j] - 1];
            //printf("%d,%d : %f %f %f\n", i, j, p.x, p.y, p.z);
            //printf("%d,%d : %f %f %f\n", i, j, n.x, n.y, n.z);
            glVertex3f(p.x, p.y, p.z);
            glNormal3f(n.x, n.y, n.z);

        }
        // printf("i : %d\n", faces[1][2]);
    }
    glEnd();
}

void init_treemap() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            treemap[i][j] = rand() % 4;
        }
    }
    for (int i = 2; i < 8; i++) {
        for (int j = 5; j < 10; j++) {
            treemap[i][j] = 1.0f;
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%.0f ", treemap[i][j]);
        }
        printf("\n");
    }

    printf("---------------------\n");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            treemap_move[i][j] = (rand() % 10);
            printf("%.0f \0", treemap_move[i][j]);
        }
        printf("\n");
    }
}

void DrawTrees() {
    //메인 숲 (ㄷ자 모양)
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (treemap[i][j] != 1) {
                glPushMatrix();
                    glTranslatef(
                        (i - 5) * 4 + treemap_move[i][j],
                        0,
                        (j - 5) * 4 + treemap_move[i][j]
                    );
                    DrawSurface(vertices, normals, f_normals, faces);
                glPopMatrix();
            }
        }
    }

    ////하늘과 경계선 나무로 채우기
    for (int k = 0; k < 4; k++) {
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, (-35.0f + (k*5)));
            glPushMatrix();
            {
                for (int i = 0; i < 10; i++) {
                    glPushMatrix();
                    if (treemap[5-k][i] != 1) {
                        glTranslatef((i - 5) * 4 + treemap_move[5-k][i], 0.0, (i - 5) * 4 + treemap_move[5 - k][i]);
                        DrawSurface(vertices, normals, f_normals, faces);
                    }
                    glPopMatrix();
                }
            }
            glPopMatrix();
            glPushMatrix();
            {
                for (int i = 0; i < 10; i++) {
                    glPushMatrix();
                    if (treemap[k][i] != 1) {
                        glTranslatef((i - 3) * 4 + treemap_move[k][i], 0.0, (i - 5) * 4 + treemap_move[5 - k][i]);
                        DrawSurface(vertices, normals, f_normals, faces);
                    }
                    glPopMatrix();
                }
            }
            glPopMatrix();
            glPushMatrix();
            {
                for (int i = 0; i < 10; i++) {
                    glPushMatrix();
                    if (treemap[k][i] != 1) {
                        glTranslatef((i-8) * 4 + treemap_move[8-k][i], 0.0, (i - 5) * 4 + treemap_move[5 - k][i]);
                        DrawSurface(vertices, normals, f_normals, faces);
                    }
                    glPopMatrix();
                }
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
}

double temp = 0;

void render(void) {
    GLfloat Moon_LightPosition[] = { 0.0, 20.0, -70.0, 1.0 };
    GLfloat Fire_LightPosition[] = { 0.0, -5.0, 10.0, 1.0 };
    GLfloat origin_position[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat no_mat_amb[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat no_mat_diff[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat no_mat_specular[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat no_shininess[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat no_mat_emission[] = { 0.0, 0.0, 0.0, 0.0 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    {
        //달 그리기
        glTranslatef(0.0f, 20.0f, -20.0f);
        glLightfv(GL_LIGHT0, GL_POSITION, Moon_LightPosition);

        glColor3f(0.5, 0.5, 0.0);  // 노란색
        // 재질 속성 설정
        GLfloat moon_mat_amb[] = { 0.3, 0.3, 0.3, 1.0 };
        GLfloat moon_mat_diff[] = { 0.7, 0.7, 0.7, 1.0 };
        GLfloat moon_mat_specular[] = { 0,0,0, 1.0 };
        GLfloat moon_shininess[] = { 0.0 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_amb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, moon_shininess);

        glutSolidSphere(10, 100, 100);
    }
    glPopMatrix();

    glColor3f(0.5, 0.7, 0.5);
    glPushMatrix();
    {
        glTranslatef(0.0f, -5.0f, 0.0f);
        glRotated(0.0f, 1.0f, 0.0f, 0.0f);
        //glRotated(30.0f, 1.0f, 0.0f, 0.0f);
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, 15.0f);
            //캠프파이어 그리기
            {
                GLfloat modelviewMatrix[16];
                glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
                Fire_LightPosition[0] = modelviewMatrix[3];
                Fire_LightPosition[1] = modelviewMatrix[7];
                Fire_LightPosition[2] = modelviewMatrix[11];
                Fire_LightPosition[3] = modelviewMatrix[15];
            }
            glutSolidSphere(0.5, 5, 5);
            glLightfv(GL_LIGHT1, GL_POSITION, Fire_LightPosition);

        }
        glPopMatrix();
        //나무 그리기
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, 5.0f);
            DrawTrees();
            //DrawSurface(vertices, normals, f_normals, faces);
        }
        glPopMatrix();
        //땅 그리기
        //glColor3f(0, 0.1, 0);
        glTranslatef(0.0f, -35.0f, 0.0f);
        glutSolidCube(70);
        /*glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 1.0, 0.0);
        glEnd();*/
    }
    glPopMatrix();
    //출력

    glEnable(GL_FOG);
    {
        GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1 };
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.5);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, 10.0);
        glFogf(GL_FOG_END, 20.0);
        glFogf(GL_FOG_START, 30.0);
        glFogf(GL_FOG_END, 100.0);
    }

    glutSwapBuffers();
}



void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 1.0, 0.0);
}

void printAll() {
    printf("-----------vertices : %d-----------\n", vertices.size());
    printf("-----------normals : %d-----------\n", normals.size());
    printf("-----------f_normals : %d-----------\n", f_normals.size());
    printf("-----------faces : %d-----------\n", faces.size());
    /*printf("-----------vertices-----------\n");
    for (int i = 0; i < vertices.size(); i++) {
        printf("%lf, %lf, %lf \n",vertices[i][0], vertices[i][1], vertices[i][2]);
    }*/
    /*printf("-----------normals : %d-----------\n",normals.size());
    for (int i = 0; i < normals.size(); i++) {
        printf("%lf, %lf, %lf \n", normals[i][0], normals[i][1], normals[i][2]);
    }*/
    /*printf("-----------faces : %d-----------\n",faces.size());
    for (int i = 1; i < faces.size(); i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", faces[i][j]);
        }
        printf("\n");
    }*/
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("195848_이대권");
    init_treemap();
    init();

    glClearColor(0.0, 0.0, 0.16, 1.0); //색 초기화 및 뒷배경 설정

    //LoadObj("Data/bunny/bunny.obj", vertices, faces, uvs, normals);
    LoadObj("Lee/tree.obj", vertices, faces, f_normals, normals);
    //LoadPly("Data/bunny/bun_zipper_res4.ply", vertices, faces, normals);

    printAll();

    PrintNormals(normals);
    glutDisplayFunc(render);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);

    glutMainLoop();
}