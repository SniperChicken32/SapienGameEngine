//
// Compiler configuration settings

// Event log file name
const std::string EVENT_LOG_FILENAME = "event_log.txt";

//
// Development options

#define  DEVELOPMENT_MODE_

#define  LOG_RENDER_DETAILS_
#define  LOG_GL_ERRORS_

#define  LOG_DETAILS_

//
// Console window output
#define  SHOW_CONSOLE_
//#define  CONSOLE_RENDER_TIMING_
//#define  CONSOLE_CHECK_ALLOCATIONS_
#define  CONSOLE_CHECK_COLLISIONS_


//
// Physics world settings
#define  GRAVITY_VECTOR  0.0, 0.0, (-9.81 * 2.0 * 2.0)

#define  ITERATER_POSITION_SOLVER  15
#define  ITERATER_VELOCITY_SOLVER  10


//
// Render settings

#define  PHYSICS_UPDATE_RATE  120.0
#define  RENDER_FRAME_RATE    120.0

// Maximum number of lights to be sampled by the shader per frame.
#define  MAX_LIGHT_COUNT      50

// Maximum number of instances per batch
#define  MAX_INSTANCE_COUNT   50

// Pixel format
//   GL_RGB  GL_RGB4  GL_RGB5  GL_RGB8  GL_RGB10  GL_RGB12  GL_RGB16
//   GL_RGBA GL_RGBA2 GL_RGBA4 GL_RGBA8 GL_RGBA12 GL_RGBA16
#define PIXEL_LAYOUT_INTERNAL_   GL_RGBA
#define PIXEL_LAYOUT_            GL_RGB



//
// Finalize settings
#ifdef  SHOW_CONSOLE_
#include <iostream>
#endif


// Engine plug-ins
#include "Configuration/Plugins.h"

// Platform specific functions
#include "Configuration/Platform.h"


