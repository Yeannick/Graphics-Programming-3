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
#include "Materials/SkyBoxMaterial.h"

void PostProcessingScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	m_SceneContext.pLights->SetDirectionalLight({ 200.6139526f,240.f,120.1850471f }, { -0.5f, -1.0f, -0.5f });
	
	

	//Materials
	//*********
	const auto pPeasantMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>(); //Shadow variant
	//const auto pPeasantMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pPeasantMaterial->SetDiffuseTexture(L"Textures/PeasantGirl_Diffuse.png");
	
	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
	//const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pGroundMaterial->SetDiffuseTexture(L"Textures/225.png");
	const auto pGrassMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
	pGrassMaterial->SetDiffuseTexture(L"Textures/926.png");
	const auto pRingMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
	pRingMaterial->SetDiffuseTexture(L"Textures/235.png");
	const auto pWallMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
	pWallMaterial->SetDiffuseTexture(L"Textures/238.png");
	const auto pTerrainMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
	pTerrainMaterial->SetDiffuseTexture(L"Textures/279.png");

	//Ground Mesh
	//***********
	const auto pGroundObj = new GameObject();
	const auto pGroundModel = new ModelComponent(L"Meshes/Palace_Dirt.ovm");
	pGroundModel->SetMaterial(pGroundMaterial,0);

	pGroundObj->AddComponent(pGroundModel);
	//pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);
	pGroundObj->GetTransform()->Rotate(90.0f, 0.0f, 00.0f);

	AddChild(pGroundObj);

	const auto pGround2Obj = new GameObject();
	const auto pGround2Model = new ModelComponent(L"Meshes/Palace_Grass.ovm");
	pGround2Model->SetMaterial(pGrassMaterial, 0);

	pGround2Obj->AddComponent(pGround2Model);
	//pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);
	pGround2Obj->GetTransform()->Rotate(90.0f, 0.0f, 00.0f);

	AddChild(pGround2Obj);

	const auto pGround3Obj = new GameObject();
	const auto pGround3Model = new ModelComponent(L"Meshes/Palace_Terrain.ovm");
	pGround3Model->SetMaterial(pTerrainMaterial, 0);

	pGround3Obj->AddComponent(pGround3Model);
	//pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);
	pGround3Obj->GetTransform()->Rotate(90.0f, 0.0f, 00.0f);

	AddChild(pGround3Obj);
	
	const auto pGround4Obj = new GameObject();
	const auto pGround4Model = new ModelComponent(L"Meshes/Palace_Outer.ovm");
	pGround4Model->SetMaterial(pRingMaterial, 0);

	pGround4Obj->AddComponent(pGround4Model);
	//pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);
	pGround4Obj->GetTransform()->Rotate(90.0f, 0.0f, 00.0f);

	AddChild(pGround4Obj);

	const auto pGround5Obj = new GameObject();
	const auto pGround5Model = new ModelComponent(L"Meshes/Palace_Ring.ovm");
	pGround5Model->SetMaterial(pWallMaterial, 0);

	pGround5Obj->AddComponent(pGround5Model);
	//pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);
	pGround5Obj->GetTransform()->Rotate(90.0f, 0.0f, 00.0f);

	AddChild(pGround5Obj);

	const auto pGround6Obj = new GameObject();
	const auto pGround6Model = new ModelComponent(L"Meshes/Palace_Build.ovm");
	pGround6Model->SetMaterial(pWallMaterial, 0);

	pGround6Obj->AddComponent(pGround6Model);
	//pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);
	pGround6Obj->GetTransform()->Rotate(90.0f, 0.0f, 00.0f);

	AddChild(pGround6Obj);

	// Skybox
	
	m_SkyBox = new GameObject();

	const auto pSkyBoxMaterial = MaterialManager::Get()->CreateMaterial<SkyBoxMaterial>();
	pSkyBoxMaterial->SetSkyBoxTexture(L"Textures/SkyBox2.dds");
	
	m_SkyBox->AddComponent(new ModelComponent(L"Meshes/Box.ovm"));
	m_SkyBox->GetComponent<ModelComponent>()->SetMaterial(pSkyBoxMaterial);
	m_SkyBox->GetTransform()->Rotate(0.f, 0.f, 90.f);
	AddChild(m_SkyBox);

	//Character Mesh
	//**************
	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/PeasantGirl.ovm"));
	pModel->SetMaterial(pPeasantMaterial);

	pObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	if (const auto pAnimator = pModel->GetAnimator())
	{
		pAnimator->SetAnimation(2);
		pAnimator->Play();
	}

	//Post Processing Stack
	//=====================
	m_pPostGrayscale = MaterialManager::Get()->CreateMaterial<PostGrayscale>();
	//m_pPostBlur = ...
	m_pPostBlur = MaterialManager::Get()->CreateMaterial<PostBlur>();

	m_PostVignette = MaterialManager::Get()->CreateMaterial<PostVignette>();

	m_pPostChromAb = MaterialManager::Get()->CreateMaterial<PostChromaticAbberation>();

	AddPostProcessingEffect(m_pPostGrayscale);
	AddPostProcessingEffect(m_pPostBlur);
	AddPostProcessingEffect(m_PostVignette);
	AddPostProcessingEffect(m_pPostChromAb);
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
}
