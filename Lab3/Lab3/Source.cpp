#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>

struct Vertex{
	float xyzw[4];
	float normal[4];
	float color[4];
};

struct BodyPart{
	Vertex *vertices;
	GLuint *indices;
	std::vector <BodyPart> *child;
	BodyPart *parent;
}head, leftArm, rightArm, leftLeg, rightLeg, body, lowerLeftArm, lowerLeftLeg, lowerRightArm, lowerRightLeg;

GLuint vboHandle[1];   // a VBO that contains interleaved positions and colors 
GLuint indexVBO;
float M_PI = 22 / 7.0;
int nIndicesHead;
int nIndicesBody;
int nvertices;
float x_angle = 0.0;
float y_angle = 0.0;
float scale_size = 1;

GLuint vboHandle1[1];   // a VBO that contains interleaved positions and colors 
GLuint indexVBO1;


int xform_mode = 0;
#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

int press_x, press_y;
int release_x, release_y;


class ChildParentOp{

public:

	

	void children(BodyPart x){
		for (int i = 0; i < 5; i++){
			//if (x.child != NULL){
			std::cout << 0;
			x.child[i];
			//	children(x);
		}

	}
	void InitBody(int nslices, int nstacks, float r, float g, float b);
//	void InitHead(int nslices, int nstacks, float r, float g, float b);
};


void ChildParentOp::InitBody(int nslices, int nstacks, float r, float g, float b)
{
	int nvertices = nslices * nstacks; 
	body.vertices = new Vertex[nvertices]; 

	printf(" M PI = %f\n", M_PI); 
	float Dangle = 2*M_PI/(float)(nslices-1); 

  // now create the index array 

	nIndicesBody = (nstacks-1)*2*(nslices+1); 
	body.indices = new GLuint[nIndicesBody]; 
	int n = 0; 
	for (int j =0; j<nstacks-1; j++)
    for (int i=0; i<=nslices; i++) {
      int mi = i % nslices;  
      int idx = j*nslices + mi; // mesh[j][mi] 
      int idx2 = (j+1) * nslices + mi; 
      body.indices[n++] = idx; 
      body.indices[n++] = idx2; 
    }
}

void InitBodyVBO(int nslices, int nstacks)
{
		int nvertices = nslices * nstacks;
		nIndicesBody = (nstacks - 1) * 2 * (nslices + 1);

		glGenBuffers(1, vboHandle);   // create an interleaved VBO object
		glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle 

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, body.vertices, GL_STATIC_DRAW); // allocate space and copy the position data over
		glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nIndicesBody, body.indices, GL_STATIC_DRAW);  // load the index data 

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // clean up 

		// by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time 
}

void display()
{

	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);   // normalize normals 

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// set up the parameters for lighting 
	GLfloat light_ambient[] = { 0, 0, 0, 1 };
	GLfloat light_diffuse[] = { .6, .6, .6, 1 };
	GLfloat light_specular[] = { 1, 1, 1, 1 };
	GLfloat light_pos[] = { 0, 10, 0, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


	GLfloat mat_specular[] = { .9, .9, .9, 1 };
	GLfloat mat_shine[] = { 10 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shine);

	//  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); 

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);

	glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY); // enable the color array on the client side

	// Tells OpenGL how to walk through the VBOs, i.e., how the data are packed 
	// number of coordinates per vertex (4 here), type of the coordinates, 
	// stride between consecutive vertices, and pointers to the first coordinate

	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), (char*)NULL + 0);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (char*)NULL + 16);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (char*)NULL + 32);

	/* Now we are ready to draw, using the triangle indices in the buffer 
	
	 The following code uses GLM, the OpenGL mathematics library. This is to replace 
	 the OpenGL2.x matrix functions which have been deprecated. 
	 
	 
	 use the GLM helper functions to construct projection and modelview matrices */

	glm::mat4 projection = glm::perspective(60.0f, 1.0f, .1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projection[0][0]);

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

//	 the following transformation is for cylinder

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 5.0f));

//	 construct the modelview and modelview projection matrices 
	glm::mat4 modelview = view * model;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&modelview[0][0]);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glDrawElements(GL_TRIANGLE_STRIP, nIndicesBody, GL_UNSIGNED_INT, (char*)NULL + 0);

	glDisableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glDisableClientState(GL_COLOR_ARRAY); // enable the color array on the client side

	glutSwapBuffers();

	glFlush();

}

void mymotion(int x, int y)
{
	if (xform_mode == XFORM_ROTATE) {
		x_angle += (x - press_x) / 5.0;
		if (x_angle > 180) x_angle -= 360;
		else if (x_angle <-180) x_angle += 360;
		press_x = x;

		y_angle += (y - press_y) / 5.0;
		if (y_angle > 180) y_angle -= 360;
		else if (y_angle <-180) y_angle += 360;
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



void mymouse(int button, int state, int x, int y)
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

void main(int argc, char** argv){
	//BodyPart head1=new BodyPart();
	//BodyPart *body = (BodyPart *)malloc(sizeof(BodyPart));
	body.child = new std::vector<BodyPart>[5];
	body.child[0].push_back(head);
	//body.child[0] -> &(BodyPart *)&head.; 
	//*body.child++;
	
	head.child = new std::vector<BodyPart>[10];
	head.child = NULL;

	body.child[1].push_back(leftArm);
//	*body.child++;

	body.child[2].push_back(rightArm);

//	*body.child = &rightArm;
//	*body.child++;

	body.child[3].push_back(leftLeg);
//	*body.child = &leftLeg;
//	*body.child++;

	body.child[4].push_back(rightLeg);

//	*body.child = &rightLeg;
//	*body.child++;


	leftArm.child = new std::vector<BodyPart>[10];
	leftArm.child[0].push_back(lowerLeftArm);

//	*leftArm.child = &lowerLeftArm;
//	*body.child++;

//	lowerLeftArm.child = new std::vector<BodyPart>[10];
//	leftArm.child[0].push_back(lowerLeftArm);

//	lowerLeftArm.child = new std::vector<BodyPart>[10];
//	leftArm.child[0].push_back(lowerLeftArm);

	lowerLeftArm.child = NULL;

	rightArm.child = new std::vector<BodyPart>[10];
	rightArm.child[0].push_back(lowerRightArm);

//	*rightArm.child = &lowerRightArm;
//	*body.child++;

	lowerRightArm.child = NULL;

	leftLeg.child = new std::vector<BodyPart>[10];
	leftLeg.child[0].push_back(lowerLeftLeg);

//	leftLeg.child = lowerLeftLeg;
//	*body.child++;

	lowerLeftLeg.child = NULL;

	rightArm.child = new std::vector<BodyPart>[10];
	rightArm.child[0].push_back(lowerRightArm);

//	rightArm.child = lowerRightLeg;
//	*body.child++;

	lowerRightLeg.child = NULL;

	
	head.parent = &body, leftArm.parent = &body, rightArm.parent = &body, leftLeg.parent = &body, rightLeg.parent = &body;
	lowerLeftArm.parent = &leftArm; lowerRightArm.parent = &rightArm; lowerLeftLeg.parent = &leftLeg; lowerRightLeg.parent = &rightLeg;

	body.parent = NULL;

	ChildParentOp obj;
	obj.children(body);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);

	glutCreateWindow("shader cube");
	glutDisplayFunc(display);
	glutMouseFunc(mymouse);
	glutMotionFunc(mymotion);
	//glutKeyboardFunc(mykey);

	// initialize GLEW 

	GLenum err = glewInit();

	if (err != GLEW_OK)  printf(" Error initializing GLEW! \n");
	else printf("Initializing GLEW succeeded!\n");

	// define the discretion level for the cylinder 
	int nslices, nstacks;
	nslices = 20;
	nstacks = 20;

	obj.InitBody(nslices, nstacks, 1.0, 0.0, 1.0);
	InitBodyVBO(nslices, nstacks);
//	InitHeadVBO(nslices, nstacks);

	glutMainLoop();
}