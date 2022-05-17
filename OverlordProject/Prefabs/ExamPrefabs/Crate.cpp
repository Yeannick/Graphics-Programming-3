#include "stdafx.h"
#include "Crate.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/ExamPrefabs/CrateParticle.h"
#include "Prefabs/ExamPrefabs/BoltPickUp.h"

Crate::Crate(const XMFLOAT3& Position, CrateType type):
	m_Position(Position),
	m_Type(type)
{
	GetTransform()->Scale(0.1f);
	
}

Crate::~Crate()
{
}

void Crate::Destroy()
{
	m_Hit = true;
	auto pos = GetTransform()->GetPosition();
	m_ParticleEffect = AddChild(new CrateParticle({pos.x , pos.y + 1.5f , pos.z}, 1.0f));
	m_ParticleEffect->SetActive();
	RemoveComponent(m_pModel);
	SceneManager::Get()->GetActiveScene()->AddChild(new BoltPickUp(GetTransform()->GetPosition()));
}

void Crate::Initialize(const SceneContext&)
{
	GetTransform()->Translate(m_Position);
	SetTag(L"Crate");

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	auto pGlassMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	const auto pActor = AddComponent(new RigidBodyComponent(true));
	const auto pTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Crate.ovpt");
	pActor->AddCollider(PxTriangleMeshGeometry(pTriangleMesh, PxMeshScale({ 0.1f,0.1f,0.1f })), *pDefaultMaterial);
	switch (m_Type)
	{
	case CrateType::BoltCrate:
		m_pModel = AddComponent(new ModelComponent(L"Meshes/Crate.ovm"));
		pMaterial->SetDiffuseTexture(L"Textures/Crate.png");
		m_pModel->SetMaterial(pMaterial);
		break;
	case CrateType::NanoTechCrate:
		m_pModel = AddComponent(new ModelComponent(L"Meshes/Crate.ovm"));
		pMaterial->SetDiffuseTexture(L"Textures/CrateNano.png");
		m_pModel->SetMaterial(pMaterial,0);
		pGlassMaterial->SetDiffuseTexture(L"Textures/CrateGlass.png");
		m_pModel->SetMaterial(pGlassMaterial,1);
		break;
	case CrateType::ExplosiveCrate:
		m_pModel = AddComponent(new ModelComponent(L"Meshes/CrateExp.ovm"));
		pMaterial->SetDiffuseTexture(L"Textures/CrateExp.png");
		m_pModel->SetMaterial(pMaterial);
		break;
	default:
		break;
	}

}

void Crate::Update(const SceneContext& sceneContext )
{
	if (sceneContext.pInput->IsActionTriggered(10))
	{
		Destroy();
	}
	if (m_Hit && m_ParticleEffect->IsFinished())
	{
	//SoundManager::Get()->GetSystem()->playSound(m_pCrateBreakSound, nullptr, false, nullptr);
		
		GetScene()->RemoveChild(this, true);
	}
}
