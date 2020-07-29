#pragma once

#include <nlohmann/json.hpp>
#include <rage/rage.hpp>

namespace CCD {
class Vector3 : public rage::vector3 {
 public:
  Vector3();
  Vector3(float x, float y, float z);
  explicit Vector3(rage::vector3 const& v);

  void setX(float x);
  void setY(float y);
  void setZ(float z);

  float getX() const;
  float getY() const;
  float getZ() const;

  virtual void toJSON(nlohmann::json& data) const;

  float calcDistance(Vector3 const& other) const;
  // Calculates distance in 2-dimensional
  float calcXYDistance(Vector3 const& other) const;

  bool operator==(Vector3 const& other) const;
  bool operator!=(Vector3 const& other) const;
};

class Transform : public Vector3 {
 public:
  Transform();
  Transform(float x, float y, float z, float h);
  explicit Transform(rage::vector3 const& v, float h);
  explicit Transform(Vector3 v, float h);

  void setH(float h);
  float getH() const;

  void toJSON(nlohmann::json& data) const override;

 private:
  float h;
};
}  // namespace CCD
