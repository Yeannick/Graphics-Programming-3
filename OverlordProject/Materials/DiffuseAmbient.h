#pragma once
class DiffuseAmbient final : public Material<DiffuseAmbient>
{
public:
	DiffuseAmbient();
	~DiffuseAmbient() override = default;

	DiffuseAmbient(const DiffuseAmbient& other) = delete;
	DiffuseAmbient(DiffuseAmbient&& other) noexcept = delete;
	DiffuseAmbient& operator=(const DiffuseAmbient& other) = delete;
	DiffuseAmbient& operator=(DiffuseAmbient&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture{};
};


