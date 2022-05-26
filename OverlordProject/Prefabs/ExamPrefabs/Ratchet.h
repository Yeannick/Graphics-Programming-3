#pragma once
#include "Prefabs/Character.h"

struct RatchetDesc
{
	RatchetDesc(PxMaterial* pMaterial, float radius = 0.5f, float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
		
	}

	float maxMovementSpeed = 40.f ;
	float maxFallingSpeed = 20.f ;

	float jumpSpeed = 40.f;

	float movementAccelerationTime = 0.3f;
	float fallOffAccelerationTime = 0.3f;

	PxCapsuleControllerDesc controller;

	float rotationSpeed = 60.f;
	
	float attackTime = 1.5f;

	int action_id_MoveLeft = -1;
	int action_id_MoveRight = -1;
	int action_id_MoveForward = -1;
	int action_id_MoveBackward = -1;
	int action_id_Jump = -1; 
	int action_id_Attack = -1;
};
enum AnimationState
{
	Idle,
	Flip,
	Attacking,
	Jumping,
	Running
};
class Ratchet : public GameObject
{
public:
	Ratchet(const RatchetDesc& ratchetDesc);
	~Ratchet() override = default;

	Ratchet(const Ratchet& other) = delete;
	Ratchet(Ratchet&& other) noexcept = delete;
	Ratchet& operator=(const Ratchet& other) = delete;
	Ratchet& operator=(Ratchet&& other) noexcept = delete;

	void DrawImGui();

	void Animation();
	void AddBolts();
	void AddLife();
	void PlayerHit();
	void Jump(const SceneContext& sceneContext);
	void DoubleJump(const SceneContext& sceneContext);
	void OnTriggerCallBack(GameObject* pThis, GameObject* pOtherObject, PxTriggerAction action);
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void Draw(const SceneContext&) override;

private:

	void Attack(const SceneContext& sceneContext);
	
	bool IsGrounded();

	int m_Bolts;

	bool m_IsGrounded = false;
	bool m_IsActive = true;
	bool m_IsAttacking = false;

	int m_JumpCounter = 0;
	float m_AttackTimer = 0.f;
	float m_AttackDuration = 1.0f;

	int m_PlayerLives = 5;
	
	XMFLOAT3 m_SpawnPosition;

	CameraComponent* m_pCameraComponent;
	ControllerComponent* m_pControllerComponent;
	ModelAnimator* m_pAnimator;

	AnimationState m_State;

	SpriteFont* m_pFont;
	GameObject* m_Sprite;
	GameObject* m_pModelObject;

	FMOD::Sound* m_pAttackSound;

	RatchetDesc m_CharacterDesc;
	float m_TotalYaw, m_TotalPitch;
	float m_MoveAcceleration;
	float m_FallAcceleration;
	float m_MoveVelocity;
	float m_MoveSpeed;
	float m_CurrentAngle = 0.f;

	XMFLOAT3 m_TotalVelocity;
	XMFLOAT3 m_CurrentDirection;

	float m_HitRange = 1.5f;
	
	GameObject* m_pRatchetAttack;

	bool IsInCollider = false;
	GameObject* m_pCollisionObject = nullptr;
	
	
};

