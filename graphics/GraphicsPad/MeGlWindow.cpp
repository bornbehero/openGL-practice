#include <gl\glew.h>
#include "MeGlWindow.h"

static GLuint sizeofvertexcolor;
extern const char* vertexshader;
extern const char* fragmentshader;
void MeGlWindow::initializeGL()
{
	glewInit();
	senddatatoOpenGL();
	installshaders();
}

void MeGlWindow::paintGL()
{
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glViewport(0, 0, width(), height());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)sizeofvertexcolor);

}

void MeGlWindow::senddatatoOpenGL()
{
	GLuint BufferID;
	GLfloat vertexcolor[] =
	{
		-1.0f,1.0f,
		0.0f,0.0f,1.0f,
		1.0f,1.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,
		1.0f,0.0f,.0f,
		-1.0f,-1.0f,
		0.0f,1.0f,0.0f,
		1.0f,-1.0f,
		0.0f,0.0f,1.0f
	};
	sizeofvertexcolor = sizeof(vertexcolor);
	GLushort vertexindices[]
	{
		0,1,2,
		2,3,4
	};
	glGenBuffers(1, &BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexcolor) + sizeof(vertexindices), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexcolor), vertexcolor);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vertexcolor), sizeof(vertexindices),vertexindices);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,5*sizeof(float),0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*)(2*sizeof(float)));
}
extern const char* vertexshader;
extern const char* fragmentshader;
void MeGlWindow::installshaders()
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(VertexShaderID, 1, &vertexshader, 0);
	glShaderSource(FragmentShaderID, 1, &fragmentshader, 0);
	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);
	glUseProgram(programID);


}
