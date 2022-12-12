#include "BubbleSphere.hpp"


extern GLuint LoadShaders(const char * vertex_path,const char * fragment_path);

extern Camera camera;

extern glm::vec3 lightsource;


BubbleSphere::BubbleSphere(const char * vertex_path,const char * fragment_path) {
    createBuffers();
    programID = LoadShaders(vertex_path, fragment_path);
    setBuffers();
}

BubbleSphere::BubbleSphere() {
}

void BubbleSphere::initialize(GLuint prog, GLuint num) {
    radius = 0.1;
    createBuffers();
    programID = prog;
    setPosition(0.0, 0.0, 5.0);
    setBubbles(num);
    setBuffers();   
}


GLfloat BubbleSphere::getRadius() {
    return radius;
}

void BubbleSphere::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        pos = glm::vec3(x, y, z);
}    

void BubbleSphere::setBubbles(GLuint num) {
    this->num = num;
    delete bubbles;
    bubbles = new glm::vec3[num];

    for (int i = 0; i < num; i++) {
        GLfloat dispX = (GLfloat)(rand()%600)/100.0 - 3.0;
        GLfloat dispY = (GLfloat)(rand()%600)/100.0 - 3.0;
        GLfloat dispZ = (GLfloat)(rand()%1050)/100.0 + 1.0;
        glm::vec3 displacement = glm::vec3(dispX, dispY, dispZ);
        bubbles[i] = displacement;
    }
}

// based on http://www.songho.ca/opengl/gl_sphere.html
void BubbleSphere::setBuffers() {
    bindVAO();
	bindBuffers();

    GLfloat x, y, z, xy;   
    GLfloat nx, ny, nz, lengthInv = 1.0f / radius;              

    GLfloat sectorStep = 2*PI/ SECTORS;
    GLfloat stackStep = PI / STACKS;
    GLfloat sectorAngle, stackAngle;

    
    for (int i = 0; i <= STACKS; ++i) {
        stackAngle = (PI / 2) - i * stackStep;
        xy = radius * cosf(stackAngle);             
        z = radius * sinf(stackAngle);              

        for(int j = 0; j <= SECTORS; ++j)
        {
            sectorAngle = j * sectorStep;          

            x = xy * cosf(sectorAngle);            
            y = xy * sinf(sectorAngle);            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    int count = vertices.size();


    int k1, k2;
    for(int i = 0; i < STACKS; ++i)
    {
        k1 = i * (SECTORS + 1);     
        k2 = k1 + SECTORS + 1;      

        for(int j = 0; j < SECTORS; ++j, ++k1, ++k2)
        {

            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (STACKS))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}


void BubbleSphere::draw() {
	bindProgram();
    bindVAO();
      
    glm::mat4 viewmat = camera.getMainView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getMainProjection();
    setUniformmat4("projection", projectionmat);

    setUniformvec3("lightPos", lightsource);

    setUniformvec3("viewPos", camera.cameraPos + camera.cameraFront);


    for (std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
        glm::mat4 modelmat = glm::mat4(1.0f);
        modelmat = glm::translate(modelmat, it->second);
        GLfloat scale = 1.0 + (it->second.y + 3.0)/18.0;
        modelmat = glm::scale(modelmat, glm::vec3(scale, scale, scale));
        setUniformmat4("model", modelmat);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    }


}

void BubbleSphere::sort(glm::vec3 playerPos) {
    this->sorted.clear();
    for (int i = 0; i < num; i++) {
        GLfloat dist = glm::length(playerPos - bubbles[i]);
        sorted[dist] = bubbles[i];
    }
}

void BubbleSphere::drawSecondary() {
 	bindProgram();
    bindVAO();

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, bubbles[0]);
    setUniformmat4("model", modelmat);

    glm::mat4 viewmat = camera.getSecondaryView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getSecondaryProjection();
    setUniformmat4("projection", projectionmat);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);   
}

void BubbleSphere::update(GLfloat speed, GLfloat delta) {

    for (int i = 0; i < num; i++) {
        GLfloat newPos = bubbles[i].y + delta * speed/6;
        bubbles[i].y = newPos < 3.0 ? newPos : -3.0;
    }

}

bool BubbleSphere::collision(glm::vec3 playerPos, GLfloat playerRadius) {
    glm::vec3 closest = sorted.begin()->second;
    GLfloat bubbleRadius = (1.0 + (closest.y + 3.0)/18.0) * 0.1;
    if (glm::length(playerPos - closest) <= playerRadius + bubbleRadius)
        return true;
    return false;

}

GLuint BubbleSphere::getNum() {
    return num;
}
