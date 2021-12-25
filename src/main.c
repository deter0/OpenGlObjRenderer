#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <math.h>

#include "parseObj.c"

objFile *cube;

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
	}
}
//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
	
	GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}
//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	// Move camera back
	gluPerspective(45.0, //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		1.0, //The near z clipping coordinate
		200.0); //The far z clipping coordinate
	// gluPerspective(45.0,                  //The camera angle
	// 			   (double)w / (double)h, //The width-to-height ratio
	// 			   0.1,                   //The near z clipping coordinate
	// 			   900.0);                //The far z clipping coordinate
}
//Draws the 3D scene
void drawScene() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	for (int i = 0; i < cube->faces->used; i++) {
		// printf("Begin tri.\n");
		// glColor3f(val / 500, val / 500, val / 500);
		glBegin(GL_TRIANGLES);
		
		for (int j = 0; j < cube->faces->array[i]->used; j++) {
			int vertexIndex = (int)cube->faces->array[i]->array[j]->x;
			// printf("Render vertex count: ");
			// printV3(cube->vertices->array[vertexIndex]);
			glNormal3f(
				cube->vertexNormals->array[vertexIndex]->x,
				cube->vertexNormals->array[vertexIndex]->y,
				cube->vertexNormals->array[vertexIndex]->z
			);
			glVertex3f(
				(GLfloat)cube->vertices->array[vertexIndex]->x * 1.5f,
				(GLfloat)cube->vertices->array[vertexIndex]->y * 1.5f,
				(GLfloat)cube->vertices->array[vertexIndex]->z * 1.5f
			);
		}
		// printf("End tri.\n");
		glEnd();
	}
	glutSwapBuffers(); //Send the 3D scene to the screen
}
int main(int argc, char** argv) {
	cube = readObjFile("assets/cube.obj"); // read model

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(400, 400); //Set the window size
	
	//Create the window
	int window = glutCreateWindow("*SUS*sane");

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glEnable(GL_DEPTH_TEST);
	// glBindFragDataLocation(p, 0, "colorOut");
	glEnable(GL_COLOR_MATERIAL);
	// glShadeModel(GL_SMOOTH); //Enable smooth shading
	initRendering(); //Initialize rendering
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop
	return 0;
}