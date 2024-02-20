#include "vector.h"

// Functions for Vec2
Vec2 vec2_create(int x, int y) {
  Vec2 v;
  v.x = x;
  v.y = y;
  return v;
}

Vec2 vec2_add(Vec2 v1, Vec2 v2) {
  Vec2 result;
  result.x = v1.x + v2.x;
  result.y = v1.y + v2.y;
  return result;
}

Vec2 vec2_subtract(Vec2 v1, Vec2 v2) {
  Vec2 result;
  result.x = v1.x - v2.x;
  result.y = v1.y - v2.y;
  return result;
}

int vec2_dot_product(Vec2 v1, Vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }

int vec2_magnitude_squared(Vec2 v) { return v.x * v.x + v.y * v.y; }

Vec2 vec2_normalize(Vec2 v) {
  int mag_squared = vec2_magnitude_squared(v);
  if (mag_squared == 0)
    return v;
  int factor = 1;
  if (mag_squared != 1) {
    factor = 1;
    while (factor * factor < mag_squared) {
      factor++;
    }
    if (factor * factor > mag_squared) {
      factor--;
    }
  }
  return vec2_create(v.x / factor, v.y / factor);
}

// Functions for Vec3
Vec3 vec3_create(int x, int y, int z) {
  Vec3 v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

Vec3 vec3_add(Vec3 v1, Vec3 v2) {
  Vec3 result;
  result.x = v1.x + v2.x;
  result.y = v1.y + v2.y;
  result.z = v1.z + v2.z;
  return result;
}

Vec3 vec3_subtract(Vec3 v1, Vec3 v2) {
  Vec3 result;
  result.x = v1.x - v2.x;
  result.y = v1.y - v2.y;
  result.z = v1.z - v2.z;
  return result;
}

int vec3_dot_product(Vec3 v1, Vec3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 vec3_cross_product(Vec3 v1, Vec3 v2) {
  Vec3 result;
  result.x = v1.y * v2.z - v1.z * v2.y;
  result.y = v1.z * v2.x - v1.x * v2.z;
  result.z = v1.x * v2.y - v1.y * v2.x;
  return result;
}

int vec3_magnitude_squared(Vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

Vec3 vec3_normalize(Vec3 v) {
  int mag_squared = vec3_magnitude_squared(v);
  if (mag_squared == 0)
    return v;
  int factor = 1;
  if (mag_squared != 1) {
    factor = 1;
    while (factor * factor < mag_squared) {
      factor++;
    }
    if (factor * factor > mag_squared) {
      factor--;
    }
  }
  return vec3_create(v.x / factor, v.y / factor, v.z / factor);
}
