# aliasing - アキュムレーションバッファによるアンチエイリアシング

## 1. 概要

本プログラムは、OpenGL のアキュムレーションバッファ（Accumulation Buffer）を利用して、シーンを微小にずらしながら複数回レンダリングして合成することにより、フルシーン・アンチエイリアシング（FSAA）を実現するサンプルプログラムです。

- 移行元ブログ記事:
  - [ビデオカード - 床井研究室](https://tokoik.github.io/blog/2004/11/30/)
  - [GLSL による画像のフィルタ処理 (アンチエイリアシング) - 床井研究室](https://tokoik.github.io/blog/2012/09/04/)

## 2. 対応環境

- **Windows**: Visual Studio 2019 / 2022 / 2026 (CMake 経由で freeglut を自動ダウンロード・ビルド)
- **macOS**: Xcode (macOS 標準の GLUT および OpenGL Framework を使用)
- **Ubuntu Linux**: GCC / Make (システム標準の freeglut3-dev, libgl1-mesa-dev を使用)

## 3. ビルド手順

### Windows (Visual Studio)

```pwsh
cmake -B build -S .
cmake --build build --config Release
```

### macOS (Xcode)

```bash
cmake -B build -G Xcode
cmake --build build --config Release
```

### Ubuntu Linux (Makefile)

```bash
sudo apt-get update
sudo apt-get install -y freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
cmake -B build -S .
cmake --build build
```

## 4. 起動方法

ビルド完了後、生成された実行ファイルを実行します。

- **Windows**: `build/Release/aliasing.exe`
- **macOS**: `build/Release/aliasing.app`
- **Linux**: `build/aliasing`

## 5. 操作方法

- **マウス左ドラッグ**: シーンの視点回転（クォータニオンによるトラックボール回転）
- **マウス右クリック**: ポップアップメニューの表示
  - **Antialiasing**: サンプリング数（1x, 2x, 3x, 4x, 8x, 16x 等）の切り替え
  - **Model**: 表示モデルの切り替え（Teapot, Cube, Sphere 等）
  - **Fog**: フォグ効果の有効/無効切り替え
  - **Quit**: プログラムの終了

## 6. プログラムの解説

アキュムレーションバッファを用いたアンチエイリアシングは、各サブピクセル位置に対応するジッタリング（微小な投影変換のオフセット）を与えて描画し、`glAccum(GL_ACCUM, weight)` でアキュムレーションバッファに加算後、最後に `glAccum(GL_RETURN, 1.0)` でカラーバッファに書き戻すことで高品質なアンチエイリアシングを実現しています。
