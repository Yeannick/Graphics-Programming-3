#pragma once
class BoltPickUp : public GameObject
{
public:
	BoltPickUp(XMFLOAT3 position);
	~BoltPickUp() override = default;

	BoltPickUp(const BoltPickUp& other) = delete;
	BoltPickUp(BoltPickUp&& other) noexcept = delete;
	BoltPickUp& operator=(const BoltPickUp& other) = delete;
	BoltPickUp& operator=(BoltPickUp&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	XMFLOAT3 m_Position;
	bool IsPickedUp = false ;

	FMOD::Sound* m_pSound{ nullptr };
	RigidBodyComponent* m_RigidBodyTrigger;
	RigidBodyComponent* m_RigidBody;
	void CollectBolt(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
};

