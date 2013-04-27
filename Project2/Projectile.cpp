#include "Projectile.h"
#include "Creep.h"

Projectile::Projectile(Drawable* _Parent, GLuint _Texture, double _Scale, double _Speed, double _Damage, double _MaxDistance, Projectile* _ProjectileToFireOnDeath, int _NumToFireOnDeath,  double _SpreadOnDeath, Vector3d _Color):
	Drawable(_Parent, _Texture, Vector2d(0,0), Vector2d(_Scale,_Scale), _Color),
	Speed(_Speed),
	Damage(_Damage),
	MaxDistance(_MaxDistance),
	ProjectileToFireOnDeath(_ProjectileToFireOnDeath),
	NumToFireOnDeath(_NumToFireOnDeath),
	SpreadOnDeath(_SpreadOnDeath),
	DistanceTravelled(0)
{
}

Projectile::Projectile(Projectile * c, Vector2d _Pos, double _Rot):
	Drawable(c->Parent, c->Texture, _Pos, c->Scale, c->Color),
	Speed(c->Speed),
	Damage(c->Damage),
	MaxDistance(c->MaxDistance),
	ProjectileToFireOnDeath(c->ProjectileToFireOnDeath),
	NumToFireOnDeath(c->NumToFireOnDeath),
	SpreadOnDeath(c->SpreadOnDeath),
	DistanceTravelled(0)
{
	Speed = Speed * (.8 + Random(.4));
	Rot = _Rot;
}

Projectile::~Projectile(void)
{
}

void Projectile::onDeath(GlobalState &GS)
{
	if(ProjectileToFireOnDeath != 0)
	{
		double NewRotation = Rot;
		NewRotation -= (double)SpreadOnDeath / 2;
		for(int n = 0; n != NumToFireOnDeath; n++)
		{
			Parent->AddChild (new Projectile(ProjectileToFireOnDeath, Pos, NewRotation));
			//otherwise the first and last projectiles overlap on a radial weapon
			if(SpreadOnDeath > 359)
				NewRotation += SpreadOnDeath / (double)(NumToFireOnDeath);
			else
				NewRotation += SpreadOnDeath / (double)(NumToFireOnDeath - 1);
		}
	}
}

UpdateResult Projectile::update2(int ms, GlobalState &GS)
{
	Vector2d PosAdder(cos(DegToRad(Rot)), sin(DegToRad(Rot)));
	PosAdder *= Speed * (ms / 1000.0);
	//Will the new position be valid?
	DistanceTravelled += PosAdder.length();
	if (DistanceTravelled >= MaxDistance)
	{
		onDeath(GS);
		return UPDATE_DELETE;
	}
	if(GetWalkable(Pos + PosAdder))
	{
		Pos += PosAdder;
		Rect2d ProjectileRect = GetBoundingRect();
		for (std::list<Drawable*>::iterator dPtr = GS.TheCreepManager->begin();dPtr != GS.TheCreepManager->end(); dPtr++) {
			Rect2d CreepRect = (*dPtr)->GetBoundingRect();
			if(ProjectileRect.overlaps(CreepRect)) 
			{
				//deal damage to the creep based on the projectile's damage
				Damage = ((Creep*)(*dPtr))->Damage(Damage);
				if (Damage <= 0)
				{
					onDeath(GS);
					return UPDATE_DELETE;
				}
			}
		}
	}
	else
	{
		onDeath(GS);
		return UPDATE_DELETE;
	}
	return UPDATE_REDRAW;
}