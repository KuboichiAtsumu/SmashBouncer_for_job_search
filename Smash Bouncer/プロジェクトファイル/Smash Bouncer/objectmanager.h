#pragma once
//==============================================================================================================================================
//
// オブジェクトマネージャーに関する処理のヘッダーファイル
// Author : Atsumu Kuboichi
//
//==============================================================================================================================================
#ifndef _OBJECTMANAGER_H_ //このマクロ定義がされなかったら
#define _OBJECTMANAGER_H_ //2重インクルード防止のマクロ定義

//===========================================================================================================
// ヘッダーインクルード
//===========================================================================================================
#include "object.h"//オブジェクト

//===========================================================================================================
// クラス定義
//===========================================================================================================
class CObjectManager : public CObject
{
public:
	//======================================
	// 関数
	//======================================
	CObjectManager(int nPriority = 97);//コンストラクタ
	~CObjectManager() override;//デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override;//終了処理
	void Update() override;//更新処理
	void Draw() override;//描画処理
};

#endif
