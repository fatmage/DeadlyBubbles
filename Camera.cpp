#include "Camera.hpp"

extern GLFWwindow *window;


Camera::Camera() {
    setMainWindow(600,600);
    lastX = 0;
    lastY = 0;
    setSecondaryWindow(240,240);
    pitch = 0.0f;
    yaw = -90.0f;

    cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraPos2 = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    cameraFront2 = glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f));
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 
}

void Camera::resize(int w, int h) {
    setMainWindow(w, h);
	setSecondaryWindow(w, h);
}

void Camera::setMainWindow(int w, int h) {
    mainWindowWidth = w;
    mainWindowHeight = h;
}

void Camera::setSecondaryWindow(int w, int h) {
    secondaryWindowHeight = h/2.5;
    secondaryWindowWidth = h/2.5;
}


int Camera::getMainWidth() {
    return mainWindowWidth;
}

int Camera::getMainHeight() {
    return mainWindowHeight;
}

int Camera::getSecondaryWidth() {
    return secondaryWindowWidth;
}

int Camera::getSecondaryHeight() {
    return secondaryWindowHeight;
}

void Camera::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    cameraPos.x = x;
    cameraPos.y = y;
    cameraPos.z = z;
}

void Camera::setSecondaryPosition(GLfloat x, GLfloat y, GLfloat z) {
    cameraPos2.x = x;
    cameraPos2.y = y;
    cameraPos2.z = z;   
}

glm::mat4 Camera::getMainProjection() {
    return glm::perspective(glm::radians(50.0f),
                            (float)mainWindowWidth / (float)mainWindowHeight, 
                            0.1f,             
                            50.0f);
}

glm::mat4 Camera::getSecondaryProjection() {
    return glm::perspective(glm::radians(45.0f),
                            (float)secondaryWindowWidth / (float)secondaryWindowHeight, 
                            0.1f,             
                            50.0f);
}


glm::mat4 Camera::getMainView() {
    return glm::lookAt( cameraPos+cameraFront,
                        cameraPos, 
                        cameraUp);
}

glm::mat4 Camera::getSecondaryView() {
    return glm::lookAt( cameraPos-cameraFront,
                        cameraPos, 
                        cameraUp);
}

void Camera::moveForward(GLfloat speed, GLfloat delta) {
    cameraPos -= speed * delta * cameraFront;
}

void Camera::moveBackward(GLfloat speed, GLfloat delta) {

    cameraPos += speed * delta * cameraFront;
}

void Camera::moveLeft(GLfloat speed, GLfloat delta) {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * delta;
}

void Camera::moveRight(GLfloat speed, GLfloat delta) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * delta;
}

void Camera::rotateMouse(float xpos, float ypos) {
    static bool first = true;
    if (first) {
        lastX = xpos;
        lastY = ypos;
        first = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)
    pitch = 89.0f;
    if (pitch < -89.0f)
    pitch = -89.0f;

    cameraFront = glm::normalize(glm::vec3( cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                            sin(glm::radians(pitch)),
                                            sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

void Camera::rotateLeft(GLfloat speed) {
    yaw -= 70.0 * speed;

    cameraFront = glm::normalize(glm::vec3( cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                            sin(glm::radians(pitch)),
                                            sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

void Camera::rotateRight(GLfloat speed) {
    yaw += 70.0 * speed;

    cameraFront = glm::normalize(glm::vec3( cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                            sin(glm::radians(pitch)),
                                            sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}


void Camera::rotateDown(GLfloat speed) {
    pitch += 70.0 * speed;

    if (pitch > 89.0f)
     pitch = 89.0f;

    cameraFront = glm::normalize(glm::vec3( cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                            sin(glm::radians(pitch)),
                                            sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

void Camera::rotateUp(GLfloat speed) {
    pitch -= 70.0 * speed;

    if (pitch < -89.0f)
        pitch = -89.0f;

    cameraFront = glm::normalize(glm::vec3( cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                            sin(glm::radians(pitch)),
                                            sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

void Camera::setMainViewport() {
	glViewport(0, 0, mainWindowWidth, mainWindowHeight);
}

void Camera::setSecondaryViewport() {
	glViewport(0, 0, secondaryWindowWidth, secondaryWindowHeight);
}





