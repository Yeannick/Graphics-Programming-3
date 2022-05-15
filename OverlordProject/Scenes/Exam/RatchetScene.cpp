#include "stdafx.h"
#include "RatchetScene.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/ColorMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

#include "Prefabs/Character.h"
#include "Prefabs\ExamPrefabs\Ratchet.h"


void RatchetScene::Initialize()
{
	// Materials

	const auto pGrassMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pGrassMaterial->SetDiffuseTexture(L"Textures/926.png");

	const auto pDirtMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pDirtMaterial->SetDiffuseTexture(L"Textures/225.png");

	const auto pTerrainMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pTerrainMaterial->SetDiffuseTexture(L"Textures/279.png");

	const auto pOuterMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pOuterMaterial->SetDiffuseTexture(L"Textures/235.png");

	const auto pWallMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); 
	pWallMaterial->SetDiffuseTexture(L"Textures/238.png");

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	// Level Models

	const auto pGrassObj = new GameObject();
	const auto pGrassModel = new ModelComponent(L"Meshes/Palace_Grass.ovm");
	pGrassModel->SetMaterial(pGrassMaterial, 0);

	pGrassObj->AddComponent(pGrassModel);
	pGrassObj->GetTransform()->Rotate(90.0f, 0.0f, 0.0f);

	const auto pGrassActor = pGrassObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Palace_Grass.ovpt");
	pGrassActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);

	AddChild(pGrassObj);

	const auto pTerrainObj = new GameObject();
	const auto pTerrainModel = new ModelComponent(L"Meshes/Palace_Terrain.ovm");
	pTerrainModel->SetMaterial(pTerrainMaterial, 0);

	pTerrainObj->AddComponent(pTerrainModel);
	pTerrainObj->GetTransform()->Rotate(90.0f, 0.0f, 0.0f);

	const auto pTerrainActor = pTerrainObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMeshTerrain = ContentManager::Load<PxTriangleMesh>(L"Meshes/Palace_Terrain.ovpt");
	pTerrainActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMeshTerrain, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);

	AddChild(pTerrainObj);

	const auto pDirtObj = new GameObject();
	const auto pDirtModel = new ModelComponent(L"Meshes/Palace_Dirt.ovm");
	pDirtModel->SetMaterial(pDirtMaterial, 0);

	pDirtObj->AddComponent(pDirtModel);
	pDirtObj->GetTransform()->Rotate(90.0f, 0.0f, 0.0f);

	const auto pDirtActor = pDirtObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMeshDirt = ContentManager::Load<PxTriangleMesh>(L"Meshes/Palace_Dirt.ovpt");
	pDirtActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMeshDirt, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);

	AddChild(pDirtObj);

	const auto pBuildObj = new GameObject();
	const auto pBuildModel = new ModelComponent(L"Meshes/Palace_Build.ovm");
	pBuildModel->SetMaterial(pWallMaterial, 0);

	pBuildObj->AddComponent(pBuildModel);
	pBuildObj->GetTransform()->Rotate(90.0f, 0.0f, 0.0f);

	const auto pBuildActor = pBuildObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMeshBuild = ContentManager::Load<PxTriangleMesh>(L"Meshes/Palace_Build.ovpt");
	pBuildActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMeshBuild, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);

	AddChild(pBuildObj);


	const auto pOuterObj = new GameObject();
	const auto pOuterModel = new ModelComponent(L"Meshes/Palace_Outer.ovm");
	pOuterModel->SetMaterial(pOuterMaterial, 0);

	pOuterObj->AddComponent(pOuterModel);
	pOuterObj->GetTransform()->Rotate(90.0f, 0.0f, 0.0f);

	const auto pOuterActor = pOuterObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMeshOuter = ContentManager::Load<PxTriangleMesh>(L"Meshes/Palace_Outer.ovpt");
	pOuterActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMeshOuter, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);

	AddChild(pOuterObj);


	const auto pRingObj = new GameObject();
	const auto pRingModel = new ModelComponent(L"Meshes/Palace_Ring.ovm");
	pRingModel->SetMaterial(pWallMaterial, 0);

	pRingObj->AddComponent(pRingModel);
	pRingObj->GetTransform()->Rotate(90.0f, 0.0f, 0.0f);

	const auto pRingActor = pRingObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMeshRing = ContentManager::Load<PxTriangleMesh>(L"Meshes/Palace_Ring.ovpt");
	pRingActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMeshRing, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);

	AddChild(pRingObj);


	// Player

	RatchetDesc characterDesc{ pDefaultMaterial };
	characterDesc.action_id_MoveForward = CharacterMoveForward;
	characterDesc.action_id_MoveBackward = CharacterMoveBackward;
	characterDesc.action_id_MoveLeft = CharacterMoveLeft;
	characterDesc.action_id_MoveRight = CharacterMoveRight;
	characterDesc.action_id_Jump = CharacterJump;
	characterDesc.action_id_Attack = CharacterAttack;

	m_pCharacter = AddChild(new Ratchet(characterDesc));
	m_pCharacter->GetTransform()->Translate(0.f, 8.f, 0.f);
	//m_pCharacter->GetTransform()->Scale(0.25f);
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

}
