#pragma once


	enum class Spell {
		None, Fire, Ice, Electric, Water
	};
	
class Spells {
public:
	static int getDamage(Spell spell) {
		int damage = 0;
		switch(spell) {
				case Spell::Fire:
					damage = 30;
					break;
				case Spell::Electric:
					damage = 20;
					break;
				case Spell::Ice:
					damage = 15;
					break;
				case Spell::Water:
					damage = 3;
					break;
			}
			std::cout << "Get damage. Spell: " << static_cast<int>(spell) << " dmg: " << damage << std::endl;
			return damage;
	}
};