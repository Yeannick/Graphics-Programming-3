#include "stdafx.h"
#include "DiffuseAmbient.h"

DiffuseAmbient::DiffuseAmbient():
	Material<DiffuseAmbient>(L"Effects/DiffuseAmbient.fx")
{

}

void DiffuseAmbient::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(L"gDiffuseMap"))
	{
		HANDLE_ERROR(pShaderVariable->AsShaderResource()->SetResource(m_pDiffuseTexture->GetShaderResourceView()));
		return;
	}
}

void DiffuseAmbient::InitializeEffectVariables()
{
}
