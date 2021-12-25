#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#include "parseObj.c"

objFile *cube;
int win_width, win_height;
#define SCALE 200
#define OFFSET 200
void display(void)
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);

	int elapsedTimeMs = glutGet(GLUT_ELAPSED_TIME);
	
	for (int i = 0; i < cube->faces->used; i++) {
		glBegin(GL_TRIANGLES);

		for (int j = 0; j < cube->faces->array[i]->used; j++) {
			int vertexIndex = (int)cube->faces->array[i]->array[j]->x;
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(
				(GLfloat)(cube->vertices->array[vertexIndex]->x * 100 + 200) * 0.5,
				-(GLfloat)cube->vertices->array[vertexIndex]->y * 100 + 200,
				0
			);
		}

		glEnd();
	}
	printf("Flushed!\n");

	glFlush();
}

void on_resize(int w, int h) {
	win_width = w;
	win_height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}
int main(int argc, char *argv[])
{	
	cube = readObjFile("assets/cube.obj");
	// quicksortV3Array_2D(cube->faces, 0, cube->faces->used - 1);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(1,1);

	glDepthFunc(GL_NEVER);
	glutCreateWindow("Susanne");
	glOrtho(0, 240, 480, 5, -1, 1);

	glutReshapeFunc(on_resize);
	glutDisplayFunc(display);
	// glutTimerFunc(16, display, 0);

	glutMainLoop();

	return EXIT_SUCCESS;
}
