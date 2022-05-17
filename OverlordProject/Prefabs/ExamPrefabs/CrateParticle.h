#pragma once
class CrateParticle : public GameObject
{
public:
	CrateParticle(const XMFLOAT3& position, float lifeTime = 2.f);
	CrateParticle(const XMFLOAT3& position,const std::wstring& assetfile ,float lifeTime = 2.f);
	~CrateParticle() override = default;

	CrateParticle(const CrateParticle& other) = delete;
	CrateParticle(CrateParticle&& other) noexcept = delete;
	CrateParticle& operator=(const CrateParticle& other) = delete;
	CrateParticle& operator=(CrateParticle&& other) noexcept = delete;

	void SetActive() { m_IsActive = true; }
	bool IsFinished() { return m_IsDone; }
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	float m_TimeCounter = 0.f;
	float m_ParticleLifeTime = 2.f;
	std::wstring m_ParticleFile = L"Textures/Smoke.png";
	bool m_IsActive = false;
	bool m_IsDone = false;
};

