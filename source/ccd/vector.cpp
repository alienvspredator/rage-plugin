#include <ccd/vector.h>

CCD::Vector3::Vector3() : Vector3(0, 0, 0) {}

CCD::Vector3::Vector3(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

CCD::Vector3::Vector3(rage::vector3 const &v) : Vector3(v.x, v.y, v.z) {}
