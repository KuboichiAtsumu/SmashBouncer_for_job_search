//==============================================================================================================================================
//
// ステージ別ゾーンの処理
// Author : Atsumu Kuboichi
//
//==============================================================================================================================================

//===========================================================================================================
// ヘッダーインクルード
//===========================================================================================================
#include "zone.h"//ゾーン
#include "enemy.h"//エネミー

//===========================================================================================================
// 静的メンバ変数初期化
//===========================================================================================================
int CZone::m_nAllNumZone = 0;//ゾーンの総数
int CZone::m_nNumZone[MAX_ZONE_TYPE] = {};//各タイプの総数
char CZone::m_aTextureFile[MAX_ZONE_TYPE][CProcess::MAX_TEXT] = {};//各タイプのテクスチャファイル

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CZone::CZone(int nPriority) : CObject3D(nPriority)
{
	//メンバ変数初期化
	m_ZoneType = STAGE;//ゾーンタイプ
	m_nStageID = 0;//各ゾーンのステージID
	m_bCollision = true;//ゾーンの当たり判定の有効化フラグ
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CZone::~CZone()
{
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT CZone::Init()
{
	//タイプ別に色を設定
	switch (m_ZoneType)
	{
	//ステージ区分
	case STAGE:
#ifdef _DEBUG
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
#else
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
#endif // _DEBUG
		break;

	default:
		break;
	}

	//ステージID設定
	m_nStageID = m_nNumZone[m_ZoneType];

	//ステージゾーンの総数を加算
	m_nNumZone[m_ZoneType]++;

	//合計ゾーンの数を加算
	m_nAllNumZone++;

	//テクスチャ生成
	CTexture* pTex = CManager::GetTexture();//取得
	int nTextureIdx = pTex->Regist(m_aTextureFile[m_ZoneType]);//登録
	BindTexture(pTex->GetAddress(nTextureIdx));//設定
	SetTextureIdx(nTextureIdx);//ID設定

	//3Dオブジェクト初期化処理
	if (FAILED(CObject3D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void CZone::Uninit()
{
	//総数を減らす
	m_nNumZone[m_ZoneType]--;
	m_nAllNumZone--;

	//オブジェクトの終了処理
	CObject3D::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void CZone::Update()
{
	//移動中の場合、不要なゾーンを使用しない
	if (CGame::GetGameState() == CGame::STATE_MOVE
		&& m_nStageID <= CGame::GetNumStage())
	{
		m_bCollision = false;
	}

	//オブジェクトの更新処理
	CObject3D::Update();
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void CZone::Draw()
{
	//オブジェクトの描画処理
	CObject3D::Draw();
}

//===========================================================================================================
// 生成処理
//===========================================================================================================
CZone* CZone::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Size, ZONETYPE type)
{
	//メモリを動的確保
	CZone* pZone = NEW CZone();

	//パラメータ設定
	pZone->SetType(TYPE::ZONE);//オブジェクト種類の設定
	pZone->SetPos(pos);//位置設定
	pZone->SetSize(Size);//サイズ設定
	pZone->SetZoneType(type);//ゾーンタイプ設定

	//初期化処理
	pZone->Init();

	return pZone;
}

//===========================================================================================================
// 対象がいるステージの判定処理
//===========================================================================================================
bool CZone::Judge(D3DXVECTOR3& pos)
{
	//情報の取得
	D3DXVECTOR3 StagePos = GetPos();//位置
	D3DXVECTOR3 StageSize = GetSize();//サイズ

	//範囲内だった場合
	//当たり判定が有効の場合
	if (pos.x >= StagePos.x - StageSize.x * 0.5f
		&& pos.x <= StagePos.x + StageSize.x * 0.5f
		&& pos.z >= StagePos.z - StageSize.z * 0.5f
		&& pos.z <= StagePos.z + StageSize.z * 0.5f
		&& m_bCollision)
	{
		//範囲内の場合trueを返す
		return true;
	}

	//範囲外の場合falseを返す
	return false;
}

//===========================================================================================================
// 当たり判定
//===========================================================================================================
void CZone::Collision(D3DXVECTOR3& pos, D3DXVECTOR3& oldpos, D3DXVECTOR3& size)
{
	//ローカル変数宣言
	D3DXVECTOR3 Pos = GetPos();//位置情報
	D3DXVECTOR3 Size = GetSize();//サイズ情報

	//左側の判定
	if (pos.x - size.x * 0.5f < Pos.x - Size.x * 0.5f
		&& oldpos.x - size.x * 0.5f >=  Pos.x - Size.x * 0.5f)
	{
		//過去の位置に戻す
		pos.x = oldpos.x;
	}

	//移動中は判定しない
	if (CGame::GetGameState() != CGame::STATE_MOVE)
	{
		//右側の判定
		if (pos.x + size.x * 0.5f > Pos.x + Size.x * 0.5f
			&& oldpos.x + size.x * 0.5f <= Pos.x + Size.x * 0.5f)
		{
			//過去の位置に戻す
			pos.x = oldpos.x;
		}
	}

	//手前側の判定
	if (pos.z - size.z * 0.5f < Pos.z - Size.z * 0.5f
		&& oldpos.z - size.z * 0.5f >= Pos.z - Size.z * 0.5f)
	{
		//過去の位置に戻す
		pos.z = oldpos.z;
	}

	//奥側の判定
	if (pos.z + size.z * 0.5f > Pos.z + Size.z * 0.5f
		&& oldpos.z + size.z * 0.5f <= Pos.z + Size.z * 0.5f)
	{
		//過去の位置に戻す
		pos.z = oldpos.z;
	}
}

//===========================================================================================================
// 各ゾーンタイプのテクスチャファイル読込処理
//===========================================================================================================
void CZone::LoadZoneTexFile()
{
	//ローカル変数宣言
	int nNumFile = 0;//ファイルパス番号
	char aDataSearch[CProcess::MAX_TEXT];//検索用変数

	//ファイルを開く
	FILE* pFile = fopen(TEXTURE_FILE, "r");

	//ファイルが開けない場合
	if (pFile == nullptr)
	{
		return;
	}

	//ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		//検索
		int nResult = fscanf(pFile, "%s", aDataSearch);

		//EOFが見つかった場合
		if (nResult == EOF)
		{//読み込み終了
			fclose(pFile);
			break;
		}
		//BLOCKが見つかった場合
		else if (!strcmp(aDataSearch, "FILE_SET"))
		{
			//項目ごとのデータを代入
			while (1)
			{
				//検索
				fscanf(pFile, "%s", aDataSearch);

				//END_BLOCKが見つかった場合
				if (!strcmp(aDataSearch, "END_FILE"))
				{//読み込みを終了
					break;
				}
				//データの読み込み
				else if (!strcmp(aDataSearch, "FILE"))
				{//ファイルパス
					fscanf(pFile, "%d", &nNumFile);//ファイルパス番号
					fscanf(pFile, "%s", &m_aTextureFile[nNumFile]);//ファイルパス
				}
			}
		}
		//上記以外が見つかった場合
		else
		{//飛ばしてそのまま読込を続ける

			continue;
		}
	}
}
