#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct {
  int x;
  int y;
} Vec2;

typedef struct {
  int x;
  int y;
  int z;
} Vec3;

// Function prototypes for Vec2
Vec2 vec2_create(int x, int y);
Vec2 vec2_add(Vec2 v1, Vec2 v2);
Vec2 vec2_subtract(Vec2 v1, Vec2 v2);
int vec2_dot_product(Vec2 v1, Vec2 v2);
int vec2_magnitude(Vec2 v);
Vec2 vec2_normalize(Vec2 v);

// Function prototypes for Vec3
Vec3 vec3_create(int x, int y, int z);
Vec3 vec3_add(Vec3 v1, Vec3 v2);
Vec3 vec3_subtract(Vec3 v1, Vec3 v2);
int vec3_dot_product(Vec3 v1, Vec3 v2);
Vec3 vec3_cross_product(Vec3 v1, Vec3 v2);
int vec3_magnitude(Vec3 v);
Vec3 vec3_normalize(Vec3 v);

#endif // __VECTOR_H__