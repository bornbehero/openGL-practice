#pragma once
#include <cyTriMesh.h>
#include <string>
#include <Qt\qtimer.h>
#include <Camera.h>
#include <Vertex_data.h>


class Mesh 
{
protected:
	GLint VertexBufferID;
	GLint IndicesBufferID;
	GLint VertexArrayID;

	cyTriMesh* geometry;

public:
	Mesh(cyTriMesh* geo = nullptr) :
		 VertexBufferID(0), IndicesBufferID(0), VertexArrayID(0), geometry(geo) {}

	GLint getVBufferID() const { return VertexBufferID; }
	GLint getIBufferID() const { return IndicesBufferID; }
	GLint getVArrayID() const { return VertexArrayID; }
	cyTriMesh* getGeometry() const { return geometry; }

	void setVBufferID(GLint id) { VertexBufferID = id; }
	void setIBufferID(GLint id) { IndicesBufferID = id; }
	void setVArrayID(GLint id) { VertexArrayID = id; }


};