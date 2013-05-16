extern const int LevelSize = 1024;
extern const int CellSize = 16;
extern const int MaxWeight = CellSize * CellSize;
extern const int GridSize = LevelSize / CellSize;
extern const double MinView = 60;
extern const double MaxView = 512;
extern const double CameraMoveSpeed = .001;
extern const int MaxCreep = 1000;
extern const unsigned int UPDATE_INTERVAL = 10;
extern const double FocusGainMult = .005;

extern const double WallForceMult = 0.2;
extern const double PathingForceMult = 0.5;
extern const double HeroForceMult = 100.0;
extern const double CreepDistanceForceMult = 2.0;
extern const double CreepVelocityForceMult = 0.5;