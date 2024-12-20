#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Player.h"

//	プレイヤーの移動スピード
#define PLAYER_MOV_SPEED	0.1f
//	プレイヤーの回転スピード
#define PLAYER_ROT_SPEED	1.0f

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void Player::Init(int model)
{
	m_model = model;
	//	Ｙ座標を少し上にしています
	m_pos.set(0.0f, 1.0f, -5.0f);
	m_rot.clear();
	//	行列の初期化
	m_mat = MGetIdent();
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void Player::Update()
{
	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_UP))
	{
		m_pos.y += 0.05f;
		if (m_pos.y > 3.0f)
		{
			m_pos.y = 3.0f;
			m_rot.x *= 0.9f;
		}
		else
		{
			m_rot.x -= PLAYER_ROT_SPEED;
		}
	}
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		m_pos.y -= 0.05f;
		if (m_pos.y < 0.5f)
		{
			m_pos.y = 0.5f;
			m_rot.x *= 0.9f;
		}
		else
		{
			m_rot.x += PLAYER_ROT_SPEED;
		}
	}
	else
	{
		m_rot *= 0.9f;
	}

	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		//	右キーを押している時
		m_rot.y += PLAYER_ROT_SPEED;
		m_rot.z -= PLAYER_ROT_SPEED;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT)) {
		//	左キーを押している時
		m_rot.y -= PLAYER_ROT_SPEED;
		m_rot.z += PLAYER_ROT_SPEED;
	}
	else {
		//	どちらも押していないとき
		//	Ｚ軸回転角度をどんどんと 0.0f に近づける
		m_rot.z *= 0.9f;
	}

	//	Ｚ軸の回転角度を -30.0f 〜 30.0f にします
	m_rot.z = max(-30.0f, min(m_rot.z, 30.0f));

	//	向いている方向に移動（Ｙ軸の回転の方向に）
	Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
	MATRIX mat = MGetRotY(TO_RADIAN(m_rot.y));
	Float3 mov = GetFloat3VTransform(base, mat);
	m_pos += mov;

	//	Ｙ軸の回転行列
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));
	//	Ｚ軸の回転行列
	MATRIX mat_z = MGetRotZ(TO_RADIAN(m_rot.z));
	//	この２つの行列を１個にします
	m_mat = MMult(mat_z, mat_y);
	m_mat = MMult(mat_x, m_mat);
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void Player::Render()
{
	//	プレイヤーモデルの描画
	//	モデルの座標の設定
	MV1SetPosition(m_model, VGet(m_pos.x, m_pos.y, m_pos.z));
	//	モデルの向きの設定（今回はこの順番の回転ではないのでコメント）
//	MV1SetRotationXYZ( m_model, VGet( TO_RADIAN( m_rot.x ), TO_RADIAN( m_rot.y ), TO_RADIAN( m_rot.z ) ) );
	//	今回はこちらで用意した行列を使ってモデルの回転をさせます
	MV1SetRotationMatrix(m_model, m_mat);
	//	モデルの描画
	MV1DrawModel(m_model);

	//	半透明の描画にします
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//	プレイヤーのいる地面の座標に円の描画
	Float3 pos = m_pos;
	//	とりあえず地面の高さにしますが、重ならないように少しだけ上にあげます
	pos.y = 0.01f;
	//	この座標にＸＺ方向の円の描画
	DrawCircle3D_XZ(pos, 0.5f, GetColor(0, 0, 0), true);
	//	次の描画のために半透明設定を戻します
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void Player::Exit()
{
}
