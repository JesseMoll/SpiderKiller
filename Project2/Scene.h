#pragma once

#include "Drawable.h"
#include <chrono>





class Scene : public Drawable
{
    public:
        static Scene* Instance();
		void draw();
		void draw2(){}; // Draw Nothing
		void InitGame();
		void DestructGame();
		UpdateResult update2(int ms, GlobalState &GS);
//----------------------------------------------------------------------
// User-input callbacks
//
// These are static functions which have access to the singleton Scene object
//----------------------------------------------------------------------

	static void pressNormalKeys(unsigned char key, int xx, int yy);
	static void releaseNormalKeys(unsigned char key, int xx, int yy);
	static void pressSpecialKey(int key, int xx, int yy);
	static void releaseSpecialKey(int key, int xx, int yy);
	static void mouseMove(int xx, int yy);
	static void mouseButton(int button, int state, int xx, int yy);

	static void Display();
	static void Reshape(int w, int h);
	static void Idle();
	static void Timer(int value);

    protected:
    private:
		static GlobalState GS;
		static std::chrono::time_point<std::chrono::system_clock> LastUpdate;
		Scene();
		static Scene* ptrInstance;
};