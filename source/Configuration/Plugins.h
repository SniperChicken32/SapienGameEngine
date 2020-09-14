//
// Hard-coded Plug-ins

#ifndef PLUGINS__
#define PLUGINS__

//
// OpenGL GLEW static
#define GLEW_STATIC
#include <gl/glew.h>

//
// Image library by STB
#define  STB_IMAGE_IMPLEMENTATION

//#define  STBI_ONLY_JPEG
#define  STBI_ONLY_PNG
#define  STBI_ONLY_BMP
//#define  STBI_ONLY_PSD
#define  STBI_ONLY_TGA
//#define  STBI_ONLY_GIF
//#define  STBI_ONLY_HDR
//#define  STBI_ONLY_PIC
//#define  STBI_ONLY_PNM // (.ppm and .pgm)
#include <stb_image/stb_image.h>

//
// Math library by GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Wavefront file loader
#include <Bly7/OBJ_Loader.h>

// Physics by ReactPhysics3D
#include <ReactPhysics3D/reactphysics3d.h>
namespace Physics = reactphysics3d;
Physics::PhysicsCommon* PhysicsCommon = nullptr;
Physics::PhysicsWorld*  PhysicsWorld = nullptr;

#endif








