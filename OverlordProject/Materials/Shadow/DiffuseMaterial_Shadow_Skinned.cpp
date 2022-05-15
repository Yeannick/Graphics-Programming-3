#include "stdafx.h"
#include "DiffuseMaterial_Shadow_Skinned.h"

DiffuseMaterial_Shadow_Skinned::DiffuseMaterial_Shadow_Skinned():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_Skinned.fx")
{}

void DiffuseMaterial_Shadow_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	 * 
	 * 2. Update the ShadowMap texture
	 * 
	 * 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	 * 
	 * 4. Update Bones
	*/
	auto model_World = XMLoadFloat4x4(&pModel->GetTransform()->GetWorld());
	auto light_viewProjection = XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP());
	auto lightWVP = model_World * light_viewProjection;
	XMFLOAT4X4 lightWVPloaded{};
	XMStoreFloat4x4(&lightWVPloaded, lightWVP);

	SetVariable_Matrix(L"gWorldViewProj_Light", lightWVPloaded);
	SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());
	SetVariable_Vector(L"gLightDirection", XMFLOAT3{ &sceneContext.pLights->GetDirectionalLight().direction.x });
	SetVariable_MatrixArray(L"gBones", &pModel->GetAnimator()->GetBoneTransforms()[0].m[0][0], (UINT)pModel->GetAnimator()->GetBoneTransforms().size());


	//Update Shadow Variables
	//const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	//...
}
