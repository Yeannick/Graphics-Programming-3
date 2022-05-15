#include "stdafx.h"
#include "Ratchet.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Prefabs/FixedCamera.h"

Ratchet::Ratchet(const RatchetDesc& ratchetDesc):
	m_CharacterDesc(ratchetDesc),
	m_MoveAcceleration(ratchetDesc.maxMovementSpeed/ ratchetDesc.movementAccelerationTime),
	m_FallAcceleration(ratchetDesc.maxFallingSpeed / ratchetDesc.fallOffAccelerationTime)
{
	
}

void Ratchet::DrawImGui()
{
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
	m_pAnimator->SetAnimation(5);
	m_TotalVelocity.y = m_CharacterDesc.jumpSpeed * sceneContext.pGameTime->GetElapsed();
	m_JumpCounter++;
	m_IsGrounded = false;
}

void Ratchet::DoubleJump(const SceneContext& sceneContext)
{
	m_pAnimator->SetAnimation(2);
	m_TotalVelocity.y = m_CharacterDesc.jumpSpeed * sceneContext.pGameTime->GetElapsed();
	m_JumpCounter++;
	m_IsGrounded = false;
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

	m_pCameraComponent->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * 0.75f, -8.f);
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


}

void Ratchet::Update(const SceneContext& sceneContext)
{
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

		if (sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_Attack)) m_IsAttacking = true;

		TransformComponent* transformComponent = GetTransform();
		//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent
		XMVECTOR forward = XMLoadFloat3(&transformComponent->GetForward());
		XMVECTOR right = XMLoadFloat3(&transformComponent->GetRight());

		m_TotalYaw += look.x * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
		//m_pCameraComponent->GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
		GetTransform()->Rotate(0, m_TotalYaw, 0);

		
		if (move.x != 0 || move.y != 0)
		{
			m_pAnimator->SetAnimation(4);
			
			XMVECTOR newDirection = (forward * move.y) + (right * move.x);


			XMStoreFloat3(&m_CurrentDirection, newDirection);
			m_MoveSpeed += m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();

			MathHelper::Clamp(m_MoveSpeed, m_CharacterDesc.maxMovementSpeed, 0.f);

			auto angle = XMVector3AngleBetweenVectors(-XMLoadFloat3(&GetTransform()->GetForward()), XMVector3Normalize(newDirection));

			if (move.x > 0)
				angle *= -1;

			
			XMStoreFloat(&m_CurrentAngle, XMVectorLerp(XMLoadFloat(&m_CurrentAngle), angle, sceneContext.pGameTime->GetElapsed() * m_CharacterDesc.rotationSpeed));

			m_pModelObject->GetTransform()->Rotate(0, m_CurrentAngle, 0, false);
		}
		else
		{
			
			m_MoveSpeed -= m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();

			
			MathHelper::Clamp(m_MoveSpeed, m_CharacterDesc.maxMovementSpeed, 0.f);
			
		}
		if (m_MoveSpeed <= 0.f)
		{
			//m_pAnimator->SetAnimation(0);

		}
		XMVECTOR newVelocity = XMLoadFloat3(&m_CurrentDirection);
		XMFLOAT3 velocity{};
		XMStoreFloat3(&velocity, newVelocity * m_MoveSpeed * sceneContext.pGameTime->GetElapsed());

		
		m_TotalVelocity.x = velocity.x;
		m_TotalVelocity.z = velocity.z;

		
		m_IsGrounded = IsGrounded(); 
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
		else if (!m_IsGrounded && m_JumpCounter == 0 && sceneContext.pInput->IsActionTriggered((int)m_CharacterDesc.action_id_Jump))
		{
			//DoubleJump(sceneContext);
		}
		
		else
		{
			//m_TotalVelocity.y is zero
			//m_TotalVelocity.y = 0;
		}

		m_pControllerComponent->Move(m_TotalVelocity);
	}
}

void Ratchet::Draw(const SceneContext& /*sceneContext*/)
{
	//TextRenderer::Get()->DrawText(m_pFont, std::to_wstring(m_Bolts), XMFLOAT2{ 50.f, 10.f }, XMFLOAT4{ Colors::Orange });
	//TextRenderer::Get()->DrawText(m_pFont, std::to_wstring(m_PlayerLives), XMFLOAT2{ sceneContext.windowWidth - 100.f, 10.f }, XMFLOAT4{ Colors::Orange });
}

void Ratchet::Attack(const SceneContext& /*sceneContext*/)
{
	m_pAnimator->SetAnimation(L"Attack");
}

bool Ratchet::IsGrounded()
{
	PxVec3 rayStart = PxVec3{ m_pControllerComponent->GetFootPosition().x, m_pControllerComponent->GetFootPosition().y + .1f ,m_pControllerComponent->GetFootPosition().z };
	PxVec3 rayDir{ 0, -1, 0 };
	float distance{ m_CharacterDesc.controller.stepOffset + m_CharacterDesc.controller.stepOffset };

	PxQueryFilterData filterData{};
	filterData.data.word0 = ~UINT(CollisionGroup::None);
	PxRaycastBuffer hit{};

	DebugRenderer::DrawLine(m_pControllerComponent->GetFootPosition(), XMFLOAT3{ rayStart.x + 0, rayStart.y - 1 * distance, rayStart.z + 0 }, XMFLOAT4{ Colors::Red });
	m_JumpCounter = 0;
	return GetScene()->GetPhysxProxy()->Raycast(rayStart, rayDir, distance, hit);
}
