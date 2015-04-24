#pragma once

#include <la.h>
#include "ray.h"

class Camera
{
public:
    Camera();
    Camera(int w, int h);

    float phi, theta;  // Rotation about the X and Y axes
    float zoom;  // Translation on the Z axis away from the model
    float fovy;
    float width, height;  // Screen dimensions
    float near_clip;  // Near clip plane distance
    float far_clip;  // Far clip plane distance

    glm::vec4 eye, ref, up;

    glm::mat4 getViewProj();

    ray raycast(float x, float y);


    void RecomputeEye();
};
