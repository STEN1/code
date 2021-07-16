#include "TileMapComponent.h"
#include "Actor.h"
#include <fstream>
#include <sstream>
#include <iostream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
:SpriteComponent(owner, drawOrder)
{
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect dsrect;
		dsrect.w = mTileSize;
		dsrect.h = mTileSize;
		for (int i = 0; i < mLoadedCSV.size(); i++)
		{
			dsrect.y = i * mTileSize;
			for (int j = 0; j < mLoadedCSV[i].size(); j++)
			{
				if (mLoadedCSV[i][j] > -1)
				{
					dsrect.x = j * mTileSize;

					// Draw (have to convert angle from radians to degrees, and clockwise to counter)
					SDL_RenderCopyEx(renderer,
						mTexture,
						&mTileRects[mLoadedCSV[i][j]],
						&dsrect,
						-Math::ToDegrees(mOwner->GetRotation()),
						nullptr,
						SDL_FLIP_NONE);
				}
			}
		}
	}
}

void TileMapComponent::LoadTilemap(std::string&& CSVPath, SDL_Texture* tilemapTexture, int tileSize)
{
	mTexture = tilemapTexture;
	mTileSize = tileSize;

	SDL_QueryTexture(tilemapTexture, nullptr, nullptr, &mTexWidth, &mTexHeight);

	// Split the texture in to tiles to use with a csv tilemap.
	int tilesW = mTexWidth / tileSize;
	int tilesH = mTexHeight / tileSize;
	for (int i = 0; i < tilesH; i++)
	{
		for (int j = 0; j < tilesW; j++)
		{
			SDL_Rect tilerect;
			tilerect.x = j * tileSize;
			tilerect.y = i * tileSize;
			tilerect.w = tileSize;
			tilerect.h = tileSize;

			mTileRects.emplace_back(std::move(tilerect));
		}
	}

	std::cout << "Tiles loaded: " << mTileRects.size() << std::endl;

	// Load desired tiles from csv file.
	std::ifstream fin(std::move(CSVPath));
	std::string s;
	char cstring[4];
	while (std::getline(fin, s))
	{
		std::istringstream ss;
		ss.str(s);
		std::vector<int> CSVLine;
		while (ss.getline(cstring, 4, ','))
		{
			CSVLine.emplace_back(stoi(std::string(cstring)));
		}
		mLoadedCSV.emplace_back(std::move(CSVLine));
	}

	// Print loaded tilemap to console.
	for (auto& intvec : mLoadedCSV)
	{
		for (int i : intvec)
		{
			std::cout << i;
		}
		std::cout << std::endl;
	}
}
