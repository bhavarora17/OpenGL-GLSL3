#include "Lab3Robot.h"

Robot::BaseStructure baseSquare[] = {
	-0.05f, -0.05f, -0.05f, 1.0f, // triangle 1 : begin
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
	0.05f, -0.05f, 0.05f,1.0f
}; 

	void Robot::InitVBOCube()
	{
		GLenum err = glewInit();

		if (err != GLEW_OK) {
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			//return -1;
		}

		glGenBuffers(1, r.Robot::vboHandleCube);   // create an interleaved VBO object
		glBindBuffer(GL_ARRAY_BUFFER, r.Robot::vboHandleCube[0]);   // bind the first handle 
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 108, &baseSquare[0].x, GL_STATIC_DRAW); // allocate space and copy the position data over
		glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 


	}

	void Robot::drawCube(glm::mat4& m, float color[3]){

		if (rr > 1)rr = 0.1;

		rr = rr + 0.1;
		if (g > 1)g = 0.1;

		g = g + 0.1;
		if (b > 1)b = 0.1;

		b = b + 0.1;
		std::cout << rr << " " << g << " " << b;

		glLoadMatrixf(&m[0][0]);
		glColor3f(rr, g, b);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 4);

	}


	void Robot::rememberParent(glm::mat4 &ModelMR){
		mat_stack.push(modelMR);
	}

	glm::mat4 Robot::getParentMatrix(){
		return mat_stack.top();
	}

	void Robot::forgetParent(){
		mat_stack.pop();
	}

	void Robot::draw(glm::mat4 &model){
		float angleRR = (3.141592653589793238463 / 180) * 90;

		//modelMR = rotate(modelMR, angleRR, glm::vec3(1.0,0.0,0.0));
		

		glEnableClientState(GL_VERTEX_ARRAY);

		
		glBindBuffer(GL_ARRAY_BUFFER, vboHandleCube[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOCube);
		
		glVertexPointer(4, GL_FLOAT, 0, 0);// sizeof(float)* 28, (char *)NULL + sizeof(float)* 4);// 4-dimension
		
	

		/////////////////////////////////////////BODY/////////////////////////////	

		rememberParent(modelMR);
		

		glBindBuffer(GL_ARRAY_BUFFER, c.vboHandle[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c.indexVBO);

		glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY); // enable the color array on the client side


		glVertexPointer(4, GL_FLOAT, sizeof(Cylinder::Vertex), (char*)NULL + 0);
		glNormalPointer(GL_FLOAT, sizeof(Cylinder::Vertex), (char*)NULL + 16);
		glColorPointer(4, GL_FLOAT, sizeof(Cylinder::Vertex), (char*)NULL + 32);


		modelMR = glm::translate(modelMR, glm::vec3(0.0f, 0.25f, 0.0f));
		float ang= 90 * (22 / 7.0) / 180.0;
		modelMR = glm::rotate(modelMR, ang,glm::vec3(1.0, 0.0, 0.0));
		modelMR = glm::scale(modelMR, glm::vec3(0.1f, 0.1f, 0.5f));

		modelMR = model*modelMR;
		glLoadMatrixf(&modelMR[0][0]);
		glDrawElements(GL_TRIANGLE_STRIP, c.nindices, GL_UNSIGNED_INT, (char*)NULL + 0);

		glDisableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
		glDisableClientState(GL_COLOR_ARRAY);
		
		
		modelMR = getParentMatrix();
		forgetParent();

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vboHandleCube[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOCube);

		glVertexPointer(4, GL_FLOAT, 0, 0);

		////////////////////////////////////Face//////////////////////////

		rememberParent(modelMR);
		modelMR = glm::translate(modelMR, glm::vec3(0.0, .31f, 0.0f));
		//std::cout << glm::to_string(modelMR[0]) << std::endl << glm::to_string(modelMR[1]) << std::endl << glm::to_string(modelMR[2]) << std::endl << glm::to_string(modelMR[3]) << std::endl;
		
		
		
		//drawCube(r.Robot::modelMR);
		glBindBuffer(GL_ARRAY_BUFFER, s.vboHandleSphere[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.indexVBOSphere);
		
		glVertexPointer(4, GL_FLOAT, sizeof(Sphere::Vertex), (char*)NULL + 0);
		glColorPointer(4, GL_FLOAT, sizeof(Sphere::Vertex), (char*)NULL + sizeof(float)* 4);
		glNormalPointer(GL_FLOAT, sizeof(Sphere::Vertex), (char*)NULL + 32);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		modelMR = glm::scale(modelMR, glm::vec3(0.3f, 0.3f, 0.3f));

		modelMR = model*modelMR;
		glLoadMatrixf(&modelMR[0][0]);
		
		glDrawElements(GL_TRIANGLE_STRIP, s.index.size(), GL_UNSIGNED_INT, (char*)NULL + 0);

		
		
		
		glDisableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
		glDisableClientState(GL_COLOR_ARRAY);

		modelMR = getParentMatrix();
		forgetParent();


		glEnableClientState(GL_VERTEX_ARRAY);

		
		glBindBuffer(GL_ARRAY_BUFFER, vboHandleCube[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOCube);

		glVertexPointer(4, GL_FLOAT, 0, 0);

		//////////////////////////////////left leg////////////////////////////////
		rememberParent(modelMR);
		modelMR = glm::translate(modelMR, glm::vec3(-.05, -0.36, 0.0f));
		modelMR = glm::rotate(modelMR, -angle1, glm::vec3(1.0f, 1.0f, 0.0f));


		rememberParent(modelMR);
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));
		//std::cout << glm::to_string(modelMR[0]) << std::endl << glm::to_string(modelMR[1]) << std::endl << glm::to_string(modelMR[2]) << std::endl << glm::to_string(modelMR[3]) << std::endl;
		
		

		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);

		
		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);


		modelMR = getParentMatrix();
		

		modelMR = glm::translate(modelMR, glm::vec3(0, -0.21, 0.0f));
		modelMR = glm::rotate(modelMR, -angle2, glm::vec3(1.0f, 0.0f, 0.0f));
		
		rememberParent(modelMR);
		
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));
		

		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);

		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;
		
		drawCube(modelMR, color);
		

		///left foot/////////////////////////////////

		modelMR = getParentMatrix();
		modelMR = glm::translate(modelMR, glm::vec3(-0.06,- 0.17, 0.0f));
		modelMR = glm::rotate(modelMR, -angle2, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMR = glm::scale(modelMR, glm::vec3(2, 1, 1));
		

		modelMR = model*modelMR;
		glLoadMatrixf(&modelMR[0][0]);

		
		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);

		forgetParent();
		forgetParent();
		////////////////////////////////right leg///////////////////////////////
		modelMR = getParentMatrix();
	//	rememberParent(modelMR);
		modelMR = glm::translate(modelMR, glm::vec3(.05, -.36f, 0.0f));


		modelMR = glm::rotate(modelMR, angle3, glm::vec3(-1.0f, 1.0f, 0.0f));

		rememberParent(modelMR);
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));
		//std::cout << glm::to_string(modelMR[0]) << std::endl << glm::to_string(modelMR[1]) << std::endl << glm::to_string(modelMR[2]) << std::endl << glm::to_string(modelMR[3]) << std::endl;
		

		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);

		
		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);


		modelMR = getParentMatrix();
		//mat_stack.pop();
		//rememberParent(modelMR);

		modelMR = glm::translate(modelMR, glm::vec3(0, -0.21, 0.0f));
		modelMR = glm::rotate(modelMR, angle4, glm::vec3(-1.0f, 0.0f, 0.0f));
		rememberParent(modelMR);
		
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));
		

		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);

		
		
		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);
		

		///////////////////////////////////////////////////////////
		////right foot

		modelMR = getParentMatrix();
		modelMR = glm::translate(modelMR, glm::vec3(0.06, -0.17, 0.0f));
		modelMR = glm::rotate(modelMR, -angle2, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMR = glm::scale(modelMR, glm::vec3(2, 1, 1));


		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);


		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);


		forgetParent();
		forgetParent();
		//forgetParent();




		///////////////////////////////////HANDS///////////////////////////

		//////////////////LEFT/////////////////////////

		modelMR = getParentMatrix();
		//rememberParent(modelMR);
		modelMR = glm::translate(modelMR, glm::vec3(-.21, 0.16, 0.0f));
		modelMR = glm::rotate(modelMR, angle3, glm::vec3(-1.0f, 1.0f, 0.0f));

		rememberParent(modelMR);
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));


		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);

		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);


		modelMR = getParentMatrix();

		modelMR = glm::translate(modelMR, glm::vec3(0, -0.21, 0.0f));
		modelMR = glm::rotate(modelMR, angle4, glm::vec3(-1.0f, 0.0f, 0.0f));
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));


		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);
		
		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);



		forgetParent();
		modelMR = getParentMatrix();
		forgetParent();


		////////////////RIGHT///////////////

		
		rememberParent(modelMR);
		modelMR = glm::translate(modelMR, glm::vec3(+.21, 0.16, 0.0f));
		modelMR = glm::rotate(modelMR, -angle1, glm::vec3(1.0f, 1.0f, 0.0f));

		rememberParent(modelMR);
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));


		modelMR = model*modelMR;
		glLoadMatrixf(&modelMR[0][0]);

		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);


		modelMR = getParentMatrix();

		modelMR = glm::translate(modelMR, glm::vec3(0, -0.21, 0.0f));
		modelMR = glm::rotate(modelMR, -angle2, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMR = glm::scale(modelMR, glm::vec3(0.9, 2, 1));



		modelMR = model*modelMR;
		//glLoadMatrixf(&modelMR[0][0]);
		glLoadMatrixf(&modelMR[0][0]);

		color[0] = 0.1; color[1] = 0.2, color[2] = 0.3;

		drawCube(modelMR, color);

		forgetParent();
		modelMR = getParentMatrix();
		forgetParent();

		glDisableClientState(GL_VERTEX_ARRAY);
		glutSwapBuffers();
		glFlush();

	}



	/*void Robot::keys1(unsigned char key, int x, int y){

		float d_angle = 10;
		switch (key){
		case 'q':
			exit(1);
			break;
		case 'a':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(-0.1f, 0.0f, 0.0f));
			mainFile::model = glm::translate(file.mainFile::model, glm::vec3(-0.1f, 0.0f, 0.0f));
			break;
		case 'd':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.1f, 0.0f, 0.0f));
			break;
		case 'w':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.0f, 0.0f, -0.1f));
			break;
		case 's':
			r.Robot::modelMR = glm::translate(r.Robot::modelMR, glm::vec3(0.0f, 0.0f, 0.1f));
			break;
		case 'z':
			r.Robot::modelMR = glm::rotate(r.Robot::modelMR, d_angle, glm::vec3(0.0, 1.0, 1.0));
			break;

		case '-':
			r.Robot::modelMR = glm::scale(r.Robot::modelMR, glm::vec3(0.8, 0.8, 1.0));
			break;

		case '+':
			r.Robot::modelMR = glm::scale(r.Robot::modelMR, glm::vec3(1.2, 1.2, 1.0));
			break;

		case '1':

			r.Robot::count1++;
			if (r.Robot::count1 % 2 == 0)r.Robot::angle1 += ((3.14159) / 180) * 50;
			else r.Robot::angle1 -= ((3.14159) / 180) * 50;
			break;

		case '2':
			r.Robot::count2++;
			if (r.Robot::count2 % 2 == 0)r.Robot::angle2 -= ((3.14159) / 180) * 50;
			else r.Robot::angle2 += ((3.14159) / 180) * 50;
			break;

		case '3':
			r.Robot::count3++;
			if (r.Robot::count3 % 2 == 0)r.Robot::angle3 += ((3.14159) / 180) * 50;
			else r.Robot::angle3 -= ((3.14159) / 180) * 50;
			break;

		case '4':
			r.Robot::count4++;
			if (r.Robot::count4 % 2 == 0)r.Robot::angle4 -= ((3.14159) / 180) * 50;
			else r.Robot::angle4 += ((3.14159) / 180) * 50;
			break;

		default:
			std::cout << "Wrong Input";

		}
		glutPostRedisplay();

	}
	*/