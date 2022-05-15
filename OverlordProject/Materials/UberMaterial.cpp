#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial():
	Material<UberMaterial>(L"Effects/UberShader.fx")
{
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(L"gTextureDiffuse"))
	{
		HANDLE_ERROR(pShaderVariable->AsShaderResource()->SetResource(m_pDiffuseTexture->GetShaderResourceView()));
		return;
	}
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalMapTexture = ContentManager::Load<TextureData>(assetFile);
	if (const auto pShaderVariable = GetVariable(L"gTextureNormal"))
	{
		HANDLE_ERROR(pShaderVariable->AsShaderResource()->SetResource(m_pNormalMapTexture->GetShaderResourceView()));
		return;
	}
}

void UberMaterial::SetEnvironmentTexture(const std::wstring& assetFile)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(assetFile);
	if (const auto pShaderVariable = GetVariable(L"gCubeEnvironment"))
	{
		HANDLE_ERROR(pShaderVariable->AsShaderResource()->SetResource(m_pEnvironmentTexture->GetShaderResourceView()));
		return;
	}
}

void UberMaterial::SetSpecularIntensityTexture(const std::wstring& assetFile)
{
	m_pSpecularTexture = ContentManager::Load<TextureData>(assetFile);
	if (const auto pShaderVariable = GetVariable(L"gTextureSpecularIntensity"))
	{
		HANDLE_ERROR(pShaderVariable->AsShaderResource()->SetResource(m_pSpecularTexture->GetShaderResourceView()));
		return;
	}
}

void UberMaterial::InitializeEffectVariables()
{

	if (const auto pShaderVariable = GetVariable(L"gUseTextureDiffuse"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetBool(true));
		
	}
	if (const auto pShaderVariable = GetVariable(L"gFlipGreenChannel"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetBool(true));
		
	}
	if (const auto pShaderVariable = GetVariable(L"gUseTextureNormal"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetBool(false));
		
	}
	if (const auto pShaderVariable = GetVariable(L"gUseEnvironmentMapping"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetBool(false));

	}
	if (const auto pShaderVariable = GetVariable(L"gReflectionStrength"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetFloat(0.8f));

	}
	if (const auto pShaderVariable = GetVariable(L"gRefractionStrength"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetFloat(0.5f));

	}
	if (const auto pShaderVariable = GetVariable(L"gRefractionIndex"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetFloat(0.9f));

	}
	if (const auto pShaderVariable = GetVariable(L"gUseFresnelFalloff"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetBool(true));

	}
	if (const auto pShaderVariable = GetVariable(L"gFresnelPower"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetFloat(2.f));

	}
	if (const auto pShaderVariable = GetVariable(L"gFresnelMultiplier"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetFloat(1.5f));

	}
	if (const auto pShaderVariable = GetVariable(L"gFresnelHardness"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetFloat(2.f));

	}
	if (const auto pShaderVariable = GetVariable(L"gShininess"))
	{
		HANDLE_ERROR(pShaderVariable->AsScalar()->SetInt(40));

	}
	if (const auto pShaderVariable = GetVariable(L"gColorDiffuse"))
	{
		auto diffuseColor = XMFLOAT4{ 1.f,0.f,0.f,1.f };
		const auto* data = &diffuseColor.x;
		
		HANDLE_ERROR(pShaderVariable->AsVector()->SetFloatVector(data));

	}
	if (const auto pShaderVariable = GetVariable(L"gColorSpecular"))
	{
		auto diffuseColor = XMFLOAT4{ 0.f,1.f,0.f,1.f };
		const auto* data = &diffuseColor.x;

		HANDLE_ERROR(pShaderVariable->AsVector()->SetFloatVector(data));

	}
	
	if (const auto pShaderVariable = GetVariable(L"gColorFresnel"))
	{
		auto diffuseColor = XMFLOAT4{ 1.f,1.f,1.f,1.f };
		const auto* data = &diffuseColor.x;

		HANDLE_ERROR(pShaderVariable->AsVector()->SetFloatVector(data));

	}
	
}
