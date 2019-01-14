#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

//#define degreesToRadians(x) x*(3.141592f/180.0f)

// Configuration -----------
#define SCREEN_WIDTH 1536
#define SCREEN_HEIGHT 864
#define FULLSCREEN false
#define BORDERLESS false
#define	RESIZABLE true
#define VSYNC true
#define TITLE "SAG Engine"

// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

#define ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))

// World Axis
#define X_AXIS float3(1, 0, 0)
#define Y_AXIS float3(0, 1, 0)
#define Z_AXIS float3(0, 0, 1)

#define WORLD_FRONT float3(0, 0,-1)
#define WORLD_UP	float3(0, 1, 0)
#define WORLD_RIGHT float3(1, 0, 0)

// Max Number of Components
#define MAX_MATERIAL_COMP  1
#define MAX_CAMERA_COMP    1
#define MAX_MESH_COMP	  -1
#define MAX_LIGHT_COMP	  -1