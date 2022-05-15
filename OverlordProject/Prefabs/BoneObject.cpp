#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length) : 
	m_pMaterial (pMaterial),
	m_Length (length)
{
	
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(XMFLOAT3(m_Length, 0,0 ));
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	auto boneWorld = XMLoadFloat4x4(&GetTransform()->GetWorld());
	auto inverse = XMMatrixInverse(nullptr, boneWorld);
	XMStoreFloat4x4(&m_BindPose, inverse);

	auto children = GetChildren<BoneObject>();
	for (auto c : children)
	{
		c->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = new GameObject();
	
	auto pModel = new ModelComponent(L"Meshes/Bone.ovm");
	pEmpty->AddComponent(pModel);
	pModel->SetMaterial(m_pMaterial);

	pEmpty->GetTransform()->Rotate(0, -90, 0, true);
	pEmpty->GetTransform()->Scale(m_Length, m_Length, m_Length);

	AddChild(pEmpty);
}
