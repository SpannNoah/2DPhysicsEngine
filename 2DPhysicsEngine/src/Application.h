#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Body.h"
#include "Physics/World.h"

class Application {
    private:
        enum Scene
        {
            SCENE_SPRING_CHAIN,
            SCENE_ORBIT,
            SCENE_LIQUID,
            SCENE_FRICTION,
            SCENE_NORMAL_GRAVITY,
            SCENE_ANGULAR_VELOCITY,
            SCENE_BOX_DEFAULT,
            SCENE_CIRCLES_COLLIDING,
            SCENE_SAT,
            SCENE_POLYGONCIRCLE,
        };

        bool debug = false;
        bool running = false;
        Scene currentScene;
        
        World* world;
        Vec2 pushForce = Vec2(0.0f, 0.0f);
        Vec2 mouseCursor = Vec2(0, 0);
        bool leftMouseButtonDown = false;

        Vec2 anchor = Vec2(0, 0);
        float k = 300;
        float restLength = 15;
        const int NUM_BODIES = 15;
        
        SDL_Rect liquid;

        void SwitchScene(SDL_Keycode keyCode);
        void ResetScene(SDL_Keycode keyCode);

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif