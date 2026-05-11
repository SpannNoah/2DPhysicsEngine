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

    world = new World(-9.8);

    switch (currentScene)
    {
    case SCENE_SPRING_CHAIN:
    {
        // spring chain setup
        anchor = Vec2(Graphics::Width() / 2.0f, 30);

        for (int i = 0; i < NUM_BODIES; ++i)
        {
            Body* p = new Body(CircleShape(6), anchor.x, anchor.y + (i * restLength), 2.0f);
            world->AddBody(p);
        }
        break;
    }
    case SCENE_FRICTION:
    {
        // friction setup
        Body* smallBall = new Body(CircleShape(6), Graphics::Width() / 2 + 50, Graphics::Height() / 2, 1.0f);
        world->AddBody(smallBall);

        Body* bigBall = new Body(CircleShape(12), Graphics::Width() / 2, Graphics::Height() / 2, 3.0f);
        world->AddBody(bigBall);
        break;

    }
    case SCENE_LIQUID:
    {
        // liquid setup
        Body* smallBall = new Body(CircleShape(6), Graphics::Width() / 2 + 50, Graphics::Height() / 2, 1.0f);
        world->AddBody(smallBall);

        Body* bigBall = new Body(CircleShape(12), Graphics::Width() / 2, Graphics::Height() / 2, 3.0f);
        world->AddBody(bigBall);

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
        world->AddBody(smallBall);

        Body* hugeBall = new Body(CircleShape(20), 500, 500, 40.0f);
        world->AddBody(hugeBall);
        break;
    }
    case SCENE_NORMAL_GRAVITY:
    {
        Body* bigBall = new Body(CircleShape(100), 100, 100, 1.0f);
        Body* smallBall = new Body(CircleShape(50), 500, 100, 1.0f);
        world->AddBody(bigBall);
        world->AddBody(smallBall);
        break;
    }
    case SCENE_ANGULAR_VELOCITY:
    {
        Body* hugeBall = new Body(CircleShape(100), Graphics::Width() / 2, Graphics::Height() / 2, 0.0f);
        world->AddBody(hugeBall);
        break;
    }
    case SCENE_BOX_DEFAULT:
    {
		Body* box = new Body(BoxShape(200, 100), Graphics::Width() / 2, Graphics::Height() / 2, 1.0f);
        world->AddBody(box);
        break;
    }
    case SCENE_CIRCLES_COLLIDING:
    {
        Body* bigBall = new Body(CircleShape(100), 100, 100, 0.0f);
        Body* smallBall = new Body(CircleShape(50), 500, 100, 1.0f);
        world->AddBody(bigBall);
        world->AddBody(smallBall);
        break;
    }
    case SCENE_SAT:
    {
        Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0f, Graphics::Height() - 50, 0.0f);
        floor->restitution = .2f;
        world->AddBody(floor);
        
        Body* boxA = new Body(BoxShape(200, 200), Graphics::Width() / 2.0f, Graphics::Height() / 2.0f, 0.0f);
        boxA->rotation = 1.4f;
        boxA->restitution = .5f;
        world->AddBody(boxA);

        break;
    }
    case SCENE_POLYGONCIRCLE:
    {
        Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0f, Graphics::Height() - 50, 0.0f);
        Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0f - 25, 0.0f);
        Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0f - 25, 0.0f);
        floor->restitution = 0.5f;
        leftWall->restitution = 0.2f;
        rightWall->restitution = 0.2f;
        world->AddBody(floor);
        world->AddBody(leftWall);
        world->AddBody(rightWall);

        Body* bigBox = new Body(BoxShape(200, 200), Graphics::Width() / 2.0f, Graphics::Height() / 2.0f, 0.0f);
        bigBox->SetTexture("./assets/crate.png");
        bigBox->restitution = 0.1f;
        bigBox->rotation = 1.4f;
        world->AddBody(bigBox);

        Body* ball = new Body(CircleShape(50), Graphics::Width() / 2.0f, Graphics::Height() / 2.0f, 1.0f);
        ball->SetTexture("./assets/basketball.png");
        ball->restitution = 0.1;
        world->AddBody(ball);
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
                if (event.key.keysym.sym == SDLK_d)
                {
                    debug = !debug;
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
                if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
                {
                    ResetScene(event.key.keysym.sym);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (currentScene != SCENE_SAT)
                    {
                        Body* body = new Body(CircleShape(40), event.button.x, event.button.y, 1.0f);
                        body->SetTexture("./assets/basketball.png");
                        body->restitution = 0.8f;
                        world->AddBody(body);
                    }
                    else
                    {
                        Body* body = new Body(BoxShape(40, 40), event.button.x, event.button.y, 5.0f);
                        world->AddBody(body);
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    std::vector<Vec2> vertices;
                    Vec2 center = Vec2(0,-1);
                    float halfHeight = 40.0f;
                    for (int i = 0; i < 5; i++)
                    {
                        float angle = (2.0f * M_PI / 5.0f) * i;
                        Vec2 vertex = center.Rotate(angle) * halfHeight;
                        vertices.push_back(vertex);
                    }
                    Body* polygon = new Body(PolygonShape(vertices), event.button.x, event.button.y, 2.0f);
                    polygon->restitution = 0.5f;
                    polygon->friction = 0.4f;
                    world->AddBody(polygon);
                }
                break;
            case SDL_MOUSEMOTION: // This is for control over circle in collision scene
                if (currentScene == SCENE_CIRCLES_COLLIDING)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    world->GetBodies()[0]->position.x = x;
                    world->GetBodies()[0]->position.y = y;
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
            for (auto body : world->GetBodies())
            {
                Vec2 dragForce = Force::GenerateDragForce(*body, .002);
                body->AddForce(dragForce);

                Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER); // Weight is the force, gravity is the acceleration (W = mg)
                body->AddForce(weight);

            }
            Vec2 springForce = Force::GenerateSpringForce(*world->GetBodies()[0], anchor, restLength, k);
            world->GetBodies()[0]->AddForce(springForce);

            for (int i = 1; i < NUM_BODIES; ++i)
            {
                int currBody = i;
                int prevBody = i - 1;
                Vec2 springForce = Force::GenerateSpringForce(*world->GetBodies()[currBody], *world->GetBodies()[prevBody], restLength, k);
                world->GetBodies()[currBody]->AddForce(springForce);
                world->GetBodies()[prevBody]->AddForce(-springForce);
            }
            break;
        }
        case SCENE_FRICTION:
        {
            for (auto body : world->GetBodies())
            {
                Vec2 friction = Force::GenerateFrictionForce(*body, 10.0f * PIXELS_PER_METER);
                body->AddForce(friction);
            }
            break;
        }
        case SCENE_LIQUID:
        {
            for (auto body : world->GetBodies())
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
        }
        case SCENE_ORBIT:
        {
            Vec2 attraction = Force::GenerateGravitationalForce(*world->GetBodies()[0], *world->GetBodies()[1], 1000.0f, 5.0f, 100.0f);
            world->GetBodies()[0]->AddForce(attraction);
            world->GetBodies()[1]->AddForce(-attraction);
            break;
        }
        case SCENE_NORMAL_GRAVITY:
        {
            for (auto body : world->GetBodies())
            {
                Vec2 dragForce = Force::GenerateDragForce(*body, .002);
                body->AddForce(dragForce);

            }
            break;
        }
        case SCENE_ANGULAR_VELOCITY:
        {
            for (auto body : world->GetBodies())
            {
                Vec2 dragForce = Force::GenerateDragForce(*body, .002);
                body->AddForce(dragForce);
            }
            float torque = 2000.0f;
            world->AddTorque(torque);
            break;
        }
        case SCENE_BOX_DEFAULT:
        {
            float torque = 2000.0f;
            world->AddTorque(torque);
            break;
        }
        case SCENE_CIRCLES_COLLIDING:
        {
            break;
        }
        case SCENE_SAT:
        {
            break;
        }
        case SCENE_POLYGONCIRCLE:
        {
            break;
        }
        default:
            break;
    }

    world->Update(deltaTime);

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
        Graphics::DrawLine(anchor.x, anchor.y, world->GetBodies()[0]->position.x, world->GetBodies()[0]->position.y, 0xFF313131);

        for (int i = 0; i < NUM_BODIES - 1; ++i)
        {
            Graphics::DrawLine(world->GetBodies()[i]->position.x, world->GetBodies()[i]->position.y, world->GetBodies()[i + 1]->position.x, world->GetBodies()[i + 1]->position.y, 0xFF313131);
        }

        for (auto body : world->GetBodies())
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawFillCircle(body->position.x, body->position.y, circleShape->radius, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_FRICTION:
        for (auto body : world->GetBodies())
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

        for (auto body : world->GetBodies())
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, angle, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_ORBIT:
        if (world->GetBodies()[0]->shape->GetType() == ShapeType::CIRCLE && world->GetBodies()[1]->shape->GetType() == ShapeType::CIRCLE)
        {
            CircleShape* circleShape = (CircleShape*)world->GetBodies()[0]->shape;
            Graphics::DrawFillCircle(world->GetBodies()[0]->position.x, world->GetBodies()[0]->position.y, circleShape->radius, 0xFFAA3300);
            circleShape = (CircleShape*)world->GetBodies()[1]->shape;
            Graphics::DrawFillCircle(world->GetBodies()[1]->position.x, world->GetBodies()[1]->position.y, circleShape->radius, 0xFF00FFFF);
        }
        break;
    case SCENE_NORMAL_GRAVITY:
        for (auto body : world->GetBodies())
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, angle, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_ANGULAR_VELOCITY:
        for (auto body : world->GetBodies())
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
            }
        }
        break;
    case SCENE_BOX_DEFAULT:
        for (auto body : world->GetBodies())
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
        for (auto body : world->GetBodies())
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, angle, 0xFF00FF00);
            }
        }
        break;
    case SCENE_SAT:
        for (auto body : world->GetBodies())
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF00FF00);
            }
            else if (body->shape->GetType() == BOX)
            {
                BoxShape* boxShape = (BoxShape*)body->shape;
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFF00FF00);
            }
        }
        break;
    case SCENE_POLYGONCIRCLE:
        for (auto body : world->GetBodies())
        {
            if (body->shape->GetType() == ShapeType::CIRCLE)
            {
                CircleShape* circleShape = (CircleShape*)body->shape;

                if (!debug)
                {
                    Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2, body->rotation, body->texture);
                }
                else
                {
                    Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF00FF00);
                }
            }
            else if (body->shape->GetType() == BOX)
            {
                BoxShape* boxShape = (BoxShape*)body->shape;

                if (!debug)
                {
                    Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
                }
                else
                {
                    Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFF00FF00);
                }
            }
            if (body->shape->GetType() == POLYGON)
            {
                PolygonShape* polygonShape = (PolygonShape*)body->shape;
                Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF00FF00);
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
    case SDLK_9:
        currentScene = SCENE_SAT;
        break;
    case SDLK_0:
        currentScene = SCENE_POLYGONCIRCLE;
        break;
    default:
        currentScene = SCENE_NORMAL_GRAVITY;
        break;
    }
}

void Application::ResetScene(SDL_Keycode keyCode)
{
    for (auto body : world->GetBodies())
    {
        delete body;
    }
    world->GetBodies().clear();
    SwitchScene(keyCode);
    Setup();
}
///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete world;
    Graphics::CloseWindow();
}