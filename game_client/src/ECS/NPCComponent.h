#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

class NPCComponent : public Component
{
private:
	Vector2D nxtCoords;
	NPC_Data npc_data;
    float prevDis = 100;
public:
	NPCComponent(NPC_Data data) {
		
		npc_data = data;
        std::cout << "Adding " << data.name << std::endl;

	};
	~NPCComponent() {}

	void init() override {
		entity->addComponent<TransformComponent>(npc_data.pos.x, npc_data.pos.y, 44, 33, 1, 1);
        nxtCoords = npc_data.pos;
        entity->addComponent<SpriteComponent>(this->npc_data.image_name, true, true);
        entity->getComponent<SpriteComponent>().addAnimation(0, 3, "Idle", 300);
        entity->getComponent<SpriteComponent>().addAnimation(1, 3, "WalkForward", 300);
        entity->getComponent<SpriteComponent>().addAnimation(2, 4, "WalkSideward", 300);
        entity->getComponent<SpriteComponent>().addAnimation(3, 4, "WalkBackward", 300);
        entity->getComponent<SpriteComponent>().Play("Idle");
        entity->addComponent<ColliderComponent>(ColliderType::Npc, 7 * Game::SCALE, 25 * Game::SCALE,
                                                20 * Game::SCALE, 26 * Game::SCALE);
		if(npc_data.name == "") {
			npc_data.name = "Friendly villager";
		}
	}

	void update() override {
        TransformComponent npcTrans = entity->getComponent<TransformComponent>();
        float dis = npcTrans.position.Distance(nxtCoords);
        if(prevDis >= dis) {
            prevDis = dis;
            if (dis < npcTrans.speed * 1.5) {
                entity->getComponent<TransformComponent>().velocity = Vector2D(0, 0);
                nxtCoords =npcTrans.position;
            }
        } else {
            entity->getComponent<TransformComponent>().velocity = Vector2D(0, 0);
            nxtCoords = npcTrans.position;
        }
        Vector2D diff = npcTrans.position - npcTrans.prevPosition;
        if (diff.x != 0) {
            entity->getComponent<SpriteComponent>().Play("WalkSideward");
            if (diff.x < 0) {
                entity->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
            } else {
                entity->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
            }
        } else if (diff.y > 0) {
            entity->getComponent<SpriteComponent>().Play("WalkForward");
        } else if (diff.y < 0) {
            entity->getComponent<SpriteComponent>().Play("WalkBackward");
        } else {
            entity->getComponent<SpriteComponent>().Play("Idle");
        }
	}

	int getId() {
		return npc_data.id;
	};
	void setDest(Vector2D dest) {
        Vector2D diff = dest - nxtCoords;
        prevDis = entity->getComponent<TransformComponent>().position.Distance(dest);
        if (diff.y == 0) {
            entity->getComponent<TransformComponent>().velocity.x = diff.x == 0 ? 0 : (diff.x > 0 ? 1 : -1);
        } else {
            entity->getComponent<TransformComponent>().velocity = Vector2D(
                    (diff.x > 0 ? 1 : -1) * fabsf(sinf(atanf(diff.x / diff.y))),
                    (diff.y > 0 ? 1 : -1) * fabsf(cosf(atanf(diff.x / diff.y))));
            std::cout << entity->getComponent<TransformComponent>().velocity << std::endl;
        }
        nxtCoords = dest;
    }
};