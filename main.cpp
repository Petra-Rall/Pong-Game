#include<iostream>
#include<raylib.h>
using namespace std;

Color Green = Color{38, 185, 154, 255}; //RGB+ alpha value
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score =  0;

class Ball{
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;

        void Draw(){
            DrawCircle(x, y, radius, Yellow);
        }
        void Update(){
            x +=speed_x;
            y +=speed_y;
            //check for collision
            if(y + radius >= GetScreenHeight() || y - radius <= 0){
                speed_y *= -1;
            }
            if(x + radius >= GetScreenWidth()){ //CPU Wins
                cpu_score++;
                ResetBall();
            } 
            if(x - radius <= 0){
                player_score++;
                ResetBall();
            }
        }
        void ResetBall(){
            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;
            int speed_choices[2] = {-1, 1};
            speed_x *= speed_choices[GetRandomValue(0,1)];
            speed_x *= speed_choices[GetRandomValue(0,1)];
        }
};

class Paddle{
    protected:
        void LimitMovement(){
            if(y >= (GetScreenHeight() - height)){
                y = GetScreenHeight() - height;
            }
            if(y <= 0){
                y = 0;
            }
        }
    public:
        float x, y;
        float width, height;
        int speed;

        void Draw(){
            DrawRectangleRounded(Rectangle{x, y, width, height}, 0.9, 0, WHITE);
        }
        void Update(){
            if(IsKeyDown(KEY_UP)){
                y -= speed;
            }
            if(IsKeyDown(KEY_DOWN)){
                y += speed;
            }

            LimitMovement();
        }
};

class CpuPaddle: public Paddle{
    public:
    void Update(int ball_y){
        if(y + height/2 > ball_y){
            y -= speed;
        }
        if(y + height/2 <= ball_y){
            y += speed;
        }
        
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;
     

int main(){
    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong Game!");
    SetTargetFPS(60);


    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 125;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 125;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;
 
    while(WindowShouldClose() == false){

        BeginDrawing();
       
        //Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);


        //Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }

        //Drawing
        ClearBackground(Dark_Green); // So the screen is cleared before drawing the next frame
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);
        ball.Draw();
        //
        //DrawRectangle(10, screen_height/2-60, 25, 120, WHITE);//the origin of rectangle is at top left, so to place it in middle of y axis y= screen_heigth/2 - rectangle_height/2
        //DrawRectangle(screen_width-35, screen_height/2-60, 25, 120, WHITE);//From top left of rect, since its width is 25 and need to place it 10px from left side, its 25+10=35px from screen width 
        player.Draw();
        cpu.Draw();
        DrawLine(screen_width/2, 0, screen_width/2, 800, WHITE);
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width/4 - 20, 20, 80, WHITE);
        
        EndDrawing();

    }

    CloseWindow();

    return 0;

}