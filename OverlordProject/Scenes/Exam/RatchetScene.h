#pragma once
class Ratchet;
class Character;
class Crate;
class RatchetScene final : public GameScene
{
public:
	RatchetScene() :GameScene(L"RatchetScene") {}
	~RatchetScene() override = default;
	RatchetScene(const RatchetScene& other) = delete;
	RatchetScene(RatchetScene&& other) noexcept = delete;
	RatchetScene& operator=(const RatchetScene& other) = delete;
	RatchetScene& operator=(RatchetScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void PostDraw() override;
	void OnGUI() override;
private:
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterAttack
	};

	Ratchet* m_pCharacter{};
	bool m_DrawShadowMap{ false };
	float m_ShadowMapScale{ 0.3f };
	std::vector<Crate*> m_pCrates;
};

