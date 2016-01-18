#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/gl.h>
#include<stack>
#include <glm/glm.hpp>
#include<glm/gtx/transform.hpp> 
#include <math.h>
#include "mainFile.h"
#include "MazeGenerator.h"
#include "Lab3Robot.h"

int width = 800;
int height = 800;
using namespace std;



vector<int> *abc;
struct BaseStructure
{
	float x, y, z;
};


BaseStructure baseSquare[] = { -0.05f, -0.05f, -0.05f, 1.0f, // triangle 1 : begin
-0.05f, -0.05f, 0.05f, 1.0f,
-0.05f, 0.05f, 0.05f, 1.0f,// triangle 1 : end
0.05f, 0.05f, -0.05f, 1.0f,// triangle 2 : begin
-0.05f, -0.05f, -0.05f, 1.0f,
-0.05f, 0.05f, -0.05f, 1.0f,// triangle 2 : end
0.05f, -0.05f, 0.05f, 1.0f,
-0.05f, -0.05f, -0.05f, 1.0f,
0.05f, -0.05f, -0.05f, 1.0f,
0.05f, 0.05f, -0.05f, 1.0f,
0.05f, -0.05f, -0.05f, 1.0f,
-0.05f, -0.05f, -0.05f, 1.0f,
-0.05f, -0.05f, -0.05f, 1.0f,
-0.05f, 0.05f, 0.05f, 1.0f,
-0.05f, 0.05f, -0.05f, 1.0f,
0.05f, -0.05f, 0.05f, 1.0f,
-0.05f, -0.05f, 0.05f, 1.0f,
-0.05f, -0.05f, -0.05f, 1.0f,
-0.05f, 0.05f, 0.05f, 1.0f,
-0.05f, -0.05f, 0.05f, 1.0f,
0.05f, -0.05f, 0.05f, 1.0f,
0.05f, 0.05f, 0.05f, 1.0f,
0.05f, -0.05f, -0.05f, 1.0f,
0.05f, 0.05f, -0.05f, 1.0f,
0.05f, -0.05f, -0.05f, 1.0f,
0.05f, 0.05f, 0.05f, 1.0f,
0.05f, -0.05f, 0.05f, 1.0f,
0.05f, 0.05f, 0.05f, 1.0f,
0.05f, 0.05f, -0.05f, 1.0f,
-0.05f, 0.05f, -0.05f, 1.0f,
0.05f, 0.05f, 0.05f, 1.0f,
-0.05f, 0.05f, -0.05f, 1.0f,
-0.05f, 0.05f, 0.05f, 1.0f,
0.05f, 0.05f, 0.05f, 1.0f,
-0.05f, 0.05f, 0.05f, 1.0f,
0.05f, -0.05f, 0.05f, 1.0f
}; 
void drawing(glm::mat4& m){

	glLoadMatrixf(&m[0][0]);
	glColor3f(1, 1, 0);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 4);
}

GLubyte tindices[6];

GLuint vboHandle[1];   // a VBO that contains interleaved positions and colors 
GLuint indexVBO;


void display() {

	glm::mat4 modelM = glm::mat4(1.0f);
	file.model = glm::mat4(1.0f);

	std::stack<glm::mat4> mat_stack;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	float angle123 = (3.141592653589793238463 / 180) * 90;

	//glm::mat4 projection = glm::ortho(0.0f, 15.0f, 15.0f, 0.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(60.0f, 1.0f, 0.01f, 100.0f);
	//glMatrixMode(GL_PROJECTION);
	//glLoadMatrixf(&projection[0][0]);


	glm::mat4 view = glm::lookAt(glm::vec3(4+file.posx, -5+file.posy, -2),
//		glm::lookAt(glm::vec3(0, 0.2, 5),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 1.0));

	view = view*file.modelV;


	file.model = glm::rotate(file.model, angle123 * 2, glm::vec3(0.0f, 1.0f, 0.0f));
	file.model = glm::translate(file.model, glm::vec3(0.0f, 0.0f, -1.0f));
    file.model = glm::rotate(file.model, file.x_angle, glm::vec3(-1.0f, 0.0f, 0.0f)); 
    file.model = glm::rotate(file.model, file.z_angle, glm::vec3(0.0f, 0.0f, -1.0f));
   // file.model = glm::rotate(file.model, file.y_angle, glm::vec3(0.0f, -1.0f, 0.0f)); 
    file.model = glm::scale(file.model, glm::vec3(file.scale_size, file.scale_size, file.scale_size)); 

	////
	//////to change the axis's of rotations.... 
    file.model = glm::translate(file.model, glm::vec3(0.0f, 0.0f, 0.0f));
	
    file.model = glm::scale(file.model, glm::vec3(5.0f, 5.0f, 5.0f));

	
	//glm::mat4 file.modelV = file.model * file.modelM;


	glm::mat4 modelv = view * file.model;
	file.model = projection * modelv;
	
	mat_stack.push(file.model);

	file.model = glm::rotate(file.model, angle123, glm::vec3(1.0f, 0.0f, 0.0f));
	file.model = glm::translate(file.model, glm::vec3(-0.75, -0.4, 0.9));
	file.model = glm::scale(file.model, glm::vec3(0.3, 0.3, 0.3));
	r.Robot::draw(file.model);
	file.model=mat_stack.top();
	mat_stack.pop();

	glColor4f(1, 1, 1, 1);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);


	glVertexPointer(4, GL_FLOAT, 0, 0);// 3-dimension

	mat_stack.push(modelM);
	modelM = glm::translate(modelM, glm::vec3(0.0, 0.0, -1.0));

	modelM = glm::scale(modelM, glm::vec3(40.0, 40.0, 0));
	
	modelM = file.model*modelM;

	glLoadMatrixf(&modelM[0][0]);

	glColor3f(0.2, 0.4, 0.2);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 4);

	modelM = mat_stack.top();



	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){

			vector <int>::iterator k = find(abc[i * 9 + j].begin(), abc[i * 9 + j].end(), i * 9 + j + 1);

			if (k != abc[i * 9 + j].end()){

				modelM = glm::translate(modelM, glm::vec3((j + 1)*0.2 + (-0.9), (i*0.2) - 0.9f, 0.0f));
				modelM = glm::rotate(modelM, angle123, glm::vec3(0.0, 0.0, 1.0));
				modelM = glm::scale(modelM, glm::vec3(1, .1, 3));

				modelM = file.model*modelM;

				drawing(modelM);
				modelM = mat_stack.top();

			}

			k = find(abc[i * 9 + j].begin(), abc[i * 9 + j].end(), (i + 1) * 9 + j);
			if (k != abc[i * 9 + j].end()){

				modelM = glm::translate(modelM, glm::vec3((j*0.2) + (-0.9) + 0.1, (i + 1)*0.2 + -0.95f, 0.0f));
				modelM = glm::scale(modelM, glm::vec3(1, .1, 3));

				modelM = file.model*modelM;
				
				drawing(modelM);
				modelM = mat_stack.top();

			}

			if (!(j == 0)){
				modelM = glm::translate(modelM, glm::vec3(j*0.2 - 0.9 + 0.1, -0.95, 0.0f));
				modelM = glm::scale(modelM, glm::vec3(1, .1, 3));

				modelM = file.model*modelM;

				drawing(modelM);
				modelM = mat_stack.top();
			}

			modelM = glm::translate(modelM, glm::vec3(-0.9, (i)*0.2 - 0.90f, 0.0f));
			modelM = glm::rotate(modelM, angle123, glm::vec3(0.0, 0.0, 1.0));
			modelM = glm::scale(modelM, glm::vec3(1, .1, 3));

			modelM = file.model*modelM;


			drawing(modelM);
			modelM = mat_stack.top();

			if (!(j == 8)){

				modelM = glm::translate(modelM, glm::vec3(j*0.2 - 0.9 + 0.1, 0.80f, 0.0f));
				modelM = glm::scale(modelM, glm::vec3(1, .1, 3));

				modelM = file.model*modelM;


				drawing(modelM);
				modelM = mat_stack.top();
			}

			//right most
			modelM = glm::translate(modelM, glm::vec3(0.9, (i)*0.2 - 0.90f, 0.0f));
			modelM = glm::rotate(modelM, angle123, glm::vec3(0.0, 0.0, 1.0));
			modelM = glm::scale(modelM, glm::vec3(1, .1, 3));

			modelM = file.model*modelM;

			drawing(modelM);
			modelM = mat_stack.top();

		}

		cout << endl;
	}




	glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();

	glFlush();

}

void InitVBO()
{
	GLenum err = glewInit();

	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	glGenBuffers(1, vboHandle);   // create an interleaved VBO object
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 144, &baseSquare[0].x, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

	tindices[0] = 0;   tindices[1] = 1;   tindices[2] = 2;
	tindices[3] = 0;   tindices[4] = 2;   tindices[5] = 3;

	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)* 6, tindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // clean up 

}

void mainFile::mymotion(int x, int y)
{
	if (xform_mode == XFORM_ROTATE) {
		z_angle += (x - press_x) / 5.0;
		if (z_angle > 180) z_angle -= 360;
		else if (z_angle <-180) z_angle += 360;
		press_x = x;

		x_angle -= (y - press_y) / 5.0;
		if (x_angle > 180) x_angle -= 360;
		else if (x_angle <-180) x_angle += 360;
		press_y = y;
	}
	else if (xform_mode == XFORM_SCALE){
		float old_size = scale_size;
		scale_size *= (1 + (y - press_y) / 60.0);
		if (scale_size <0) scale_size = old_size;
		press_y = y;
	}
	glutPostRedisplay();
}


void keys(unsigned char key, int x, int y){

//	//r.Robot::keys1(key, x, y);

//	float d_angle = 10;

	glutPostRedisplay();
}

void keys1234(unsigned char key, int x, int y){

	//	//r.Robot::keys1(key, x, y);

	//	float d_angle = 10;

	r.Robot::keys1(key, x, y);

}
	

void mainFile::mymouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		press_x = x; press_y = y;
		if (button == GLUT_LEFT_BUTTON)
			xform_mode = XFORM_ROTATE;
		else if (button == GLUT_RIGHT_BUTTON)
			xform_mode = XFORM_SCALE;
	}
	else if (state == GLUT_UP) {
		xform_mode = XFORM_NONE;
	}
}

void mymouse1(int button, int state, int x, int y){
	file.mymouse(button,state,x,y);
}

void mymotion1(int x, int y){
	file.mymotion(x,y);
}





void Robot::keys1(unsigned char key, int x, int y){
	float x1 = -0.1f / 40.0f;
	float d_angle = 15.0 * (22 / 7.0) / 180;
	switch (key){
	case (char)27:
		exit(1);
		break;
	
		/*case 'q':
			exit(1);
			break;*/
		case 'a':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(-0.1f, 0.0f, 0.0f));
			//file.modelV = glm::translate(file.modelV, glm::vec3(-x1, 0.0f, 0.0f));
			file.posx = file.posx + 0.1;
			
			break;
		case 'd':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.1f, 0.0f, 0.0f));
			file.modelV = glm::translate(file.modelV, glm::vec3(x1, 0.0f, 0.0f));
			file.posx = file.posx - 0.1;
			break;
		case 'w':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.0f, 0.0f, -0.1f));
			file.modelV = glm::translate(file.modelV, glm::vec3(0.0f, 0.0f, x1));
			file.posy = file.posy + 0.1;
			break;
		case 's':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.0f, 0.0f, 0.1f));
			file.modelV = glm::translate(file.modelV, glm::vec3(0.0f, 0.0f, -x1));
			file.posy = file.posy - 0.1;
			break;


	case 'q':
		modelMR = glm::rotate(modelMR, d_angle, glm::vec3(0.0, 1.0, 1.0));
		break;

	case 'e':
		modelMR = glm::rotate(modelMR, -d_angle, glm::vec3(0.0, 1.0, 1.0));
		break;

	case 'u':
		modelMR = glm::rotate(modelMR, d_angle, glm::vec3(0.0, 0.0, 1.0));
		break;

	case 'i':
		modelMR = glm::rotate(modelMR, -d_angle, glm::vec3(0.0, 0.0, 1.0));
		break;

	case 'c':
		modelMR = glm::mat4(1.0f);
		angle1 = angle2 = angle3 = angle4 = d_angle = 0;

	case '-':
		modelMR = glm::scale(modelMR, glm::vec3(0.8, 0.8, 1.0));
		break;

	case '+':
		modelMR = glm::scale(modelMR, glm::vec3(1.2, 1.2, 1.0));
		break;

	case '1':
		count1++;
		if (count1 % 2 == 0)angle1 += ((3.14159) / 180) * 50;
		else angle1 -= ((3.14159) / 180) * 50;
		break;

	case '2':
		count2++;
		if (count2 % 2 == 0)angle2 += ((3.14159) / 180) * 50;
		else angle2 -= ((3.14159) / 180) * 50;
		break;

	case '3':
		count3++;
		if (count3 % 2 == 0)angle3 += ((3.14159) / 180) * 50;
		else angle3 -= ((3.14159) / 180) * 50;
		break;

	case '4':
		count4++;
		if (count4 % 2 == 0)angle4 += ((3.14159) / 180) * 50;
		else angle4 -= ((3.14159) / 180) * 50;
		break;

	case '6':
		angle4 += ((3.14159) / 180) * 50;
		break;

	case'7':
		angle3 += ((3.14159) / 180) * 50;

	default:
		cout << "Wrong Input";

	}
	glutPostRedisplay();
	//float d_angle = 10;
	//switch (key){
	//case 'q':
	//	exit(1);
	//	break;
	//case 'a':
	//	r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(-0.1f, 0.0f, 0.0f));
	//	//file.modelV = glm::translate(file.modelV, glm::vec3(-x1, 0.0f, 0.0f));
	//	file.posx = file.posx + 0.1;
	//	
	//	break;
	//case 'd':
	//	r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.1f, 0.0f, 0.0f));
	//	file.modelV = glm::translate(file.modelV, glm::vec3(x1, 0.0f, 0.0f));
	//	file.posx = file.posx - 0.1;
	//	break;
	//case 'w':
	//	r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.0f, 0.0f, -0.1f));
	//	file.modelV = glm::translate(file.modelV, glm::vec3(0.0f, 0.0f, x1));
	//	file.posy = file.posy + 0.1;
	//	break;
	//case 's':
	//	r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.0f, 0.0f, 0.1f));
	//	file.modelV = glm::translate(file.modelV, glm::vec3(0.0f, 0.0f, -x1));
	//	file.posy = file.posy - 0.1;
	//	break;


	//case 'z':
	//	r.Robot::modelMR = glm::rotate(r.Robot::modelMR, d_angle, glm::vec3(0.0, 1.0, 1.0));
	//	break;

	//case '-':
	//	r.Robot::modelMR = glm::scale(r.Robot::modelMR, glm::vec3(0.8, 0.8, 1.0));
	//	break;

	//case '+':
	//	r.Robot::modelMR = glm::scale(r.Robot::modelMR, glm::vec3(1.2, 1.2, 1.0));
	//	break;

	//case '1':

	//	r.Robot::count1++;
	//	if (r.Robot::count1 % 2 == 0)r.Robot::angle1 += ((3.14159) / 180) * 50;
	//	else r.Robot::angle1 -= ((3.14159) / 180) * 50;
	//	break;

	//case '2':
	//	r.Robot::count2++;
	//	if (r.Robot::count2 % 2 == 0)r.Robot::angle2 -= ((3.14159) / 180) * 50;
	//	else r.Robot::angle2 += ((3.14159) / 180) * 50;
	//	break;

	//case '3':
	//	r.Robot::count3++;
	//	if (r.Robot::count3 % 2 == 0)r.Robot::angle3 += ((3.14159) / 180) * 50;
	//	else r.Robot::angle3 -= ((3.14159) / 180) * 50;
	//	break;

	//case '4':
	//	r.Robot::count4++;
	//	if (r.Robot::count4 % 2 == 0)r.Robot::angle4 -= ((3.14159) / 180) * 50;
	//	else r.Robot::angle4 += ((3.14159) / 180) * 50;
	//	break;

	//default:
	//	std::cout << "Wrong Input";

	//}
	//glutPostRedisplay();

}



void initializeGlut(int argc, char** argv) {

	glutInit(&argc, argv);
	width = 800;
	height = 800;
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Bhavya's Program");
	glutDisplayFunc(display);
	glutMouseFunc(mymouse1);
	glutMotionFunc(mymotion1);
	glutKeyboardFunc(keys1234);
}


void main(int argc, char** argv){

	initializeGlut(argc, argv);	//initialize glut
	InitVBO();	//initialize ticks vbo
	//InitVBORobot();
	Graph g(9);
	abc = g.getRemoved();


		c.InitCylinder(r.nslices, r.nstacks, 1.0, 0.0, 1.0);
		c.InitCylVBO(r.nstacks, r.nslices);
		r.Robot::InitVBOCube();
		s.InitSphere(0.5, r.nstacks, r.nslices, 1.0, 0.0, 1.0);
		s.InitVBOSphere(r.nstacks, r.nslices);


		glutMainLoop();
}