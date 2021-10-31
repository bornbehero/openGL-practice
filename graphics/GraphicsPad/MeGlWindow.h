#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <Mesh.h>
#include <QtOpenGL\qglwidget>


class MeGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
	void senddatatoOpenGL();
	void installshaders();
	void keyPressEvent(QKeyEvent*);
	void LoadCubeMap();
	void DrawObjects(Camera & camera);
	bool checkShaderStatus(GLuint ShaderID);
	bool checkProgramStatus(GLuint ProgramID);
	Mesh* importObject(char * filename);
	std::string ReadShaderCode(const char* file);
	static const char* TexFile[];
	GLuint bindandfillvertexbuffer(cyTriMesh* geometry);
	GLuint bindvertexarray(GLuint vbufferID);

	QTimer	*Mytimer;
	Mesh* TestMesh;
	Mesh* StoneMesh;
public:
};

#endif
