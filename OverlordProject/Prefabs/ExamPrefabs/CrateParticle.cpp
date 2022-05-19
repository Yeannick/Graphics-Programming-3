#include "stdafx.h"
#include "CrateParticle.h"

CrateParticle::CrateParticle(const XMFLOAT3& position, float lifeTime):
	m_ParticleLifeTime(lifeTime)
{
	GetTransform()->Translate(position);
}

CrateParticle::CrateParticle(const XMFLOAT3& position, const std::wstring& assetfile, float lifeTime):
	m_ParticleLifeTime(lifeTime),
	m_ParticleFile(assetfile)
{
	GetTransform()->Translate(position);
}

void CrateParticle::Initialize(const SceneContext&)
{
	ParticleEmitterSettings setting;
	setting.velocity = { 0.f,0.50f,0.f };
	setting.minSize = 1.f;
	setting.maxSize = 5.f;
	setting.minEnergy = 1.f;
	setting.maxEnergy = 3.5f;
	setting.minScale = 1.0f;
	setting.maxScale = 5.0f;
	setting.minEmitterRadius = 2.f;
	setting.maxEmitterRadius = 20.f;
	setting.color = { 1.f,1.f,1.f,0.5f };

	AddComponent(new ParticleEmitterComponent(m_ParticleFile, setting, 1600));
}

void CrateParticle::Update(const SceneContext& sceneContext)
{
	
		m_TimeCounter += sceneContext.pGameTime->GetElapsed();

		if (m_TimeCounter >= m_ParticleLifeTime)
		{
			m_IsDone = true;
		}
		if (m_IsDone)
		{
			SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
		}
	
}
