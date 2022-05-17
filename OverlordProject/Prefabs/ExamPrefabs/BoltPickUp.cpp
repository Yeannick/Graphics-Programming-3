#include "stdafx.h"
#include "BoltPickUp.h"
#include "Materials/DiffuseAmbient.h"
#include "Prefabs/ExamPrefabs/Ratchet.h"
BoltPickUp::BoltPickUp(XMFLOAT3 position) :
	m_Position(position)
{
	GetTransform()->Scale(0.05f);
	GetTransform()->Translate(m_Position);
}

void BoltPickUp::Initialize(const SceneContext&)
{
	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Bolt.ovm"));
	const auto pBottomMaterial = MaterialManager::Get()->CreateMaterial<DiffuseAmbient>();
	const auto pTopMaterial = MaterialManager::Get()->CreateMaterial<DiffuseAmbient>();

	pBottomMaterial->SetDiffuseTexture(L"Textures/BoltBot.png");
	pTopMaterial->SetDiffuseTexture(L"Textures/BoltTop.png");

	pModel->SetMaterial(pBottomMaterial, 1);
	pModel->SetMaterial(pTopMaterial, 0);

	auto pDefaultMaterial = PxGetPhysics().createMaterial(.5f, .5f, 1.f);

	const auto rigidBody = AddComponent(new RigidBodyComponent(true));
	//const auto pTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Bolt.ovpt");
	rigidBody->AddCollider(PxSphereGeometry{ 0.25f }, *pDefaultMaterial, true);
	

	

	SetOnTriggerCallBack(std::bind(&BoltPickUp::CollectBolt, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void BoltPickUp::Update(const SceneContext& sceneContext)
{
	GetTransform()->Rotate(0.f, 45.f * sceneContext.pGameTime->GetTotal(), 0.f);

	if (IsPickedUp)
		SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
}

void BoltPickUp::CollectBolt(GameObject* , GameObject* pOtherObject, PxTriggerAction action )
{
	if (pOtherObject->GetTag() == L"Player" && action == PxTriggerAction::ENTER)
	{
		auto ratchet = static_cast<Ratchet*>(pOtherObject);
		ratchet->AddBolts();
		IsPickedUp = true;

		//SoundManager::Get()->GetSystem()->playSound(m_pSound, nullptr, false, nullptr);
	}
}
