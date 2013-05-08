#include "Drawable.h"

class Health : public Drawable
{
public:
	Health(Drawable* _Parent, Vector2d _Pos = Vector2d(0,0));
	UpdateResult update2(int ms, GlobalState &GS);

private:
	GLuint PrevTexture;
	int RespawnTime;
	int RespawnClock;
	int HealthAmount;
};