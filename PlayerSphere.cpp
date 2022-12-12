#include "PlayerSphere.hpp"


extern GLuint LoadShaders(const char * vertex_path,const char * fragment_path);

extern GLuint N;

extern Camera camera;

extern glm::vec3 lightsource;


PlayerSphere::PlayerSphere(const char * vertex_path,const char * fragment_path) {
    createBuffers();
    programID = LoadShaders(vertex_path, fragment_path);
    setBuffers();
}

PlayerSphere::PlayerSphere() {
}

void PlayerSphere::initialize(GLuint prog) {
    radius = 0.1;
    createBuffers();
    programID = prog;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int texWidth, texHeight, nrChannels;
    unsigned char *texData;

    texData = stbi_load("kulatarskiego.jpg", &texWidth, &texHeight, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

    stbi_image_free(texData);
    setPosition(0.0, 0.0, 0.0);
    setBuffers();   

}

void PlayerSphere::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        pos = glm::vec3(x, y, z);
}       

void PlayerSphere::setRotation(GLfloat a, GLfloat b, GLfloat c) {
    rot[0] = a;
    rot[1] = b;
    rot[2] = c;
}

glm::vec3 PlayerSphere::getCenter() {
    return pos;
}

GLfloat PlayerSphere::getRadius() {
    return radius;
}

// based on http://www.songho.ca/opengl/gl_sphere.html
void PlayerSphere::setBuffers() {
    bindVAO();
	bindBuffers();

    GLfloat x, y, z, xy;   
    GLfloat s, t;            

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
            // tex coords push bak
            s = (GLfloat)j / SECTORS;
            t = (GLfloat)i / STACKS;
            vertices.push_back(s);
            vertices.push_back(t);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

}


void PlayerSphere::draw() {
	bindProgram();
    glBindTexture(GL_TEXTURE_2D, texture);
    bindVAO();


      

    glm::mat4 modelmat = glm::mat4(1.0f);

    modelmat = glm::translate(modelmat, pos);

    modelmat = glm::rotate(modelmat, (float)glm::radians(-90.0), glm::vec3(0.0f, 1.0f, 0.0f));
    modelmat = glm::rotate(modelmat, (float)glm::radians(-90.0), glm::vec3(1.0f, 0.0f, 0.0f));
    

    setUniformmat4("model", modelmat);

    glm::mat4 viewmat = camera.getMainView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getMainProjection();
    setUniformmat4("projection", projectionmat);


    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void PlayerSphere::drawSecondary() {
 	bindProgram();
    bindVAO();

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, pos);
    setUniformmat4("model", modelmat);

    glm::mat4 viewmat = camera.getSecondaryView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getSecondaryProjection();
    setUniformmat4("projection", projectionmat);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);   
}

void PlayerSphere::moveForward(GLfloat speed, GLfloat delta) {
    pos -= speed * delta * camera.cameraFront;
}

void PlayerSphere::moveBackward(GLfloat speed, GLfloat delta) {
    pos += speed * delta * camera.cameraFront;
}

void PlayerSphere::moveLeft(GLfloat speed, GLfloat delta) {
    pos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * speed * delta;
}

void PlayerSphere::moveRight(GLfloat speed, GLfloat delta) {
    pos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * speed * delta;

}


