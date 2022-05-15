#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	TODO_W7_();

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)

		auto passedTick = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		
		MathHelper::Clamp(passedTick, m_CurrentClip.duration, 0.0f);
		

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTick;
			if (m_TickCount < 0)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		else
		{
			m_TickCount += passedTick;
			if (m_TickCount > m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}
		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for (size_t i = 0; i < m_CurrentClip.keys.size(); ++i)
		{

			if (m_CurrentClip.keys[i].tick > m_TickCount)
			{
				keyA = m_CurrentClip.keys[i - 1];
				keyB = m_CurrentClip.keys[i];
				break;
			}
		}
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendFactor = (m_TickCount - keyA.tick)/ (keyB.tick - keyA.tick);
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		// 
		if (keyA.boneTransforms.size() > 0)
		{
			m_Transforms.clear();
			//FOR every boneTransform in a key (So for every bone)

			for (auto i = 0; i < m_pMeshFilter->m_BoneCount; ++i)
			{
				auto transformA = keyA.boneTransforms[i];
				auto transformB = keyB.boneTransforms[i];
				XMMATRIX transformAMat = XMLoadFloat4x4(&transformA);;
				XMMATRIX transformBMat = XMLoadFloat4x4(&transformB);;

				XMVECTOR scaleA, rotA, transA;
				XMMatrixDecompose(&scaleA, &rotA, &transA, transformAMat);
				XMVECTOR scaleB, rotB, transB;
				XMMatrixDecompose(&scaleB, &rotB, &transB, transformBMat);

				auto scale = XMVectorLerp(scaleA, scaleB, blendFactor);
				auto rot = XMQuaternionSlerp(rotA, rotB, blendFactor);
				auto trans = XMVectorLerp(transA, transB, blendFactor);

				XMMATRIX transMat = XMMatrixTranslationFromVector(trans);
				XMMATRIX scaleMat = XMMatrixScalingFromVector(scale);
				XMMATRIX rotMat = XMMatrixRotationQuaternion(rot);

				auto mat = scaleMat * rotMat * transMat;
				XMFLOAT4X4 finalmat;
				XMStoreFloat4x4(&finalmat, mat);
				m_Transforms.push_back(finalmat);

			}
		}
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector


	}
}

void ModelAnimator::SetAnimation(const std::wstring& /*clipName*/)
{
	TODO_W7_()
	//Set m_ClipSet to false
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//TODO_W7_()
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	if (clipNumber >= m_pMeshFilter->m_AnimationClips.size())
	{
		Reset();
		Logger::LogWarning(L"Clipnumber larger than amount of clips");
	}
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
	
	else
	{
		auto clip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(clip);
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;

	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//TODO_W7_()
	//If pause is true, set m_IsPlaying to false
	if (pause)
	{
		// Set m_TickCount to zero
			//Set m_AnimationSpeed to 1.0f
		m_IsPlaying = false;
		m_AnimationSpeed = 1.0f;
		m_TickCount = 0;
	}

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	if (m_ClipSet)
	{
		m_Transforms.assign(m_CurrentClip.keys[0].boneTransforms.begin(), m_CurrentClip.keys[0].boneTransforms.end());
	}
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
	else
	{
		auto mat = XMMatrixIdentity();
		XMFLOAT4X4 xmat;
		XMStoreFloat4x4(&xmat, mat);
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, xmat);
	}

}
