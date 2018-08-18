#include <vector>
#include <Vector2D.h>
#include <sqlite3.h>
#include <data_struct.h>

class NPC {
	public:
		NPC();
		~NPC();
		void getAllNpcs(sqlite3* db);
		void getNpcsOnScreen();
		void UpdateNPCs();
		uint32_t getUpdateTimeout();
		void resetUpdateTimeout();
		void update();
		std::vector<NPC_Data> getNpcsOnScreen(int mapId, Vector2D pos);
		std::vector<NPC_Data> getNpcsOnMap(int mapId);
	private:
		uint32_t update_timeout = 5000;
		std::vector<NPC_Data> orig_npc_data;
		std::vector<NPC_Data> npc_data;
};