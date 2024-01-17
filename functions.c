#include "functions.h"
//TODO: Vertex = Vector3, remplacer tt les "Vertex" par des Vector3


float distance3D(Vector3 v1, Vector3 v2) {
	return sqrt(pow(v2.v[0]-v1.v[0], 2)+ pow(v2.v[1] - v1.v[1], 2)+ pow(v2.v[2] - v1.v[2], 2));
}


void vectMatrixMultiplication(Vector4 in, Vector4* out, Matrix4x4 m) {
	//openGL uses a column-major system, which means all the vectors are columns, and matrices are read from top to bottom=>in a multiplication, the vector is on the right

	for (int i = 0; i < 4; i++) {
		out->v[i] = 0;
		for (int j = 0; j < 4; j++) {
			out->v[i] += in.v[j] * m.c[i][j];
		}
	}
}

void mat4x4Multiplication(Matrix4x4* m1, Matrix4x4* m2, Matrix4x4* m3) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				m3->c[i][j] += m1->c[i][k] * m2->c[k][j];
			}
		}
	}
}

float dotProduct(Vector4 v1, Vector4 v2) {
	return (v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2]);
}

float dotProductVector3(Vector3 v1, Vector3 v2) {
	return (v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2]);
}

Vector4 crossProductVector4(Vector4 v1, Vector4 v2) {
	return (Vector4) {
		v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
		v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
		v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0],
		0.0f
	};
}

Vector3 crossProductVector3(Vector3 v1, Vector3 v2) {
	return (Vector3) {
		v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
		v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
		v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]
	};
}

Vector4 normalizeVector4(Vector4 v) {
	float norme = sqrt(pow(v.v[0], 2) + pow(v.v[1], 2) + pow(v.v[2], 2));
	return (Vector4) { v.v[0] / norme, v.v[1] / norme, v.v[2] / norme, 0.0f };
}

Vector3 normaliseVector3(Vector3 vector) {
	float norme = sqrt(pow(vector.v[0], 2) + pow(vector.v[1], 2) + pow(vector.v[2], 2));
	return (Vector3) { vector.v[0] / norme, vector.v[1] / norme, vector.v[2] / norme};
}

Vector4 multiplyVector4(Vector4 vector, float x) {
	return (Vector4){ vector.v[0] * x, vector.v[1] * x, vector.v[2] * x, vector.v[3] * x };
}

Vector4 vector4Addition(Vector4 v1, Vector4 v2) {
	return (Vector4) {
		v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2], v1.v[3] + v2.v[3]
	};
}

Vector4 vector4Substraction(Vector4 v1, Vector4 v2) {
	return (Vector4) {
		v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2], v1.v[3] - v2.v[3]
	};
}


void rotateVectorAroundAxis(Vector4* vector, Vector4 axis, float angle) {
	//uses rodrigues rotating formula
	Vector4 tmp1,tmp2,tmp3;
	tmp1 = multiplyVector4(*vector, cos(angle));
	tmp2 = multiplyVector4(crossProductVector4(*vector, axis),sin(angle));
	tmp3 = multiplyVector4(axis, dotProduct(*vector, axis) * (1 - cos(angle)));
	*vector = normalizeVector4(vector4Addition(tmp1, vector4Addition(tmp2, tmp3)));
}



//plane defined by its normal and a point thats on the normal, the segment is defined by its directional vector, and its starting and finishing point
//return true if intersection, false otherwise. if true, out contains the intersection point
bool segmentPlaneIntersection(Vector3 normal, Vector3 point, Vector3 line, Vector3 start, Vector3 finish, Vector3* out) {
	//calcul de l.n
	float a = dotProductVector3(normal, line);
	if (a!=0){

	}
	else {
		//no intersection (sur le plan ou parallel
		return false;
	}
}


void updatePositionForward(Vector4 forward, Vector4* position, float distance) {
	//can be used to go backward if distance<0
	*position = vector4Addition(*position,multiplyVector4(forward, distance));
}

void updatePositionUp(Vector4 up, Vector4* position, float distance) {
	//can be used to go down if distance<0
	*position = vector4Addition(*position, multiplyVector4(up, distance));
}

void updatePositionRight(Vector4 right, Vector4* position, float distance) {
	//can be used to go left if distance<0
	*position = vector4Addition(*position, multiplyVector4(right, distance));
}


bool backFaceCulling(Vector4 pos, Triangle t, Vector3* normal) {
	Vector3 tmp1, tmp2, tmp3, N, V;
	tmp1 = (Vector3){ t.vertices[1].c[0] - t.vertices[0].c[0], t.vertices[1].c[1] - t.vertices[0].c[1], t.vertices[1].c[2] - t.vertices[0].c[2] };
	tmp2 = (Vector3){ t.vertices[2].c[0] - t.vertices[0].c[0], t.vertices[2].c[1] - t.vertices[0].c[1], t.vertices[2].c[2] - t.vertices[0].c[2] };
	*normal = normaliseVector3(crossProductVector3(tmp1, tmp2));
	tmp3 = (Vector3){ t.vertices[0].c[0] - pos.v[0], t.vertices[0].c[1] - pos.v[1], t.vertices[0].c[2] - pos.v[2] };
	V = normaliseVector3(tmp3);
	return dotProductVector3(*normal, V) < 0;
};

float lighting(Vector3 lightDirection, Vector3 triangleNormal) {
	//calculer 
	float result= dotProductVector3(lightDirection, triangleNormal);
	if (result <= 0.4f) {
		return 0.4f;
	}
	else {
		return result;
	}

}


Mesh initMesh() {
	Mesh mesh;
	mesh.count = 13;//bug qd on met pas n+1 rtriangles
	mesh.triangles = malloc(mesh.count * sizeof(Triangle));
	/*
	Triangle t1 = {
		{{-0.5f, -0.5f, 0.0f},
		 {0.5f, -0.5f, 0.0f},
		 {0.0f, 0.5f, 0.0f}}
	};

	Triangle t2 = {
		{{0.5f, -0.5f, 0.5f},
		 {0.7f, -0.5f, 0.5f},
		 {1.0f, 0.5f, 0.5f}}
	};
	*/
	//TOP
	
	Triangle t1 = {
		.vertices = {
			{.c = {0.0f,  1.0f, 8.0f}},
			{.c = {-1.0f, 1.0f, 8.0f}},
			{.c = {0.0f,  1.0f, 9.0f}}
	}
	};
	Triangle t2 = {
		.vertices = {
			{.c = {-1.0f, 1.0f, 8.0f}},
			{.c = {-1.0f, 1.0f, 9.0f}},
			{.c = {0.0f,  1.0f, 9.0f}}
	}
	};
	

	//front
	Triangle t3 = {
		.vertices = {
			{.c = {0.0f, 0.0f, 8.0f}},
			{.c = {-1.0f, 0.0f, 8.0f}},
			{.c = {-1.0f,  1.0f, 8.0f}}
	}
	};
	Triangle t4 = {
		.vertices = {
			{.c = {0.0f, 0.0f, 8.0f}},
			{.c = {-1.0f, 1.0f, 8.0f}},
			{.c = {0.0f, 1.0f, 8.0f}}
	}
	};

	
	//BACK
	Triangle t5 = {
		.vertices = {
			{.c = {0.0f, 0.0f, 9.0f}},
			{.c = {-1.0f,  1.0f, 9.0f}},
			{.c = {-1.0f, 0.0f, 9.0f}}
	}
	};
	Triangle t6 = {
		.vertices = {
			{.c = {0.0f, 0.0f, 9.0f}},
			{.c = {0.0f, 1.0f, 9.0f}},
			{.c = {-1.0f,  1.0f, 9.0f}}
	}
	};
	//RIGHT
	Triangle t7 = {
		.vertices = {
			{.c = {-1.0f, 0.0f, 8.0f}},
			{.c = {-1.0f,  1.0f, 9.0f}},
			{.c = {-1.0f, 1.0f, 8.0f}}
	}
	};
	Triangle t8 = {
		.vertices = {
			{.c = {-1.0f, 0.0f, 8.0f}},
			{.c = {-1.0f,  0.0f, 9.0f}},
			{.c = {-1.0f, 1.0f, 9.0f}}
	}
	};
	//LEFT
	Triangle t9 = {
		.vertices = {
			{.c = {0.0f, 0.0f, 8.0f}},
			{.c = {0.0f, 1.0f, 8.0f}},
			{.c = {0.0f,  1.0f, 9.0f}}
	}
	};
	Triangle t10 = {
		.vertices = {
			{.c = {0.0f, 0.0f, 8.0f}},
			{.c = {0.0f, 1.0f, 9.0f}},
			{.c = {0.0f,  0.0f, 9.0f}}
	}
	};
	//BOTOM
	Triangle t11 = {
		.vertices = {
			{.c = {0.0f,  0.0f, 8.0f}},
			{.c = {0.0f,  0.0f, 9.0f}},
			{.c = {-1.0f, 0.0f, 8.0f}}
	}
	};
	Triangle t12 = {
		.vertices = {
			{.c = {-1.0f, 0.0f, 8.0f}},
			{.c = {0.0f,  0.0f, 9.0f}},
			{.c = {-1.0f, 0.0f, 9.0f}}
	}
	};


	
	/*
	for (int i = 0; i < 3;i++) {
		for (int j=0;j<3;j++){
			mesh.triangles[(i*3+j*3)*9] = (Triangle){
				{-1.0f, 0.0f, 8.0f},
				{-1.0f, 0.0f, 9.0f},
				{0.0f,  0.0f, 9.0f}
			};
		}
	}
	*/

	//mesh.triangles[0] = t1;
	//mesh.triangles[1] = t2;
	mesh.triangles[0] = t1;
	mesh.triangles[1] = t2;
	
	mesh.triangles[2] = t3;
	mesh.triangles[3] = t4;
	mesh.triangles[4] = t5;
	mesh.triangles[5] = t6;
	mesh.triangles[6] = t7;
	mesh.triangles[7] = t8;
	mesh.triangles[8] = t9;
	mesh.triangles[9] = t10;
	mesh.triangles[10] = t11;
	mesh.triangles[11] = t12;
	
	return mesh;
}



Mesh initMesh2() {
	Mesh mesh;
	mesh.count = 1;
	mesh.triangles = malloc(mesh.count * sizeof(Triangle));
	


	Triangle t1 = {
		.vertices = {
			{.c = {0.0f, 0.0f, 8.0f}},
			{.c = {-1.0f, 0.0f, 8.0f}},
			{.c = {-1.0f,  1.0f, 8.0f}}
	}
	};
	mesh.triangles[0] = t1;
	//mesh.triangles[1] = t2;
	return mesh;
}

int applyViewPPMatrix(Mesh in, GLfloat out[999], Matrix4x4 view, Matrix4x4 pp, Vector4 pos,float near, float far) {
	//applies to the Mesh both matrices to obtain the necessary 2D vertices (they re actually 4d, with the last coordinate being unimportant, and the z coordinate being used
	//to 'decide' wich triangle is in front of wich
	Vector4 tmp1, tmp2;
	int countTriangle = 0;
	float shade;
	Vector3 normal;
	Vector3 lightDirection = { view.c[2][0], view.c[2][1], view.c[2][2]};
	bool debug = true;
	float distanceVertex;
	for (int i = 0; i < in.count; i++) {
		//check if we have to render the triangle using back face curling
		if (backFaceCulling(pos, in.triangles[i],&normal)) {//technically, pos is in the view matrix, but "inside" the dot product, getting it out would be less efficient
			
			for (int j = 0; j < 3; j++) {//1 triangle contient 3 vertex
				tmp1.v[0] = in.triangles[i].vertices[j].c[0];
				tmp1.v[1] = in.triangles[i].vertices[j].c[1];
				tmp1.v[2] = in.triangles[i].vertices[j].c[2];
				tmp1.v[3] = 1.0f; //1 bc its the easiest value to correct the effect of in the multiplication after they re done
				//printf("vertex numero %d du triangle %d:  x % f y % f z % f   MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM \n", j,i,tmp1.v[0], tmp1.v[1], tmp1.v[2]);
				vectMatrixMultiplication(tmp1, &tmp2, view);//resultat stocké dans tmp2
				printf("resultats multiplication view: x %f y %f z %f w %f \n", tmp2.v[0], tmp2.v[1], tmp2.v[2], tmp2.v[3]);
				//------------------------------/------------------------------/------------------------------/------------------------------
				// PROBLEM ICI, qd on tourne la camera, w approche 0, et ca creer des prblm
				//------------------------------/------------------------------/------------------------------/------------------------------
				vectMatrixMultiplication(tmp2, &tmp1, pp);//tmp1 "effacé" dans la fonction, il stock mtn le resultat
				printf("resultats multiplication pp(avant homo): x %f y %f z %f w %f \n", tmp1.v[0], tmp1.v[1], tmp1.v[2], tmp1.v[3]);
				//faut mtn homogeniser
				//tmp1.v[3] *= 2;

				tmp1.v[0] /= tmp1.v[3];
				tmp1.v[1] /= tmp1.v[3];
				tmp1.v[2] /= tmp1.v[3];

				printf("resultats multiplication pp(apres homo): x %f y %f z %f w %f\n", tmp1.v[0], tmp1.v[1], tmp1.v[2], tmp1.v[3]);
				out[countTriangle * 18 + j * 6] = tmp1.v[0];
				out[countTriangle * 18 + j * 6 + 1] = tmp1.v[1];
				out[countTriangle * 18 + j * 6 + 2] = tmp1.v[2];
				//the z coordinate of each vertices ranges from -1 to 1. at -1, its on the near plane, at 1 on the far plane. we want to scale the shade according to the 
				//distance, so we use a factor that is between 0 and 1 (bc color cant have negative values)
				
				distanceVertex = distance3D((Vector3) { pos.v[0], pos.v[1], pos.v[2] }, (Vector3) { in.triangles[i].vertices[j].c[0], in.triangles[i].vertices[j].c[1], in.triangles[i].vertices[j].c[2] });


				shade = lighting(lightDirection, normal)*(1.0f- (tmp1.v[2]+1)/4);
				out[countTriangle * 18 + j * 6 + 3] = shade;
				out[countTriangle * 18 + j * 6 + 4] = shade;
				out[countTriangle * 18 + j * 6 + 5] = shade;
				if (debug) {
					out[countTriangle * 18 + j * 6 + 3] = 0;
					out[countTriangle * 18 + j * 6 + 4] = 0;
					out[countTriangle * 18 + j * 6 + 5] = 0;
					out[countTriangle * 18 + j * 6 + 3 + j] = 1.0f;
				}


			}
			printf("\n\n\n");
			countTriangle++;
		}
	}
	return countTriangle*3 +3;
	
}

/*
GLfloat* flattening(const Mesh* mesh) {
	int size = mesh->count * 9;// 3coord par point(x,y,z),3  points par triangle
	GLfloat* vertices = malloc(size * sizeof(GLfloat));
	if (vertices == NULL) {
		printf("malloc fail");
		return NULL;
	}
	int k = 0;
	for (int i = 0; i < mesh->count; i++) {
		for (int j = 0; j < 3; j++) {
			vertices[k++] = mesh->triangles[i].vertices[j].x;
			vertices[k++] = mesh->triangles[i].vertices[j].y;
			vertices[k++] = mesh->triangles[i].vertices[j].z;
		}
	}
	return vertices;
}

*/
void updateViewMatrix(Matrix4x4 *view, Vector4 f, Vector4 r, Vector4 u, Vector4 p) {
	view->c[0][0] = r.v[0];
	view->c[0][1] = r.v[1];
	view->c[0][2] = r.v[2];
	view->c[0][3] = -dotProduct(r, p);
	view->c[1][0] = u.v[0];
	view->c[1][1] = u.v[1];
	view->c[1][2] = u.v[2];
	view->c[1][3] = -dotProduct(u, p);
	view->c[2][0] = f.v[0];
	view->c[2][1] = f.v[1];
	view->c[2][2] = f.v[2];
	view->c[2][3] = -dotProduct(f, p);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){//, InputState* inputState) {
	InputState* inputState = (InputState*)glfwGetWindowUserPointer(window);
	
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W:
			inputState->isZpressed = true;
			break;
		case GLFW_KEY_A:
			inputState->isQpressed = true;
			break;
		case GLFW_KEY_S:
			inputState->isSpressed = true;
			break;
		case GLFW_KEY_D:
			inputState->isDpressed = true;
			break;
		case GLFW_KEY_Q:
			inputState->isApressed = true;
			break;
		case GLFW_KEY_E:
			inputState->isEpressed = true;
			break;
		case GLFW_KEY_LEFT:
			inputState->isLEFTpressed = true;
			break;
		case GLFW_KEY_RIGHT:
			inputState->isRIGHTpressed = true;
			break;
		case GLFW_KEY_UP:
			inputState->isUPpressed = true;
			break;
		case GLFW_KEY_DOWN:
			inputState->isDOWNpressed = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W:
			inputState->isZpressed = false;
			break;
		case GLFW_KEY_A:
			inputState->isQpressed = false;
			break;
		case GLFW_KEY_S:
			inputState->isSpressed = false;
			break;
		case GLFW_KEY_D:
			inputState->isDpressed = false;
			break;
		case GLFW_KEY_Q:
			inputState->isApressed = false;
			break;
		case GLFW_KEY_E:
			inputState->isEpressed = false;
			break;
		case GLFW_KEY_LEFT:
			inputState->isLEFTpressed = false;
			break;
		case GLFW_KEY_RIGHT:
			inputState->isRIGHTpressed = false;
			break;
		case GLFW_KEY_UP:
			inputState->isUPpressed = false;
			break;
		case GLFW_KEY_DOWN:
			inputState->isDOWNpressed = false;
			break;
		}
	}
	/*
	if (action == GLFW_PRESS && key == GLFW_KEY_D) {
		presse = 1;
	}else if (action == GLFW_RELEASE && key == GLFW_KEY_D){
		presse = 0;
	}
	*/
}
//dans le tab de tab qui représente le labyrinthe, les 0 sont les espace libre, les 1 sont les murs,
//les 2 sont les pieges
int x_hasard, y_hasard, compteur = 0;
int nb_pieges = ? ;
while (compteur != nb_pieges) {
	x_hasard=random(...)//jsplus comment generer chiffres au hasard, faut les generer entre 0 et la dimension de ton labyrinthe
	y_hasard=random(...)
	if (tab_labyrinthe[x_hasard,y_hasard]==0){
		tab_labyrinthe[x_hasard, y_hasard] = 2;
		compteur++;
	}
}