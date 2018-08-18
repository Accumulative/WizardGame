#include "npc.h"

NPC::NPC() {

}
NPC::~NPC() {}
uint32_t NPC::getUpdateTimeout() {
	return update_timeout;
};

void NPC::update() {
	if(update_timeout > 0)
		update_timeout --;
}

void NPC::resetUpdateTimeout() {
	update_timeout = 5000;
}
void NPC::getAllNpcs(sqlite3* db) {
	char q[999];
	sqlite3_stmt*        stmt;;
	q[sizeof q - 1] = '\0';
	snprintf(
					q,
					sizeof q - 1,
					"SELECT * FROM npcs;"
	);
	std::cout <<"1" << std::endl;
	sqlite3_prepare(db, q, sizeof q, &stmt, NULL);
	std::cout <<"2" << std::endl;
	bool done = false;
	while (!done) {
		// printf("In select while\n");
		switch (sqlite3_step (stmt)) {
		case SQLITE_ROW: {
			NPC_Data new_npc;
			new_npc.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
			new_npc.image_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
			new_npc.mapId = sqlite3_column_int(stmt, 3);
			new_npc.id = sqlite3_column_int(stmt, 0);
			new_npc.pos.x = sqlite3_column_int(stmt, 4);
			new_npc.pos.y = sqlite3_column_int(stmt, 5);
			std::cout <<"36 "<<new_npc.pos.y << std::endl;
			new_npc.canFight = sqlite3_column_int(stmt, 6) != 0;
			std::cout <<"3 "<<new_npc.canFight << std::endl;
			orig_npc_data.push_back(new_npc);
			std::cout << "4" << std::endl;
			break;
		}
		case SQLITE_DONE: {
			done = true;
			break;
		}
		default:
			fprintf(stderr, "Failed.\n");
			return;
		}
	}
	npc_data = orig_npc_data;
	std::cout << "5" << std::endl;
}
std::vector<NPC_Data> NPC::getNpcsOnScreen(int mapId, Vector2D pos) {
	std::vector<NPC_Data> results;
	for(auto& n : npc_data) {
//		std::cout << n.mapId << " vs " << mapId << " and " << n.pos << " vs " << pos << std::endl;
		if(n.mapId == mapId &&
					n.pos.x < pos.x + 500 &&
					n.pos.x > pos.x - 500 &&
					n.pos.y < pos.y + 500 &&
					n.pos.y > pos.y - 500) {
						results.push_back(n);
			}
	}
	return results;
}
std::vector<NPC_Data> NPC::getNpcsOnMap(int mapId) {
	std::vector<NPC_Data> results;
	for(auto& n : npc_data) {
		if(n.mapId == mapId) {
						results.push_back(n);
			}
	}
	return results;
};

void NPC::UpdateNPCs() {
	auto ItA = orig_npc_data.begin();
	auto ItB = npc_data.begin();

	while(ItA != orig_npc_data.end() || ItB != npc_data.end())
	{
        ItB->pos = ItA->pos + Vector2D(static_cast<float>(75 - rand() % 151), static_cast<float>(75 - rand() % 151));
		if(ItA != orig_npc_data.end())
		{
			++ItA;
		}
		if(ItB != npc_data.end())
		{
			++ItB;
		}
	}
}
// void NPC::getNPCByName(std::string name){ 
// 	auto it = std::find_if(npc_data.begin(), npc_data.end(), [&name](const NPC_Data& obj) {return obj.name == name;});

// 	if (it != npc_data.end())
// 	{
// 			// found element. it is an iterator to the first matching element.
// 			// if you really need the index, you can also get it:
// 			auto index = std::distance(v.begin(), it);
// 	}
// }