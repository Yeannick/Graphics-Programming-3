#pragma once
class Ratchet;
class CrateParticle;
enum class CrateType
{
	BoltCrate,
	NanoTechCrate,
	ExplosiveCrate
};
class Crate : public GameObject
{
public:


	Crate(const XMFLOAT3& Position, CrateType type = CrateType::BoltCrate);
	~Crate() override;

	Crate(const Crate& other) = delete;
	Crate(Crate&& other) noexcept = delete;
	Crate& operator=(const Crate& other) = delete;
	Crate& operator=(Crate&& other) noexcept = delete;

	void Destroy();
	bool IsHit() { return m_Hit; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private: 
	void OnBoxHit(GameObject* pTriggerObj, GameObject* pOtherObject, PxTriggerAction action);

	ModelComponent* m_pModel;
	XMFLOAT3 m_Position;
	CrateType m_Type = CrateType::BoltCrate;
	ParticleEmitterComponent* m_pEmmiter;
	FMOD::Sound* m_pCrateBreakSound{};
	bool m_Hit = false ;
	CrateParticle* m_ParticleEffect;
};

