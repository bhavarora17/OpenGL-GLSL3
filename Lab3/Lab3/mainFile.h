#include "MazeGenerator.h"
#include "Lab3Robot.h"

#ifndef _MAIN_FILE_H
#define _MAIN_FILE_H


class mainFile{
	public:

		glm::mat4 model = glm::mat4(1.0f);
		float posx = 0.0f;
		float posy = 0.0f;
		float posz = 0.0f;
		glm::mat4 &modelV = glm::mat4(1.0);
	int xform_mode = 0;
#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

	int press_x, press_y;
	int release_x, release_y;

	float M_PI = 22 / 7.0; 
		
	float x_angle = 0.0;
	float y_angle = 0.0;
	float z_angle = 0.0;
	float scale_size = 1;

	int width = 800;
	int height = 800;
	
	std::vector<int> *abc;
	

public:
	struct BaseStructure
	{
		float x, y, z;
	};



	
		void mymouse(int button, int state, int x, int y);

	void drawing(glm::mat4& m);
	GLubyte tindices[6];

	GLuint vboHandle[1];   // a VBO that contains interleaved positions and colors 
	GLuint indexVBO;
	void mymotion(int x, int y);

	glm::mat4 modelM = glm::mat4(1.0f);
	std::stack<glm::mat4> mat_stack;

}file;

#endif _MAIN_FILE_H