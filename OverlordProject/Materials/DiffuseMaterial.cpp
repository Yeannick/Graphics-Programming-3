#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial():
	Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx")
{
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{

	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(L"gDiffuseMap"))
	{
		HANDLE_ERROR(pShaderVariable->AsShaderResource()->SetResource(m_pDiffuseTexture->GetShaderResourceView()));
		return;
	}
}

void DiffuseMaterial::InitializeEffectVariables()
{
}
