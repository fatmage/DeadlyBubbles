#include "Cuboid.hpp"

extern glm::vec3 lightsource;

Cuboid::Cuboid() {
}


void Cuboid::initialize(GLuint prog) {
    createBuffers();
    programID = prog;
    setBuffers();   
}


void Cuboid::setBuffers() {
    bindVAO();
	bindBuffers();

    static const glm::vec3 vs[] = {
        glm::vec3(-3.0, -3.0, -1.0 ),        // 0
        glm::vec3( 3.0, -3.0, -1.0 ),        // 1
        glm::vec3(-3.0,  3.0, -1.0 ),        // 2
        glm::vec3( 3.0,  3.0, -1.0 ),        // 3
        glm::vec3(-3.0, -3.0,  13.0),        // 4
        glm::vec3( 3.0, -3.0,  13.0),        // 5
        glm::vec3(-3.0,  3.0,  13.0),        // 6
        glm::vec3( 3.0,  3.0,  13.0),        // 7
    };

    static const glm::vec3 ns[] = {
        glm::vec3(-1.0, 0.0, 0.0),           // 0 lewo
        glm::vec3( 1.0, 0.0, 0.0),           // 1 prawo
        glm::vec3( 0.0, 1.0, 0.0),           // 2 góra
        glm::vec3( 0.0, 0.0, 1.0),           // 3 przód
        glm::vec3( 0.0, 0.0, -1.0)           // 4 tył
    };


    
    static const glm::vec3 vertices[] = {
        vs[0], ns[3], vs[1], ns[3], vs[3], ns[3],
        vs[0], ns[3], vs[3], ns[3], vs[2], ns[3],
        vs[5], ns[0], vs[3], ns[0], vs[1], ns[0],
        vs[5], ns[0], vs[7], ns[0], vs[3], ns[0],
        vs[0], ns[1], vs[6], ns[1], vs[4], ns[1],
        vs[0], ns[1], vs[2], ns[1], vs[6], ns[1],
        vs[4], ns[4], vs[6], ns[4], vs[7], ns[4],
        vs[4], ns[4], vs[7], ns[4], vs[5], ns[4],
        vs[0], ns[2], vs[5], ns[2], vs[1], ns[2],
        vs[0], ns[2], vs[4], ns[2], vs[5], ns[2]
    };
    



    glBufferData(GL_ARRAY_BUFFER, 6 * 10 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Cuboid::draw() {
	bindProgram();
    bindVAO();


    glm::mat4 viewmat = camera.getMainView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getMainProjection();
    setUniformmat4("projection", projectionmat);

    setUniformvec3("lightPos", lightsource);

    setUniformvec3("viewPos", camera.cameraPos + camera.cameraFront);

    glDrawArrays(GL_TRIANGLES, 0, 30);
}


