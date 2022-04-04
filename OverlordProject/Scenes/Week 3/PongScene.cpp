#include "stdafx.h"
#include "PongScene.h"
#include "Prefabs\CubePrefab.h"
#include "Prefabs\SpherePrefab.h"
PongScene::PongScene(): GameScene(L"PongScene") {}

void PongScene::Initialize()
{
	m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.enableOnGUI = true;

	auto& physx = PxGetPhysics();
	auto pDefaultMaterial = physx.createMaterial(.5f, .5f, 1.f);

	const auto pFixedCamera = new FixedCamera();
	
	pFixedCamera->GetTransform()->Translate(0, 20, -80);
	AddChild(pFixedCamera);

	auto pDefaultCamera = pFixedCamera->GetComponent<CameraComponent>();
	SetActiveCamera(pDefaultCamera);
	

	// Paddle

	m_pRightPaddle = new CubePrefab(3.f, 8.f, 2.f, XMFLOAT4{ Colors::White });
	AddChild(m_pRightPaddle);
	m_pRightPaddle->GetTransform()->Translate(45.f, 20.f, 0.f);
	
	auto rigidBodyComponent = new RigidBodyComponent(true);
	m_pRightPaddle->AddComponent(rigidBodyComponent);
	rigidBodyComponent->AddCollider(PxBoxGeometry(1.5f, 4.f, 2.f), *pDefaultMaterial);
	m_pLeftPaddle = new CubePrefab(3.f, 8.f, 2.f, XMFLOAT4{ Colors::White });
	AddChild(m_pLeftPaddle);
	m_pLeftPaddle->GetTransform()->Translate(-45.f, 20.f, 0.f);

	auto rigidBodyComponentLeft = new RigidBodyComponent(true);
	m_pLeftPaddle->AddComponent(rigidBodyComponentLeft);
	rigidBodyComponentLeft->AddCollider(PxBoxGeometry(1.5f, 4.f, 2.f), *pDefaultMaterial);

	
	//Walls
	m_WallTop = new CubePrefab(120.f, 2, 2, XMFLOAT4{ Colors::White });
	AddChild(m_WallTop);
	m_WallTop->GetTransform()->Translate(0, 53.f, 0.f);

	auto rigidBodyComponentWall = new RigidBodyComponent(true);
	m_WallTop->AddComponent(rigidBodyComponentWall);
	rigidBodyComponentWall->AddCollider(PxBoxGeometry(60.f, 2.f, 2.f), *pDefaultMaterial);


	
	m_WallBot = new CubePrefab(120.f, 2, 2, XMFLOAT4{ Colors::White });
	AddChild(m_WallBot);
	m_WallBot->GetTransform()->Translate(0, -13.f, 0.f);
	auto rigidBodyComponentWallbot = new RigidBodyComponent(true);
	m_WallBot->AddComponent(rigidBodyComponentWallbot);
	rigidBodyComponentWallbot->AddCollider(PxBoxGeometry(60.f, 2.f, 2.f), *pDefaultMaterial);


	// Ball 

	m_pBall = new SpherePrefab(2.f);
	AddChild(m_pBall);
	m_pBall->GetTransform()->Translate(0.f, 20.f, 0.f);
	auto rigidBodyComponentBall = new RigidBodyComponent(false);
	m_pBall->AddComponent(rigidBodyComponentBall);
	rigidBodyComponentBall->AddCollider(PxSphereGeometry(2.f), *pDefaultMaterial);
	rigidBodyComponentBall->GetPxRigidActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_pBall->GetComponent<RigidBodyComponent>()->AddForce({ -25, -25, 0 }, PxForceMode::eIMPULSE);
}

void PongScene::Update()
{
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down,VK_UP))
	{
		//m_pRightPaddle->GetComponent<RigidBodyComponent>()->AddForce({0,15,0});
		m_pRightPaddle->GetComponent<RigidBodyComponent>()->GetTransform()->Translate(m_pRightPaddle->GetTransform()->GetPosition().x, m_pRightPaddle->GetTransform()->GetPosition().y + 0.45f, m_pRightPaddle->GetTransform()->GetPosition().z);

	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_DOWN))
	{
		//m_pRightPaddle->GetComponent<RigidBodyComponent>()->AddForce({0,15,0});
		m_pRightPaddle->GetComponent<RigidBodyComponent>()->GetTransform()->Translate(m_pRightPaddle->GetTransform()->GetPosition().x, m_pRightPaddle->GetTransform()->GetPosition().y - 0.45f, m_pRightPaddle->GetTransform()->GetPosition().z);

	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'W'))
	{
		//m_pRightPaddle->GetComponent<RigidBodyComponent>()->AddForce({0,15,0});
		m_pLeftPaddle->GetComponent<RigidBodyComponent>()->GetTransform()->Translate(m_pLeftPaddle->GetTransform()->GetPosition().x, m_pLeftPaddle->GetTransform()->GetPosition().y + 0.45f, m_pLeftPaddle->GetTransform()->GetPosition().z);

	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'S'))
	{
		//m_pRightPaddle->GetComponent<RigidBodyComponent>()->AddForce({0,15,0});
		m_pLeftPaddle->GetComponent<RigidBodyComponent>()->GetTransform()->Translate(m_pLeftPaddle->GetTransform()->GetPosition().x, m_pLeftPaddle->GetTransform()->GetPosition().y - 0.45f, m_pLeftPaddle->GetTransform()->GetPosition().z);

	}
	
}

void PongScene::Draw()
{
}

void PongScene::OnGUI()
{
}
