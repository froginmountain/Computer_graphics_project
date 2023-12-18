////#include <GL/glew.h>
////#include <GL/freeglut.h>
////#include <GL/GL.h>
////#include <GL/GLU.h>
////#include <GL/glut.h>
////
////#include <vector>
////#include <glm/glm.hpp>
////
////using namespace std;
////using namespace glm;
////
////void drawCoordinateAxes() {
////	// X 축 (빨간색)
////	glColor3f(1.0, 0.0, 0.0);
////	glBegin(GL_LINES);
////	glVertex3f(-1.0, 0.0, 0.0); // 시작점
////	glVertex3f(1.0, 0.0, 0.0);  // 끝점
////	glEnd();
////
////	// Y 축 (녹색)
////	glColor3f(0.0, 1.0, 0.0);
////	glBegin(GL_LINES);
////	glVertex3f(0.0, -1.0, 0.0); // 시작점
////	glVertex3f(0.0, 1.0, 0.0);  // 끝점
////	glEnd();
////
////	// Z 축 (파란색)
////	glColor3f(0.0, 0.0, 1.0);
////	glBegin(GL_LINES);
////	glVertex3f(0.0, 0.0, -1.0); // 시작점
////	glVertex3f(0.0, 0.0, 1.0);  // 끝점
////	glEnd();
////}
////
////void DrawBox(vector<vec4> points, vec3 color)
////{
////	glColor3f(color[0], color[1], color[2]);
////	glBegin(GL_LINES);
////
////	for (int i = 0; i < 3; i++)
////	{
////		glVertex3f(points[i][0], points[i][1], 0);
////		glVertex3f(points[i + 1][0], points[i + 1][1], 0);
////	}
////
////	glVertex3f(points[3][0], points[3][1], 0);
////	glVertex3f(points[0][0], points[0][1], 0);
////
////	glEnd();
////}
////
////void display()
////{
////	glMatrixMode(GL_MODELVIEW);
////	float size = 0.2;
////	vector<vec4> points;
////	points.push_back(vec4(-size / 2, -size / 2, 0, 1));
////	points.push_back(vec4(-size / 2, size / 2, 0, 1));
////	points.push_back(vec4(size / 2, size / 2, 0, 1));
////	points.push_back(vec4(size / 2, -size / 2, 0, 1));
////
////	glClear(GL_COLOR_BUFFER_BIT);
////	glLoadIdentity();
////
////	// 물체 변환과 같은 순서로 좌표계 이동
////	glPushMatrix();
////
////	// 1. 원점을 중심으로 하는 각 변 길이 0.2인 정사각형을 흰색으로 그린다.
////	DrawBox(points, vec3(1.0, 1.0, 1.0));
////
////	// 2. (0.3, 0) 방향으로 이동하고 빨간색으로 그린다. 
////	glTranslatef(0.0, 0.4, 0.0);
////	DrawBox(points, vec3(1.0, 0.0, 0.0));
////
////	// 3. z축을 중심으로 30도 회전하고 파란색으로 그린다.
////	glRotatef(30.0, 0.0, 0.0, 1.0);
////	DrawBox(points, vec3(0.0, 0.0, 1.0));
////
////	// 4. (0, 0.4) 방향으로 이동하고 초록색으로 그린다.
////	glTranslatef(0.3, 0.0, 0.0);
////	DrawBox(points, vec3(0.0, 1.0, 0.0));
////
////	glPopMatrix();
////
////	glFlush();
////}
////
////
////int main(int argc, char** argv)
////{
////	glutInit(&argc, argv);
////	glutInitDisplayMode(GLUT_RGB);
////	glutInitWindowSize(500, 500);
////	glutCreateWindow("195848");
////	glutDisplayFunc(display);
////	glutMainLoop();
////	return 0;
////}
////
////--------------------------------------------------------------------
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//
//#include <vector>
//#include <glm/glm.hpp> 
//#include <glm/gtc/matrix_transform.hpp> 
//#include <glm/gtc/type_ptr.hpp>
//
//
//using namespace std;
//using namespace glm;
//
//static int Day = 0, Time = 0;
//vec2 preMouse, currentMouse;
//int windowHeight, windowWidth;
//float plusangle = 0;
//
//
////define camera class
//class camera {
//public:
//    vec3 eye; //center position
//    vec3 at; //point where this camera is pointing out
//    vec3 up; // up direction (normalized)
//};
//
//camera myCamera;
//
//void InitLight() {
//    GLfloat sun_light_amb[] = { 0.5, 0, 0, 1.0 };
//    GLfloat sun_light_diffuse[] = { 1, 0.5, 0.5, 1.0 };
//    GLfloat sun_light_specular[] = { 1, 1, 1, 1.0 };
//
//    GLfloat moon_light_amb[] = { 0.5, 0.5, 0, 1.0 };
//    GLfloat moon_light_diffuse[] = { 1, 1, 0.5, 1.0 };
//    GLfloat moon_light_specular[] = { 1, 1, 1, 1.0 };
//
//    glShadeModel(GL_SMOOTH); //구로 셰이딩
//    glEnable(GL_DEPTH_TEST); //깊이 버퍼 활성화
//    glEnable(GL_LIGHTING); //조명 활성화
//
//    glEnable(GL_LIGHT0);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_amb); //주변광 설정
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse); //확산광 설정
//    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular); //정반사광 설정
//    //glDisable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
//    glLightfv(GL_LIGHT1, GL_AMBIENT, moon_light_amb);
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, moon_light_diffuse);
//    glLightfv(GL_LIGHT1, GL_SPECULAR, moon_light_specular);
//}
//
//void DrawSolarSystem() {
//    //재질 설정
//    //태양
//    GLfloat sun_mat_amb[] = { 0.2, 0 , 0, 1.0 };
//    GLfloat sun_mat_diffuse[] = { 1, 0.5, 0.5, 1.0 };
//    GLfloat sun_mat_specular[] = { 0, 0, 0, 1 };
//    GLfloat sun_mat_emission[] = { 0.3, 0.1, 0.1, 0.0 };
//    //달
//    GLfloat moon_mat_amb[] = { 0.1, 0.1, 0.1, 1.0 };
//    GLfloat moon_mat_diff[] = { 0.5, 0.5, 0.1, 1.0 };
//    GLfloat moon_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat moon_shininess[] = { 100.0 };
//    GLfloat moon_mat_emission[] = { 0.3, 0.3, 0.1, 0.0 };
//    //지구
//    GLfloat earth_mat_amb[] = { 0.1, 0.1, 0.1, 1.0 };
//    GLfloat earth_mat_diff[] = { 0.1, 0.1, 0.8, 1.0 };
//    GLfloat earth_mat_specular[] = { 0.5, 0.5, 1.0, 1.0 };
//    GLfloat earth_shininess[] = { 80.0 };
//
//    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
//    GLfloat no_shininess[] = { 0.0 };
//    GLfloat no_emission[] = { 0.3, 0.2, 0.2, 0.0 };
//
//    //get camera variables from camera class
//    vec3 eye = myCamera.eye;
//    vec3 at = myCamera.at;
//    vec3 up = myCamera.up;
//
//    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    GLfloat LightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
//    GLfloat moonPosition[4];
//
//    //draw solar system
//    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
//    glColor3f(1.0, 0.3, 0.3);
//    glutSolidSphere(0.2, 200, 160);
//    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_amb);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
//    glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
//    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
//    glPushMatrix();
//    glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);
//    glTranslatef(0.7, 0.0, 0.0);
//    glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
//    glColor3f(0.5, 0.6, 0.7);
//    glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_amb);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diff);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, earth_shininess);
//    //glMaterialfv(GL_FRONT, GL_SHININESS, no_emission);
//    glutSolidSphere(0.1, 100, 80);
//    glPushMatrix();
//    glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
//    glTranslatef(0.2, 0.0, 0.0);
//    GLfloat modelviewMatrix[16];
//    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
//    moonPosition[0] = modelviewMatrix[3];
//    moonPosition[1] = modelviewMatrix[7];
//    moonPosition[2] = modelviewMatrix[11];
//    moonPosition[3] = modelviewMatrix[15];
//    glLightfv(GL_LIGHT1, GL_POSITION, moonPosition);
//    glColor3f(0.9, 0.8, 0.2);
//    glutSolidSphere(0.04, 100, 80);
//    glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_amb);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diff);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
//    glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
//    glMaterialfv(GL_FRONT, GL_SHININESS, moon_shininess);
//    glPopMatrix();
//    glPopMatrix();
//}
//
//void MyDisplay() {
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(20, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1, 100);
//    glMatrixMode(GL_MODELVIEW); //set matrix mode
//    glLoadIdentity(); //set Identity
//    DrawSolarSystem();
//    glutSwapBuffers();
//}
//
//
//void MyReshape(int w, int h) {
//    windowWidth = w;
//    windowHeight = h;
//
//    //Set viewport
//    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//}
//
//void MyTimer(int Value) {
//    Day = (Day + 2) % 360;
//    Time = (Time + 1) % 360;
//    plusangle = 1.0f;
//
//    // 회전을 위한 각도를 라디안으로 변환
//    float angle = plusangle * glm::pi<float>() / 180.0f;
//
//    // 회전 행렬 생성
//    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
//
//    glm::vec3 up = myCamera.up;
//    up = glm::vec3(rotation * glm::vec4(up, 0.0f));
//    myCamera.up = glm::normalize(up);
//
//    // at을 중심으로 eye를 회전시킴
//    glm::vec4 eyePosition = glm::vec4(myCamera.eye - myCamera.at, 1.0f);
//    eyePosition = rotation * eyePosition;
//    myCamera.eye = glm::vec3(eyePosition) + myCamera.at;
//
//
//    glutPostRedisplay();
//    glutTimerFunc(40, MyTimer, 1);
//}
//
//int main(int argc, char** argv) {
//    glutInit(&argc, argv); //초기 설정
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //창 모드
//    glutInitWindowSize(500, 500); //창 크기 설정
//    glutInitWindowPosition(0, 0); //창 초기 위치 설정
//    glutCreateWindow("195848"); //창 이름 설정
//
//    //glClearColor(0.0, 0.0, 0.0, 1.0); //색 초기화 및 뒷배경 설정
//    windowWidth = glutGet(GLUT_WINDOW_WIDTH); //창 가로 크기
//    windowHeight = glutGet(GLUT_WINDOW_HEIGHT); //창 세로 크기
//
//    //Init camera
//    vec3 center(0, 2, 4);
//    vec3 at(0.2, 0, 0);
//    vec3 up = cross(vec3(1, 0, 0), at - center);
//    up = normalize(up);
//    myCamera.eye = center;
//    myCamera.at = at;
//    myCamera.up = up;
//
//    InitLight();
//    glutDisplayFunc(MyDisplay);
//    glutReshapeFunc(MyReshape);
//    glutTimerFunc(40, MyTimer, 1);
//    glutMainLoop();
//    return 0;
//}


//-----------------------------------------------------------------------11/30
//#define _CRT_SECURE_NO_WARNINGS
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <stdio.h>
//#include <iostream>
//
//
//#include <vector>
//#include <glm/glm.hpp> 
//#include <glm/gtc/matrix_transform.hpp> 
//#include <glm/gtc/type_ptr.hpp>
//
//
//float g_fDistance = -4.5f;
//float g_fSpinX = 0.0f;
//float g_fSpinY = 0.0f;
//
//
//static POINT ptLastMousePosit;
//static POINT ptCurrentMousePosit;
//static bool bMousing;
//
//std::vector < glm::vec3 > vertices;
//std::vector < glm::ivec3 > faces;
//std::vector < glm::vec2 > uvs;
//std::vector < glm::vec3 > normals;
//
//
//// 조명
//GLfloat light_amb[] = { 0.5, 0.5, 0.5, 1.0 };
//GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
//GLfloat light_specular[] = { 1, 1, 1, 1.0 };
//
//
//
//bool LoadObj(const char* path,
//    std::vector < glm::vec3 >& out_vertices,
//    std::vector < glm::ivec3 >& out_faces,
//    std::vector < glm::vec2 >& out_uvs,
//    std::vector < glm::vec3 >& out_normals)
//{
//    //init variables
//    out_vertices.clear();
//    out_faces.clear();
//    out_uvs.clear();
//    out_normals.clear();
//
//    FILE* file = fopen(path, "r");
//    if (file == NULL) {
//        printf("Impossible to open the file !\n");
//        return false;
//    }
//
//    while (1) {
//        char lineHeader[128];
//        // read the first word of the line
//        int res = fscanf(file, "%s", lineHeader);
//        if (res == -1)
//            break;
//
//        if (strcmp(lineHeader, "v") == 0) {
//            glm::vec3 vertex;
//            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
//            out_vertices.push_back(vertex);
//        }
//        else if (strcmp(lineHeader, "vt") == 0) {
//            glm::vec2 uv;
//            fscanf(file, "5f 5f\n", &uv.x, &uv.y);
//            out_uvs.push_back(uv);
//        }
//        else if (strcmp(lineHeader, "vn") == 0) {
//            glm::vec3 normal;
//            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
//            out_normals.push_back(normal);
//        }
//        else if (strcmp(lineHeader, "f") == 0) {
//            std::string vertex1, vertex2, vertex3;
//            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//            int matches = fscanf(file, "%d//%d %d//%d %d//%d\n",
//                &vertexIndex[0], &normalIndex[0],
//                &vertexIndex[1], &normalIndex[1],
//                &vertexIndex[2], &normalIndex[2]);
//
//            out_faces.push_back(glm::ivec3(vertexIndex[0] - 1, vertexIndex[1] - 1, vertexIndex[2] - 1));
//        }
//    }
//
//}
//
//
//
//bool LoadPly(const char* path,
//    std::vector < glm::vec3 >& out_vertices,
//    std::vector < glm::ivec3 >& out_faces,
//    std::vector < glm::vec3 >& out_normals)
//{
//    FILE* file = fopen(path, "r");
//    if (file == NULL) {
//        printf("Impossible to open the file !\n");
//        return false;
//    }
//
//    //init variables
//    out_vertices.clear();
//    out_faces.clear();
//    out_normals.clear();
//
//    int nbVertices = 0;
//    int nbFaces = 0;
//
//    while (1) {
//        char lineHeader1[128];
//        char lineHeader2[128];
//        // read the first word of the line
//        int res = fscanf(file, "%s", lineHeader1);
//        if (res == -1)
//            break;
//        if (strcmp(lineHeader1, "element") == 0) {
//            fscanf(file, "%s", lineHeader2);
//            if (strcmp(lineHeader2, "vertex") == 0) {
//                fscanf(file, "%d\n", &nbVertices);
//            }
//            else if (strcmp(lineHeader2, "face") == 0) {
//                fscanf(file, "%d\n", &nbFaces);
//            }
//        }
//        else if (strcmp(lineHeader1, "end_header") == 0) {
//            break;
//        }
//    }
//
//    for (int i = 0; i < nbVertices; i++)
//    {
//        //fsacnf로 xyz 정보 읽어오기
//        glm::vec3 vertex;
//        float temp1, temp2;
//        fscanf(file, "%f %f %f %f %f\n", &vertex.x, &vertex.y, &vertex.z, &temp1, &temp2);
//        out_vertices.push_back(vertex);
//    }
//
//    for (int i = 0; i < nbFaces; i++)
//    {
//        //vertex normal이 없어서 flat shading을 쓰거나
//        //vertex normal을 계산해준다
//        //for 모든 face
//        //  for 각 face , vertexindex
//        //      if vertexindex=2 / push_back(vertexindex);
//        //for (int j = 0; j < )
//        //
//        //for 모든 점 i
//        //  for 모든 면 j
//        //      for 면의 각점 k
//        //          for i == face[j][k]
//        //              face_list.push_back(해당 점);
//
//        //normal vector 계산
//        //for 모든 점 i
//        //  for 해당 점의 인접한 면에 대해서 face_list[i] j
//        //      vec3 avg_normal += ComputeFaceNormal(face_list[i][j])
//        //end
//        //avg_normal /= avg_normal.size();
//        //out_normal.push_back(avg_normal);
//    }
//
//}
//void MyMouse(int button, int state, int x, int y) {
//    switch (button) {
//    case GLUT_LEFT_BUTTON:
//        if (state == GLUT_DOWN) {
//            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
//            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
//            bMousing = true;
//        }
//        else
//            bMousing = false;
//        break;
//    case GLUT_MIDDLE_BUTTON:
//    case GLUT_RIGHT_BUTTON:
//        break;
//    default:
//        break;
//    }
//
//    glutPostRedisplay();
//}
//
//void MyMotion(int x, int y) {
//    ptCurrentMousePosit.x = x;
//    ptCurrentMousePosit.y = y;
//
//    if (bMousing)
//    {
//        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
//        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
//    }
//
//    ptLastMousePosit.x = ptCurrentMousePosit.x;
//    ptLastMousePosit.y = ptCurrentMousePosit.y;
//
//    glutPostRedisplay();
//}
//
//void InitLight()
//{
//    GLfloat LightPosition[] = { 0.0, 2.0, 0.0, 1.0 };
//    glEnable(GL_LIGHTING);      //조명 활성화
//    glEnable(GL_LIGHT0);
//
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb); //주변광 설정
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //확산광 설정
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); //반사광 설정
//    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
//
//}
//
//void init(void) {
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glShadeModel(GL_SMOOTH);    //구로 셰이딩
//    glEnable(GL_DEPTH_TEST); // 깊이버퍼
//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_FRONT);
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
//    InitLight();
//}
//
//
//
//void DrawSurface(std::vector < glm::vec3 >& vectices,
//    std::vector < glm::vec3 >& normals,
//    std::vector < glm::ivec3 >& faces)
//{
//    glBegin(GL_TRIANGLES);
//    for (int i = 0; i < faces.size(); i++) {
//        for (int j = 0; j < 3; j++) {
//            glm::vec3 p = vertices[faces[i][j]];
//
//            if (normals.size() == vertices.size()) {
//                glm::vec3 n = normals[faces[i][j]];
//                glNormal3f(n[0], n[1], n[2]);
//            }
//            glVertex3f(p[0], p[1], p[2]);
//        }
//    }
//
//    glEnd();
//}
//
//void DrawWireSurface(std::vector < glm::vec3 >& vectices,
//    std::vector < glm::ivec3 >& faces)
//{
//    glBegin(GL_LINES);
//    for (int i = 0; i < faces.size(); i++) { //각 면마다
//        for (int j = 0; j < 3; j++) {   //각 선마다
//            glm::vec3 p = vertices[faces[i][j]];
//
//            if (normals.size() == vertices.size()) {
//                glm::vec3 n = normals[faces[i][j]];
//                glNormal3f(n[0], n[1], n[2]);
//            }
//            glVertex3f(p[0], p[1], p[2]);
//        }
//    }
//
//    glEnd();
//}
//
//
//
//void render(void) {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//
//    glTranslatef(0.0f, 0.0f, g_fDistance);
//    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
//    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);
//
//    //Draw here
//    DrawSurface(vertices, normals, faces);
//    glutSwapBuffers();
//}
//
//
//
//void MyReshape(int w, int h) {
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//}
//
//void main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowPosition(100, 100);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("Texture Mapping - Passivity");
//    init();
//
//    //LoadObj("Data/bunny/bunny.obj", vertices, faces, uvs, normals);
//    LoadPly("Data/bunny/bun_zipper_res4.ply", vertices, faces, normals);
//
//    glutDisplayFunc(render);
//    glutReshapeFunc(MyReshape);
//    glutMouseFunc(MyMouse);
//    glutMotionFunc(MyMotion);
//
//    glutMainLoop();
//}