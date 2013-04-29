#pragma once
#include "drawable.h"
#include <map>
#include <string>
#include "Projectile.h"
#include "Weapon.h"

class weapon_manager :
	public Drawable
{
public:
	weapon_manager(Drawable* _Parent);
	~weapon_manager(void);
	Projectile* get_projectile(std::string projectile_name);
	void equip_weapon(std::string weapon_name);

	Projectile* add_projectile(std::string projectile_name, std::string texture_name = "", double size = .2, double speed = 40, double damage = 100, double max_distance = 1e6, std::string on_death_projectile_name = "", int on_death_number = 0, double on_death_spread = 0);
	Weapon* add_weapon(std::string weapon_name, std::string texture_name, double fire_rate, std::string projectile_name);
	GLuint getWeaponIcon(Weapon::EquipStatus WeaponSlot);
	void SwitchLeftWeapon(GlobalState &GS);

//Overridden Functions
	void draw2(){};
	UpdateResult update2(int ms, GlobalState &GS);
	
protected:
	std::map<std::string, Projectile*> projectiles;
	std::map<std::string, Weapon*> weapons;

	std::map<Weapon::EquipStatus, std::string> equipped_weapons;
};

