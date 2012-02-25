#include <stdlib.h>
#ifdef WIN32
#  undef GL_GLEXT_PROTOTYPES
#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
