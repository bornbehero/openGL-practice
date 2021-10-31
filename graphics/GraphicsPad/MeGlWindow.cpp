#include <gl\glew.h>
#include <MeGlWindow.h>
#include <Vertex.h>
#include <ShapeFactory.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>
#include <QtGui\qkeyevent>
#include <Camera.h>
#include <iostream>
#include <fstream>

static GLuint   CubeElementArrayOffset;
static GLuint   PlaneElementArrayOffset;
static GLuint	CubenumIndices;
static GLuint	PlanenumIndices;
static GLuint	CubeObjectID;
static GLuint	PlaneObjectID;

GLuint programID;
GLuint LightProgramID;
GLuint CubeMapProgramID;
GLuint PlaneShadowProgramID;
GLuint TestProgramID;
GLuint PBRProgram;
GLuint StoneShadowProgram;

GLuint FrameBufferID;
GLuint FrameTextureID;
GLuint FrameDepthID;

GLfloat AttenuationFactor = 0.1;

Camera MainCamera;
Camera LightCamera;

bool changeTex = true;

glm::vec3 LightPosition(0.0f, 2.5f, -5.0f);
float RotationAngle = 0.0f;

const char* MeGlWindow::TexFile[] = { "right.png","left.png","bottom.png","top.png","back.png","front.png" };

void MeGlWindow::senddatatoOpenGL()
{
	glClearColor(0, 0, 0, 1);

	Shapedata Cube = ShapeFactory::MakeCube();
	Shapedata Plane = ShapeFactory::MakePlane();

	GLuint BufferID;
	GLuint	currentbufferoffset = 0;

	glGenBuffers(1, &BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glBufferData(GL_ARRAY_BUFFER, Cube.VertexBufferSize()+Cube.IndicesBufferSize()+Plane.VertexBufferSize()+Plane.IndicesBufferSize(), 0, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, Cube.VertexBufferSize(), Cube.vertices);
	currentbufferoffset = Cube.VertexBufferSize();
	CubeElementArrayOffset = currentbufferoffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentbufferoffset, Cube.IndicesBufferSize(), Cube.Indices);
	currentbufferoffset += Cube.IndicesBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentbufferoffset, Plane.VertexBufferSize(),Plane.vertices);
	currentbufferoffset += Plane.VertexBufferSize();
	PlaneElementArrayOffset = currentbufferoffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentbufferoffset, Plane.IndicesBufferSize(), Plane.Indices);
	CubenumIndices = Cube.numIndices;
	PlanenumIndices = Plane.numIndices;

	glGenVertexArrays(1, &CubeObjectID);
	glGenVertexArrays(1, &PlaneObjectID);

	glBindVertexArray(CubeObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(11 * sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);

	glBindVertexArray(PlaneObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(Cube.VertexBufferSize()+Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float) + Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float) + Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(9 * sizeof(float) + Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(11 * sizeof(float)+ Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);

	//ImportMesh Here
	TestMesh = importObject("Assets\\Window.obj");
	if (!TestMesh)
		return;

	StoneMesh = importObject("Assets\\Stone.obj");
	if (!StoneMesh)
		return;


	const char* texName = "MyTexture.png";
	QImage texture = QGLWidget::convertToGLFormat(QImage(texName, "PNG"));

	glActiveTexture(GL_TEXTURE0);

	GLuint TextureBufferID;
	glGenTextures(1, &TextureBufferID);
	glBindTexture(GL_TEXTURE_2D, TextureBufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	

	const char* normalMapName = "Assets\\WindowNormal.png";
	QImage Normalmap = QGLWidget::convertToGLFormat(QImage(normalMapName, "PNG"));

	glActiveTexture(GL_TEXTURE1);

	GLuint NormalBufferID;
	glGenTextures(1,&NormalBufferID);
	glBindTexture(GL_TEXTURE_2D, NormalBufferID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Normalmap.width(), Normalmap.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Normalmap.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	LoadCubeMap();

	glActiveTexture(GL_TEXTURE3);

	glGenFramebuffers(1, &FrameBufferID);
	glBindBuffer(GL_FRAMEBUFFER, FrameBufferID);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_DEPTH_ATTACHMENT);

	glGenTextures(1, &FrameTextureID);
	glBindTexture(GL_TEXTURE_2D, FrameTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_FLOAT,NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, FrameTextureID, 0);

	glActiveTexture(GL_TEXTURE4);
	glGenTextures(1, &FrameDepthID);
	glBindTexture(GL_TEXTURE_2D, FrameDepthID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, FrameDepthID, 0);

	const char* tex2Name = "Wood.png";
	QImage texture2 = QGLWidget::convertToGLFormat(QImage(tex2Name, "PNG"));

	glActiveTexture(GL_TEXTURE5);

	GLuint Texture2BufferID;
	glGenTextures(1, &Texture2BufferID);
	glBindTexture(GL_TEXTURE_2D, Texture2BufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture2.width(), texture2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	const char* tex3Name = "FlatNormalMap.png";
	QImage texture3 = QGLWidget::convertToGLFormat(QImage(tex3Name, "PNG"));

	glActiveTexture(GL_TEXTURE6);

	GLuint Texture3BufferID;
	glGenTextures(1, &Texture3BufferID);
	glBindTexture(GL_TEXTURE_2D, Texture3BufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture3.width(), texture3.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture3.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	const char* tex4Name = "Assets\\WindowBaseColor.png";
	QImage texture4 = QGLWidget::convertToGLFormat(QImage(tex4Name, "PNG"));
	
	glActiveTexture(GL_TEXTURE7);
	
	GLuint Texture4BufferID;
	glGenTextures(1, &Texture4BufferID);
	glBindTexture(GL_TEXTURE_2D, Texture4BufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture4.width(), texture4.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture4.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	const char* tex5Name = "Assets\\WindowAlpha.png";
	QImage texture5 = QGLWidget::convertToGLFormat(QImage(tex5Name, "PNG"));
	
	glActiveTexture(GL_TEXTURE8);
	
	GLuint Texture5BufferID;
	glGenTextures(1, &Texture5BufferID);
	glBindTexture(GL_TEXTURE_2D, Texture5BufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture5.width(), texture5.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture5.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	const char* tex6Name = "Assets\\WindowMetallic.png";
	QImage texture6 = QGLWidget::convertToGLFormat(QImage(tex6Name, "PNG"));
	
	glActiveTexture(GL_TEXTURE9);
	
	GLuint Texture6BufferID;
	glGenTextures(1, &Texture6BufferID);
	glBindTexture(GL_TEXTURE_2D, Texture6BufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture6.width(), texture6.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture6.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	const char* tex7Name = "Assets\\WindowRoughness.png";
	QImage texture7 = QGLWidget::convertToGLFormat(QImage(tex7Name, "PNG"));
	
	glActiveTexture(GL_TEXTURE10);
	
	GLuint Texture7BufferID;
	glGenTextures(1, &Texture7BufferID);
	glBindTexture(GL_TEXTURE_2D, Texture7BufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture7.width(), texture7.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture7.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	const char* tex8Name = "Assets\\StoneBaseColor.png";
	QImage texture8 = QGLWidget::convertToGLFormat(QImage(tex8Name, "PNG"));

	glActiveTexture(GL_TEXTURE11);

	GLuint Texture8BufferID;
	glGenTextures(1, &Texture8BufferID);
	glBindTexture(GL_TEXTURE_2D, Texture8BufferID);
	//	int a = texture.height();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture8.width(), texture8.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture8.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	Cube.cleanup();
	Plane.cleanup();
}
void MeGlWindow::LoadCubeMap() {
	glActiveTexture(GL_TEXTURE2);
	GLuint CubeBufferID;
	glGenTextures(1, &CubeBufferID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeBufferID);
	
	for (int i = 0; i < 6; ++i) {
		QImage Texdata = QGLWidget::convertToGLFormat(QImage(TexFile[i], "PNG"));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, Texdata.width(), Texdata.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Texdata.bits());
	}
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool MeGlWindow::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeGlWindow::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

Mesh * MeGlWindow::importObject(char * filename)
{
	if (!filename)
	{
		return nullptr;
	}
	cyTriMesh* geometry = new cyTriMesh();
	bool Loaded = geometry->LoadFromFileObj(filename);
	std::vector<cyTriMesh::TriFace> vt;
	for (int i = 0; i < geometry->NF(); ++i)
	{
		vt.push_back(geometry->FT(i));
	}
	if (!Loaded)
	{
		printf("File Import Failed");
		return nullptr;
	}
	geometry->ComputeBoundingBox();
	if (!geometry->IsBoundBoxReady())
	{
		printf("BoundBox Generate Failed");
	}
	Mesh* m = new Mesh(geometry);
	m->setVBufferID(bindandfillvertexbuffer(geometry));
	m->setVArrayID(bindvertexarray(m->getVBufferID()));

	return m;
}

std::string MeGlWindow::ReadShaderCode(const char* fileName)
{
	std::ifstream meInput(fileName);
	if (!meInput.good())
	{
		std::cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

GLuint MeGlWindow::bindandfillvertexbuffer(cyTriMesh * geometry)
{
	std::vector<Vertex_data> All_ver_data;
	for (int i = 0; i < geometry->NF() * 3; ++i)
	{
		Vertex_data vtx(geometry->V(geometry->F(i / 3).v[i % 3]), geometry->VN(geometry->FN(i / 3).v[i % 3]), geometry->VT(geometry->FT(i / 3).v[i % 3]));
		All_ver_data.push_back(vtx);
	}

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, All_ver_data.size() * sizeof(Vertex_data), &All_ver_data[0], GL_STATIC_DRAW);
	return bufferID;
}

GLuint MeGlWindow::bindvertexarray(GLuint vbufferID)
{
	GLuint GeometryID;
	glGenVertexArrays(1, &GeometryID);
	glBindVertexArray(GeometryID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	return GeometryID;
}

void MeGlWindow::installshaders()
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	std::string temp = ReadShaderCode("VertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("FragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("CubeLightVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("CubeLightFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	LightProgramID = glCreateProgram();
	glAttachShader(LightProgramID, VertexShaderID);
	glAttachShader(LightProgramID, FragmentShaderID);
	glLinkProgram(LightProgramID);
	if (!checkProgramStatus(LightProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("CubeMapVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("CubeMapFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	CubeMapProgramID = glCreateProgram();
	glAttachShader(CubeMapProgramID, VertexShaderID);
	glAttachShader(CubeMapProgramID, FragmentShaderID);
	glLinkProgram(CubeMapProgramID);
	if (!checkProgramStatus(CubeMapProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("ShadowMapVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("ShadowMapFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	TestProgramID = glCreateProgram();
	glAttachShader(TestProgramID, VertexShaderID);
	glAttachShader(TestProgramID, FragmentShaderID);
	glLinkProgram(TestProgramID);
	if (!checkProgramStatus(TestProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("PlaneShadowVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("PlaneShadowFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	PlaneShadowProgramID = glCreateProgram();
	glAttachShader(PlaneShadowProgramID, VertexShaderID);
	glAttachShader(PlaneShadowProgramID, FragmentShaderID);
	glLinkProgram(PlaneShadowProgramID);
	if (!checkProgramStatus(PlaneShadowProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("PBRVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("PBRFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	PBRProgram = glCreateProgram();
	glAttachShader(PBRProgram, VertexShaderID);
	glAttachShader(PBRProgram, FragmentShaderID);
	glLinkProgram(PBRProgram);
	if (!checkProgramStatus(PBRProgram))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("StoneShadowVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("StoneShadowFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	StoneShadowProgram = glCreateProgram();
	glAttachShader(StoneShadowProgram, VertexShaderID);
	glAttachShader(StoneShadowProgram, FragmentShaderID);
	glLinkProgram(StoneShadowProgram);
	if (!checkProgramStatus(StoneShadowProgram))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	installshaders();
	senddatatoOpenGL();
	Mytimer = new QTimer(this);

	connect(Mytimer, SIGNAL(timeout()), this, SLOT(update()));
	Mytimer->setInterval(0);
	Mytimer->start();
}

void MeGlWindow::paintGL()
{
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	glViewport(0, 0, 320, 180);

	glViewport(0, 0, width(), height());

	//render to shadow map
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBufferID);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FrameTextureID, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FrameDepthID, 0);
	GLuint status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	LightCamera.setPosition(LightPosition);
	DrawObjects(LightCamera);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glm::mat4 CameraMatrix = MainCamera.getWorldToViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(90.0f, ((float)width() / height()), 0.3f, 100.0f);
	glm::mat4 LightProjectionMatrix = glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f);

	glm::mat4 World2ProjectionMatrix = projectionMatrix * CameraMatrix;

	glm::mat4 FullTransformMatrix;

	//plane
	glUseProgram(PlaneShadowProgramID);
	glBindVertexArray(PlaneObjectID);

	GLint TextureUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "MyTexture");
	glUniform1i(TextureUniformLocation, 5);
	GLint NormalmapUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "NormalMap");
	glUniform1i(NormalmapUniformLocation, 6);

	GLint ShadowColorUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "ShadowColorTex");
	glUniform1i(ShadowColorUniformLocation, 3);

	GLint ShadowMapUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "ShadowMap");
	glUniform1i(ShadowMapUniformLocation, 4);

	glm::mat4 LightWorld2ProjectionMatrix = projectionMatrix * LightCamera.getWorldToViewMatrix();
	GLint LightFullTransformMatrixUniformLocaiton = glGetUniformLocation(PlaneShadowProgramID, "LightFullTransformMatrix");
	glUniformMatrix4fv(LightFullTransformMatrixUniformLocaiton, 1, GL_FALSE, &LightWorld2ProjectionMatrix[0][0]);

	glm::mat4 TransformMatrix;
	glm::mat4 RotationMatrix;

	TransformMatrix = glm::translate(glm::mat4(), glm::vec3(-0.0f, -2.0f, -5.0f));
	RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 PlaneScaleMatrix = glm::scale(glm::mat4(), glm::vec3(2.0f, 2.0f, 2.0f));

	FullTransformMatrix = World2ProjectionMatrix * TransformMatrix* PlaneScaleMatrix * RotationMatrix ;
	glm::mat4 PlaneModel2WorldMatrix = TransformMatrix * PlaneScaleMatrix * RotationMatrix ;

	//Light Begins Here
	glm::vec3 AmbientLight(0.2f, 0.2f, 0.2f);

	GLuint AmbientLightUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "AmbientLight");
	glUniform3fv(AmbientLightUniformLocation, 1, &AmbientLight[0]);

	GLuint LightPositionUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "LightPosition");
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);

	GLuint ViewPositionUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "ViewPosition");
	glUniform3fv(ViewPositionUniformLocation, 1, &MainCamera.getPosition()[0]);

	GLuint AttenuationUniformLocation = glGetUniformLocation(PlaneShadowProgramID, "AttenuationFactor");
	glUniform1f(AttenuationUniformLocation, AttenuationFactor);
	GLuint FullTransformMatrixUniformLocaiton;
	FullTransformMatrixUniformLocaiton = glGetUniformLocation(PlaneShadowProgramID, "FullTransformMatrix");
	GLuint Model2WorldMatrixUniformLocaiton;
	Model2WorldMatrixUniformLocaiton = glGetUniformLocation(PlaneShadowProgramID, "Model2WorldMatrix");
	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(Model2WorldMatrixUniformLocaiton, 1, GL_FALSE, &PlaneModel2WorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, PlanenumIndices, GL_UNSIGNED_SHORT, (void*)(PlaneElementArrayOffset));

	//Stone
	glUseProgram(StoneShadowProgram);
	glBindVertexArray(StoneMesh->getVArrayID());
	TextureUniformLocation = glGetUniformLocation(StoneShadowProgram, "MyTexture");
	glUniform1i(TextureUniformLocation, 11);
	NormalmapUniformLocation = glGetUniformLocation(StoneShadowProgram, "NormalMap");
	glUniform1i(NormalmapUniformLocation, 6);

	ShadowColorUniformLocation = glGetUniformLocation(StoneShadowProgram, "ShadowColorTex");
	glUniform1i(ShadowColorUniformLocation, 3);

	ShadowMapUniformLocation = glGetUniformLocation(StoneShadowProgram, "ShadowMap");
	glUniform1i(ShadowMapUniformLocation, 4);

	LightWorld2ProjectionMatrix = projectionMatrix * LightCamera.getWorldToViewMatrix();
	LightFullTransformMatrixUniformLocaiton = glGetUniformLocation(StoneShadowProgram, "LightFullTransformMatrix");
	glUniformMatrix4fv(LightFullTransformMatrixUniformLocaiton, 1, GL_FALSE, &LightWorld2ProjectionMatrix[0][0]);

	TransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -2.0f, -8.0f));
	RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	PlaneScaleMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

	FullTransformMatrix = World2ProjectionMatrix * TransformMatrix* PlaneScaleMatrix * RotationMatrix;
	glm::mat4 StoneModel2WorldMatrix = TransformMatrix * PlaneScaleMatrix * RotationMatrix;

	FullTransformMatrixUniformLocaiton = glGetUniformLocation(StoneShadowProgram, "FullTransformMatrix");

	Model2WorldMatrixUniformLocaiton = glGetUniformLocation(StoneShadowProgram, "Model2WorldMatrix");

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(Model2WorldMatrixUniformLocaiton, 1, GL_FALSE, &StoneModel2WorldMatrix[0][0]);

	AmbientLightUniformLocation = glGetUniformLocation(StoneShadowProgram, "AmbientLight");
	glUniform3fv(AmbientLightUniformLocation, 1, &AmbientLight[0]);

	LightPositionUniformLocation = glGetUniformLocation(StoneShadowProgram, "LightPosition");
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);

	ViewPositionUniformLocation = glGetUniformLocation(StoneShadowProgram, "ViewPosition");
	glUniform3fv(ViewPositionUniformLocation, 1, &MainCamera.getPosition()[0]);

	AttenuationUniformLocation = glGetUniformLocation(StoneShadowProgram, "AttenuationFactor");
	glUniform1f(AttenuationUniformLocation, AttenuationFactor);

	unsigned int StoneNumIndices = StoneMesh->getGeometry()->NF() * 3;
	glDrawArrays(GL_TRIANGLES, 0, StoneNumIndices);

	//CubeLight
	glUseProgram(LightProgramID);
	glBindVertexArray(CubeObjectID);
	TransformMatrix = glm::translate(glm::mat4(), LightPosition);
	RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.08f, 0.08f, 0.08f));

	GLuint LightTransformMatrixUniformLocation = glGetUniformLocation(LightProgramID, "LightTransformMatrix");
	FullTransformMatrix = World2ProjectionMatrix  *  TransformMatrix * ScaleMatrix * RotationMatrix;
	glUniformMatrix4fv(LightTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, CubenumIndices, GL_UNSIGNED_SHORT, (void*)CubeElementArrayOffset);

	RotationAngle += 5.0f;
	if (RotationAngle > 360.0f) RotationAngle -= 360.0f;

	//CubeMap
	glUseProgram(CubeMapProgramID);

	GLint CubeMapUniformLocation = glGetUniformLocation(CubeMapProgramID, "CubeMap");
	glUniform1i(CubeMapUniformLocation, 2);

	glBindVertexArray(CubeObjectID);
	ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(50.0f, 50.0f, 50.0f));

	GLuint SkyboxTransformMatrixUniformLocation = glGetUniformLocation(CubeMapProgramID, "SkyboxTransformMatrix");
	CameraMatrix[3][0] = 0.0;
	CameraMatrix[3][1] = 0.0;
	CameraMatrix[3][2] = 0.0;
	FullTransformMatrix = projectionMatrix * CameraMatrix * ScaleMatrix;
	glUniformMatrix4fv(SkyboxTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, CubenumIndices, GL_UNSIGNED_SHORT, (void*)CubeElementArrayOffset);

	

	glUseProgram(PBRProgram);

	TextureUniformLocation = glGetUniformLocation(PBRProgram, "MyTexture");
	if (changeTex)
		glUniform1i(TextureUniformLocation, 7);
	else
		glUniform1i(TextureUniformLocation, 0);
	NormalmapUniformLocation = glGetUniformLocation(PBRProgram, "NormalMap");
	glUniform1i(NormalmapUniformLocation, 1);
	GLint AlphaUniformLocation = glGetUniformLocation(PBRProgram, "AlphaTexture");
	glUniform1i(AlphaUniformLocation, 8);
	GLint MetallicUniformLocation = glGetUniformLocation(PBRProgram, "MetallicTexture");
	glUniform1i(MetallicUniformLocation, 9);
	GLint RoughnessUniformLocation = glGetUniformLocation(PBRProgram, "RoughnessTexture");
	glUniform1i(RoughnessUniformLocation, 10);
	
	FullTransformMatrixUniformLocaiton = glGetUniformLocation(PBRProgram, "FullTransformMatrix");
	
	Model2WorldMatrixUniformLocaiton = glGetUniformLocation(PBRProgram, "Model2WorldMatrix");
	

	AmbientLightUniformLocation = glGetUniformLocation(PBRProgram, "AmbientLight");
	glUniform3fv(AmbientLightUniformLocation, 1, &AmbientLight[0]);

	LightPositionUniformLocation = glGetUniformLocation(PBRProgram, "LightPosition");
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);

	ViewPositionUniformLocation = glGetUniformLocation(PBRProgram, "ViewPosition");
	glUniform3fv(ViewPositionUniformLocation, 1, &MainCamera.getPosition()[0]);

	AttenuationUniformLocation = glGetUniformLocation(PBRProgram, "AttenuationFactor");
	glUniform1f(AttenuationUniformLocation, AttenuationFactor);

	//Cube2
	glBindVertexArray(TestMesh->getVArrayID());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				
				
	TransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -2.0f, -5.0f));
	RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	PlaneScaleMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

	FullTransformMatrix = World2ProjectionMatrix * TransformMatrix* PlaneScaleMatrix * RotationMatrix;
	glm::mat4 Cube2Model2WorldMatrix = TransformMatrix * PlaneScaleMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(Model2WorldMatrixUniformLocaiton, 1, GL_FALSE, &Cube2Model2WorldMatrix[0][0]);

	unsigned int NumIndices = TestMesh->getGeometry()->NF() * 3;
	glDrawArrays(GL_TRIANGLES, 0, NumIndices);
	
	//glUseProgram(TestProgramID);
	//
	//TextureUniformLocation = glGetUniformLocation(TestProgramID, "MyTexture");
	//glUniform1i(TextureUniformLocation, 4);
	//glBindVertexArray(CubeObjectID);
	//CameraMatrix = MainCamera.getWorldToViewMatrix();
	//projectionMatrix = glm::perspective(60.0f, ((float)width() / height()), 0.1f, 100.0f);
	//
	//World2ProjectionMatrix = projectionMatrix * CameraMatrix;
	//
	//TransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f,0.0f,-5.0f));
	//RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	//
	//GLuint FullTransformMatrixUniformLocation = glGetUniformLocation(TestProgramID, "FullTransformMatrix");
	//FullTransformMatrix = World2ProjectionMatrix  *  TransformMatrix * ScaleMatrix * RotationMatrix;
	//glUniformMatrix4fv(FullTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	//
	//glDrawElements(GL_TRIANGLES, CubenumIndices, GL_UNSIGNED_SHORT, (void*)CubeElementArrayOffset);
	glDisable(GL_BLEND);
}
void MeGlWindow::DrawObjects(Camera & camera){

	glm::mat4 CameraMatrix = camera.getWorldToViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(90.0f, ((float)width() / height()), 0.3f, 100.0f);

	glm::mat4 World2ProjectionMatrix = projectionMatrix * CameraMatrix;

	glm::mat4 FullTransformMatrix;

	glUseProgram(PBRProgram);
	glBindVertexArray(TestMesh->getVArrayID());
	GLint TextureUniformLocation = glGetUniformLocation(PBRProgram, "MyTexture");
	if (changeTex)
		glUniform1i(TextureUniformLocation, 7);
	else 
		glUniform1i(TextureUniformLocation, 0);
	GLint NormalmapUniformLocation = glGetUniformLocation(PBRProgram, "NormalMap");
	glUniform1i(NormalmapUniformLocation, 1);
	GLint AlphaUniformLocation = glGetUniformLocation(PBRProgram, "AlphaTexture");
	glUniform1i(AlphaUniformLocation, 8);
	GLint MetallicUniformLocation = glGetUniformLocation(PBRProgram, "MetallicTexture");
	glUniform1i(MetallicUniformLocation, 9);
	GLint RoughnessUniformLocation = glGetUniformLocation(PBRProgram, "RoughnessTexture");
	glUniform1i(RoughnessUniformLocation, 10);
	
	GLuint FullTransformMatrixUniformLocaiton;
	FullTransformMatrixUniformLocaiton = glGetUniformLocation(PBRProgram, "FullTransformMatrix");
	GLuint Model2WorldMatrixUniformLocaiton;
	Model2WorldMatrixUniformLocaiton = glGetUniformLocation(PBRProgram, "Model2WorldMatrix");
	glm::mat4 TransformMatrix;
	glm::mat4 RotationMatrix;
	//Light Begins Here
	glm::vec3 AmbientLight(0.2f, 0.2f, 0.2f);

	

	GLuint AmbientLightUniformLocation = glGetUniformLocation(PBRProgram, "AmbientLight");
	glUniform3fv(AmbientLightUniformLocation, 1, &AmbientLight[0]);

	GLuint LightPositionUniformLocation = glGetUniformLocation(PBRProgram, "LightPosition");
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);

	GLuint ViewPositionUniformLocation = glGetUniformLocation(PBRProgram, "ViewPosition");
	glUniform3fv(ViewPositionUniformLocation, 1, &camera.getPosition()[0]);

	GLuint AttenuationUniformLocation = glGetUniformLocation(PBRProgram, "AttenuationFactor");
	glUniform1f(AttenuationUniformLocation,AttenuationFactor);

	////Cube1
	//glBindVertexArray(CubeObjectID);
	//TransformMatrix = glm::translate(glm::mat4(), glm::vec3(+3.0f, 0.0f, -5.0f));
	//RotationMatrix = glm::rotate(glm::mat4(), RotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//
	//FullTransformMatrix = World2ProjectionMatrix * TransformMatrix * RotationMatrix;
	//glm::mat4 Cube1Model2WorldMatrix = TransformMatrix * RotationMatrix;
	//
	//
	//glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	//glUniformMatrix4fv(Model2WorldMatrixUniformLocaiton, 1, GL_FALSE, &Cube1Model2WorldMatrix[0][0]);
	//
	//glDrawElements(GL_TRIANGLES, CubenumIndices, GL_UNSIGNED_SHORT, (void*)(CubeElementArrayOffset));

	//Cube2
	TransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -2.0f, -5.0f));
	RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 PlaneScaleMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

	FullTransformMatrix = World2ProjectionMatrix * TransformMatrix* PlaneScaleMatrix * RotationMatrix ;
	glm::mat4 Cube2Model2WorldMatrix = TransformMatrix * PlaneScaleMatrix * RotationMatrix ;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(Model2WorldMatrixUniformLocaiton, 1, GL_FALSE, &Cube2Model2WorldMatrix[0][0]);

	unsigned int NumIndices = TestMesh->getGeometry()->NF() * 3;
	glDrawArrays(GL_TRIANGLES, 0, NumIndices);

	////plane
	//glBindVertexArray(PlaneObjectID);
	//TransformMatrix = glm::translate(glm::mat4(), glm::vec3(-0.0f, -2.0f, -5.0f));
	//RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//
	//FullTransformMatrix = World2ProjectionMatrix * TransformMatrix * RotationMatrix;
	//glm::mat4 PlaneModel2WorldMatrix = TransformMatrix * RotationMatrix;
	//
	//glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	//glUniformMatrix4fv(Model2WorldMatrixUniformLocaiton, 1, GL_FALSE, &PlaneModel2WorldMatrix[0][0]);
	//
	//glDrawElements(GL_TRIANGLES, PlanenumIndices, GL_UNSIGNED_SHORT, (void*)(PlaneElementArrayOffset));
	//
	////CubeLight
	//glUseProgram(LightProgramID);
	//glBindVertexArray(CubeObjectID);
	//TransformMatrix = glm::translate(glm::mat4(),LightPosition);
	//RotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.08f, 0.08f, 0.08f));
	//
	//GLuint LightTransformMatrixUniformLocation = glGetUniformLocation(LightProgramID, "LightTransformMatrix");
	//FullTransformMatrix = World2ProjectionMatrix  *  TransformMatrix * ScaleMatrix * RotationMatrix;
	//glUniformMatrix4fv(LightTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	//
	//glDrawElements(GL_TRIANGLES, CubenumIndices, GL_UNSIGNED_SHORT, (void*)CubeElementArrayOffset);
	//
	//RotationAngle += 5.0f;
	//if (RotationAngle > 360.0f) RotationAngle -= 360.0f;
	//
	////CubeMap
	//glUseProgram(CubeMapProgramID);
	//
	//GLint CubeMapUniformLocation = glGetUniformLocation(CubeMapProgramID, "CubeMap");
	//glUniform1i(CubeMapUniformLocation, 2);
	//
	//glBindVertexArray(CubeObjectID);
	//ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(50.0f, 50.0f, 50.0f));
	//
	//GLuint SkyboxTransformMatrixUniformLocation = glGetUniformLocation(CubeMapProgramID, "SkyboxTransformMatrix");
	//CameraMatrix[3][0] = 0.0;
	//CameraMatrix[3][1] = 0.0;
	//CameraMatrix[3][2] = 0.0;
	//FullTransformMatrix = projectionMatrix * CameraMatrix * ScaleMatrix;
	//glUniformMatrix4fv(SkyboxTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	//
	//glDrawElements(GL_TRIANGLES, CubenumIndices, GL_UNSIGNED_SHORT, (void*)CubeElementArrayOffset);
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		MainCamera.move_forward();
		break;
	case Qt::Key::Key_S:
		MainCamera.move_backward();
		break;
	case Qt::Key::Key_A:
		MainCamera.move_leftward();
		break;
	case Qt::Key::Key_D:
		MainCamera.move_rightward();
		break;
	case Qt::Key::Key_R:
		MainCamera.move_upward();
		break;
	case Qt::Key::Key_F:
		MainCamera.move_downward();
		break;
	case Qt::Key::Key_Q:
		MainCamera.rotate_left();
		break;
	case Qt::Key::Key_E:
		MainCamera.rotate_right();
		break;
	case Qt::Key::Key_Z:
		MainCamera.rotate_up();
		break;
	case Qt::Key::Key_C:
		MainCamera.rotate_down();
		break;
	case Qt::Key::Key_T:
		LightCamera.rotate_left();
		break;
	case Qt::Key::Key_Y:
		LightCamera.rotate_right();
		break;
	case Qt::Key::Key_G:
		LightCamera.rotate_up();
		break;
	case Qt::Key::Key_H:
		LightCamera.rotate_down();
		break;
	case Qt::Key::Key_I:
		LightPosition += glm::vec3 (0,0,-0.2);
		break;
	case Qt::Key::Key_K:
		LightPosition += glm::vec3(0, 0, 0.2);
		break;
	case Qt::Key::Key_J:
		LightPosition += glm::vec3(-0.2, 0, -0.0);
		break;
	case Qt::Key::Key_L:
		LightPosition += glm::vec3(0.2, 0, -0.0);
		break;
	case Qt::Key::Key_U:
		LightPosition += glm::vec3(0, 0.2, -0.0);
		break;
	case Qt::Key::Key_O:
		LightPosition += glm::vec3(0, -0.2, -0.0);
		break;
	case Qt::Key::Key_Space:
		changeTex = !changeTex;
	}
	repaint();
}