#include <math.h>
#include "aliasing.h"
#include "menu.h"

#define PI 3.14159265358979323846

/* ボタンの状態 */
static int pbutton, pstate;

/* ドラッグ開始位置 */
static int cx, cy;

/* マウスの絶対位置→ウィンドウ内での相対位置の換算係数 */
static double sx, sy;

/* 回転の初期値 (クォータニオン) */
static double cq[4] = { 1.0, 0.0, 0.0, 0.0 };

/* ドラッグ中の回転 (クォータニオン) */
static double tq[4];

/* 回転の変換行列 */
static double rt[16];

/* クォータニオンの積 */
extern void qmul(double [], const double [], const double []);

/* クォータニオン→回転の変換行列 */
extern void qrot(double [], double []);

/* スクリーンのアスペクト比 */
static double aspect;

void scene(double ox, double oy)
{
  /* 光源の位置 */
  static GLfloat lightpos[] = { 4.0, 5.0, 6.0, 0.0 };

  /* 物体の色 */
  static GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };

  /* 床の色 */
  static GLfloat ground[][4] = {
    { 0.6, 0.6, 0.6, 1.0 },
    { 0.3, 0.3, 0.3, 1.0 }
  };

  int i, j;

  /* 画面クリア */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* 透視変換行列の指定 */
  glMatrixMode(GL_PROJECTION);

  /* 透視変換行列の初期化 */
  glLoadIdentity();

  /* 表示領域をサブピクセル単位にずらす */
  glTranslated(ox * sx, oy * sy, 0.0);

  /* 視野の設定 */
  glFrustum(-0.5, 0.5, -aspect, aspect, 2.0, 20.0);

  /* モデルビュー変換行列の指定 */
  glMatrixMode(GL_MODELVIEW);

  /* モデルビュー変換行列の初期化 */
  glLoadIdentity();

  /* 視点の移動 */
  glTranslated(0.0, 0.0, -10.0);

  /* 回転 */
  glMultMatrixd(rt);

  /* 光源の位置を設定 */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  /* 床 */
  glNormal3d(0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
  for (j = -5; j <= 5; j++) {
    for (i = -5; i < 5; i++) {
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ground[(i + j) & 1]);
      glVertex3d((GLdouble)i, -1.0, (GLdouble)j);
      glVertex3d((GLdouble)i, -1.0, (GLdouble)(j + 1));
      glVertex3d((GLdouble)(i + 1), -1.0, (GLdouble)(j + 1));
      glVertex3d((GLdouble)(i + 1), -1.0, (GLdouble)j);
    }
  }
  glEnd();

  /* 黄色いティーポット */
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
  if (model)
    glutSolidTeapot(1.0);
  else
    glutWireTeapot(1.0);

  glFlush();
}

void display(void)
{
  if (fog)
    glEnable(GL_FOG);
  else
    glDisable(GL_FOG);

  if (method == 0) {
    /*
    ** アンチエリアシングなし
    */
    scene(0.0, 0.0);
  } else if (method == 1) {
    /*
    ** OpenGL 自身のアンチエリアシング機能（ワイヤフレームのみ）
    */
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    scene(0.0, 0.0);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);

  } else if (2 <= method && method <= 8) {
    /*
    ** グリッドに沿ってスーパーサンプリング
    */
    int i, j, n = method;
    double step = 1.0 / (double)n;
    
    for (j = 0; j < n; j++) {
      for (i = 0; i < n; i++) {
        scene((double)i * step, (double)j * step);
        if (i | j)
          glAccum(GL_ACCUM, step * step);
        else
          glAccum(GL_LOAD, step * step);
      }
    }
    glAccum(GL_RETURN, 1.0);
  }
  else if (method == 9) {
    /*
    ** RGSS (Rotated Grid SuperSampling)
    */
    scene(0.0, 0.25);
    glAccum(GL_LOAD, 0.25);
    scene(0.5, 0.0);
    glAccum(GL_ACCUM, 0.25);
    scene(0.75, 0.5);
    glAccum(GL_ACCUM, 0.25);
    scene(0.25, 0.75);
    glAccum(GL_ACCUM, 0.25);
    glAccum(GL_RETURN, 1.0);
  }
  else if (10 <= method && method <= 16) {
    /*
    ** ランダムにスーパーサンプリング
    */
    int i, n = (method - 8) * (method - 8);
    double weight = 1.0 / (double)n;

    srand(34761);
    glAccum(GL_LOAD, weight);
    scene((double)rand() / ((double)RAND_MAX + 1.0),
      (double)rand() / ((double)RAND_MAX + 1.0));
    for (i = 1; i < n; i++) {
      scene((double)rand() / ((double)RAND_MAX + 1.0),
        (double)rand() / ((double)RAND_MAX + 1.0));
      glAccum(GL_ACCUM, weight); 
    }
    glAccum(GL_RETURN, 1.0);
    /*
    ** この方法は全ピクセルを同じパターンでサンプリングしているが，
    ** ピクセルごとに独立して（乱数を発生させて）サンプリングする
    ** 方がエリアシングが目立たない（少ないサンプリング数で済む）．
    */
  }
  else if (method == 17) {
    /*
    ** Quincunx
    */
    scene(0.0, 0.0);
    glAccum(GL_LOAD, 0.5);
    scene(-0.5, -0.5);
    glAccum(GL_ACCUM, 0.125);
    scene( 0.5, -0.5);
    glAccum(GL_ACCUM, 0.125);
    scene( 0.5, 0.5);
    glAccum(GL_ACCUM, 0.125);
    scene(-0.5, 0.5);
    glAccum(GL_ACCUM, 0.125);
    glAccum(GL_RETURN, 1.0);
    /*
    ** この手法はは隣接するピクセルの値を使うので，本当なら
    ** レンダリング (scene() の呼び出し)は２回で済む．
    */
  }
  glutSwapBuffers();
}

void idle(void)
{
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  /* ボタンの状態を記録 */
  pbutton = button;
  pstate = state;

  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* ドラッグ開始点を記録 */
      cx = x;
      cy = y;
      /* アニメーション開始 */
      glutIdleFunc(idle);
      break;
    case GLUT_UP:
      /* アニメーション終了 */
      glutIdleFunc(0);
      /* 回転の保存 */
      cq[0] = tq[0];
      cq[1] = tq[1];
      cq[2] = tq[2];
      cq[3] = tq[3];
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void motion(int x, int y)
{
  if (pbutton == GLUT_LEFT_BUTTON) {
    double dx, dy, a;
    
    /* マウスポインタの位置のドラッグ開始位置からの変位 */
    dx = (x - cx) * sx;
    dy = (y - cy) * sy;
    
    /* マウスポインタの位置のドラッグ開始位置からの距離 */
    a = sqrt(dx * dx + dy * dy);
    
    if (a != 0.0) {
      double ar = a * PI * 0.5;
      double as = sin(ar) / a;
      double dq[4] = { cos(ar), dy * as, dx * as, 0.0 };
      
      /* クォータニオンを掛けて回転を合成 */
      qmul(tq, dq, cq);
      /* クォータニオンから回転の変換行列を求める */
      qrot(rt, tq);
    }
  }
}

void resize(int w, int h)
{
  /* 表示領域の１画素の正規化デバイス座標系での大きさ */
  sx = 2.0 / (double)w;
  sy = 2.0 / (double)h;

  /* スクリーンのアスペクト比の２分の１ */
  aspect = 0.5 * (double)h / (double)w;

  /* ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
  /* ESC か q をタイプしたら終了 */
  if (key == '\033' || key == 'q') {
    exit(0);
  }
}

void init(void)
{
  float fogcolor[] = { 0.5, 0.5, 0.5, 0.0 };
  /* 初期設定 */
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  /* glAccum() の処理対称とするバッファ */
  glReadBuffer(GL_BACK);

  /* フォグの設定 */
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_DENSITY, 0.5);
  glFogfv(GL_FOG_COLOR, fogcolor);
  glFogf(GL_FOG_START, 2.0);
  glFogf(GL_FOG_END, 15.0);

  /* アンチエリアシングの設定 */
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* 回転行列の初期化 */
  qrot(rt, cq);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ACCUM);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  init();
  menu();
  glutMainLoop();
  return 0;
}
