#pragma once

#include "../Game.h"

class KeyboardController : public Component
{
public:
  TransformComponent *transform;
  SpriteComponent *sprite;

  void init() override
  {
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
  }

  void update(SDL_Event event)
  {
    if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_w:
        transform->velocity.y = -1;
        sprite->Play("WalkBackward");
        break;
      case SDLK_a:
        transform->velocity.x = -1;
        sprite->Play("WalkSideward");
        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        break;
      case SDLK_d:
        transform->velocity.x = 1;
        sprite->Play("WalkSideward");
        break;
      case SDLK_s:
        transform->velocity.y = 1;
        sprite->Play("WalkForward");
        break;
      default:
        break;
      }
    }
  
    if (event.type == SDL_KEYUP)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_w:
        transform->velocity.y = 0;
        sprite->Play("Idle");
        break;
      case SDLK_a:
        transform->velocity.x = 0;
        sprite->Play("Idle");
        sprite->spriteFlip = SDL_FLIP_NONE;
        break;
      case SDLK_d:
        transform->velocity.x = 0;
        sprite->Play("Idle");
        break;
      case SDLK_s:
        transform->velocity.y = 0;
        sprite->Play("Idle");
        break;
      case SDLK_t:
        transform->position.x += 100;
        sprite->Play("Idle");
        break;
      default:
        break;
      }
    }
  }
};
