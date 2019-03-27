/*---------------------------------------------------------*/
/* ----------------   Práctica 8 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Alumno: Esparza V{azquez Luis Mauricio ---------------*/

/* Las texturas comienzan abajo a la izquierda, terminan arriba a la derecha. Todo con coordenadas (0,0) Origen ; (1,1) Destino
Si queremos que la textura se repita, cambiamos el 1 por un 2 a la direccion que queremos que se repita la textura. 2 en x es 2 veces la textura hacia x, usw.
Las texturas las trabajamos en 2D, con los ejes (u, v)*/

#define STB_IMAGE_IMPLEMENTATION
#include "esfera.h"
#include "camera.h"
#include <stb_image.h> //Nueva libreria para esta practica

Esfera my_sphere(1.0f);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void myData(void);
void display(void);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//Texture
unsigned int texture1, texture2; //Indice que va a tener cada textura, i.e., 2 índices = 2 texturas

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}


void myData() //Recordemos que antes aquí teníamos un cubo, pero ahora hay un plano.
{	
	float vertices[] = { //La posicion es el vértice que se dibuja y la coordenada de la textura es la que le corresponde a ese vértice.
		// positions          // texture coords
												//front
		0.5f, 0.5f, 0.5f,		0.5f, 0.66f,	//V0 rig up
		0.5f, -0.5f, 0.5f,		0.5f, 0.33f,	//V1 rig dwn
		-0.5f, -0.5f, 0.5f,		0.25f, 0.33f,	//V2 lft dwn
		-0.5f, 0.5f, 0.5f,		0.25f, 0.66f,	//V3 lft up

												// Trasera
		0.5f, -0.5f, -0.5f,		0.75f, 0.33f,	//V4 lft dwn
		0.5f, 0.5f, -0.5f,		0.75f, 0.66f,	//V5 lft up
		-0.5f, 0.5f, -0.5f,		1.0f, 0.66f,	//V6 rig up
		-0.5f, -0.5f, -0.5f,	1.0f, 0.33f,	//V7 rig dwn

												//Izq
		-0.5f, 0.5f, 0.5f,		0.25f, 0.66f,	//V8  
		-0.5f, 0.5f, -0.5f,		0.0f, 0.66f,	//V9
		-0.5f, -0.5f, -0.5f,	0.0f, 0.33f,	//V10
		-0.5f, -0.5f, 0.5f,		0.25f, 0.33f,	//V11

												//Der
		0.5f, 0.5f, 0.5f,		0.75f, 0.66f,	//V12
		0.5f, 0.5f, -0.5f,		0.5f, 0.66f,	//V13
		0.5f, -0.5f, -0.5f,		0.5f, 0.33f,	//V14
		0.5f, -0.5f, 0.5f,		0.75f, 0.33f,	//V15

												//Sup
		-0.5f, 0.5f, 0.5f,		0.25f, 1.0f,	//V16
		0.5f, 0.5f, 0.5f,		0.5f, 1.0f,		//V17
		0.5f, 0.5f, -0.5f,		0.5f, 0.66f,	//V18
		-0.5f, 0.5f, -0.5f,		0.25f, 0.66f,	//V19

												//Inf
		-0.5f, -0.5f, 0.5f,		0.25f, 0.33f,	//V20
		-0.5f, -0.5f, -0.5f,	0.25f, 0.0f,	//V21
		0.5f, -0.5f, -0.5f,		0.50f, 0.0f,	//V22
		0.5f, -0.5f, 0.5f,		0.50f, 0.33f	//V22
	};
	unsigned int indices[] = {
		//rig up -> rig dwn -> lft up ; rig dwn -> lft down -> lft up

		0, 1, 3, // first triangle arr der cub fron
		1, 2, 3,  // second triangle aba izq cub fron

		6, 7, 5,	// First triangle arr izq cub der1
		7, 4, 5,	//Second triangle aba der cub der1

		8, 9, 10,	//1° tringulo izq
		8, 10, 11, //2° triang izq

		12, 13, 14,	//1° tiang der
		12, 14, 15, //2° triang der

		16, 17, 18,
		16, 18, 19,

		20, 22, 21,
		20, 23, 22
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //El atributo 0 corresponde a la posicion
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //El atributo 1 corresponde a la textura
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------
	
	// texture 1
	// ---------
	glGenTextures(1, &texture1); //Leemos una textura dandole un indice y su nombre
	glBindTexture(GL_TEXTURE_2D, texture1); //Es de tipo 2D y se aplica al nombre que le definimos en la linea anterior
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Textura 2D, Wrap, Cómo actua si la coordenada es mayor a 1.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Qué filtro va a tener: 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels; //<-- parámetros que solicita la librería. No los manipulamos, pero debemos tenerlos. Ancho * Alto # de canales de la textura.
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis. La textura gira depende de donde se vea?
	unsigned char *data = stbi_load("Texturas/Cube01.png", &width, &height, &nrChannels, 0); //Direccion de la textura en carpeta del proyecto.
	if (data)
	{
		// IMPORTANT note !!! !!! !!! !!! !!! !!! !!! !!! !!!
		//remember to change to RGBA if image has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		//Basicamente si usamos .png, cambiamos GL_RGB por GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); //Pinta la textura
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//texture 2
	// ---------
	/*glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("Texturas/toalla.tga", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);*/
	

}

void animate(void)
{

}

void display(void)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);			//Lo primero que debe de hacerse es esto, activar la textura a 0. Es como la localidad de memoria de la textura
	glBindTexture(GL_TEXTURE_2D, texture1); //Y la bindeamos con la textura que definimos hace algunas lineas con el nombre texture1
	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);*/

	//Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	Shader projectionShader("shaders/shader_texture.vs", "shaders/shader_texture.fs");
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	//To Use Lighting
	projectionShader.use();
	projectionShader.setInt("texture1", 0); //Definimos un entero pasandole el nombre de la textura y le asignamos su direccion de memoria
	//projectionShader.setInt("texture2", 1);

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 8", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}