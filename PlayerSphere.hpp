#ifndef PLAYER_SPHERE_HPP
#define PLAYER_SPHERE_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "DrawableObject.hpp"
#include "Camera.hpp"
#include <iostream>

#include "stb_image.h"








#define STACKS 30
#define SECTORS 30

extern GLuint N;

class PlayerSphere : public DrawableObject {
    private:
    GLfloat rot[3];
    glm::vec3 pos;
    GLfloat radius;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    GLuint ebo;
    GLuint texture;



    void setBuffers();

    public:
    PlayerSphere(const char * vertex_path,const char * fragment_path);
    PlayerSphere();
    void initialize(GLuint prog);
    void draw();
    void drawSecondary();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setRotation(GLfloat a, GLfloat b, GLfloat c);
    void moveForward(GLfloat speed, GLfloat delta);
    void moveBackward(GLfloat speed, GLfloat delta);
    void moveLeft(GLfloat speed, GLfloat delta);
    void moveRight(GLfloat speed, GLfloat delta);
    glm::vec3 getCenter();
    GLfloat getRadius();

};


#endif
