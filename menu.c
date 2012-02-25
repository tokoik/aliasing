#include "aliasing.h"
#include "menu.h"

int method = 0; /* サンプリング方法　　 */
int model = 0;  /* テーポットの表示方法 */
int fog = 0;    /* フォグの状態　　　　 */

static void changeEntry(int i, char c, char *s)
{
  char buffer[100], *p = buffer;
  int n = sizeof(buffer) / sizeof(char) - 1;

  *p = c;
  do {
    if (--n <= 0) {
      *p = '\0';
      break;
    }
  }
  while ((*++p = *++s) != '\0');

  glutChangeToMenuEntry(i + 1, buffer, i);
}

static char *methodName[] = {
  "*None",
  " (OpenGL)",
  " Grid 2x2",
  " Grid 3x3",
  " Grid 4x4",
  " Grid 5x5",
  " Grid 6x6",
  " Grid 7x7",
  " Grid 8x8",
  " 2x2 RGSS",
  " Random  4",
  " Random  9",
  " Random 16",
  " Random 25",
  " Random 36",
  " Random 49",
  " Random 64",
  " Quincunx",
};

static void methodMenu(int m)
{
  changeEntry(method, ' ', methodName[method]);
  method = m;
  changeEntry(method, '*', methodName[method]);
  glutPostRedisplay();
}

static char *modelName[] = {
  "*Wireframe",
  " Solid",
};

static void modelMenu(int m)
{
  changeEntry(model, ' ', modelName[model]);
  model = m;
  changeEntry(model, '*', modelName[model]);
  glutPostRedisplay();
}

static char *fogStatus[] = {
  "*Off",
  " On",
};

static void fogMenu(int m)
{
  changeEntry(fog, ' ', fogStatus[fog]);
  fog = m;
  changeEntry(fog, '*', fogStatus[fog]);
  glutPostRedisplay();
}

static void mainMenu(int m)
{
  switch(m) {
  case 0:
    exit(0);
  default:
    break;
  }
}

void menu(void)
{
  int i, methodId, modelId, fogId;

  methodId = glutCreateMenu(methodMenu);
  for (i = 0; i < (sizeof methodName) / (sizeof (char *)); i++)
    glutAddMenuEntry(methodName[i], i);

  modelId = glutCreateMenu(modelMenu);
  for (i = 0; i < (sizeof modelName) / (sizeof (char *)); i++)
    glutAddMenuEntry(modelName[i], i);

  fogId = glutCreateMenu(fogMenu);
  for (i = 0; i < (sizeof fogStatus) / (sizeof (char *)); i++)
    glutAddMenuEntry(fogStatus[i], i);

  glutCreateMenu(mainMenu);
  glutAddSubMenu("Super Sampling", methodId);
  glutAddSubMenu("Display", modelId);
  glutAddSubMenu("Fog", fogId);
  glutAddMenuEntry("Quit", 0);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

