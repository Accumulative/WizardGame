#pragma once

#include "TransformComponent.h"
#include "SDL2/SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
  TransformComponent * transform;
  SDL_Texture *texture;
  SDL_Rect srcRect, destRect;

  bool animated = false;
  int frames = 0;
  int speed = 100;
  bool show = true;
  int target = -1;
  bool followCam = true;

public:

  int animIndex = 0;
  std::map<std::string, Animation> animations;

  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

  SpriteComponent() = default;
  SpriteComponent(std::string id)
  {
    setTex(id);
  }

  SpriteComponent(std::string id, bool isAnimated, bool followCam)
  {
    animated = isAnimated;
    setTex(id);
    this->followCam = followCam;
  }

  ~SpriteComponent()
  {
  }
  void addAnimation(int row, int column, std::string animName, int speed) {
    Animation ani = Animation(row, column, speed);
    animations.emplace(animName, ani);
  }

  bool getShow() {
    return show;
  }
  int getTarget() {
    return target;
  }
  void setShow(bool show) {
    this->show = show;
  }
  void setTarget(int netId) {
    this->target = netId;
  }

  void setTex(std::string id)
  {
    texture = Game::assets->GetTexture(id);
  }

  void init() override
  {

    transform = &entity->getComponent<TransformComponent>();

    srcRect.x = srcRect.y = 0;
    srcRect.w = transform->width;
    srcRect.h = transform->height;
  }

  void update() override
  {

    if (animated && frames != 0)
    {
      srcRect.y = srcRect.h * static_cast<int>((SDL_GetTicks() / speed) % frames);
    }

    srcRect.x = animIndex * transform->width;
    // std::cout << srcRect.x << " " << animIndex << " " << transform->width << std::endl;

    destRect.x = static_cast<int>(transform->position.x - (followCam ? Game::camera.x : 0));
    destRect.y = static_cast<int>(transform->position.y - (followCam ? Game::camera.y : 0));
    destRect.w = transform->width * transform->scale;
    destRect.h = transform->height * transform->scale;
    // std::cout << srcRect.x << " " <<srcRect.y << " " <<srcRect.w << " " << srcRect.h << std::endl;
  }

  void draw() override
  {
    if(show) {
      // std::cout << srcRect.x <<" "<< srcRect.y <<" "<< srcRect.w <<" "<< srcRect.h <<" " << destRect.x <<" "<< destRect.y <<" "<< destRect.w <<" "<< destRect.h << std::endl;
      TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }
  }

  void Play(std::string animName)
  {
    frames = animations[animName].frames;
    animIndex = animations[animName].index;
    speed = animations[animName].speed;
  }

};
