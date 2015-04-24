#include "camera.h"

#include <la.h>


Camera::Camera() {}

Camera::Camera(int w, int h)
{
    theta = -45 * DEG2RAD;
    phi = -45 * DEG2RAD;
    zoom = 10;
    fovy = 45 * DEG2RAD;
    width = w;
    height = h;
    near_clip = 0.1f;
    far_clip = 1000;

    eye = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom)) * glm::vec4(0, 0, 0, 1);
    ref = glm::vec4(0, 0, 0, 1);
    up = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::vec4(0, 1, 0, 0);
}

void Camera::RecomputeEye()
{
    eye = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom)) * glm::vec4(0, 0, 0, 1);
    up = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::vec4(0, 1, 0, 0);
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(fovy, width / height, near_clip, far_clip) * glm::lookAt(glm::vec3(eye), glm::vec3(ref), glm::vec3(up));
}

ray Camera::raycast(float x, float y){
    float sx = (2*x/width) - 1;
    float sy = 1 - (2*y/height);
    float len = glm::length(ref-eye);
    float a = fovy/2;

    glm::vec4 f = ref-eye/len;
    glm::vec3 f3 = glm::vec3(f[0], f[1], f[2]);
    glm::vec3 right3 = glm::cross(f3,glm::vec3(0,1,0));

    glm::vec4 right = glm::vec4(right3[0], right3[1], right3[2], 0);
    right /= glm::length(right);


    float aspect = width/height;

    glm::vec4 V = up*len*tanf(a);
    glm::vec4 H = right*len*aspect*tanf(a);
    glm::vec4 point = ref + sx*H + sy*V;
    ray ra = ray(eye, point);
    return ra;
}
