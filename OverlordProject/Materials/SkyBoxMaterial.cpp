#include "stdafx.h"
#include "SkyBoxMaterial.h"
SkyBoxMaterial::SkyBoxMaterial() :
	Material<SkyBoxMaterial>(L"Effects/SkyBox.fx")
{
}
void SkyBoxMaterial::SetSkyBoxTexture(const std::wstring& mat)
{
	m_pSkyBoxTexture = ContentManager::Load<TextureData>(mat);
	if (const auto pShaderVariable = GetVariable(L"m_CubeMap"))
	{
		HANDLE_ERROR(pShaderVariable->AsShaderResource()->SetResource(m_pSkyBoxTexture->GetShaderResourceView()));
		return;
	}
}

void SkyBoxMaterial::InitializeEffectVariables()
{
}


