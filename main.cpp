/*********************************************************************
 * \file   main.cpp
 * \brief  
 * 
 * \author Harukichimaru
 * \date   January 2025
 * \note   
 *********************************************************************/
#include <Novice.h>
const char kWindowTitle[] = "学籍番号";
//========================================
// 数学構造体関数
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Transform.h"
//========================================
// 数学関数
#include "AffineTransformations.h"
#include "MathFunc4x4.h"
#include "RenderingMatrices.h"

///=============================================================================
///						任意軸回転行列作成
Matrix4x4 MakeRotateAxisMatrix(const Vector3& axis, float angle) {
	//========================================
	// 軸ベクトルを正規化
	Vector3 n = Normalize(axis);
	//========================================
	// 角度のcos, sin, 1-cosを計算
	float c = cosf(angle);
	float s = sinf(angle);
	//========================================
	// 回転行列を計算
	float t = 1.0f - c;
	// 回転行列
	Matrix4x4 m;
	m.m[0][0] = t * n.x * n.x + c;
	m.m[0][1] = t * n.x * n.y + s * n.z;
	m.m[0][2] = t * n.x * n.z - s * n.y;
	m.m[0][3] = 0.0f;
	m.m[1][0] = t * n.x * n.y - s * n.z;
	m.m[1][1] = t * n.y * n.y + c;
	m.m[1][2] = t * n.y * n.z + s * n.x;
	m.m[1][3] = 0.0f;
	m.m[2][0] = t * n.x * n.z + s * n.y;
	m.m[2][1] = t * n.y * n.z - s * n.x;
	m.m[2][2] = t * n.z * n.z + c;
	m.m[2][3] = 0.0f;
	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;
	//========================================
	// 計算結果を返す
	return m;
}

///=============================================================================
///						マトリックス出力
void ScreenPrintMatrix(const char* name, const Matrix4x4 &m, int x, int y) {
	Novice::ScreenPrintf(x, y, name);
	Novice::ScreenPrintf(x, y + 20, "%.3f %.3f %.3f %.3f", m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	Novice::ScreenPrintf(x, y + 40, "%.3f %.3f %.3f %.3f", m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	Novice::ScreenPrintf(x, y + 60, "%.3f %.3f %.3f %.3f", m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	Novice::ScreenPrintf(x, y + 80, "%.3f %.3f %.3f %.3f", m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
}

///=============================================================================
///	Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//========================================
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	//========================================
	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	///--------------------------------------------------------------
	///	変数宣言
	//========================================
	// 任意軸回転行列
	// 軸
	Vector3 axis = { 1.0f, 1.0f, 1.0f };
	// 正規化
	axis = Normalize(axis);
	// 角度
	float angle = 0.44f;
	// 行列の計算
	Matrix4x4 rotateMatrix = MakeRotateAxisMatrix(axis, angle);

	///--------------------------------------------------------------
	/// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		//========================================
		// フレームの開始
		Novice::BeginFrame();
		//========================================
		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//========================================
		// 任意軸回転行列の表示
		ScreenPrintMatrix("RotateMatrix", rotateMatrix, 10, 10);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
