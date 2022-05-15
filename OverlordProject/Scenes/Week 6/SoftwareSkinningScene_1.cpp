#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Materials\ColorMaterial.h"
#include "Prefabs\BoneObject.h"

void SoftwareSkinningScene_1::Initialize()
{
	
	ColorMaterial* pColor = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pColor->SetVariable_Vector(L"gColor", XMFLOAT4{ Colors::Red });
	GameObject* pRoot = new GameObject();

	m_pBone0 = new BoneObject(pColor,15.f);
	
	m_pBone1 = new BoneObject(pColor,15.f);
	m_pBone0->AddBone(m_pBone1);
	pRoot->AddChild(m_pBone0);
	AddChild(pRoot);
	
}

void SoftwareSkinningScene_1::Update()
{
	auto scene = GetSceneContext();
	m_BoneRotation += m_RotationSign * 45 * scene.pGameTime->GetElapsed();
	if (m_RotationSign < 0 && m_BoneRotation <= -45)
		m_RotationSign = 1;
	if (m_RotationSign > 0 && m_BoneRotation >= 45)
		m_RotationSign = -1;

	m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
	m_pBone1->GetTransform()->Rotate(0, 0, -m_BoneRotation * 2);
}

void SoftwareSkinningScene_1::OnGUI()
{
}
