#ifndef BUBBLE_SPHERE_HPP
#define BUBBLE_SPHERE_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <map>
#include "DrawableObject.hpp"
#include "Camera.hpp"
#include <iostream>




#define STACKS 30
#define SECTORS 30

extern GLuint N;

class BubbleSphere : public DrawableObject {
    private:
    glm::vec3 pos;
    GLfloat radius;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::map<GLfloat, glm::vec3> sorted;
    GLuint ebo;
    GLuint num;
    glm::vec3 *bubbles = NULL;

    void setBuffers();

    public:
    BubbleSphere(const char * vertex_path,const char * fragment_path);
    BubbleSphere();
    void initialize(GLuint prog, GLuint num);
    void setBubbles(GLuint num);
    void draw();
    void drawSecondary();
    glm::vec3 getCenter();
    GLfloat getRadius();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setRotation(GLfloat a, GLfloat b, GLfloat c);
    void update(GLfloat speed, GLfloat delta);
    void sort(glm::vec3 playerPos);
    bool collision(glm::vec3 playerPos, GLfloat playerRadius);
    GLuint getNum();

};


#endif
