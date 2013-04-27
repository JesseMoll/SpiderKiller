#include "weapon_manager.h"
#include "texture_manager.h"
#include "exceptions.h"

weapon_manager::weapon_manager(Drawable* _Parent):
	Drawable(_Parent),
	LeftWeapon(weapons.end())
{




}

Projectile* weapon_manager::add_projectile(std::string projectile_name, std::string texture_name, double size, double speed, double damage, double max_distance, std::string on_death_projectile_name, int on_death_number, double on_death_spread)
{
	
	GLuint Tex = 0;
	if(texture_name != "")
		Tex = texture_manager::get_texture_name(texture_name);
	Projectile* on_death_projectile = 0;
	if(on_death_projectile_name != "")
		on_death_projectile = get_projectile(on_death_projectile_name);
	projectiles[projectile_name] = new Projectile(this, Tex, size, speed, damage, max_distance, on_death_projectile, on_death_number, on_death_spread);
	return projectiles[projectile_name];
}

Weapon* weapon_manager::add_weapon(std::string weapon_name, double fire_rate, std::string projectile_name)
{
	if(LeftWeapon != weapons.end())
	{
		//unequip the old weapon
		(*LeftWeapon).second->Equip(Weapon::EQUIP_NONE);
	}
	weapons[weapon_name] = new Weapon(this, fire_rate, projectiles[projectile_name]);
	weapons[weapon_name]->Equip(Weapon::EQUIP_LEFT);
	AddChild(weapons[weapon_name]);
	LeftWeapon = weapons.find(weapon_name);
	return weapons[weapon_name];
}

void weapon_manager::SwitchLeftWeapon()
{
	//unequipCurrentWeapon
	(*LeftWeapon).second->Equip(Weapon::EQUIP_NONE);

	//Go to the next weapon
	LeftWeapon++;
	//If there are no more weapons, go back to the begining of the container
	if(LeftWeapon == weapons.end())
	{
		LeftWeapon = weapons.begin();
	}
	//Equip the new weapon
	(*LeftWeapon).second->Equip(Weapon::EQUIP_LEFT);
	std::cout << "Equipping: " << (*LeftWeapon).first << std::endl;
}



UpdateResult weapon_manager::update2(int ms, GlobalState &GS)
{
	if (GS.KeyDown(Q_KEY))
	{
		SwitchLeftWeapon();
		GS.KeyStates &= ~Q_KEY;
	}
	return UPDATE_NONE;
}

weapon_manager::~weapon_manager(void)
{
	for (std::pair<std::string, Weapon*> w : weapons) 
	{
		delete w.second;
	}
}

Projectile* weapon_manager::get_projectile(std::string projectile_name)
{
	if(projectiles.count(projectile_name) != 1)
		throw(projectile_not_found(projectile_name));
	return projectiles[projectile_name]; 
}