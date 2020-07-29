#pragma once

#include <cstdint>

namespace CCD {
typedef uint32_t resourceType_t;

class Resource {
 public:
  resourceType_t getType() const;

 private:
  resourceType_t _type;
};
}  // namespace CCD
