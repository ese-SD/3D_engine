#define GLFW_INCLUDE_NONE // Don't include OpenGL headers here since we are using glad.h for that
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions.h"
#include <time.h>
#include <windows.h>

/*EXPLICATIONS DE LA LOGIQUE:
de base, opengl fait presque tt (faire passer les triangles qui sont en world coordinate a travers tte les matrices, puis dessiner chaque pixels sur 
l ecran) dans le GPU. Il fait ca a travers des shaders, c ce qui fait que c fait dans le GPU et pas le CPU, mm la partie math, mm si c pas du graphique.
comme je veut faire toute la partie math (en gros tt sauf ouvrir la window, et dessiner les pixels), et que je sais pas (encore) faire mes 
propres shaders en GLSL, tt ce que je vais faire sera executé dans le CPU et pas le GPU comme ce sera pas dans un shader, dc les perfs seront moins
bonne. il faut qd mm un vertexShader (qui de base s occupe de faire les transformations matricielle) dans lequel on lui dit de juste renvoyer les
vertex qu il a recu sans rien leur faire pcq opengl a besoin d'un vertex shader pr fonctionner. Le fragmentShader est celui qui contient la logique
pour dessiner les pixels, c'est aussi celui qui decrit la couleur de ces pixels. 

*/





// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = aColor;\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0);\n"
"}\n\0";



void setup3(GLFWwindow** window, GLuint* shaderProgram, GLuint* VAO, GLuint* VBO, GLfloat* vertices, size_t vertices_size) {
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*window = glfwCreateWindow(1000, 700, "3D_engine", NULL, NULL);
	if (*window == NULL)
	{
		glfwTerminate();
		exit(1);
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(*window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800

	glViewport(0, 0, 1000, 700);

	GLint success;

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);



	//checks if both shader's compilation was sucessful
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fprintf(stderr, "vertexShader Compilation Failed\n%s\n", infoLog);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		fprintf(stderr, "fragmentShader  Compilation Failed\n%s\n", infoLog);
	}


	// get Shader Program Object reference
	*shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(*shaderProgram, vertexShader);
	glAttachShader(*shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(*shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "Shader Program Linking Failed\n%s\n", infoLog);
	}



	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(*VAO);



	// PAS BESOIN ICI, COMMME ON LE FAIT DANS LA BOUCLE DANS TT LES CAS, RETIRER PLUS TARD
	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	// Introduce the vertices into the VBO
	//glBufferData(GL_ARRAY_BUFFER, mesh.count * 9 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);


	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	//
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	//pas besoin pr l instant comme on les modifie dans la main loop
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}




int main()
{
	GLFWwindow* window;
	GLuint* shaderProgram;
	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	//GLuint* VAO, VBO;
	GLfloat vertices[990];//permet de mettre 111 triangles (9 données par triangle) =
	double total_t;
	clock_t start_t, end_t, timer1;
	int time_for_frame, tmp_delai;

	GLuint VAO, VBO;
	setup3(&window, &shaderProgram, &VAO, &VBO, vertices, sizeof(vertices));
	//setup2(&window, &shaderProgram);
	//setup(&window);
	printf("test: %f \n", vertices[0]);
	
	float nearPlane, farPlane, FOV, aspectRatio, FOVrad;
	nearPlane = 2.0f;
	farPlane = 100.0;
	FOV = 90.0;
	aspectRatio = 1000.0 / 700.0;
	FOVrad = 1.0 / tan(FOV * M_PI / 360.0);
	
	Matrix4x4 PPMatrix = {
		.c = {
			{1.0f / (aspectRatio * tan((float)FOV * (float)M_PI / (float)360)), 0.0f, 0.0f, 0.0f},
			{0.0f,1.0f / (tan(FOV * M_PI / 360)), 0.0f, 0.0f},
			{0.0f,0.0f,-(farPlane + nearPlane) / (farPlane - nearPlane),-2 * farPlane * nearPlane / (farPlane - nearPlane)},
			{0.0f,0.0f,-1.0f, 0.0f}
		}
	};

	Vector4 forward = { { 0.0f, 0.0f ,-1.0f, 0.0f } };
	Vector4 up = { { 0.0f, 1.0f ,0.0f, 0.0f } };
	Vector4 right = { { 1.0f, 0.0f ,0.0f, 0.0f } };
	Vector4 pos = { { 0.0f, 0.0f ,0.0f, 0.0f } };

	Matrix4x4 viewMatrix = {
		.c = {
			{right.v[0],  right.v[1],  right.v[2],  -dotProduct(right, pos)},
			{up.v[0],     up.v[1],     up.v[2],     -dotProduct(up, pos)},
			{forward.v[0],forward.v[1],forward.v[2],-dotProduct(forward, pos)},
			{0.0f,		  0.0f,		   0.0f,		1.0f}
		}
	};
	//updateViewMatrix(viewMatrix, forward, right, up, pos);
	
	printf("fov %f, fov rad:%f , 1er element: %f\n", FOV, FOV * M_PI / 180, PPMatrix.c[0][0]);
	printf("aspect ratio %f, tan: %f , tan2 %f\n", aspectRatio, tanf(FOV * M_PI / 360), tanf((float)0.78535));
	printf("pp matrix: \n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%f ", PPMatrix.c[i][j]);
		}
		printf("\n");
	}

	printf("view matrix: \n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%f ", viewMatrix.c[i][j]);
		}
		printf("\n");
	}
	// Vertices coordinates
	Mesh mesh=initMesh2();
	int verticesNumber;// = mesh.count * 3;
	printf("affichage mesh: \n");
	for (int i = 0; i < mesh.count;i++) {
		for (int j = 0; j < 3; j++) {
			printf("x %f, y %f, z %f \n", mesh.triangles[i].vertices[j].c[0], mesh.triangles[i].vertices[j].c[1], mesh.triangles[i].vertices[j].c[2]);
		}
	}

	InputState inputState = { false };//at first, we consider every key unpressed
	glfwSetWindowUserPointer(window, &inputState);

	//GLfloat* vertices = flattening(&mesh);

	printf("check\n");
	// Main while loop
	int compteur = 0;
	int presse = 0;
	glfwSetKeyCallback(window, key_callback);
	start_t = clock();
	timer1 = clock();
	int verification = 0;
	while (!glfwWindowShouldClose(window))// && compteur <40)
	{
		compteur++;
		//update du mesh:
		//printf("DEBUTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT frame \n");

		//update de l array pr match le nouvo mesh
		//free(vertices); pas besoin (pr l instant) comme c statique
		//printf("check3 \n");
		//memset(vertices, 0, sizeof(vertices));
		verticesNumber=applyViewPPMatrix(mesh, vertices, viewMatrix, PPMatrix, pos, nearPlane, farPlane);
		
		
		for (int i = 0; i < 55; i++) {
			if (vertices[i * 18] != -107374176.000000) {
				printf("triangle %d \n", i);
				for (int j = 0; j < 3; j++) {
					printf("vertex %d: ", j);

					printf("%f %f %f   %f %f %f \n", vertices[i * 18 + j * 6], vertices[i * 18 + j * 6 + 1], vertices[i * 18 + j * 6 + 2], vertices[i * 18 + j * 6 + 3], vertices[i * 18 + j * 6 + 4], vertices[i * 18 + j * 6 + 5]);

				}
				printf("\n");
			}
		}
		

		printf("\n\n");
		//test(mesh, vertices, viewMatrix, PPMatrix);
		//printf("check4 \n");
		//vertices = flattening(&mesh);
		//vertices[0] += 0.0001f;//test
		//printf("array vertices: \n");
		//for (int i = 0; i < 9; i++) {
		//	printf("%f \n", vertices[i]);
		//}


		//'vertices' changed, so we need to update GL_ARRAY_BUFFER to reflect that change on the screen
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		
		
		// Specify the color of the background

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangle using the GL_TRIANGLES primitive

		//glDrawArrays(GL_TRIANGLES, 0, mesh.count * 3);
		
		glDrawArrays(GL_TRIANGLES, 0, verticesNumber);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		//on prend en compte les user input:
		if (inputState.isZpressed==true) {
			//pos.v[2] += 0.1f;
			updatePositionForward(forward, &pos, -0.1f);
		}
		if (inputState.isQpressed == true) {
			updatePositionRight(right, &pos, -0.1f);
		}
		if (inputState.isSpressed == true) {
			updatePositionForward(forward, &pos, 0.1f);
		}
		if (inputState.isDpressed == true) {
			updatePositionRight(right, &pos, 0.1f);
		}
		if (inputState.isApressed == true) {
			updatePositionUp(up, &pos, 0.1f);
		}
		if (inputState.isEpressed == true) {
			updatePositionUp(up, &pos, -0.1f);
		}

		if (inputState.isLEFTpressed == true) {
			//rotate approximately 1 degree(in radian) to the left
			rotateVectorAroundAxis(&forward, up, 0.0174f);
			//the 3 directional vectors need to be orthogonal, so instead or rotating the 'right' vector around 'up', we compute the cross product
			rotateVectorAroundAxis(&right, up, 0.0174f);
		}
		if (inputState.isRIGHTpressed == true) {
			//rotate approximately 1 degree(in radian) to the left
			rotateVectorAroundAxis(&forward, up, -0.0174f);
			//the 3 directional vectors need to be orthogonal, so instead or rotating the 'right' vector around 'up', we compute the cross product
			rotateVectorAroundAxis(&right, up, -0.0174f);
		}
		if (inputState.isDOWNpressed == true) {
			//rotate approximately 1 degree(in radian) to the left
			//up = crossProduct(forward, right);
			//up = crossProduct(right, forward);
			rotateVectorAroundAxis(&up, right, -0.0174f);
			rotateVectorAroundAxis(&forward, right, -0.0174f);
		}
		if (inputState.isUPpressed == true) {
			
			//up = crossProduct(forward, right);
			//up = crossProduct(right, forward);
			rotateVectorAroundAxis(&up, right, 0.0174f);
			rotateVectorAroundAxis(&forward, right, 0.0174f);
		}

		/*
		printf("view matrix: \n");
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				printf("%f ", viewMatrix.c[i][j]);
			}
			printf("\n");
		}
		*/
		printf("FORWARD: %f  %f  %f %f, norme: %f\n", forward.v[0], forward.v[1], forward.v[2], forward.v[3],sqrt(pow(forward.v[0],2)+ pow(forward.v[1],2)+ pow(forward.v[2],2)));
		printf("RIGHT: %f  %f  %f , norme: %f\n", right.v[0], right.v[1], right.v[2], sqrt(pow(right.v[0], 2) + pow(right.v[1], 2) + pow(right.v[2], 2)));
		printf("UP: %f  %f  %f , norme: %f \n", up.v[0], up.v[1], up.v[2], sqrt(pow(up.v[0], 2) + pow(up.v[1], 2) + pow(up.v[2], 2)));
		printf("\n");

		/*
		printf("dot product forward up: %f \n", dotProduct(forward, up));
		printf("dot product forward right: %f \n", dotProduct(forward, right));
		printf("dot product right up: %f \n", dotProduct(right, up));
		*/


		updateViewMatrix(&viewMatrix, forward, right, up, pos);


		// Take care of all GLFW events
		glfwPollEvents();
		//we want to limit fps to 60 so that game speed is unchanged (simpler than not controlling fps and instead controlling everything else according to how much
		//time the last few frames took)

		//trouver cmb de temps la frame a pris
		time_for_frame = (clock() - timer1)*1000/ (int)CLOCKS_PER_SEC;//bc 1000ms in 1s. In my case, CLOCKS_PER_SEC=1000 so its useless but its for portability
		//printf("temps: %d \n", (clock() - timer1));
		//printf("time for frame: %d \n",time_for_frame);
		tmp_delai = 16 - time_for_frame;
		if (tmp_delai> 0) {
			//printf("DEBUG: %ld", clock());
			Sleep(tmp_delai/2);
			//printf("DEBUG: %ld", clock());
			//printf("ON ATTEND !!!!!!!!!!!!!!!!!!!! \n");
		}
		//printf("temps total de la frame: %d , mmmmmmmmm %d\n", clock()-timer1, tmp_delai);
		//Sleep(8);

		timer1 = clock();//reset le timer

	}



	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	//free(vertices);
	end_t = clock();
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("temps total: %f, compteur: %d", total_t,compteur);
	printf("fps moyen: %f", (float)compteur / total_t);

	
	return 0;
}