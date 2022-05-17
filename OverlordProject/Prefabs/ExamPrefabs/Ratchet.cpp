#include "stdafx.h"
#include "Ratchet.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Prefabs/FixedCamera.h"
#include "Prefabs/ExamPrefabs/Crate.h"

Ratchet::Ratchet(const RatchetDesc& ratchetDesc):
	m_CharacterDesc(ratchetDesc),
	m_MoveAcceleration(ratchetDesc.maxMovementSpeed/ ratchetDesc.movementAccelerationTime),
	m_FallAcceleration(ratchetDesc.maxFallingSpeed / ratchetDesc.fallOffAccelerationTime)
{
	
}

void Ratchet::DrawImGui()
{
}

void Ratchet::Animation()
{
	switch (m_State)
	{
	case Idle:
		m_pAnimator->SetAnimation(0);
		break;
	case Flip:
		m_pAnimator->SetAnimation(1);

		break;
	case Attacking:
		m_pAnimator->SetAnimation(2);
		
		break;
	case Jumping:
		m_pAnimator->SetAnimation(3);

		break;
	case Running:
		m_pAnimator->SetAnimation(4);

		break;
	default:
		break;
	}
}

void Ratchet::AddBolts()
{
	m_Bolts++;
}

void Ratchet::AddLife()
{
	if (m_PlayerLives < 5.f)
	{
		m_PlayerLives++;
	}

}

void Ratchet::PlayerHit()
{
	m_PlayerLives--;
	if (m_PlayerLives < 0)
	{
		Logger::LogInfo(L"Player DIED ");
	}
	GetTransform()->Translate(m_SpawnPosition);
	m_TotalVelocity.y = 0.f;
}

void Ratchet::Jump(const SceneContext& sceneContext)
{
	if (!m_IsAttacking)
	{
		m_State = AnimationState::Jumping;
		m_TotalVelocity.y = m_CharacterDesc.jumpSpeed * sceneContext.pGameTime->GetElapsed();
		m_JumpCounter++;
		m_IsGrounded = false;
	}
	
}

void Ratchet::DoubleJump(const SceneContext& sceneContext)
{
	if (!m_IsAttacking)
	{
		m_pAnimator->SetAnimation(2);
		m_TotalVelocity.y = m_CharacterDesc.jumpSpeed * sceneContext.pGameTime->GetElapsed();
		m_JumpCounter++;
		m_IsGrounded = false;
	}
	
}

void Ratchet::Initialize(const SceneContext&)
{
	SetTag(L"Player");
	m_CharacterDesc.controller.stepOffset = 0.1f;
	//m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/RatchetNClank.otf");
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));
	//SoundManager::Get()->GetSystem()->createStream("Resources/Audio/Attack.wav", FMOD_DEFAULT, nullptr, &m_pAttackSound);
	const auto pCamera = AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera

	m_pCameraComponent->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * 1.5f, -8.f);
	m_pCameraComponent->GetTransform()->Rotate(15.f, 0.f, 0.f , true);
	//m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	// Model 

	const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Ratchet/Texture0.png");
	const auto pMaterial1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial1->SetDiffuseTexture(L"Textures/Ratchet/Texture1.png");
	const auto pMaterial2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial2->SetDiffuseTexture(L"Textures/Ratchet/Texture2.png");
	const auto pMaterial3 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial3->SetDiffuseTexture(L"Textures/Ratchet/Texture3.png");

	m_pModelObject = AddChild(new GameObject());
	auto model = m_pModelObject->AddComponent(new ModelComponent(L"Meshes/Ratchet.ovm"));
	model->SetMaterial(pMaterial,0);
	model->GetTransform()->Rotate(0.f, 180.f, 0.f);
	model->GetTransform()->Scale(0.05f);
	model->GetTransform()->Translate(0.f,-1.25f,0.f);
	//model->SetMaterial(pMaterial1,1);
	//model->SetMaterial(pMaterial2,2);
	//model->SetMaterial(pMaterial3,3);

	m_pAnimator = model->GetAnimator();
	m_pAnimator->SetAnimation(0);
	m_pAnimator->Play();

	m_State = AnimationState::Idle;
}

void Ratchet::Update(const SceneContext& sceneContext)
{
	Animation();
	if (m_pCameraComponent->IsActive())
	{
		if (m_IsAttacking)
		{
			Attack(sceneContext);
		}

		// input Movement
		XMFLOAT2 move{};

		if (sceneContext.pInput->IsActionTriggered((int) m_CharacterDesc.action_id_MoveForward))
		{
			move.y = 1;
		}
		if (sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_MoveBackward))
		{
			move.y = -1;
		}

		if (sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_MoveRight))
		{
			move.x = 1;
		}
		if (sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_MoveLeft))
		{
			move.x = -1;
		}
		// Input look

		XMFLOAT2 look = { 0.f,0.f };

		if (sceneContext.pInput->IsMouseButton(InputState::down, VK_LBUTTON))
		{
			look.x = (float)InputManager::GetMouseMovement().x;
			look.y = (float)InputManager::GetMouseMovement().y;
		}
		if (!m_IsAttacking)
		{
			if (sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_Attack)) m_IsAttacking = true;
		}
		

		TransformComponent* transformComponent = GetTransform();
		//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent
		XMVECTOR forward = XMLoadFloat3(&transformComponent->GetForward());
		XMVECTOR right = XMLoadFloat3(&transformComponent->GetRight());

		m_TotalYaw += look.x * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
		//m_pCameraComponent->GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
		GetTransform()->Rotate(0, m_TotalYaw, 0);

		m_IsGrounded = IsGrounded();
		
		if (move.x != 0 || move.y != 0)
		{
			if (m_IsGrounded && !m_IsAttacking)
			{
				m_State = AnimationState::Running;
			}
			XMVECTOR newDirection = (forward * move.y) + (right * move.x);


			XMStoreFloat3(&m_CurrentDirection, newDirection);
			if (!m_IsAttacking)
			{
				m_MoveSpeed += m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();
			}

			MathHelper::Clamp(m_MoveSpeed, m_CharacterDesc.maxMovementSpeed, 0.f);

			auto angle = XMVector3AngleBetweenVectors(-XMLoadFloat3(&GetTransform()->GetForward()), XMVector3Normalize(newDirection));

			if (move.x > 0)
				angle *= -1;

			
			XMStoreFloat(&m_CurrentAngle, XMVectorLerp(XMLoadFloat(&m_CurrentAngle), angle, sceneContext.pGameTime->GetElapsed() * m_CharacterDesc.rotationSpeed));
			if (!m_IsAttacking)
			{
				m_pModelObject->GetTransform()->Rotate(0, m_CurrentAngle, 0, false);
			}
			
		}
		else
		{
			
			m_MoveSpeed -= m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();

			
			MathHelper::Clamp(m_MoveSpeed, m_CharacterDesc.maxMovementSpeed, 0.f);
			
		}
		if (m_MoveSpeed <= 0.f && !m_IsAttacking && m_IsGrounded)
		{
			//m_pAnimator->SetAnimation(0);
			m_State = AnimationState::Idle;

		}
		XMVECTOR newVelocity = XMLoadFloat3(&m_CurrentDirection);
		XMFLOAT3 velocity{};
		XMStoreFloat3(&velocity, newVelocity * m_MoveSpeed * sceneContext.pGameTime->GetElapsed());

		
		m_TotalVelocity.x = velocity.x;
		m_TotalVelocity.z = velocity.z;

		
		
		if (!m_IsGrounded )
		{
			m_TotalVelocity.y -= m_FallAcceleration * (sceneContext.pGameTime->GetElapsed() * sceneContext.pGameTime->GetElapsed());
			if (m_TotalVelocity.y < -m_CharacterDesc.maxFallingSpeed)
			{
				m_TotalVelocity.y = -m_CharacterDesc.maxFallingSpeed;
			}

			
		}
		else if (sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_Jump))
		{

			Jump(sceneContext);
		}
		else if (!m_IsGrounded && m_JumpCounter == 1 && sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_Jump))
		{
			DoubleJump(sceneContext);
		}
		

		m_pControllerComponent->Move(m_TotalVelocity);
	}
}

void Ratchet::Draw(const SceneContext& /*sceneContext*/)
{
	//TextRenderer::Get()->DrawText(m_pFont, std::to_wstring(m_Bolts), XMFLOAT2{ 50.f, 10.f }, XMFLOAT4{ Colors::Orange });
	//TextRenderer::Get()->DrawText(m_pFont, std::to_wstring(m_PlayerLives), XMFLOAT2{ sceneContext.windowWidth - 100.f, 10.f }, XMFLOAT4{ Colors::Orange });
}

void Ratchet::Attack(const SceneContext& sceneContext)
{
	m_State = AnimationState::Attacking;

	m_AttackTimer += sceneContext.pGameTime->GetElapsed();

	if (m_AttackTimer >= m_AttackDuration)
	{
		m_IsAttacking = false;
		m_AttackTimer = 0.f;
	}

	int AmountOfRays = 10;
	XMVECTOR forward = XMLoadFloat3(&GetTransform()->GetForward());
	XMMATRIX rotationMatrix{};
	XMVECTOR rayPosition{};
	XMVECTOR rayDirection{};
	XMVECTOR position = XMLoadFloat3(&GetTransform()->GetPosition());

	rayPosition = position + forward * m_CharacterDesc.controller.radius;
	PxVec3 rayStart(rayPosition.m128_f32[0], rayPosition.m128_f32[1], rayPosition.m128_f32[2]);
	PxVec3 raydir(forward.m128_f32[0], forward.m128_f32[1], forward.m128_f32[2]);

	PxRaycastBuffer hit{};
	PxQueryFilterData filterData{};

	XMFLOAT3 debugPosition{};
	XMFLOAT3 debugDirection{};

	for (int i = 0; i < AmountOfRays; ++i)
	{
		rotationMatrix = XMMatrixRotationY(i * 18.f);
		XMVECTOR dir = XMLoadFloat3(&m_pModelObject->GetTransform()->GetForward());
		XMVector3Normalize(dir);
		rayDirection = XMVector3TransformNormal(dir, rotationMatrix);
		rayPosition = position + rayDirection * m_CharacterDesc.controller.radius;

		if (i % 2 == 0)
		{
			XMVECTOR up = XMLoadFloat3(&GetTransform()->GetUp());

			rayPosition += -up * 0.5f;
			;
		}

		rayStart = PxVec3(rayPosition.m128_f32[0], rayPosition.m128_f32[1], rayPosition.m128_f32[2]);
		raydir = PxVec3{ rayDirection.m128_f32[0],rayDirection.m128_f32[1] ,rayDirection.m128_f32[2] };


		if (GetScene()->GetPhysxProxy()->Raycast(rayStart, raydir, m_HitRange, hit, PxHitFlag::eDEFAULT, filterData))
		{
			XMStoreFloat3(&debugPosition, rayPosition);
			XMStoreFloat3(&debugDirection, rayDirection);
			DebugRenderer::DrawLine(debugPosition, debugDirection, XMFLOAT4{ Colors::Blue });

			auto actor = static_cast<RigidBodyComponent*>(hit.block.actor->userData);
			Logger::LogDebug(L"Hit object {}", actor->GetGameObject()->GetTag());
			GameObject* pGameobject = actor->GetGameObject();
			if (pGameobject->GetTag() == L"Crate")
			{
				if (pGameobject->GetParent() != nullptr)
					pGameobject = pGameobject->GetParent();

				Crate* pCrate = static_cast<Crate*>(pGameobject);
				if (!pCrate->IsHit())
				{
					pCrate->Destroy();
				}
				
				return;
			}
		}
	}
}

bool Ratchet::IsGrounded()
{
	PxVec3 rayStart = PxVec3{ m_pControllerComponent->GetFootPosition().x, m_pControllerComponent->GetFootPosition().y + .1f ,m_pControllerComponent->GetFootPosition().z };
	PxVec3 rayDir{ 0, -1, 0 };
	float distance{ 2*m_CharacterDesc.controller.stepOffset + m_CharacterDesc.controller.stepOffset };

	PxQueryFilterData filterData{};
	filterData.data.word0 = ~UINT(CollisionGroup::None);
	PxRaycastBuffer hit{};

	DebugRenderer::DrawLine(m_pControllerComponent->GetFootPosition(), XMFLOAT3{ rayStart.x + 0, rayStart.y - 1 * distance, rayStart.z + 0 }, XMFLOAT4{ Colors::Red });
	m_JumpCounter = 0;
	return GetScene()->GetPhysxProxy()->Raycast(rayStart, rayDir, distance, hit);
}
