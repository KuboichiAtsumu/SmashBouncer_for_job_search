#pragma once
//==============================================================================================================================================
//
// タイトルロゴに関する処理のヘッダーファイル
// Author : Atsumu Kuboichi
//
//==============================================================================================================================================
#ifndef _LOGO_H_ //このマクロ定義がされなかったら
#define _LOGO_H_ //2重インクルード防止のマクロ定義

//===========================================================================================================
// ヘッダーインクルード
//===========================================================================================================
#include "object3D.h"//3Dオブジェクト

//===========================================================================================================
// クラス定義
//===========================================================================================================
class CLogo : public CObject3D
{
public:
	//======================================
	// 関数
	//======================================
	CLogo(int nPriority);//コンストラクタ
	~CLogo() override;//デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override;//終了処理
	void Update() override;//更新処理
	void Draw() override;//描画処理
	static CLogo* Create();//生成処理

private:
	//======================================
	// 変数
	//======================================
	static constexpr const char* TEXTURE_FILE = "data/TEXTURE/Title/Titlelogo000.png";//テクスチャファイル
	const D3DXVECTOR3 POS = { 10.0f, 50.0f, 70.0f };//生成位置
	const D3DXVECTOR3 ROT = { -D3DX_PI * 0.4f, -0.2f, 0.0f };//生成角度
	const D3DXVECTOR3 SIZE = { 420.0f, 0.0f, 100.0f };//生成サイズ
	int m_nCnt;//フラグ切り替えカウント
	bool m_bUp;//移動方向フラグ
};

#endif
