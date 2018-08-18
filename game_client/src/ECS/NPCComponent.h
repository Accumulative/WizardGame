#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

class NPCComponent : public Component
{
private:
	Vector2D nxtCoords;
	NPC_Data npc_data;
public:
	NPCComponent(NPC_Data data) {
		
		npc_data = data;

	};
	~NPCComponent() {}

	void init() override {
		entity->addComponent<TransformComponent>(npc_data.pos.x, npc_data.pos.y, 32, 32, 1, 2);
        nxtCoords = npc_data.pos;
		entity->addComponent<SpriteComponent>(npc_data.image_name, true, true);
		entity->getComponent<SpriteComponent>().addAnimation(0, 1, "Idle", 100);
		entity->getComponent<SpriteComponent>().Play("Idle");
        entity->addComponent<ColliderComponent>(ColliderType::Npc, 7 * Game::SCALE, 25 * Game::SCALE,
                                                20 * Game::SCALE, 26 * Game::SCALE);
		if(npc_data.name == "") {
			npc_data.name = "Friendly villager";
		}
	}

	void update() override {
//        std::cout << entity->getComponent<TransformComponent>().position << " " << nxtCoords << " " << entity->getComponent<TransformComponent>().position.Distance(nxtCoords) << " speed : " << entity->getComponent<TransformComponent>().speed << std::endl;
		if(entity->getComponent<TransformComponent>().position.Distance(nxtCoords) < entity->getComponent<TransformComponent>().speed * 1.5) {
			entity->getComponent<TransformComponent>().velocity = Vector2D(0,0);
            nxtCoords = entity->getComponent<TransformComponent>().position;
        }
	}

	int getId() {
		return npc_data.id;
	};
	void setDest(Vector2D dest) {
		Vector2D diff = dest - nxtCoords;
        if(diff.y == 0) {
            entity->getComponent<TransformComponent>().velocity.x = diff.x == 0 ? 0 : (diff.x > 0 ? 1 : -1);
        } else {
            std::cout << diff << dest << nxtCoords << std::endl;
            entity->getComponent<TransformComponent>().velocity = Vector2D((diff.x > 0 ? 1 : -1) * fabsf(sinf(atanf(diff.x / diff.y))),
                                                                           (diff.y > 0 ? 1 : -1) * fabsf(cosf(atanf(diff.x / diff.y))));
            std::cout << entity->getComponent<TransformComponent>().velocity << std::endl;
        }
		nxtCoords = dest;
	}
//    void draw() override {
//        std::cout << "Im being drawn: " << npc_data.name << " and my coords are: " << this->entity->getComponent<TransformComponent>().position << std::endl;
//    }
};