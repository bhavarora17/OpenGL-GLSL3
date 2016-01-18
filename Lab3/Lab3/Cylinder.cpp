#include "Cylinder.h"



void Cylinder::InitCylinder(int nslices, int nstacks, float r, float g, float b)
{
	int nvertices = nslices * nstacks;
	cyverts = new Vertex[nvertices];

	float Dangle = 2 * M_PI1 / (float)(nslices - 1);

	for (int j = 0; j<nstacks; j++)
	for (int i = 0; i<nslices; i++) {
		int idx = j*nslices + i; // mesh[j][i] 
		float angle = Dangle * i;
		cyverts[idx].location[0] = cyverts[idx].normal[0] = cos(angle);
		cyverts[idx].location[1] = cyverts[idx].normal[1] = sin(angle);
		cyverts[idx].location[2] = j*1.0 / (float)(nstacks - 1);
		cyverts[idx].normal[2] = 0.0;
		cyverts[idx].location[3] = 1.0;  cyverts[idx].normal[3] = 0.0;
		cyverts[idx].color[0] = r; cyverts[idx].color[1] = g; cyverts[idx].color[2] = b;
		cyverts[idx].color[3] = 1.0;
	}
	// now create the index array 

	nindices = (nstacks - 1) * 2 * (nslices + 1);
	cindices = new GLuint[nindices];
	int n = 0;
	for (int j = 0; j<nstacks - 1; j++)
	for (int i = 0; i <= nslices; i++) {
		int mi = i % nslices;
		int idx = j*nslices + mi; // mesh[j][mi] 
		int idx2 = (j + 1) * nslices + mi;
		cindices[n++] = idx;
		cindices[n++] = idx2;
	}
}

////////////////////////////////////////////////////////////////////////////////////
void Cylinder::drawCylinder(){

	/*Cylinder::Vertex *x = c.getCylinderVertices();
	std::cout << x[1].location[1];*/

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

	// Now we are ready to draw, using the triangle indices in the buffer 
	//
	// The following code uses GLM, the OpenGL mathematics library. This is to replace 
	// the OpenGL2.x matrix functions which have been deprecated. 
	// 
	// 
	// use the GLM helper functions to construct projection and modelview matrices 

	
	// the following transformation is for cylinder

	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glDrawElements(GL_TRIANGLE_STRIP, nindices, GL_UNSIGNED_INT, (char*)NULL + 0);

	glDisableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glDisableClientState(GL_COLOR_ARRAY); // enable the color array on the client side

	glutSwapBuffers();

}
 
void Cylinder::InitCylVBO(int nslices, int nstacks)
{
	int nvertices = nslices * nstacks;
	nindices = (nstacks - 1) * 2 * (nslices + 1);

	glGenBuffers(1, vboHandle);   // create an interleaved VBO object
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle 

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, cyverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, cindices, GL_STATIC_DRAW);  // load the index data 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // clean up 

	// by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time 
}

Cylinder::Vertex* Cylinder::getCylinderVertices(){
	return cyverts;
}

GLuint* Cylinder::getCylinderIndex(){
	return cindices;
}

