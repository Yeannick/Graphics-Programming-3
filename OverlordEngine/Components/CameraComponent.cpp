#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active?this:nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups) const
{
	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed get active camera. Parent game object is null");

	const auto pScene = pGameObject->GetScene();
	auto mousePos = pScene->GetSceneContext().pInput->GetMousePosition(); 
	auto pContext = GetScene()->GetSceneContext();
	XMFLOAT2 halfViewportDimensions;
	halfViewportDimensions.y = 0.5f * pContext.windowHeight;
	halfViewportDimensions.x = 0.5f * pContext.windowWidth;

	auto Xndc = (mousePos.x - halfViewportDimensions.x) / halfViewportDimensions.x;
	auto Yndc = (halfViewportDimensions.y - mousePos.y) / halfViewportDimensions.y;

	XMMATRIX VPI = XMLoadFloat4x4(&m_ViewProjectionInverse);
	XMFLOAT4 nearPointFloat = { Xndc, Yndc, 0, 0 };
	XMFLOAT4 farPointFloat = { Xndc, Yndc, 1, 0 };
	XMVECTOR nearPointVector = XMLoadFloat4(&nearPointFloat);
	XMVECTOR farPointVector = XMLoadFloat4(&farPointFloat);
	XMVECTOR nearPoint = XMVector3TransformCoord(nearPointVector, VPI);
	XMVECTOR farPoint = XMVector3TransformCoord(farPointVector, VPI);
	XMFLOAT3 nearP;
	XMStoreFloat3(&nearP, nearPoint);
	XMFLOAT3 farP;
	XMStoreFloat3(&farP, farPoint);

	PxQueryFilterData filterData;
	filterData.data.word0 = ~UINT(ignoreGroups);

	PxRaycastBuffer hit;
	auto PhysXProxy = m_pGameObject->GetScene()->GetPhysxProxy();

	auto origin = PxVec3(nearP.x, nearP.y, nearP.z);
	auto direction = PxVec3(farP.x - nearP.x, farP.y - nearP.y, farP.z - nearP.z);
	direction.normalize();

	if (PhysXProxy->Raycast(origin, direction, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
	{
		auto actor = hit.getAnyHit(0).actor;

		if (actor)
		{
			auto ud = actor->userData;
			if (ud != nullptr)
			{
				return reinterpret_cast<BaseComponent*>(ud)->GetGameObject();
			}
		}
	}
	return nullptr;
}