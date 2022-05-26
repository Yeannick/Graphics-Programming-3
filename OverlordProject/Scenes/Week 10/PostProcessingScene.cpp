#include "stdafx.h"
#include "PostProcessingScene.h"

//Uncomment for shadow variants
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

#include "Materials/DiffuseMaterial.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"

#include "Materials/Post/PostGrayscale.h"
#include "Materials/Post/PostBlur.h"
#include "Materials/Post/PostVignette.h"
#include "Materials/Post/PostChromaticAbberation.h"
//#include "Materials/Post/PostBrightness.h"
#include "Materials/SkyBoxMaterial.h"

void PostProcessingScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	//m_SceneContext.useDeferredRendering = false;
	m_SceneContext.pLights->SetDirectionalLight({ 200.6139526f,240.f,120.1850471f }, { -0.5f, -1.0f, -0.5f });



	//Materials
	//*********
	const auto pPeasantMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pPeasantMaterial->SetDiffuseTexture(L"Textures/PeasantGirl_Diffuse.png");

	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pGroundMaterial->SetDiffuseTexture(L"Textures/GroundBrick.jpg");

	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/PeasantGirl.ovm"));
	pModel->SetMaterial(pPeasantMaterial);

	pObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	if (const auto pAnimator = pModel->GetAnimator())
	{
		pAnimator->SetAnimation(2);
		pAnimator->Play();
	}
	// Skybox

	m_SkyBox = new GameObject();

	const auto pSkyBoxMaterial = MaterialManager::Get()->CreateMaterial<SkyBoxMaterial>();
	pSkyBoxMaterial->SetSkyBoxTexture(L"Textures/SkyBox2.dds");

	m_SkyBox->AddComponent(new ModelComponent(L"Meshes/Box.ovm"));
	m_SkyBox->GetComponent<ModelComponent>()->SetMaterial(pSkyBoxMaterial);
	m_SkyBox->GetTransform()->Rotate(0.f, 0.f, 90.f);
	AddChild(m_SkyBox);

	//Ground Mesh
	//***********
	const auto pGroundObj = new GameObject();
	const auto pGroundModel = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel->SetMaterial(pGroundMaterial);

	pGroundObj->AddComponent(pGroundModel);
	pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);

	AddChild(pGroundObj);
	//Character Mesh
	//**************


	//Post Processing Stack
	//=====================
	m_pPostGrayscale = MaterialManager::Get()->CreateMaterial<PostGrayscale>();
	//m_pPostBlur = ...
	m_pPostBlur = MaterialManager::Get()->CreateMaterial<PostBlur>();

	m_PostVignette = MaterialManager::Get()->CreateMaterial<PostVignette>();

	m_pPostChromAb = MaterialManager::Get()->CreateMaterial<PostChromaticAbberation>();

	//m_pPostBrightness = MaterialManager::Get()->CreateMaterial<PostBrightness>();

	AddPostProcessingEffect(m_pPostGrayscale);
	AddPostProcessingEffect(m_pPostBlur);
	AddPostProcessingEffect(m_PostVignette);
	AddPostProcessingEffect(m_pPostChromAb);
	//AddPostProcessingEffect(m_pPostBrightness);
	m_SceneContext.pInput->AddInputAction(InputAction(0, InputState::pressed, VK_SPACE));

	//TODO_W10
	//Create and add a PostBlur material class (using Blur.fx)
	//Add the material to the PostProcessing Stack + Uncomment the corresponding ImGui code below
	//If you already have shadow mapping working you can change the code above to use the shadow variant shaders
}

void PostProcessingScene::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(0))
	{
		const auto pCameraTransform = m_SceneContext.pCamera->GetTransform();
		m_SceneContext.pLights->SetDirectionalLight(pCameraTransform->GetPosition(), pCameraTransform->GetForward());
	}

}

void PostProcessingScene::OnGUI()
{
	bool isEnabled = m_pPostGrayscale->IsEnabled();
	ImGui::Checkbox("Grayscale PP", &isEnabled);
	m_pPostGrayscale->SetIsEnabled(isEnabled);

	isEnabled = m_pPostBlur->IsEnabled();
	ImGui::Checkbox("Blur PP", &isEnabled);
	m_pPostBlur->SetIsEnabled(isEnabled);

	isEnabled = m_PostVignette->IsEnabled();
	ImGui::Checkbox("Vignette PP", &isEnabled);
	m_PostVignette->SetIsEnabled(isEnabled);

	isEnabled = m_pPostChromAb->IsEnabled();
	ImGui::Checkbox("Chromattic Abbreviation PP", &isEnabled);
	m_pPostChromAb->SetIsEnabled(isEnabled);

	/*isEnabled = m_pPostBrightness->IsEnabled();
	ImGui::Checkbox("Brightness PP", &isEnabled);
	m_pPostBrightness->SetIsEnabled(isEnabled);

	float multiplier = m_pPostBrightness->GetMultiplier();
	ImGui::SliderFloat("Brightness Multiplier", &multiplier, 0.f, 5.f, "%.1f", 1.f);
	m_pPostBrightness->SetMultiplier(multiplier);*/
}
