#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>

// Window Constants
#define WIDTH 900
#define HEIGHT 600
#define MAX_TREES 20
#define MAX_STARS 150
#define MAX_LEAVES 100
#define MAX_FIREFLIES 20
#define MAX_RAIN 200
#define MAX_SNOW 200
#define MAX_BIRDS 4
#define GROUND_HEIGHT 80.0f

typedef enum { SPRING, SUMMER, AUTUMN, WINTER } Season;
typedef enum { NIGHT, RAIN, SNOW, DAY } Weather;

typedef struct { Vector2 pos; float size; } Star;
typedef struct { Vector2 pos; float speed; float phase; } Bird;
typedef struct { Vector2 pos; float speedY; float phase; float amp; } Leaf;
typedef struct { Vector2 pos; float speedY; } RainDrop;
typedef struct { Vector2 pos; float speedY; float speedX; } SnowFlake;
typedef struct { Vector2 pos; float timeOffset; } Firefly;

typedef struct {
    Vector2 pos;
    float growth;
    float maxLength;
    bool active;
} Tree;

Tree trees[MAX_TREES];
int treeCount = 0;

Star stars[MAX_STARS];
Bird birds[MAX_BIRDS];
Leaf leaves[MAX_LEAVES];
RainDrop rain[MAX_RAIN];
SnowFlake snow[MAX_SNOW];
Firefly fireflies[MAX_FIREFLIES];

Season currentSeason = SPRING;
Weather currentWeather = DAY;

float seasonTimer = 0.0f;
float windStrength = 1.0f;
float baseWind = 1.0f;

#define GRASS_GREEN (Color){20,80,20,255}

Color GetSkyColor()
{
    switch(currentWeather)
    {
        case NIGHT: return (Color){5,5,20,255};
        case RAIN:  return (Color){40,45,60,255};
        case SNOW:  return (Color){100,110,130,255};
        case DAY:   return (Color){100,160,240,255};
    }
    return BLACK;
}

Color GetLeafColor()
{
    switch(currentSeason)
    {
        case SPRING: return (Color){120,230,100,200};
        case SUMMER: return (Color){30,120,30,200};
        case AUTUMN: return (Color){220,120,20,200};
        case WINTER: return BLANK;
    }
    return GREEN;
}

void DrawBranch(Vector2 start,float length,float angle,float thickness,float sway,float growth)
{
    if(length < 4 || thickness < 0.5) return;

    float a = angle + sway;

    Vector2 end =
    {
        start.x + length*sinf(a)*growth,
        start.y - length*cosf(a)*growth
    };

    Color color = (currentSeason==WINTER)?
    (Color){80,70,60,255}:BROWN;

    if(length<15 && currentSeason!=WINTER)
        color = GetLeafColor();

    DrawLineEx(start,end,thickness*growth,color);

    DrawBranch(end,length*0.75f,angle+0.45f,thickness*0.7f,sway*1.1f,growth);
    DrawBranch(end,length*0.75f,angle-0.45f,thickness*0.7f,sway*1.1f,growth);
}

void InitSimulation()
{
    trees[treeCount++] =
    (Tree){ {WIDTH/2,HEIGHT-GROUND_HEIGHT},0,120,true };

    for(int i=0;i<MAX_STARS;i++)
        stars[i]=(Star){
            {(float)(rand()%WIDTH),
            (float)(rand()%(HEIGHT/2))},
            (float)(rand()%20)/10.0f};

    for(int i=0;i<MAX_BIRDS;i++)
        birds[i]=(Bird){
            {(float)(rand()%WIDTH),
            (float)(rand()%200+50)},
            (float)(rand()%40+60),
            (float)rand()};

    for(int i=0;i<MAX_LEAVES;i++)
        leaves[i]=(Leaf){
            {(float)(rand()%WIDTH),
            (float)(rand()%HEIGHT)},
            (float)(rand()%50+30),
            (float)rand(),
            (float)(rand()%20+10)};

    for(int i=0;i<MAX_RAIN;i++)
        rain[i]=(RainDrop){
            {(float)(rand()%WIDTH),
            (float)(rand()%HEIGHT)},
            (float)(rand()%400+500)};

    for(int i=0;i<MAX_SNOW;i++)
        snow[i]=(SnowFlake){
            {(float)(rand()%WIDTH),
            (float)(rand()%HEIGHT)},
            (float)(rand()%50+50),
            (float)(rand()%40-20)};

    for(int i=0;i<MAX_FIREFLIES;i++)
        fireflies[i]=(Firefly){
            {(float)(rand()%WIDTH),
            (float)(rand()%HEIGHT)},
            (float)rand()};
}

int main()
{
    InitWindow(WIDTH,HEIGHT,"");
    SetTargetFPS(60);

    InitSimulation();

    Camera2D camera={0};
    camera.offset=(Vector2){WIDTH/2,HEIGHT/2};
    camera.target=(Vector2){WIDTH/2,HEIGHT/2};
    camera.zoom=1;

    while(!WindowShouldClose())
    {
        float dt=GetFrameTime();
        float time=GetTime();

        seasonTimer+=dt;

        if(seasonTimer>30)
        {
            seasonTimer=0;
            currentSeason=(currentSeason+1)%4;
        }

        if(IsKeyPressed(KEY_ONE)) currentWeather=NIGHT;
        if(IsKeyPressed(KEY_TWO)) currentWeather=RAIN;
        if(IsKeyPressed(KEY_THREE)) currentWeather=SNOW;
        if(IsKeyPressed(KEY_FOUR)) currentWeather=DAY;

        if(IsKeyDown(KEY_RIGHT)) baseWind+=dt;
        if(IsKeyDown(KEY_LEFT)) baseWind-=dt;

        baseWind=Clamp(baseWind,0.1f,5);

        float gust=sinf(time*0.5f)*0.5f
        +(float)(rand()%100)/500.0f;

        windStrength=baseWind+gust;

        if(currentWeather==RAIN)
            windStrength*=1.5f;

        float sway=
        (0.05f*windStrength)*
        sinf(time*2);

        if(IsKeyDown(KEY_W)) camera.target.y-=200*dt;
        if(IsKeyDown(KEY_S)) camera.target.y+=200*dt;
        if(IsKeyDown(KEY_A)) camera.target.x-=200*dt;
        if(IsKeyDown(KEY_D)) camera.target.x+=200*dt;

        camera.zoom+=GetMouseWheelMove()*0.05f;
        camera.zoom=Clamp(camera.zoom,0.5f,3);

        for(int i=0;i<treeCount;i++)
            if(trees[i].growth<1)
                trees[i].growth+=0.1f*dt;

        // Update Objects
        for (int i = 0; i < MAX_BIRDS; i++) {
            birds[i].pos.x += birds[i].speed * dt;
            birds[i].pos.y += sinf(time * 2 + birds[i].phase) * 0.5f;
            if (birds[i].pos.x > WIDTH + 50) birds[i].pos.x = -50;
        }

        // Update Particles
        if (currentSeason != WINTER) {
            for (int i = 0; i < MAX_LEAVES; i++) {
                leaves[i].pos.y += leaves[i].speedY * dt;
                leaves[i].pos.x += sinf(time + leaves[i].phase) * leaves[i].amp * dt;
                if (leaves[i].pos.y > HEIGHT - GROUND_HEIGHT) {
                    leaves[i].pos.y = -20;
                    leaves[i].pos.x = (float)(rand() % WIDTH);
                }
            }
        }
        if (currentWeather == RAIN) {
            for (int i = 0; i < MAX_RAIN; i++) {
                rain[i].pos.y += rain[i].speedY * dt;
                if (rain[i].pos.y > HEIGHT) {
                    rain[i].pos.y = -10;
                    rain[i].pos.x = (float)(rand() % WIDTH);
                }
            }
        }
        if (currentWeather == SNOW) {
            for (int i = 0; i < MAX_SNOW; i++) {
                snow[i].pos.y += snow[i].speedY * dt;
                snow[i].pos.x += snow[i].speedX * dt;
                if (snow[i].pos.y > HEIGHT) {
                    snow[i].pos.y = -10;
                    snow[i].pos.x = (float)(rand() % WIDTH);
                }
            }
        }
        if (currentWeather == NIGHT) {
            for (int i = 0; i < MAX_FIREFLIES; i++) {
                fireflies[i].pos.x += sinf(time * 0.5f + fireflies[i].timeOffset) * 20 * dt;
                fireflies[i].pos.y += cosf(time * 0.3f + fireflies[i].timeOffset) * 15 * dt;
            }
        }

        BeginDrawing();

        ClearBackground(GetSkyColor());

        BeginMode2D(camera);

        if(currentWeather==DAY)
            DrawCircle(100,80,40,YELLOW);

        if(currentWeather==NIGHT)
        {
            for(int i=0;i<MAX_STARS;i++)
                DrawCircleV(stars[i].pos,
                stars[i].size,WHITE);

            DrawCircle(WIDTH-120,100,35,LIGHTGRAY);
        }
        
        for (int i = 0; i < MAX_BIRDS; i++) {
            DrawLine(birds[i].pos.x, birds[i].pos.y, birds[i].pos.x - 10, birds[i].pos.y - 5, BLACK);
            DrawLine(birds[i].pos.x, birds[i].pos.y, birds[i].pos.x + 10, birds[i].pos.y - 5, BLACK);
        }

        DrawRectangle(0,
        HEIGHT-GROUND_HEIGHT,
        WIDTH,
        GROUND_HEIGHT,
        GRASS_GREEN);

        for(int i=0;i<treeCount;i++)
            DrawBranch(
            trees[i].pos,
            trees[i].maxLength,
            0,
            15,
            sway,
            trees[i].growth);

        // Draw Particles
        if (currentSeason != WINTER) {
            for (int i = 0; i < MAX_LEAVES; i++)
                DrawCircleV(leaves[i].pos, 3, GetLeafColor());
        }

        if (currentWeather == RAIN) {
            for (int i = 0; i < MAX_RAIN; i++)
                DrawLine(rain[i].pos.x, rain[i].pos.y, rain[i].pos.x, rain[i].pos.y + 10, BLUE);
        }

        if (currentWeather == SNOW) {
            for (int i = 0; i < MAX_SNOW; i++)
                DrawCircleV(snow[i].pos, 2, WHITE);
        }

        if (currentWeather == NIGHT) {
            for (int i = 0; i < MAX_FIREFLIES; i++) {
                float glow = (sinf(time * 3 + fireflies[i].timeOffset) + 1.0f) * 0.5f;
                DrawCircleV(fireflies[i].pos, 2 + glow * 2, (Color){ 200, 255, 0, (unsigned char)(150 * glow) });
            }
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}