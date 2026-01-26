#include "Stage.h"
#include "Math2D.h"
#include "Player.h"
#include <DxLib.h>
#include "Globals.h"
#include "Bullet.h"
#include "Input.h"
#include <vector>
#include "Enemy.h"

namespace
{
	//Player Parameters :ƒvƒŒƒCƒ„[‚Ì‰Šú’l
	const Vector2D START_POS = { WIN_WIDTH / 2,WIN_HEIGHT / 2 };
	const Vector2D START_VEL = { 0.0f,0.0f };
	const Vector2D START_DIR = { 0.0f,-1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = GetColor(255, 0, 0);
	const unsigned int ENEMY_MAX = 10;//“G‚ÌÅ‘å”
	Player* player = nullptr;
	std::vector<Bullet*>bullets;//’eŠÛ•ÛŠÇŒÉiÅ‰‚Í‹ój
	std::vector<Enemy*>enemies;
	

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
	for (int i = 0;i < ENEMY_MAX;i++)
	{
		Enemy* e = new Enemy(8);
		enemies.push_back(e);
	}
	
}

void Stage::Update()
{

	//“G‚ÌˆÊ’u‚Æ“–‚½‚è”»’è‚Ì”¼Œa
	// ’e‚ÌˆÊ’u
	// isAlive_ ->false‚É‚·‚éè’i
	
	for (auto& itr : bullets)
	{
		for (int i = 0;i < ENEMY_MAX;i++)
		{
			float dist = Math2D::Length(Math2D::Sub(itr->GetPos(),
				enemies[i]->GetPos()));
			if (dist < enemies[i]->GetCollisionRadius())
			{
				//“–‚½‚Á‚½‚ç
				enemies[i]->Dead();
			}
		}
	}

	

	//Ü–¡ŠúŒÀØ‚ê‚Ì’e‚ğÁ‚·
	DeleteBullet();
	for (int i = 0;i < ENEMY_MAX;i++)
	{
		enemies[i]->Update();
	}
	
	player->Update();
	if (!bullets.empty())
	{
		for (auto& itr : bullets)
		{
			itr->Update();
		}
	}

	//ZƒL[‚ª‰Ÿ‚³‚ê‚½‚ç’eŠÛ¶¬
	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		ShootBullet();
	}

}

void Stage::Draw()
{
	for (int i = 0;i < ENEMY_MAX;i++)
	{
		if (enemies[i]->IsAlive())
		{
			enemies[i]->Draw();//“G‚ğ•`‰æ‚·‚é
		}
	}
	if (!bullets.empty())
	{
		for (auto& itr : bullets)
		{
			itr->Draw();
		}
	}
	player->Draw();
	
}

void Stage::Release()
{
	if (player != nullptr)
	{
		delete player;
	}
	for (int i = 0;i < ENEMY_MAX;i++)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
		}
	}
}

void Stage::DeleteBullet()
{
	//Ü–¡ŠúŒÀØ‚ê‚Ì’e‚ğÁ‚·
	for (auto it = bullets.begin(); it != bullets.end(); it)
	{
		if ((*it)->IsDead() == true)
		{
			it = bullets.erase(it);
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
	bullets.push_back(b);
}
