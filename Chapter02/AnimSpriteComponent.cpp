// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
	, mCurrentAnimation(-1)
	, mNextAnimationTimer(0.f)
	, mCanChangeAnimation(true)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (!mCanChangeAnimation)
	{
		mNextAnimationTimer += deltaTime;
		
		if (mNextAnimationTimer > 0.5f)
		{
			mCanChangeAnimation = true;
			mNextAnimationTimer = 0.f;
		}
	}
	
	if (!mAnimTextures.empty())
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;
		
		// Wrap current frame if needed
		while (mCurrFrame >= mAnimSpriteInfos[mCurrentAnimation].end + 1)
		{
			if (!mAnimSpriteInfos[mCurrentAnimation].looping)
			{
				mCurrFrame = mAnimSpriteInfos[mCurrentAnimation].end;
			}
			else
			{
				mCurrFrame -= mAnimSpriteInfos[mCurrentAnimation].end - mAnimSpriteInfos[mCurrentAnimation].start;
			}
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures, bool looping)
{
	if (!textures.empty())
	{
		mCurrFrame = mAnimTextures.size();
		mAnimSpriteInfos.emplace_back(mCurrFrame, (mCurrFrame + textures.size() - 1), looping);

		SDL_Log("Animation start: %i Animation end: %i", mAnimSpriteInfos.back().start, mAnimSpriteInfos.back().end);

		mAnimTextures.insert(mAnimTextures.end(), textures.begin(), textures.end());
		
		// Set the active texture to first frame
		SetTexture(mAnimTextures[mCurrFrame]);
		mCurrentAnimation++;
	}
}

void AnimSpriteComponent::NextAnimation()
{
	if (mAnimSpriteInfos.size() > 1 && mCanChangeAnimation)
	{
		if (++mCurrentAnimation == mAnimSpriteInfos.size())
		{
			mCurrentAnimation = 0;
		}

		SetTexture(mAnimTextures[mAnimSpriteInfos[mCurrentAnimation].start]);

		mCanChangeAnimation = false;
	}
}
