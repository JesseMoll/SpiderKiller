#include "weapon_manager.h"
#include "texture_manager.h"
#include "exceptions.h"

weapon_manager::weapon_manager(Drawable* _Parent):
	Drawable(_Parent)
{




}

GLuint weapon_manager::getWeaponIcon(Weapon::EquipStatus WeaponSlot)
{
	return weapons[equipped_weapons[WeaponSlot]]->getTexture();
}


void weapon_manager::equip_weapon(std::string weapon_name)
{
	std::string CurWeaponName = equipped_weapons[Weapon::EQUIP_LEFT];
	if(CurWeaponName != "")
	{
		weapons[CurWeaponName]->Equip(Weapon::EQUIP_NONE);
	}
	if(weapons.count(weapon_name) != 1)
		throw(weapon_not_found(weapon_name));
	weapons[weapon_name]->Equip(Weapon::EQUIP_LEFT);
	equipped_weapons[Weapon::EQUIP_LEFT] = weapon_name;

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

Weapon* weapon_manager::add_weapon(std::string weapon_name, std::string texture_name, double fire_rate, std::string projectile_name)
{
	GLuint Tex = 0;
	if(texture_name != "")
		Tex = texture_manager::get_texture_name(texture_name);

	weapons[weapon_name] = new Weapon(this, Tex, fire_rate, projectiles[projectile_name]);
	equip_weapon(weapon_name);

	AddChild(weapons[weapon_name]);

	return weapons[weapon_name];
}

void weapon_manager::SwitchLeftWeapon(GlobalState &GS)
{

	std::string CurrentWeaponName = equipped_weapons[Weapon::EQUIP_LEFT];
	auto CurrentWeaponItr = weapons.find(CurrentWeaponName);
	//Go to the next weapon
	++CurrentWeaponItr;
	//If there are no more weapons, go back to the begining of the container
	if(CurrentWeaponItr == weapons.end())
	{
		CurrentWeaponItr = weapons.begin();
	}
	//Equip the new weapon
	equip_weapon(CurrentWeaponItr->first);
	GS.LeftWeaponTex = ((*CurrentWeaponItr).second)->getTexture();
	std::cout << "Equipping: " << (*CurrentWeaponItr).first << std::endl;
}



UpdateResult weapon_manager::update2(int ms, GlobalState &GS)
{
	if (GS.KeyDown(Q_KEY))
	{
		SwitchLeftWeapon(GS);
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