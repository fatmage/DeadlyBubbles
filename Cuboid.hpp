#ifndef CUBOID_HPP
#define CUBOID_HPP

#include "DrawableObject.hpp"
#include "Camera.hpp"

extern Camera camera;


class Cuboid : DrawableObject {


    void setBuffers();

    public:
    Cuboid();

    void initialize(GLuint prog);
    void draw();
    void drawSecondary();

};

#endif