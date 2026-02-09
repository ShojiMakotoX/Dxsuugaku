#include "ExplosionEffect.h"
#include "DxLib.h"

ExplosionEffect::ExplosionEffect(const Vector2D& pos, int particleCount)
	:Base(pos,{0.0f,0.0f},GetColor(255,255,255))
{
	particles_.resize(particleCount);
	isFinished_ = false;

	//パーティクル1個の初期化
	Particle particle;
	particle.Offset = { 0.0f,0.0f };
	particle.vel = { 0.0f,0.0f };
	particle.life = 3.0f;//寿命3秒
	particle.radius = 5.0f;//半径5.0f

	//リストに追加
	//particles_[0] = particle;
	particles_.push_back(particle);




}

void ExplosionEffect::Update()
{
	float dt = GetDeltaTime();
	bool allDead = true;

	for (auto& particle : particles_)
	{
		//寿命が残っていたら更新を行う
		if (particle.life > 0.0f)
		{
			allDead = false;//まだ生きているパーティクルあり
			
			//寿命減少
			particle.life -= dt;
			if (particle.life < 0.0f)
			{
				particle.life = 0.0f;
			}
		}
	}
	//全パーティクルが寿命切れならエフェクト終了
	if (allDead)//allDead == true と同じ
	{
		isFinished_ = true;
	}
}

void ExplosionEffect::Draw()
{
	if (isFinished_)
	{
		return;//終了していたら描画しない
	}

	for (auto& particle : particles_)
	{
		if (particle.life > 0.0f)
		{
			//パーティクル描画
			Vector2D drawPos = Math2D::Add(GetPos(), particle.Offset);
			Vector2D screenPos = Math2D::World2Screen(drawPos);

			DrawCircle((int)screenPos.x, (int)screenPos.y,
				particle.radius, GetColor(255, 0, 0));
		}
	}
}

