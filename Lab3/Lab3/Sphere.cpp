#include "Sphere.h"

void Sphere::InitSphere(const float radius, const unsigned int stacks, const unsigned int slices, float r, float g, float b){
	for(unsigned int stackNumber = 0; stackNumber <= stacks; ++stackNumber)
	{
		for (unsigned int sliceNumber = 0; sliceNumber < slices; ++sliceNumber)
		{
			float theta = stackNumber * PI / (stacks);

			float phi = sliceNumber * 2 * PI / slices;
			float sinTheta = std::sin(theta);
			float sinPhi = std::sin(phi);
			float cosTheta = std::cos(theta);
			float cosPhi = std::cos(phi);
			vertex.normal[0]=vertex.xyzw[0] = radius * cosPhi * sinTheta;
			vertex.normal[1]=vertex.xyzw[1] = radius * sinPhi * sinTheta;
			vertex.xyzw[2] = radius * cosTheta;
			vertex.normal[2] =vertex.xyzw[3] = 1.0;
			vertex.normal[3] = 0.0;
			
			//vertex.xyzw[3] = 0.0f;
			vertex.color[0] = r; vertex.color[1] = g; vertex.color[2] = b;
			vertex.color[3] = 1.0;
			vertices.push_back(vertex);
		}
	}
	int n = 0;

	for (unsigned int stackNumber = 0; stackNumber < stacks; ++stackNumber)
	{	
		for (unsigned int sliceNumber = 0; sliceNumber <= slices; ++sliceNumber)
		{
			index.push_back((stackNumber * slices) + (sliceNumber % slices));
			index.push_back(((stackNumber + 1) * slices) + (sliceNumber % slices));
		}
	}	
	std::cout << index.size();
}


void Sphere::draw(){
	

	glColor4f(1, 0.2, 1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandleSphere[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOSphere);

	glm::mat4 model = glm::mat4(1.0f);

//	model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
//	model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
//	model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

	glLoadMatrixf(&model[0][0]);

	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), (char*)NULL + 0);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (char*)NULL + sizeof(float)* 4);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (char*)NULL + 32);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	
	glm::mat4 projection = glm::perspective(90.0f, 1.0f, .1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projection[0][0]);

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));

	// the following transformation is for cylinder

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 5.0f));

	// construct the modelview and modelview projection matrices 
	glm::mat4 modelview = view * model;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&modelview[0][0]);
	
	glDrawElements(GL_TRIANGLE_STRIP, index.size(), GL_UNSIGNED_INT, (char*)NULL + 0);//replace 3 with index.size()

	glutSwapBuffers();
	glDisableClientState(GL_VERTEX_ARRAY);
	//r++;
	glFlush();
	//s.draw();

}


void Sphere::InitVBOSphere(int nslices, int nstacks){

	int nvertices = nslices * (nstacks+1);

	glGenBuffers(1, vboHandleSphere);   // create an interleaved VBO object
	glBindBuffer(GL_ARRAY_BUFFER, vboHandleSphere[0]);   // bind the first handle 

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, &vertices[0], GL_STATIC_DRAW); // allocate space and copy the position data over 
	glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

	glGenBuffers(1, &indexVBOSphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOSphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*index.size(), &index[0], GL_STATIC_DRAW);  // load the index data 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // clean up 

}



std::vector<Sphere::Vertex> Sphere::getVertices(){
	return (s.vertices);
}


std::vector<GLuint> Sphere::getIndices(){
	return s.index;
}
