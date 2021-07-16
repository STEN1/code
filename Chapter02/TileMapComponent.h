#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include "Math.h"



class TileMapComponent : public SpriteComponent
{
public:
    TileMapComponent(Actor* owner, int drawOrder = 50);
    void LoadTilemap(std::string&& CSVPath, SDL_Texture* tilemapTexture, int tileSize);
    virtual void Draw(SDL_Renderer* renderer);
private:
    std::vector<std::vector<int>> mLoadedCSV;
    std::vector<SDL_Rect> mTileRects;
    int mTileSize;
};

