#ifndef ALIASING_H
#define ALIASING_H

#include <stdlib.h>

#if defined(_WIN32)
#  ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#  endif
#  include <GL/freeglut.h>
#elif defined(__APPLE__) || defined(MACOSX)
#  define GL_SILENCE_DEPRECATION
#  include <GLUT/glut.h>
#else
#  include <GL/freeglut.h>
#endif

#endif /* ALIASING_H */
