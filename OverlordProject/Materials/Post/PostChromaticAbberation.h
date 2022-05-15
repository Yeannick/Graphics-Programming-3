#pragma once

class TextureData;
class ID3D11EffectShaderResourceVariable;
class PostChromaticAbberation : public PostProcessingMaterial
{

public:
	PostChromaticAbberation();
	~PostChromaticAbberation() override = default;
	PostChromaticAbberation(const PostChromaticAbberation& other) = delete;
	PostChromaticAbberation(PostChromaticAbberation&& other) noexcept = delete;
	PostChromaticAbberation& operator=(const PostChromaticAbberation& other) = delete;
	PostChromaticAbberation& operator=(PostChromaticAbberation&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
	

	

};