#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    if (!running)
    {
        running = Graphics::OpenWindow();
    }

    switch (currentScene)
    {
    case SCENE_SPRING_CHAIN:
    {
        // spring chain setup
        anchor = Vec2(Graphics::Width() / 2.0f, 30);

        for (int i = 0; i < NUM_BODIES; ++i)
        {
            Body* p = new Body(CircleShape(6), anchor.x, anchor.y + (i * restLength), 2.0f);
            bodies.push_back(p);
        }
        break;
    }
    case SCENE_FRICTION:
    {
        // friction setup
        Body* smallBall = new Body(CircleShape(6), Graphics::Width() / 2 + 50, Graphics::Height() / 2, 1.0f);
        bodies.push_back(smallBall);

        Body* bigBall = new Body(CircleShape(12), Graphics::Width() / 2, Graphics::Height() / 2, 3.0f);
        bodies.push_back(bigBall);
        break;

    }
    case SCENE_LIQUID:
    {
        // liquid setup
        Body* smallBall = new Body(CircleShape(6), Graphics::Width() / 2 + 50, Graphics::Height() / 2, 1.0f);
        bodies.push_back(smallBall);

        Body* bigBall = new Body(CircleShape(12), Graphics::Width() / 2, Graphics::Height() / 2, 3.0f);
        bodies.push_back(bigBall);

        liquid.x = 0;
        liquid.y = Graphics::Height() * .75f; // starts at 75% of screen
        liquid.w = Graphics::Width();
        liquid.h = Graphics::Height() / 4; // fills 25% of screen
        break;
    }
    case SCENE_ORBIT:
    {
        // planet setup
        Body* smallBall = new Body(CircleShape(10), 200, 200, 1.0f);
        bodies.push_back(smallBall);

        Body* hugeBall = new Body(CircleShape(20), 500, 500, 40.0f);
        bodies.push_back(hugeBall);
        break;
    }
    case SCENE_NORMAL_GRAVITY:
    {
        Body* bigBall = new Body(CircleShape(100), 100, 100, 1.0f);
        Body* smallBall = new Body(CircleShape(50), 500, 100, 1.0f);
        bodies.push_back(bigBall);
        bodies.push_back(smallBall);
        break;
    }
    case SCENE_ANGULAR_VELOCITY:
    {
        Body* hugeBall = new Body(CircleShape(50), Graphics::Width() / 2, Graphics::Height() / 2, 40.0f);
        bodies.push_back(hugeBall);
        break;
    }
    case SCENE_BOX_DEFAULT:
    {
		Body* box = new Body(BoxShape(200, 100), Graphics::Width() / 2, Graphics::Height() / 2, 1.0f);
		bodies.push_back(box);
        break;
    }
    case SCENE_CIRCLES_COLLIDING:
    {
        Body* bigBall = new Body(CircleShape(100), 100, 100, 1.0f);
        Body* smallBall = new Body(CircleShape(50), 500, 100, 1.0f);
        bodies.push_back(bigBall);
        bodies.push_back(smallBall);
        break;
    }
    default:
        break;
    }

}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_UP)
                {
                    pushForce.y = -50 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    pushForce.y = 50 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    pushForce.x = -50 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    pushForce.x = 50 * PIXELS_PER_METER;
                }
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                {
                    pushForce.y = 0;
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    pushForce.y = 0;
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    pushForce.x = 0;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    pushForce.x = 0;
                }
                if (event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_8)
                {
                    ResetScene(event.key.keysym.sym);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    Body* body = new Body(CircleShape(4), event.button.x, event.button.y, 1.0f);
                    bodies.push_back(body);
                }
                break;
            case SDL_MOUSEMOTION: // This is for control over circle in collision scene
                if (currentScene == SCENE_CIRCLES_COLLIDING)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    bodies[0]->position.x = x;
                    bodies[0]->position.y = y;
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    Graphics::ClearScreen(0xFF056263); // TODO: Remove this after done debugging or find more elegant way to debug
    
    // Check if we are too fast and if so waste some milliseconds until we reach the millisecs_per_frame
    static int timePreviousFrame;

    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
    {
        SDL_Delay(timeToWait);
    }

    // calculate delta time in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f; 

    // protects delta time 
    if (deltaTime > 0.016f)
    {
        deltaTime = 0.016f;
    }

    timePreviousFrame = SDL_GetTicks();

    switch (currentScene)
    {
    case SCENE_SPRING_CHAIN:
    {
        for (auto body : bodies)
        {
            Vec2 dragForce = Force::GenerateDragForce(*body, .002);
            body->AddForce(dragForce);
            
            Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER); // Weight is the force, gravity is the acceleration (W = mg)
            body->AddForce(weight);
            
        }
        Vec2 springForce = Force::GenerateSpringForce(*bodies[0], anchor, restLength, k);
        bodies[0]->AddForce(springForce);

        for (int i = 1; i < NUM_BODIES; ++i)
        {
            int currBody = i;
            int prevBody = i - 1;
            Vec2 springForce = Force::GenerateSpringForce(*bodies[currBody], *bodies[prevBody], restLength, k);
            bodies[currBody]->AddForce(springForce);
            bodies[prevBody]->AddForce(-springForce);
        }
        break;
    }
    case SCENE_FRICTION:
        for (auto body : bodies)
        {
            Vec2 friction = Force::GenerateFrictionForce(*body, 10.0f * PIXELS_PER_METER);
            body->AddForce(friction);
        }
        break;
    case SCENE_LIQUID:
        for (auto body : bodies)
        {
            Vec2 dragForce = Force::GenerateDragForce(*body, .002);
            body->AddForce(dragForce);

            Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER); // Weight is the force, gravity is the acceleration (W = mg)
            body->AddForce(weight);
            
            if (body->position.y >= liquid.y)
            {
                Vec2 drag = Force::GenerateDragForce(*body, 0.03f);
                body->AddForce(drag);
            }
            else
            {
                Vec2 wind = Vec2(0.2f * PIXELS_PER_METER, 0.0f);
                body->AddForce(wind);
            }
        }
        break;
    case SCENE_ORBIT:
    {
        Vec2 attraction = Force::GenerateGravitationalForce(*bodies[0], *bodies[1], 1000.0f, 5.0f, 100.0f);
        bodies[0]->AddForce(attraction);
        bodies[1]->AddForce(-attraction);
        break;
    }
    case SCENE_NORMAL_GRAVITY:
        for (auto body : bodies)
        {
            Vec2 dragForce = Force::GenerateDragForce(*body, .002);
            body->AddForce(dragForce);

            Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER); // Weight is the force, gravity is the acceleration (W = mg)
            body->AddForce(weight);
        }
        break;
    case SCENE_ANGULAR_VELOCITY:
        for (auto body : bodies)
        {
            Vec2 dragForce = Force::GenerateDragForce(*body, .002);
            body->AddForce(dragForce);

            Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER); // Weight is the force, gravity is the acceleration (W = mg)
            body->AddForce(weight);

            float torque = 2000.0f;
            body->AddTorque(torque);
        }
        break;
    case SCENE_BOX_DEFAULT:
    {
        for (auto body : bodies)
        {
            float torque = 2000.0f;
            body->AddTorque(torque);
        }
        break;
    }
    case SCENE_CIRCLES_COLLIDING:
        for (auto body : bodies)
        {
            //Vec2 windForce = Vec2(20.0f * PIXELS_PER_METER, 0.0f);
            //body->AddForce(windForce);

            //Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER); // Weight is the force, gravity is the acceleration (W = mg)
            //body->AddForce(weight);
        }
        break;
    default:
        break;

    }

    // Apply Forces
    for(auto body : bodies)
    {
        body->AddForce(pushForce);
    }

    // Integrate
    for (auto body : bodies)
    {
        body->Update(deltaTime);
    }

    for (auto body : bodies)
    {
        body->IsColliding = false;
    }

    // Check for collisions
    for (int i = 0; i < bodies.size() - 1; i++)
    {
        for (int j = i + 1; j < bodies.size(); j++)
        {
            Body* a = bodies[i];
            Body* b = bodies[j];

            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)) 
            {
                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFF00FFFF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFF00FFFF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFF00FF);
                a->IsColliding = true;
                b->IsColliding = true;
            }
        }
    }

    // Check bounds of window
    for (auto body : bodies)
    {
        if (body->shape->GetType() == ShapeType::CIRCLE)
        {
            CircleShape* circleShape = (CircleShape*)body->shape;
            // TODO: Fix this. This is just to keep our Body in our window for now with fake physics
            if (body->position.x - circleShape->radius <= 0)
            {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9f;
            }
            else if (body->position.x + circleShape->radius >= Graphics::Width())
            {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9f;
            }

            if (body->position.y - circleShape->radius <= 0)
            {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9f;
            }
            else if (body->position.y + circleShape->radius >= Graphics::Height())
            {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -0.9f;
            }
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    //Graphics::ClearScreen(0xFF056263); Moving to update just for debuging contact info

    static float angle = 0.0f;

    switch (currentScene)
    {
    case SCENE_SPRING_CHAIN:
        Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
        Graphics::DrawLine(anchor.x, anchor.y, bodies[0]->position.x, bodies[0]->position.y, 0xFF313131);

        for (int i = 0; i < NUM_BODIES - 1; ++i)
        {
            Graphics::DrawLine(bodies[i]->position.x, bodies[i]->position.y, bodies[i + 1]->position.x, bodies[i + 1]->position.y, 0xFF313131);
        }

        for (auto body : bodies)
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawFillCircle(body->position.x, body->position.y, circleShape->radius, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_FRICTION:
        for (auto body : bodies)
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, angle, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_LIQUID:
        Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF6E3713);

        for (auto body : bodies)
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, angle, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_ORBIT:
        if (bodies[0]->shape->GetType() == ShapeType::CIRCLE && bodies[1]->shape->GetType() == ShapeType::CIRCLE)
        {
            CircleShape* circleShape = (CircleShape*)bodies[0]->shape;
            Graphics::DrawFillCircle(bodies[0]->position.x, bodies[0]->position.y, circleShape->radius, 0xFFAA3300);
            circleShape = (CircleShape*)bodies[1]->shape;
            Graphics::DrawFillCircle(bodies[1]->position.x, bodies[1]->position.y, circleShape->radius, 0xFF00FFFF);
        }
        break;
    case SCENE_NORMAL_GRAVITY:
        for (auto body : bodies)
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, angle, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_ANGULAR_VELOCITY:
        for (auto body : bodies)
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_BOX_DEFAULT:
        for (auto body : bodies)
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
            }
            else if (body->shape->GetType() == BOX)
            {
                BoxShape* boxShape = (BoxShape*) body->shape;
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_CIRCLES_COLLIDING:
        for (auto body : bodies)
        {
            Uint32 color = body->IsColliding ? 0xFF0000FF : 0xFFFFFFFF;

            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, angle, color);
            }
        }
        break;
    default:
        break;
    }

    angle += 0.01f;
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////////////////////////////////
void Application::SwitchScene(SDL_Keycode keyCode)
{
    switch (keyCode)
    {
    case SDLK_1:
        currentScene = SCENE_SPRING_CHAIN;
        break;
    case SDLK_2:
        currentScene = SCENE_FRICTION;
        break;
    case SDLK_3:
        currentScene = SCENE_LIQUID;
        break;
    case SDLK_4:
        currentScene = SCENE_ORBIT;
        break;
    case SDLK_5:
        currentScene = SCENE_NORMAL_GRAVITY;
        break;
    case SDLK_6:
        currentScene = SCENE_ANGULAR_VELOCITY;
        break;
    case SDLK_7:
        currentScene = SCENE_BOX_DEFAULT;
        break;
    case SDLK_8:
        currentScene = SCENE_CIRCLES_COLLIDING;
        break;
    default:
        currentScene = SCENE_NORMAL_GRAVITY;
        break;
    }
}

void Application::ResetScene(SDL_Keycode keyCode)
{
    for (auto body : bodies)
    {
        delete body;
    }
    bodies.clear();
    SwitchScene(keyCode);
    Setup();
}
///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto body : bodies)
    {
        delete body;
    }

    Graphics::CloseWindow();
}