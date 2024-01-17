#ifndef _FUNCTIONS_H_
#define	_FUNCTIONS_H_
#define GLFW_INCLUDE_NONE // Don't include OpenGL headers here since we are using glad.h for that
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 

#define M_PI 3.141592

typedef struct {
	GLfloat c[3];
} Vertex;

typedef struct {
	Vertex vertices[3];
} Triangle;

typedef struct {
	Triangle* triangles;
	int count;
} Mesh;

typedef struct {
	float c[4][4];
} Matrix4x4;

typedef struct {
	float v[4];
}Vector4;

typedef struct {
	float v[3];
}Vector3;


typedef struct {
	bool isZpressed;
	bool isQpressed;
	bool isSpressed;
	bool isDpressed;
	bool isApressed;
	bool isEpressed;
	bool isLEFTpressed;
	bool isRIGHTpressed;
	bool isUPpressed;
	bool isDOWNpressed;
} InputState;


float distance3D(Vector3 v1, Vector3 v2);

void vectMatrixMultiplication(Vector4 in, Vector4* out, Matrix4x4 m);

void mat4x4Multiplication(Matrix4x4* m1, Matrix4x4* m2, Matrix4x4* m3);

float dotProduct(Vector4 v1, Vector4 v2);

float dotProductVector3(Vector3 v1, Vector3 v2);

Vector4 crossProductVector4(Vector4 v1, Vector4 v2);

Vector3 crossProductVector3(Vector3 v1, Vector3 v2);

Vector4 normalizeVector4(Vector4 v);

Vector3 normaliseVector3(Vector3 v);

Vector4 multiplyVector4(Vector4 vector, float x);

Vector4 vector4Addition(Vector4 v1, Vector4 v2);

Vector4 vector4Substraction(Vector4 v1, Vector4 v2);

void rotateVectorAroundAxis(Vector4* vector, Vector4 axis, float angle);

void updatePositionForward(Vector4 forward, Vector4* position, float distance);

void updatePositionUp(Vector4 up, Vector4* position, float distance);

void updatePositionRight(Vector4 right, Vector4* position, float distance);

bool backFaceCulling(Vector4 pos, Triangle t, Vertex* normal);

float lighting(Vertex lightDirection, Vertex triangleNormal);

Mesh initMesh();

Mesh initMesh2();

//GLfloat* flattening(const Mesh* mesh);

int applyViewPPMatrix(Mesh in, GLfloat out[999], Matrix4x4 view, Matrix4x4 pp, Vector4 pos, float near, float far);

void updateViewMatrix(Matrix4x4* view, Vector4 f, Vector4 r, Vector4 u, Vector4 p);

void updateViewMatrix(Matrix4x4 *view, Vector4 f, Vector4 r, Vector4 u, Vector4 p);


//void rotateVectorAroundAxis2(Vector4* vector, Vector4 axis, float angle);



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods, int presse);


#endif
