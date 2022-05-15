#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_pParticleBuffer(new VertexParticle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	//TODO_W9(L"Implement Destructor")
	delete m_ParticlesArray;
	delete m_pParticleBuffer;

	m_pVertexBuffer->Release();
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Initialize")

	if (!m_pParticleMaterial)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
		CreateVertexBuffer(sceneContext);
		m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
	}
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement CreateVertexBuffer")
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
	}
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexParticle) * m_ParticleCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	HANDLE_ERROR(sceneContext.d3dContext.pDevice->CreateBuffer(&vertexBufferDesc, nullptr, &m_pVertexBuffer))
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Update")
	float particleInterval = ((m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / 2) / m_MaxParticles;
	m_LastParticleSpawn += sceneContext.pGameTime->GetElapsed();
	m_ActiveParticles = 0;
	D3D11_MAPPED_SUBRESOURCE pData;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);

	for (UINT i = 0; i < m_ParticleCount; i++)
	{
		auto& currentParticle = m_ParticlesArray[i];

		if (currentParticle.isActive)
			UpdateParticle(currentParticle, sceneContext.pGameTime->GetElapsed());
		if (!currentParticle.isActive && m_LastParticleSpawn >= particleInterval)
			SpawnParticle(currentParticle);

		m_pParticleBuffer[m_ActiveParticles] = currentParticle.vertexInfo;
		if (currentParticle.isActive)
			++m_ActiveParticles;
	}

	memcpy(pData.pData, m_pParticleBuffer, sizeof(VertexParticle) * m_ActiveParticles);
	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);

}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	//TODO_W9(L"Implement UpdateParticle")
	if (!p.isActive)
		return;

	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy < 0.0f)
		p.isActive = false;

	auto position = XMLoadFloat3(&p.vertexInfo.Position);
	auto velocity = XMLoadFloat3(&m_EmitterSettings.velocity);
	XMStoreFloat3(&p.vertexInfo.Position, position + (velocity * elapsedTime));

	float lifePercent = p.currentEnergy / p.totalEnergy;

	// Match the color
	p.vertexInfo.Color.w = p.vertexInfo.Color.w;

	//p.vertexInfo.Size = p.sizeChange < 1 ? p.initialSize - (p.sizeChange * lifePercent) : p.initialSize + (p.sizeChange * lifePercent);
	p.vertexInfo.Size = p.initialSize + (p.sizeChange * lifePercent);
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	p.isActive = true;

	p.currentEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);

	auto randomDirection = XMFLOAT3{ 1,0,0 };
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI));

	auto randomRotation{ XMVector3TransformNormal(XMLoadFloat3(&randomDirection), rotationMatrix) };
	float randomDisttance = MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius);
	auto position = XMLoadFloat3(&GetTransform()->GetPosition());

	XMStoreFloat3(&p.vertexInfo.Position, position + randomRotation * randomDisttance);

	auto randomSize = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);
	p.vertexInfo.Size = randomSize;
	p.initialSize = randomSize;

	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement PostDraw")
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	auto pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	auto techContext = m_pParticleMaterial->GetTechniqueContext();

	// Set Input layout
	pDeviceContext->IASetInputLayout(techContext.pInputLayout);

	// Set Primitive Topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//Set Vertex Buffer
	const UINT offset = 0;
	const UINT stride = sizeof(VertexParticle);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	D3DX11_TECHNIQUE_DESC techDesc{};
	techContext.pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		techContext.pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->Draw(static_cast<UINT>(m_ActiveParticles), 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}