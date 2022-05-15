#pragma once
class UberMaterial final : public Material<UberMaterial>
{
public:
	UberMaterial();
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring & assetFile);
	void SetNormalTexture(const std::wstring & assetFile);
	void SetEnvironmentTexture(const std::wstring& assetFile);
	void SetSpecularIntensityTexture(const std::wstring& assetFile);
protected:
	void InitializeEffectVariables() override;
	

private:
	TextureData* m_pDiffuseTexture{};
	TextureData* m_pNormalMapTexture{};
	TextureData* m_pEnvironmentTexture{};
	TextureData* m_pSpecularTexture{};
	
	
};

