#include "stdafx.h"
#include "RatchetScene.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/ColorMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseAmbient.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

#include "Prefabs/Character.h"
#include "Prefabs\ExamPrefabs\Ratchet.h"
#include "Prefabs/ExamPrefabs/Crate.h"
#include "Prefabs/ExamPrefabs/CrateParticle.h"
#include "Prefabs/ExamPrefabs/BoltPickUp.h"


void RatchetScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.pLights->SetDirectionalLight({ -20.f,30.f,-5.f }, { 0.305f, -0.307f, 0.901f });
	// Materials

	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pGroundMaterial->SetDiffuseTexture(L"Textures/Lava/Ground.png");


	const auto pLavaMaterial = MaterialManager::Get()->CreateMaterial<DiffuseAmbient>();
	pLavaMaterial->SetDiffuseTexture(L"Textures/Lava/Lava.png");


	const auto pRock1Material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pRock1Material->SetDiffuseTexture(L"Textures/Lava/Rock1.png");

	const auto pRock2Material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pRock2Material->SetDiffuseTexture(L"Textures/Lava/Rock2.png");
	const auto pRock3Material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pRock3Material->SetDiffuseTexture(L"Textures/Lava/Rock3.png");
	const auto pRock4Material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pRock4Material->SetDiffuseTexture(L"Textures/Lava/Rock4.png");
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	// Level Models

	const auto pIsland = new GameObject();
	const auto pIslandModel = new ModelComponent(L"Meshes/Lava.ovm");
	pIslandModel->SetMaterial(pGroundMaterial,0);
	pIslandModel->SetMaterial(pLavaMaterial,1);
	pIslandModel->SetMaterial(pRock1Material,4);
	pIslandModel->SetMaterial(pRock2Material,5);
	pIslandModel->SetMaterial(pRock3Material,3);
	pIslandModel->SetMaterial(pRock4Material,2);
	pIslandModel->SetMaterial(pLavaMaterial,6);

	const auto pIslandActor = pIsland->AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMeshIsland = ContentManager::Load<PxTriangleMesh>(L"Meshes/Lava.ovpt");
	pIslandActor->AddCollider(PxTriangleMeshGeometry(pTriangleMeshIsland, PxMeshScale({ 10.f,10.f,10.f })), *pDefaultMaterial);

	pIsland->AddComponent(pIslandModel);
	//pIsland->GetTransform()->Rotate(, 0, 0);
	pIsland->GetTransform()->Scale(10.f);
	AddChild(pIsland);
	// Crates

	AddChild(new Crate(XMFLOAT3{ 10,20,0 }, CrateType::ExplosiveCrate));
	//AddChild(new BoltPickUp(XMFLOAT3{ 5,10,0 }));
	//m_pCrates.push_back(AddChild(new Crate(XMFLOAT3{ 0,5,0 }, CrateType::BoltCrate)));
	//AddChild(new CrateParticle(XMFLOAT3{ 0,8,0 }, 2.0f));
	
	 //Player

	RatchetDesc characterDesc{ pDefaultMaterial,2.5f,10.f };
	characterDesc.action_id_MoveForward = CharacterMoveForward;
	characterDesc.action_id_MoveBackward = CharacterMoveBackward;
	characterDesc.action_id_MoveLeft = CharacterMoveLeft;
	characterDesc.action_id_MoveRight = CharacterMoveRight;
	characterDesc.action_id_Jump = CharacterJump;
	characterDesc.action_id_Attack = CharacterAttack;

	m_pCharacter = AddChild(new Ratchet(characterDesc));
	m_pCharacter->GetTransform()->Translate(0.f, 50.f, 0.f);
	//m_pCharacter->GetTransform()->Scale(.f);
	//m_pCharacter->GetTransform()->Rotate(0.f,0.f,90.f);

	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterAttack, InputState::pressed, 'Q');
	m_SceneContext.pInput->AddInputAction(inputAction);
	
	
}

void RatchetScene::Update()
{
	
		//const auto pCameraTransform = m_SceneContext.pCamera->GetTransform();
		//m_SceneContext.pLights->SetDirectionalLight({ -200.f,100.f,-750.f }, pCameraTransform->GetForward());
		
}

void RatchetScene::PostDraw()
{
	/*if (m_DrawShadowMap) {

		ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { m_ShadowMapScale, m_ShadowMapScale }, { 1.f,0.f });
	}*/
}

void RatchetScene::OnGUI()
{
	/*ImGui::Checkbox("Draw ShadowMap", &m_DrawShadowMap);
	ImGui::SliderFloat("ShadowMap Scale", &m_ShadowMapScale, 0.f, 1.f);
	MaterialManager::Get()->GetMaterial(1)->DrawImGui();*/
}
