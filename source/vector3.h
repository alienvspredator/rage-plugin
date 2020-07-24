#pragma once

#include <rage/rage.hpp>

namespace CCD {
class Vector3 : public rage::vector3 {
 public:
  Vector3();
  Vector3(float x, float y, float z);
  explicit Vector3(rage::vector3 const &v);
};
}  // namespace CCD
