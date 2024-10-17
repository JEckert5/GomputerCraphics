#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

struct OrbitCamera {
    static inline double theta = 0, phi = M_PI / 2, radius = 5, x = 0, y = 0, z = 5;
};

struct FlyCamera {
    static inline double angle = 0.01, lx = 0, lz = 0.1, x = 0, z = 5;

    static inline double getX() {
        return FlyCamera::x + FlyCamera::lx;
    }

    static inline double getZ() {
        return FlyCamera::z + FlyCamera::lz;
    }
};

enum CameraMode {
    ORBIT,
    FLY
};

#endif