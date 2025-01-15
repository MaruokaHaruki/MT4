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
//PI
const float PI = 3.14159265358979323846f;
#include <cmath>
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
#include "Quaternion.h"

///=============================================================================
///						任意軸回転行列作成
Matrix4x4 MakeRotateAxisMatrix(const Vector3 &axis, float angle) {
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
/// 2つのベクトルのなす角度を求める
Vector3 OrthogonalVector(const Vector3 &v) {
	if(fabs(v.x) < fabs(v.y)) {
		if(fabs(v.x) < fabs(v.z)) {
			return { 0.0f, -v.z, v.y };
		} else {
			return { -v.y, v.x, 0.0f };
		}
	} else {
		if(fabs(v.y) < fabs(v.z)) {
			return { v.z, 0.0f, -v.x };
		} else {
			return { -v.y, v.x, 0.0f };
		}
	}
}

///=============================================================================
///						ある方向からある方向への回転行列作成
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	Vector3 f = Normalize(from);
	Vector3 t = Normalize(to);
	float dot = Dot(f, t);

	// クロス積を計算
	Vector3 axis = Cross(f, t);

	// 軸の大きさをチェック
	if (Length(axis) < FLT_EPSILON) {
		if (dot > 0.0f) {
			// ベクトルが同方向の場合、単位行列を返す
			return Identity4x4();
		} else {
			// ベクトルが逆方向の場合、直交する軸で 180 度回転
			Vector3 orthoAxis = OrthogonalVector(f);
			return MakeRotateAxisMatrix(orthoAxis, PI);
		}
	} else {
		float angle = acosf(dot);
		return MakeRotateAxisMatrix(axis, angle);
	}
}


///=============================================================================
///						マトリックス出力
void ScreenPrintMatrix(const char *name, const Matrix4x4 &m, int x, int y) {
	Novice::ScreenPrintf(x, y, name);
	Novice::ScreenPrintf(x, y + 20, "%.3f %.3f %.3f %.3f", m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	Novice::ScreenPrintf(x, y + 40, "%.3f %.3f %.3f %.3f", m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	Novice::ScreenPrintf(x, y + 60, "%.3f %.3f %.3f %.3f", m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	Novice::ScreenPrintf(x, y + 80, "%.3f %.3f %.3f %.3f", m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
}

void ScreenPrintQuaternion(const char *name, const Quaternion &q, int x, int y) {
	Novice::ScreenPrintf(x, y, name);
	Novice::ScreenPrintf(x, y + 20, "%.3f %.3f %.3f %.3f", q.x, q.y, q.z, q.w);
}

///=============================================================================
///	Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//========================================
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	//========================================
	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

	//========================================
	// ある方向からある方向への回転行列
	Vector3 from0 = Normalize({ 1.0f, 0.7f, 0.5f });
	Vector3 to0 = -from0;

	Vector3 from1 = Normalize({ -0.6f, 0.9f, 0.2f });
	Vector3 to1 = Normalize({ 0.4f, 0.7f, -0.5f });
	Matrix4x4 rotateMatrix0 = DirectionToDirection(
		Normalize(Vector3{ 1.0f,0.0f,0.0f }), Normalize(Vector3{ -1.0f,0.0f,0.0f }));
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

	//========================================
	// クオータニオン
	Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
	Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };
	Quaternion identity = Identity();
	Quaternion conjugate = Conjugate(q1);
	Quaternion inverse = Inverse(q1);
	Quaternion normalize = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

	///--------------------------------------------------------------
	/// ウィンドウの×ボタンが押されるまでループ
	while(Novice::ProcessMessage() == 0) {
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
		//ScreenPrintMatrix("RotateMatrix", rotateMatrix, 10, 10);

		//========================================
		// ある方向からある方向への回転行列の表示
		//ScreenPrintMatrix("RotateMatrix0", rotateMatrix0, 10, 10);
		//ScreenPrintMatrix("RotateMatrix1", rotateMatrix1, 10, 110);
		//ScreenPrintMatrix("RotateMatrix2", rotateMatrix2, 10, 210);

		//========================================
		// クオータニオンの表示
		int y = 40;
		ScreenPrintQuaternion("Identity", identity, 10, y * 1);
		ScreenPrintQuaternion("Quaternion1", q1, 10, y * 2);
		ScreenPrintQuaternion("Quaternion2", q2, 10, y * 3);
		ScreenPrintQuaternion("Conjugate", conjugate, 10, y * 4);
		ScreenPrintQuaternion("Inverse", inverse, 10, y * 5);
		ScreenPrintQuaternion("Normalize", normalize, 10, y * 6);
		ScreenPrintQuaternion("Multiply1", mul1, 10, y * 7);
		ScreenPrintQuaternion("Multiply2", mul2, 10, y * 8);
		Novice::ScreenPrintf(10, y*9, "Norm: %.3f", norm);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if(preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
