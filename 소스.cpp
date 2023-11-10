#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<cstdlib>
#include<ctime>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <math.h>

using namespace std;



GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Drag(int x, int y);
void TimerFunction(int value);
GLvoid Special(int key, int x, int y);
void convert(int x, int y, float* ox, float* oy);
void InitBuffer();
void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
void set();
void moveB();
void setcurve();
void moveToCurve(float t);
BOOL checkcrash();
BOOL Collide(int shape);
void generatepiece();
BOOL checkcrashpiece();


bool left_button;
bool solid = true;
bool movebasket = true;

float mx = 0, my = 0;
float t = 0, speed = 0.01;
int shape = 0;

BOOL draw = true;
BOOL mousedraw = false;
BOOL pass = false;
BOOL line = false;







GLfloat HexahedronShape[36][3];
GLfloat HexahedronColors[36][3];
GLfloat TetrahedronShape[18][3];
GLfloat TetrahedronColors[18][3];
GLfloat TriangleShape[3][3]={ { -0.2, -0.2, 0.0 }, { -0.2, 0.2, 0.0 }, { 0.2, -0.2, 0.0} };
GLfloat TriangleColors[3][3];
GLfloat RectShape[6][3] = { 
	{ -0.2, -0.2, 0.0 }, { -0.2, 0.2, 0.0 }, { 0.2, -0.2, 0.0},
	{ -0.2, 0.2, 0.0 },{0.2,0.2,0.0},{0.2,-0.2,0.0}
};





GLfloat RectColors[6][3];




GLfloat BasketShape[6][3] = {
{ -0.2, -0.05, 0.0 }, { -0.2, 0.05, 0.0 }, { 0.2, -0.05, 0.0},
{ -0.2, 0.05, 0.0 }, { 0.2, -0.05, 0.0}, { 0.2, 0.05, 0.0}
};
GLfloat BasketColors[6][3];


GLfloat LinePos[2][3] = { 0, };
GLfloat RectPos[3] = { 0, };
GLfloat TriPos[3] = { 0, };
GLfloat BasketPos[3] = { 0, };
GLfloat Curve[3][3] = { {-1.0,0.0,0.0},{0.0,0.5,0.0},{1.0,0.0,0.0} };
GLfloat PiecePos[10][3] = { 0, };
BOOL PieceDraw[10];


GLuint HexPosVbo, TetPosVbo,RectPosVbo,TriPosVbo, BasketPosVbo;
GLuint HexColor, TetColor,RectColor,TriColor, BasketColor;



GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체







void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	set();
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutMouseFunc(Mouse);
	glutMotionFunc(Drag);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(Special);
	glutTimerFunc(10, TimerFunction, 1);

	glutMainLoop();
}


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 0.0;
	bColor = 0.0;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(shaderProgramID);







	GLuint changePos = glGetUniformLocation(shaderProgramID, "Pos");
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1

	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(ColorLocation);


	if (pass)
	{
		glUniform2f(changePos, 0, 0);
		for (float t = 0;t < 1;t)
		{
			float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			x1 = pow((1 - t), 2) * Curve[0][0] + 2 * t * (1 - t) * Curve[1][0] + pow(t, 2) * Curve[2][0];
			y1 = pow((1 - t), 2) * Curve[0][1] + 2 * t * (1 - t) * Curve[1][1] + pow(t, 2) * Curve[2][1];

			t = t + 0.01;
			x2 = pow((1 - t), 2) * Curve[0][0] + 2 * t * (1 - t) * Curve[1][0] + pow(t, 2) * Curve[2][0];
			y2 = pow((1 - t), 2) * Curve[0][1] + 2 * t * (1 - t) * Curve[1][1] + pow(t, 2) * Curve[2][1];


			glBegin(GL_LINE_LOOP);
			glVertex3f(x1, y1, 0);
			glVertex3f(x2, y2, 0);
			glEnd();
		}

	}
	

	
	if (draw)
	{
		if (shape == 0)
		{
			glUniform2f(changePos, mx, my);
			glBindBuffer(GL_ARRAY_BUFFER, RectPosVbo); // VBO Bind
			glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			{
				glBindBuffer(GL_ARRAY_BUFFER, RectColor); // VBO Bind
				glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			}
			if (line)
				glDrawArrays(GL_LINE_LOOP, 0, 6);
			else
				glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else
		{
			glUniform2f(changePos, mx, my);
			glBindBuffer(GL_ARRAY_BUFFER, TriPosVbo); // VBO Bind
			glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			{
				glBindBuffer(GL_ARRAY_BUFFER, TriColor); // VBO Bind
				glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			}
			if (line)
				glDrawArrays(GL_LINE_LOOP, 0, 3);
			else
				glDrawArrays(GL_TRIANGLES, 0, 3);
			
		}


	}
	






	glUniform2f(changePos, BasketPos[0], BasketPos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, BasketPosVbo); // VBO Bind
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	{
		glBindBuffer(GL_ARRAY_BUFFER, BasketColor); // VBO Bind
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);




	for (int i = 0;i < 10;i++)
	{
		if (PieceDraw[i])
		{
			glUniform2f(changePos, PiecePos[i][0], PiecePos[i][1]);
			glBindBuffer(GL_ARRAY_BUFFER, TriPosVbo); // VBO Bind
			glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			{
				glBindBuffer(GL_ARRAY_BUFFER, BasketColor); // VBO Bind
				glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			}

			if(line)
				glDrawArrays(GL_LINE_LOOP, 0, 3);
			else
				glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}






	if (mousedraw)
	{
		glUniform2f(changePos, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex3f(LinePos[0][0], LinePos[0][1], 0);
		glVertex3f(LinePos[1][0], LinePos[1][1], 0);
		glEnd();
	}



	glDisableVertexAttribArray(PosLocation); // Disable 필수!
	glDisableVertexAttribArray(ColorLocation);




	glutSwapBuffers();
	glutPostRedisplay();
}


GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	srand(unsigned(time(NULL)));

	switch (key)
	{
	case 'p':
		if (pass)
			pass = false;
		else
			pass = true;
		break;
	case 'l':
		if (line)
			line = false;
		else
			line = true;
		break;
	case '+':
		if (speed <= 0.1)
			speed += 0.01;
		break;
	case '-':
		if (speed > 0.01)
			speed -= 0.01;
		break;
	case 'a':
		setcurve();
		break;
	case 'q':
		exit(1);
		break;
	}


	glutPostRedisplay();
}
GLvoid Special(int key, int x, int y)
{
	switch (key)
	{

	}

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	srand((unsigned int)time(NULL));

	float Rx, Ry, Wx, Wy;


	convert(x, y, &Wx, &Wy);
	convert(x, y, &Rx, &Ry);



	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		LinePos[0][0] = Wx;
		LinePos[0][1] = Wy;

		mousedraw = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{

		draw=!Collide(shape);
		if (!draw)
		{
			t = 2;
			generatepiece();
		}
			
		mousedraw = false;
	}
}

void Drag(int x, int y)
{
	float Rx, Ry, Wx, Wy;

	convert(x, y, &Wx, &Wy);
	convert(x, y, &Rx, &Ry);


	LinePos[1][0] = Wx;
	LinePos[1][1] = Wy;



	glutPostRedisplay();
}





void convert(int x, int y, float* ox, float* oy)
{
	int w = 800;
	int h = 800;
	*ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	*oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));
}




void TimerFunction(int value)
{
	moveB();
	moveToCurve(t);
	if (t < 1.1)
		t += speed;
	else
	{
		t = 0;
		draw = true;
		shape = rand() % 2;
		setcurve();
	}
	
	for (int i = 0;i < 10;i++)
	{
		if (PieceDraw[i])
		{
			PieceDraw[i] = checkcrashpiece();

			if (PiecePos[i][1] <= -1.2)
				PieceDraw[i] = false;
			PiecePos[i][1] -= 0.01;

			
		}
	}



	if(draw)
		draw = checkcrash();
	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(10, TimerFunction, 1); // 타이머함수 재 설정
}


GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}


void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}



char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}



void InitBuffer()
{
	glGenBuffers(1, &HexPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, HexPosVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(HexahedronShape), HexahedronShape, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &HexColor);
	glBindBuffer(GL_ARRAY_BUFFER, HexColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(HexahedronColors), HexahedronColors, GL_STATIC_DRAW);

	glGenBuffers(1, &TetPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, TetPosVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TetrahedronShape), TetrahedronShape, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &TetColor);
	glBindBuffer(GL_ARRAY_BUFFER, TetColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TetrahedronColors), TetrahedronColors, GL_STATIC_DRAW);

	glGenBuffers(1, &RectPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, RectPosVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectShape), RectShape, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &RectColor);
	glBindBuffer(GL_ARRAY_BUFFER, RectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectColors), RectColors, GL_STATIC_DRAW);

	glGenBuffers(1, &TriPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, TriPosVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleShape), TriangleShape, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &TriColor);
	glBindBuffer(GL_ARRAY_BUFFER, TriColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleColors), TriangleColors, GL_STATIC_DRAW);

	glGenBuffers(1, &BasketPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, BasketPosVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BasketShape), BasketShape, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &BasketColor);
	glBindBuffer(GL_ARRAY_BUFFER, BasketColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BasketColors), BasketColors, GL_STATIC_DRAW);
}


void set()
{
	printf("도형모드 변경:;\n경로 출력:p\n속도 증감:+, -\n프로그램 종료:q\n");
	BasketPos[1] = -0.7;
	for (int i = 0;i < 10;i++)
	{
		PieceDraw[i] = false;
	}

	for (int i = 0;i < 3;i++)
	{
		TetrahedronColors[i][0] = 1;
		TetrahedronColors[i][1] = 0;
		TetrahedronColors[i][2] = 0;
	}
	for (int i = 3;i < 6;i++)
	{
		TetrahedronColors[i][0] = 0;
		TetrahedronColors[i][1] = 1;
		TetrahedronColors[i][2] = 0;
	}
	for (int i = 6;i < 9;i++)
	{
		TetrahedronColors[i][0] = 0;
		TetrahedronColors[i][1] = 0;
		TetrahedronColors[i][2] = 1;
	}
	for (int i = 9;i < 12;i++)
	{
		TetrahedronColors[i][0] = 1;
		TetrahedronColors[i][1] = 1;
		TetrahedronColors[i][2] = 0;
	}
	for (int i = 12;i < 15;i++)
	{
		TetrahedronColors[i][0] = 1;
		TetrahedronColors[i][1] = 0;
		TetrahedronColors[i][2] = 1;
	}


	for (int i = 0;i < 6;i++)
	{
		HexahedronColors[i][0] = 1;
		HexahedronColors[i][1] = 0;
		HexahedronColors[i][2] = 0;
	}
	for (int i = 6;i < 12;i++)
	{
		HexahedronColors[i][0] = 0;
		HexahedronColors[i][1] = 1;
		HexahedronColors[i][2] = 0;
	}
	for (int i = 12;i < 18;i++)
	{
		HexahedronColors[i][0] = 0;
		HexahedronColors[i][1] = 0;
		HexahedronColors[i][2] = 1;
	}
	for (int i = 18;i < 24;i++)
	{
		HexahedronColors[i][0] = 1;
		HexahedronColors[i][1] = 1;
		HexahedronColors[i][2] = 0;
	}
	for (int i = 24;i < 30;i++)
	{
		HexahedronColors[i][0] = 0;
		HexahedronColors[i][1] = 1;
		HexahedronColors[i][2] = 1;
	}
	for (int i = 30;i < 36;i++)
	{
		HexahedronColors[i][0] = 1;
		HexahedronColors[i][1] = 0;
		HexahedronColors[i][2] = 1;
	}
	HexahedronShape[0][0] = 0.0;HexahedronShape[0][1] = 0.0;HexahedronShape[0][2] = 0.0;
	HexahedronShape[1][0] = 0.5;HexahedronShape[1][1] = 0.5;HexahedronShape[1][2] = 0.0;
	HexahedronShape[2][0] = 0.5;HexahedronShape[2][1] = 0.0;HexahedronShape[2][2] = 0.0;

	HexahedronShape[3][0] = 0.0;HexahedronShape[3][1] = 0.0;HexahedronShape[3][2] = 0.0;
	HexahedronShape[4][0] = 0.0;HexahedronShape[4][1] = 0.5;HexahedronShape[4][2] = 0.0;
	HexahedronShape[5][0] = 0.5;HexahedronShape[5][1] = 0.5;HexahedronShape[5][2] = 0.0;

	HexahedronShape[6][0] = 0.0;HexahedronShape[6][1] = 0.0;HexahedronShape[6][2] = 0.0;
	HexahedronShape[7][0] = 0.0;HexahedronShape[7][1] = 0.5;HexahedronShape[7][2] = 0.5;
	HexahedronShape[8][0] = 0.0;HexahedronShape[8][1] = 0.5;HexahedronShape[8][2] = 0.0;

	HexahedronShape[9][0] = 0.0;HexahedronShape[9][1] = 0.0;HexahedronShape[9][2] = 0.0;
	HexahedronShape[10][0] = 0.0;HexahedronShape[10][1] = 0.0;HexahedronShape[10][2] = 0.5;
	HexahedronShape[11][0] = 0.0;HexahedronShape[11][1] = 0.5;HexahedronShape[11][2] = 0.5;

	HexahedronShape[12][0] = 0.0;HexahedronShape[12][1] = 0.5;HexahedronShape[12][2] = 0.0;
	HexahedronShape[13][0] = 0.5;HexahedronShape[13][1] = 0.5;HexahedronShape[13][2] = 0.5;
	HexahedronShape[14][0] = 0.5;HexahedronShape[14][1] = 0.5;HexahedronShape[14][2] = 0.0;

	HexahedronShape[15][0] = 0.0;HexahedronShape[15][1] = 0.5;HexahedronShape[15][2] = 0.0;
	HexahedronShape[16][0] = 0.0;HexahedronShape[16][1] = 0.5;HexahedronShape[16][2] = 0.5;
	HexahedronShape[17][0] = 0.5;HexahedronShape[17][1] = 0.5;HexahedronShape[17][2] = 0.5;

	HexahedronShape[18][0] = 0.5;HexahedronShape[18][1] = 0.0;HexahedronShape[18][2] = 0.0;
	HexahedronShape[19][0] = 0.5;HexahedronShape[19][1] = 0.5;HexahedronShape[19][2] = 0.0;
	HexahedronShape[20][0] = 0.5;HexahedronShape[20][1] = 0.5;HexahedronShape[20][2] = 0.5;

	HexahedronShape[21][0] = 0.5;HexahedronShape[21][1] = 0.0;HexahedronShape[21][2] = 0.0;
	HexahedronShape[22][0] = 0.5;HexahedronShape[22][1] = 0.5;HexahedronShape[22][2] = 0.5;
	HexahedronShape[23][0] = 0.5;HexahedronShape[23][1] = 0.0;HexahedronShape[23][2] = 0.5;

	HexahedronShape[24][0] = 0.0;HexahedronShape[24][1] = 0.0;HexahedronShape[24][2] = 0.0;
	HexahedronShape[25][0] = 0.5;HexahedronShape[25][1] = 0.0;HexahedronShape[25][2] = 0.0;
	HexahedronShape[26][0] = 0.5;HexahedronShape[26][1] = 0.0;HexahedronShape[26][2] = 0.5;

	HexahedronShape[27][0] = 0.0;HexahedronShape[27][1] = 0.0;HexahedronShape[27][2] = 0.0;
	HexahedronShape[28][0] = 0.5;HexahedronShape[28][1] = 0.0;HexahedronShape[28][2] = 0.5;
	HexahedronShape[29][0] = 0.0;HexahedronShape[29][1] = 0.0;HexahedronShape[29][2] = 0.5;

	HexahedronShape[30][0] = 0.0;HexahedronShape[30][1] = 0.0;HexahedronShape[30][2] = 0.5;
	HexahedronShape[31][0] = 0.5;HexahedronShape[31][1] = 0.0;HexahedronShape[31][2] = 0.5;
	HexahedronShape[32][0] = 0.5;HexahedronShape[32][1] = 0.5;HexahedronShape[32][2] = 0.5;

	HexahedronShape[33][0] = 0.0;HexahedronShape[33][1] = 0.0;HexahedronShape[33][2] = 0.5;
	HexahedronShape[34][0] = 0.5;HexahedronShape[34][1] = 0.5;HexahedronShape[34][2] = 0.5;
	HexahedronShape[35][0] = 0.0;HexahedronShape[35][1] = 0.5;HexahedronShape[35][2] = 0.5;


	TetrahedronShape[0][0] = 0.0;TetrahedronShape[0][1] = 0.0;TetrahedronShape[0][2] = 0.0;
	TetrahedronShape[1][0] = 0.5;TetrahedronShape[1][1] = 0.0;TetrahedronShape[1][2] = 0.0;
	TetrahedronShape[2][0] = 0.0;TetrahedronShape[2][1] = 0.0;TetrahedronShape[2][2] = 0.5;

	TetrahedronShape[3][0] = 0.5;TetrahedronShape[3][1] = 0.0;TetrahedronShape[3][2] = 0.5;
	TetrahedronShape[4][0] = 0.5;TetrahedronShape[4][1] = 0.0;TetrahedronShape[4][2] = 0.0;
	TetrahedronShape[5][0] = 0.0;TetrahedronShape[5][1] = 0.0;TetrahedronShape[5][2] = 0.5;

	TetrahedronShape[6][0] = 0.5;TetrahedronShape[6][1] = 0.0;TetrahedronShape[6][2] = 0.0;
	TetrahedronShape[7][0] = 0.0;TetrahedronShape[7][1] = 0.0;TetrahedronShape[7][2] = 0.0;
	TetrahedronShape[8][0] = 0.25;TetrahedronShape[8][1] = 0.4;TetrahedronShape[8][2] = 0.25;

	TetrahedronShape[9][0] = 0.0;TetrahedronShape[9][1] = 0.0;TetrahedronShape[9][2] = 0.5;
	TetrahedronShape[10][0] = 0.0;TetrahedronShape[10][1] = 0.0;TetrahedronShape[10][2] = 0.0;
	TetrahedronShape[11][0] = 0.25;TetrahedronShape[11][1] = 0.4;TetrahedronShape[11][2] = 0.25;

	TetrahedronShape[12][0] = 0.5;TetrahedronShape[12][1] = 0.0;TetrahedronShape[12][2] = 0.0;
	TetrahedronShape[13][0] = 0.5;TetrahedronShape[13][1] = 0.0;TetrahedronShape[13][2] = 0.5;
	TetrahedronShape[14][0] = 0.25;TetrahedronShape[14][1] = 0.4;TetrahedronShape[14][2] = 0.25;

	TetrahedronShape[15][0] = 0.0;TetrahedronShape[15][1] = 0.0;TetrahedronShape[15][2] = 0.5;
	TetrahedronShape[16][0] = 0.5;TetrahedronShape[16][1] = 0.0;TetrahedronShape[16][2] = 0.5;
	TetrahedronShape[17][0] = 0.25;TetrahedronShape[17][1] = 0.4;TetrahedronShape[17][2] = 0.25;



}
void moveB()
{
	if (movebasket)
	{
		BasketPos[0] += 0.01;
		if (BasketPos[0] >= 0.8)
			movebasket = false;
	}
	else
	{
		BasketPos[0] -= 0.01;
		if (BasketPos[0] <= -0.8)
			movebasket = true;
	}

}
void setcurve()
{
	srand((unsigned int)time(NULL));

	
	
	Curve[0][0] = -1.0;

	if (rand() % 2 == 0)
		Curve[0][1] = (rand() % 5 + 2) / 10.0;
	else
		Curve[0][1] = (rand() % 3 + 3) / 10.0 * -1;
	
		

	Curve[2][0] = 1.0;
	Curve[2][1] = Curve[0][1] + (rand() % 5+1) / 10.0;
	


	Curve[1][0] = (Curve[0][0] + Curve[2][0])/2.0;
	Curve[1][1] = Curve[2][1] + (rand() % 3 + 3) / 10.0;
	



	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 2;j++)
		{
			printf("%f\t", Curve[i][j]);
		}
		printf("\n");
	}
}
void moveToCurve(float t)
{
	float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	mx = pow((1 - t), 2) * Curve[0][0] + 2 * t * (1 - t) * Curve[1][0] + pow(t, 2) * Curve[2][0];
	my = pow((1 - t), 2) * Curve[0][1] + 2 * t * (1 - t) * Curve[1][1] + pow(t, 2) * Curve[2][1];
}


BOOL checkcrash()
{
	float left = mx - 0.2;
	float top = my + 0.2;
	float right = mx + 0.2;
	float bottom = my - 0.2;

	float basleft = BasketPos[0] - 0.2;
	float bastop = BasketPos[1] + 0.05;
	float basright = BasketPos[0] + 0.2;
	float basbottom = BasketPos[1] - 0.05;
	

	if (left<basright && right>basleft)
	{
		if (top > basbottom && bottom < bastop)
		{
			return false;
		}

	}



	if (left<basright && right>basleft)
	{
		if (top > basbottom && bottom < bastop)
		{
			return false;
		}

	}

	
	return true;
}
BOOL checkcrashpiece()
{
	float basleft = BasketPos[0] - 0.2;
	float bastop = BasketPos[1] + 0.05;
	float basright = BasketPos[0] + 0.2;
	float basbottom = BasketPos[1] - 0.05;


	for (int i = 0;i < 10;i++)
	{
		float left = PiecePos[i][0] - 0.05;
		float top = PiecePos[i][1] + 0.05;
		float right = PiecePos[i][0] + 0.05;
		float bottom = PiecePos[i][1] - 0.05;


		if (left<basright && right>basleft)
		{
			if (top > basbottom && bottom < bastop)
			{
				PiecePos[i][0] = 100;
				PiecePos[i][1] = 100;
				return false;
			}

		}



		if (left<basright && right>basleft)
		{
			if (top > basbottom && bottom < bastop)
			{
				PiecePos[i][0] = 100;
				PiecePos[i][1] = 100;
				return false;
			}

		}
	}




	

	return true;
}
BOOL Collide(int shape)
{
	int count = 0;

	float x = 0, y = 0, x2 = 0, y2 = 0;
	float rectleft = 0, recttop = 0, rectright = 0, rectbottom = 0;
	BOOL left = false, right = false, top = false, bottom = false;

	rectleft = mx - 0.2, recttop = my + 0.2, rectright = mx + 0.2, rectbottom = my - 0.2;

	if (shape == 0)//사각
	{
		for (float t = 0;t <= 1;t += 0.01)
		{
			x = (1 - t) * LinePos[0][0] + t * LinePos[1][0];
			y = (1 - t) * LinePos[0][1] + t * LinePos[1][1];

			for (float t2 = 0;t2 <= 1;t2 += 0.01)
			{
				x2 = (1 - t2) * rectleft + t2 * rectright;
				y2 = recttop;


				if (abs(x2 - x) <= 0.01 && abs(y2 - y) <= 0.01)
				{
					left = true;
					break;
				}

			}

			for (float t2 = 0;t2 <= 1;t2 += 0.01)
			{
				x2 = (1 - t2) * rectleft + t2 * rectright;
				y2 = rectbottom;


				if (abs(x2 - x) <= 0.01 && abs(y2 - y) <= 0.01)
				{
					right = true;
					break;
				}

			}

			for (float t2 = 0;t2 <= 1;t2 += 0.01)
			{
				x2 = rectleft;
				y2 = (1 - t2) * recttop + t2 * rectbottom;


				if (abs(x2 - x) <= 0.01 && abs(y2 - y) <= 0.01)
				{
					top = true;
					break;
				}

			}

			for (float t2 = 0;t2 <= 1;t2 += 0.01)
			{
				x2 = rectright;
				y2 = (1 - t2) * recttop + t2 * rectbottom;


				if (abs(x2 - x) <= 0.01 && abs(y2 - y) <= 0.01)
				{
					bottom = true;
					break;
				}
			}
		}
	}
	else
	{
		for (float t = 0;t <= 1;t += 0.01)
		{
			x = (1 - t) * LinePos[0][0] + t * LinePos[1][0];
			y = (1 - t) * LinePos[0][1] + t * LinePos[1][1];

			for (float t2 = 0;t2 <= 1;t2 += 0.01)
			{
				x2 = (1 - t2) * rectleft + t2 * rectright;
				y2 = rectbottom;


				if (abs(x2 - x) <= 0.01 && abs(y2 - y) <= 0.01)
				{
					right = true;
					break;
				}

			}

			for (float t2 = 0;t2 <= 1;t2 += 0.01)
			{
				x2 = rectleft;
				y2 = (1 - t2) * recttop + t2 * rectbottom;


				if (abs(x2 - x) <= 0.01 && abs(y2 - y) <= 0.01)
				{
					top = true;
					break;
				}

			}

			for (float t2 = 0;t2 <= 1;t2 += 0.01)
			{
				x2 = (1 - t2) * rectleft + t2 * rectright;
				y2 = (1 - t2) * recttop + t2 * rectbottom;


				if (abs(x2 - x) <= 0.01 && abs(y2 - y) <= 0.01)
				{
					bottom = true;
					break;
				}

			}
		}
	}


	if (left)
		count++;
	if (right)
		count++;
	if (top)
		count++;
	if (bottom)
		count++;



	if (count >= 2)
		return true;



	return false;
}
void generatepiece()
 {
	for (int i = 0;i < 10;i++)
	{
		if (!PieceDraw[i])
		{
			PiecePos[i][0] = mx - 0.05;
			PiecePos[i][1] = my - 0.05;
			PieceDraw[i] = true;


			PiecePos[i+1][0] = mx + 0.05;
			PiecePos[i+1][1] = my - 0.05;
			PieceDraw[i+1] = true;
			break;
		}
	}
}