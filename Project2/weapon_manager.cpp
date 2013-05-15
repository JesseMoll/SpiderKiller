#include "weapon_manager.h"
#include "texture_manager.h"
#include "exceptions.h"

weapon_manager::weapon_manager(Drawable* _Parent):
	Drawable(_Parent)
{
}

void weapon_manager::Clear()
{
	weapons.clear();
	equipped_weapons.clear();
	Drawable::Clear();
}


Weapon* weapon_manager::getEquippedWeapon(Weapon::EquipStatus WeaponSlot)
{
	if (equipped_weapons.count(WeaponSlot) == 0)
		return NULL;
	return weapons[equipped_weapons[WeaponSlot]];
}

void weapon_manager::equip_weapon(std::string weapon_name)
{
	if(weapons.count(weapon_name) != 1)
		throw(weapon_not_found(weapon_name));
	Weapon::EquipStatus EquipSlot = weapons[weapon_name]->getEquipSlot();

	std::string CurWeaponName = equipped_weapons[EquipSlot];
	if(CurWeaponName != "")
	{
		weapons[CurWeaponName]->Unequip();
	}

	weapons[weapon_name]->Equip();
	equipped_weapons[EquipSlot] = weapon_name;
}

Projectile* weapon_manager::add_projectile(std::string projectile_name, Projectile* new_projectile)
{
	projectiles[projectile_name] = new_projectile;
	return new_projectile;
}
Projectile* weapon_manager::add_projectile(std::string projectile_name, std::string texture_name, double size, double speed, double damage, double max_distance, std::string on_death_projectile_name, int on_death_number, double on_death_spread)
{
	
	GLuint Tex = 0;
	if(texture_name != "")
		Tex = texture_manager::get_texture_name(texture_name);
	Projectile* on_death_projectile = 0;
	if(on_death_projectile_name != "")
		on_death_projectile = get_projectile(on_death_projectile_name);
	projectiles[projectile_name] = new Projectile(this, Tex, size, speed, damage, max_distance, on_death_projectile, on_death_number, DegToRad(on_death_spread));
	return projectiles[projectile_name];
}

Weapon* weapon_manager::add_weapon(std::string weapon_name, std::string texture_name, double fire_rate, std::string projectile_name, Weapon::EquipStatus EquipSlot)
{
	GLuint Tex = 0;
	if(texture_name != "")
		Tex = texture_manager::get_texture_name(texture_name);

	weapons[weapon_name] = new Weapon(this, Tex, fire_rate, projectiles[projectile_name],EquipSlot);
	equip_weapon(weapon_name);

	AddChild(weapons[weapon_name]);

	return weapons[weapon_name];
}

void weapon_manager::SwitchWeapon(Weapon::EquipStatus EquipSlot)
{
	std::string CurrentWeaponName = equipped_weapons[EquipSlot];
	auto CurrentWeaponItr = weapons.find(CurrentWeaponName);
	//Go to the next weapon
	do
	{
		++CurrentWeaponItr;
		//If there are no more weapons, go back to the begining of the container
		if(CurrentWeaponItr == weapons.end())
		{
			CurrentWeaponItr = weapons.begin();
		}
	} while(CurrentWeaponItr->second->getEquipSlot() != EquipSlot);
	//Equip the new weapon
	equip_weapon(CurrentWeaponItr->first);
	std::cout << "Equipping: " << (*CurrentWeaponItr).first << std::endl;
}



UpdateResult weapon_manager::update2(int ms, GlobalState &GS)
{
	if (GS.KeyDown(Q_KEY))
	{
		SwitchWeapon(Weapon::EQUIP_LEFT);
		GS.KeyStates &= ~Q_KEY;
	}
	if (GS.KeyDown(E_KEY))
	{
		SwitchWeapon(Weapon::EQUIP_RIGHT);
		GS.KeyStates &= ~E_KEY;
	}
	if (GS.KeyDown(F_KEY))
	{
		SwitchWeapon(Weapon::EQUIP_SPACE);
		GS.KeyStates &= ~F_KEY;
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