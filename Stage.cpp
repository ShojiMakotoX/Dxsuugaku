#include "Base.h"
#include "Stage.h"
#include "Math2D.h"
#include "Player.h"
#include <DxLib.h>
#include "Globals.h"
#include "Bullet.h"
#include "Input.h"
#include <vector>
#include "Enemy.h"
#include "ExplosionEffect.h"

namespace
{
	//Player Parameters :プレイヤーの初期値
	const Vector2D START_POS = { WIN_WIDTH / 2,WIN_HEIGHT / 2 };
	const Vector2D START_VEL = { 0.0f,0.0f };
	const Vector2D START_DIR = { 0.0f,-1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = GetColor(255, 0, 0);
	const unsigned int ENEMY_MAX = 100;//敵の最大数
	const unsigned int ENEMY_NUM = 10;//最初に出現する敵の数
	Player* player = nullptr;
	//std::vector<Bullet*>bullets;//弾丸保管庫（最初は空）
	//std::vector<Enemy*>enemies;//敵の保管庫
	//std::vector <ExplosionEffect*> effects;//エフェクトの保管庫

	std::vector<Base*> objects;//すべてのオブジェクトの保管庫
	//オブジェクトの保管庫にオブジェクトを追加する関数
	void AddObject(Base* obj)
	{
		objects.push_back(obj);
	}
	void UpdateAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Update();
		}
	}
	void DrawAllObjects()
	{
		for (auto& obj:objects)
		{
			obj->Draw();
		}
	}
}

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	player = new Player(START_POS, START_VEL, START_COLOR,
		START_DIR, START_RADIUS, START_OMEGA);

	AddObject(player);
	

	/*enemies.clear();
	enemies.reserve(ENEMY_NUM);*/

	for (int i = 0;i < ENEMY_NUM;i++)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE,8);
		//enemies.push_back(e);
		AddObject(e);
	}
	
}

void Stage::Update()
{

	//敵の位置と当たり判定の半径
	// 弾の位置
	// isAlive_ ->falseにする手段
	std::vector<Enemy*>aliveEnemies;
	std::vector<Bullet*>aliveBullets;

	aliveEnemies.clear();//念のため枚フレーム空にする
	aliveBullets.clear();//念のため枚フレーム空にする
	/*for (int i = 0;i < objects.size();i++)*/
	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::ENEMY)
		{
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive())
			{
				aliveEnemies.push_back(e);
			}
		}
		else if (obj->GetType() == OBJ_TYPE::BULLET)
		{
			Bullet* b = (Bullet*)obj;
			if (!b->IsDead())
			{
				aliveBullets.push_back(b);
			}

		}
	}
		
	for (auto& bullet : aliveBullets)
	{
		for (auto& enemy : aliveEnemies)
		{
			float dist = Math2D::Length(Math2D::Sub(bullet->GetPos(), enemy->GetPos()));
			if (dist < enemy->GetCollisionRadius())
			{
				enemy->Dead();
				if (enemy->GetSize() != Enemy::Size::SMALL)
				{
					int num = GetRand(3) + 2;
					for (int i = 0;i < num;i++)
					{
						Enemy* e = nullptr;
						if (enemy->GetSize() == Enemy::Size::LARGE)
						{
							e = new Enemy(Enemy::Size::MEDIUM, 8);
						}
						else
						{
							e = new Enemy(Enemy::Size::SMALL, 8);
						}
						e->SetPos(enemy->GetPos());
						e->SetVel({ (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) });
						AddObject(e);
					}
				}
				else
				{
					//小は何もしない
					ExplosionEffect* effect = new ExplosionEffect(enemy->GetPos());
					//effects.push_back(effect);
					AddObject(effect);
				}
				bullet->Dead();//弾を消す
			}
		}
	}
	
	
	
//			if (dist < enemies[i]->GetCollisionRadius())
//			{
//				//当たったら
//				enemies[i]->Dead();//敵けし（生存フラグをfalseに）
//				//分裂処理をここでやる
//				//大か中か小かを判定して
//				//大なら2～4つ、中なら2～4つ、小なら敵を消したい
//				Vector2D enemyPos = enemies[i]->GetPos();
//				Enemy::Size enemySize = enemies[i]->GetSize();
//				if (enemySize == Enemy::Size::SMALL)
//				{
//					//小は何もしない
//					ExplosionEffect* effect = new ExplosionEffect(enemyPos);
//					//effects.push_back(effect);
//					AddObject(effect);
//				}
//				else if (enemySize == Enemy::Size::MEDIUM)
//				{
//					for (int i = 0;i < GetRand(2)+2;i++)
//					{
//						Enemy* e = new Enemy(Enemy::Size::SMALL,8);
//						e->SetPos(enemyPos);
//						e->SetVel({ (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) });
//						//vel_ = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
//						enemies.push_back(e);
//						AddObject(e);
//					}
//
//				}
//				else if (enemySize == Enemy::Size::LARGE)
//				{
//					for (int i = 0;i < GetRand(2)+2;i++)
//					{
//						Enemy* e = new Enemy(Enemy::Size::MEDIUM, 8);
//						e->SetPos(enemyPos);
//						e->SetVel({ (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) });
//						//vel_ = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
//						enemies.push_back(e);
//						AddObject(e);
//					}
//
//				}
//				itr->Dead();//弾けし
//			}
//		}
//	}

	//賞味期限切れの弾を消す
	DeleteBullet();

	//死んでいる敵を消す
	DeleteEnemy();
	UpdateAllObjects();
	
	//Zキーが押されたら弾丸生成
	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		ShootBullet();
	}

}

void Stage::Draw()
{
	DrawAllObjects();
}

void Stage::Release()
{
	/*if (player != nullptr)
	{
		delete player;
	}
	for (int i = 0;i < enemies.size();i++)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
		}
	}*/
}

void Stage::DeleteBullet()
{
	//賞味期限切れの弾を消す
	//箱の中身を確認、死んでいる弾があったらdelete（箱は残るからnullptrにしておく）
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::BULLET)
		{
			//base->継承クラスの時は継承クラスのポインタに変換してあげないと継承クラスのメンバ関数は呼び出せない
			//継承クラス→baseクラスの返還は暗黙的に
			Bullet* b = (Bullet*)(itr);
			if (b->IsDead())
			{
				delete b;
				itr = nullptr;//ポインタをnullptrに
			}
		}
	}
	//次に箱の中身を確認してnullptrがあったら箱から消す（箱自体を詰める）
	for (auto it = objects.begin();it != objects.end();)
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}

	/*for (auto it = bullets.begin(); it != bullets.end();)
	{
		if ((*it)->IsDead() == true)
		{
			it = bullets.erase(it);
		}
		else
		{
			it++;
		}
	}*/
}

void Stage::DeleteEnemy()
{
	//死んでいる敵を消す
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::ENEMY)
		{
			//base->継承クラスの時は継承クラスのポインタに変換してあげないと継承クラスのメンバ関数は呼び出せない
			//継承クラス→baseクラスの返還は暗黙的に
			Enemy* b = (Enemy*)(itr);
			if (!b->IsAlive())
			{
				delete b;
				itr = nullptr;//ポインタをnullptrに
			}
		}
	}
	//次に箱の中身を確認してnullptrがあったら箱から消す（箱自体を詰める）
	for (auto it = objects.begin();it != objects.end();)
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::ShootBullet()
{
	Vector2D pos = player->GetPos();
	Vector2D v = Math2D::Mul(player->GetDirVec(), 300.0f);
	unsigned int bcol = GetColor(255, 255, 255);
	float r = 2;
	float life = 2.0f;

	Bullet* b = new Bullet(pos, v, bcol, r, life);
	//bullets.push_back(b);
	AddObject(b);
}
