// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DrawableObject.hpp"
#include "PlayerSphere.hpp"
#include "Camera.hpp"
#include "Cuboid.hpp"
#include "BubbleSphere.hpp"




#include <time.h>

GLfloat speed = 1.5f;

Camera camera = Camera();
glm::vec3 lightsource;


// Tutorial 1
GLuint LoadShaders(const char * vertex_path,const char * fragment_path) {

    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vsCode;
    std::ifstream VertexShaderStream(vertex_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        vsCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string fsCode;
    std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        fsCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader: %s\n", vertex_path);
    char const *vsPointer = vsCode.c_str();
    glShaderSource(vsID, 1, &vsPointer , NULL);
    glCompileShader(vsID);

    // Check Vertex Shader
    glGetShaderiv(vsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(vsID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader: %s\n", fragment_path);
    char const * FragmentSourcePointer = fsCode.c_str();
    glShaderSource(fsID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(fsID);

    // Check Fragment Shader
    glGetShaderiv(fsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(fsID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }


    GLuint programID = glCreateProgram();
    glAttachShader(programID, vsID);
    glAttachShader(programID, fsID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    
    glDetachShader(programID, vsID);
    glDetachShader(programID, fsID);
    
    glDeleteShader(vsID);
    glDeleteShader(fsID);

    return programID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera.resize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.rotateMouse(xpos, ypos);
}

// glew and glfw initialization from tutorial02 (Zadanie 1) 
int main(int argc, char** argv)
{
	// Initialise GLFW
	if ( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(camera.getMainWidth(), camera.getMainHeight(), "Deadly bubbles", NULL, NULL);
	if ( window == NULL ) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, camera.getMainWidth(), camera.getMainHeight());
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);

	// Dark blue background
	glClearColor(0.4f, 0.2f, 0.4f, 0.0f);



	GLuint playerProg = LoadShaders("player.vs", "player.fs");
	PlayerSphere player;
	player.initialize(playerProg);
	player.setPosition(0.0, 0.0, 0.0);

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = glfwGetTime();
	GLfloat start = lastTime;

	GLuint aquariumProg = LoadShaders("aquarium.vs", "aquarium.fs");
	Cuboid aquarium;
	aquarium.initialize(aquariumProg);

	GLuint bubbleProg = LoadShaders("bubble.vs", "bubble.fs");
	BubbleSphere bbl;
	bbl.initialize(bubbleProg, 500);


	unsigned int levelCount = 0;
	do{

		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glm::vec3 prevPos = player.getCenter();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward(speed, deltaTime);
			player.moveForward(speed, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera.moveBackward(speed, deltaTime);
			player.moveBackward(speed, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.moveLeft(speed, deltaTime);
			player.moveLeft(speed, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.moveRight(speed, deltaTime);
			player.moveRight(speed, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			camera.rotateUp(deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			camera.rotateDown(deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			camera.rotateLeft(deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			camera.rotateRight(deltaTime);
		}

		glm::vec3 playerPos = player.getCenter();
		GLfloat playerRad = player.getRadius();

		if (playerPos.x - playerRad < -3.0) {
			player.setPosition(-3.0 + playerRad, playerPos.y, playerPos.z);
			camera.setPosition(-3.0 + playerRad, playerPos.y, playerPos.z);
			playerPos = player.getCenter();
		}

		
		if (playerPos.x + playerRad > 3.0) {
			player.setPosition(3.0 - playerRad, playerPos.y, playerPos.z);
			camera.setPosition(3.0 - playerRad, playerPos.y, playerPos.z);
			playerPos = player.getCenter();
		}


		if (playerPos.y - playerRad < -3.0) {
			player.setPosition(playerPos.x, -3.0 + playerRad, playerPos.z);
			camera.setPosition(playerPos.x, -3.0 + playerRad, playerPos.z);
			playerPos = player.getCenter();
		}


		if (playerPos.y + playerRad > 3.0) {
			player.setPosition(playerPos.x, 3.0 - playerRad, playerPos.z);
			camera.setPosition(playerPos.x, 3.0 - playerRad, playerPos.z);
			playerPos = player.getCenter();
		}
			
		if (playerPos.z - playerRad < -1.0) {
			player.setPosition(playerPos.x, playerPos.y, -1.0 + playerRad);
			camera.setPosition(playerPos.x, playerPos.y, -1.0 + playerRad);
			playerPos = player.getCenter();
		}

		if (playerPos.z + playerRad > 13.0) {
			player.setPosition(playerPos.x, playerPos.y, 13.0 - playerRad);
			camera.setPosition(playerPos.x, playerPos.y, 13.0 - playerRad);
			// NEXT LVL
			bbl.setBubbles(floor((GLfloat)bbl.getNum() * 1.5));
			levelCount++;
			player.setPosition(0.0, 0.0, -0.7);
			camera.setPosition(0.0, 0.0, -0.7);
			continue;
		}



		// update bubbles

		bbl.update(speed, deltaTime);

		bbl.sort(player.getCenter());

		// drawing

		lightsource = player.getCenter();


		camera.setMainViewport();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		aquarium.draw();		
		player.draw();

		bbl.draw();

		GLenum er;
		while (er=glGetError())
			fprintf(stderr, "\nOpenGL ERROR: 0x%04x    =============%s===\n", er, "main afterdraw");



		// kolizje z bubblami
		if (bbl.collision(player.getCenter(), player.getRadius())) {
			// koniec gry
			printf("Koniec gry, ukończone poziomy: %d\n", levelCount);
			return 0;
		}


		glfwSwapBuffers(window);
		glfwPollEvents();

	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );



	glfwTerminate();

	return 0;
}

