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
		while (mCurrFrame >= mAnimationStartEnd[mCurrentAnimation].second + 1)
		{
			mCurrFrame -= mAnimationStartEnd[mCurrentAnimation].second - mAnimationStartEnd[mCurrentAnimation].first;
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	if (!textures.empty())
	{
		mCurrFrame = mAnimTextures.size();
		mAnimationStartEnd.emplace_back(std::make_pair(mCurrFrame, mCurrFrame + textures.size() - 1));

		SDL_Log("Animation start: %i Animation end: %i", mAnimationStartEnd.back().first, mAnimationStartEnd.back().second);

		mAnimTextures.insert(mAnimTextures.end(), textures.begin(), textures.end());
		
		// Set the active texture to first frame
		SetTexture(mAnimTextures[mCurrFrame]);
		mCurrentAnimation++;
	}
}

void AnimSpriteComponent::NextAnimation()
{
	if (mAnimationStartEnd.size() > 1 && mCanChangeAnimation)
	{
		if (++mCurrentAnimation == mAnimationStartEnd.size())
		{
			mCurrentAnimation = 0;
		}

		SetTexture(mAnimTextures[mAnimationStartEnd[mCurrentAnimation].first]);

		mCanChangeAnimation = false;
	}
}
