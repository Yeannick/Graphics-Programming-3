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
	setting.velocity = { 0.f,0.25f,0.f };
	setting.minSize = 0.1f;
	setting.maxSize = 0.5f;
	setting.minEnergy = 1.f;
	setting.maxEnergy = 3.5f;
	setting.minScale = 0.1f;
	setting.maxScale = 0.5f;
	setting.minEmitterRadius = 0.2f;
	setting.maxEmitterRadius = 2.f;
	setting.color = { 1.f,1.f,1.f,0.5f };

	AddComponent(new ParticleEmitterComponent(m_ParticleFile, setting, 1600));
}

void CrateParticle::Update(const SceneContext& sceneContext)
{
	if (m_IsActive)
	{
		m_TimeCounter += sceneContext.pGameTime->GetElapsed();

		if (m_TimeCounter >= m_ParticleLifeTime)
		{
			m_IsDone = true;
		}
	}
	
}
