

#include "raylib.h"
#include <unistd.h>

// Constant size of 100 (from raylib [core] example - 2D Camera system)

#define MAX_BUILDINGS 100

// screenWidth and screenHeight are both set to 800

const int screenWidth = 800;
const int screenHeight = 800;

/* Empty array 'buildings' created with type Rectangle and size of MAX_BUILDINGS
 *Empty array 'buildColors' created with type Color and size of MAX_BUILDINGS
 *These arrays are empty as values will be added later on
 *(from raylib [core] example - 2D Camera system) */

Rectangle buildings[MAX_BUILDINGS] = { 0 };
Color buildColors[MAX_BUILDINGS] = { 0 };

// Textures for player and enemy

static Texture2D playerTexture;
static Texture2D boostTexture;
static Texture2D shrinkTexture;
static Texture2D pinkTexture;
static Texture2D invincibleTexture;
static Texture2D smallInvincibleTexture;
static Texture2D defaultTexture;

static Texture2D enemyTexture;
static Texture2D defaultEnemyTexture;
static Texture2D biggerEnemyTexture;
static Texture2D massiveEnemyTexture;

bool gameOver = false;

// Empty Rectangle created. This will be altered later and will display one of the frames
// for the enemy square.

Rectangle enemyFrames = {};

float angryTime = 0.0f; // Timer for the enemy square's changing frames.

// Rectangle 'player' created
Rectangle player;

// Array 'walls' created with type Rectangle, and size of Array is set
Rectangle walls[4];

// Rectangle bigEnemy is created, and its speed variables.
Rectangle bigEnemy;
float bigEnemySpeedX;
float bigEnemySpeedY;
Vector2 enemyPosition; 
// This is for the enemy textures, as a texture with changing frames will only accept
// one Vector2 variable for its position.

int currentFrame; // Represents the current frame for the enemy texture

Rectangle pellet; // Rectangle pellet is created.

/** Integer wallArrayLength is assigned and calculated to be the number of values in 'walls' 
 * divided by the position value of the first values in 'walls'*/

int wallArrayLength = sizeof(walls) / sizeof(walls[0]);

// Many variables are created

int count;
int score;
int highscore;
float timer;
float timeForSpeed;

bool boosting;
float boostTimer;
int boostCount;

bool shrinking;
float shrinkTimer;
int shrinkCount;

bool invincible;
float invincibleTimer;
int invincibleCount;

// (raylib [core] example - 2D Camera system)
int spacing;

/** A Camera2D called camera is created that sets the position of the camera so the
 * player always spawns in the centre of the screen, and is later zoomed in slightly */

 // (From raylib [core] example - 2D Camera system)
Camera2D camera;


static void InitGame(void);         
static void UpdateGame(void);       
static void DrawGame(void);         
static void UnloadGame(void);       
static void UpdateDrawFrame(void);  

//------------------------------------------------------------------------------------------
// Main function of program
//------------------------------------------------------------------------------------------
int main(void)
{
    // Creates the screen for the game with the title below.
    InitWindow(screenWidth, screenHeight, "Game Project");

    // Initialises game
    InitGame();
    
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateDrawFrame(); // Updates and draws game for every frame
    }

    UnloadGame(); // Unloads textures

    // De-Initialization
    
    CloseWindow();        // Close window and OpenGL context
    
    return 0;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------


void InitGame(void) {

    // Loads all possible textures for the player
    defaultTexture = LoadTexture("resources/green_square.png");
    boostTexture = LoadTexture("resources/boost_square.png");
    shrinkTexture = LoadTexture("resources/shrink_square.png");
    pinkTexture = LoadTexture("resources/pink_square.png");
    invincibleTexture = LoadTexture("resources/invincible_square.png");
    smallInvincibleTexture = LoadTexture("resources/small_invincible_square.png");


    // The three textures below will have different frames, and are for the enemy

    defaultEnemyTexture = LoadTexture("resources/angry_frames.png");
    biggerEnemyTexture = LoadTexture("resources/furious_frames.png");
    massiveEnemyTexture = LoadTexture("resources/crazed_frames.png");

    // Rectangle 'player' has position and size is set
    player = (Rectangle){ 410, 225, 40, 40 };

    

    // All positions and sizes for each value in walls is set 
    walls[0] = (Rectangle){ 800, -300, 70, GetScreenHeight() + 10};
    walls[1] = (Rectangle){ -20, -300, 70, GetScreenHeight()};
    walls[2] = (Rectangle){ 0, -200, 800, 60};
    walls[3] = (Rectangle){ -20, 450, 820, 60};

    /** Rectangle bigEnemy spawns in a random position between limits on both axis.
     * And its initial size is 70 in terms of width and height. Its speed variables are set to 1 in either axis.*/

    bigEnemy = (Rectangle){GetRandomValue(200, 700), GetRandomValue(-50, 50), 70, 70};
    bigEnemySpeedX = 1;
    bigEnemySpeedY = 1;

    currentFrame = 0; // Represents the current frame for the enemy texture

    /** Rectangle pellet spawns in a random position between limits on both axis.
     * and its size is 10 in terms of width and height. */

    pellet = (Rectangle){ GetRandomValue(80, 700), GetRandomValue(100, 370), 10, 10};

    /** Integer wallArrayLength is assigned and calculated to be the number of values in 'walls' 
     * divided by the position value of the first values in 'walls'*/

    wallArrayLength = sizeof(walls) / sizeof(walls[0]);

    // Many variables are initialised (this will be their values at the beginning of each game)

    count = 0;
    score = 0;
    highscore = 0;
    timer = 0.0f;
    timeForSpeed = 0.0f;

    boosting = false;
    boostTimer = 0.0f;
    boostCount = 3;

    shrinking = false;
    shrinkTimer = 0.0f;
    shrinkCount = 3;

    invincible = false;
    invincibleTimer = 0.0f;
    invincibleCount = 1;

    // (raylib [core] example - 2D Camera system)
    spacing = 0;

    // Loop that repeats so long as int 'i' is less than the size of MAX_BUILDINGS

    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        /** Every value in buildings is assigned a random height and width within certain limits
         * and is assigned a position on the y-axis of -300.
         * For the position of each value in x-axis is at first set to -6000.0 + spacing (0).
         * spacing is then made equal to itself plus the width of the current building 'i', 
         * ensuring that no 'buildings' overlap. */
        // (From raylib [core] example - 2D Camera system)
        buildings[i].width = (float)GetRandomValue(50, 200);
        buildings[i].height = (float)GetRandomValue(600, 1500);
        buildings[i].y = -300;
        buildings[i].x = -6000.0f + spacing;
        
        spacing += (int)buildings[i].width;

        // Every value in buildColors is set to a random height and width within limits 
        /* and a random position on the x-axis and 255 on the y-axis */

        buildColors[i] = (Color){ GetRandomValue(200, 240), GetRandomValue(200, 240), GetRandomValue(200, 250), 255 };
    }
    

    /** A Camera2D called camera is created that sets the position of the camera so the
     * player always spawns in the centre of the screen, and in zoomed in slightly */

     // (From raylib [core] example - 2D Camera system)
    camera = (Camera2D){ 0 };
    camera.target = (Vector2){ player.x + 20.0f, player.y + 20.0f };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.zoom = 0.9f;

}

void UpdateGame(void) {

    // So long as the boolean 'gameOver' is false, the the rest of the if statement should run

        if(!gameOver) {

        angryTime += GetFrameTime(); // Becomes an actual timer

        // When the timer for the enemy textures reaches 0.5 seconds, the enemy frame changes
        // to the next one, and the timer resets to 0 seconds.
        if (angryTime >= 0.5f) {
            currentFrame++;
            angryTime = 0.0f;

            // Since there is only two frames in any of the enemy textures, the current frame
            // of the enemy textures resets to 0, to avoid currentFrame from going out of bounds.
            if(currentFrame > 1) {
                currentFrame = 0;
            }
        }
        
        // bigEnemy movement, so the position of bigEnemy becomes itself plus the value of bigEnemySpeed.

        bigEnemy.x += bigEnemySpeedX;
        bigEnemy.y += bigEnemySpeedY;

        // This is so the enemy texture moves alongside the bigEnemy square
        enemyPosition = (Vector2){(float)bigEnemy.x, (float)bigEnemy.y};

        /** player movement. If any one of the arrow keys is pressed, the position of player becomes itself
         * plus or minus 3 depending on the arrow key and direction */

        if (IsKeyDown(KEY_RIGHT)) player.x += 3;
        else if (IsKeyDown(KEY_LEFT)) player.x -= 3;
        if (IsKeyDown(KEY_UP)) player.y += -3;
        else if (IsKeyDown(KEY_DOWN)) player.y += 3;

        if (IsKeyPressed(KEY_A) && boostCount > 0) {
           boosting = true;
           boostTimer = 0.0f;
           /** So the boost ability is limited, whenever this if statement runs, boostCount
           * decreases until boostCount is 0 or lower. Then this statement cannot run until a retry*/
           boostCount -= 1;
        }

        /** If boolean boosting is true, boostTimer begins and becomes a timer due to GetFrameTime().
         * When boostTimer becomes equal to or greater than 5.00 seconds, boosting becomes false.
         * This makes it so the boost ability is only active for 5 seconds. */

        if(boosting) {
          boostTimer += GetFrameTime();

          // Increases player speed by 4
          if (IsKeyDown(KEY_RIGHT)) player.x += 4;
          else if (IsKeyDown(KEY_LEFT)) player.x -= 4;
          if (IsKeyDown(KEY_UP)) player.y -= 4;
          else if (IsKeyDown(KEY_DOWN)) player.y += 4;       

          if (boostTimer >= 5.0f) {
            boosting = false;
          }
        }
 
        if (IsKeyPressed(KEY_S) && shrinkCount > 0) {
           shrinking = true;
           shrinkTimer = 0.0f;
           /** So the shrink ability is limited, whenever this if statement runs, shrinkCount
           * decreases until shrinkCount is 0 or lower. Then this statement cannot run until a retry*/
           shrinkCount -= 1;
        }

        /** If boolean shrinking is true, shrinkTimer begins and becomes a timer due to GetFrameTime().
         * When shrinkTimer becomes equal to or greater than 5.00 seconds, shrinking becomes false.
         * This makes it so the shrinking ability is only active for 5 seconds. */

        if(shrinking) {
        shrinkTimer += GetFrameTime();

          // Shrinks player width and height to 20.
          player.width = 20;
          player.height = 20;

          if (shrinkTimer >= 5.0f) {
            shrinking = false;
            player.width = 40;
            player.height = 40;
          }
        }

        if(IsKeyPressed(KEY_D) && invincibleCount > 0) {
           invincible = true;
           invincibleTimer = 0.0f;
           /** So the invincibility ability is limited, whenever this if statement runs, invincibleCount
           * decreases until invincibleCount is 0 or lower. Then this statement cannot run until a retry*/
           invincibleCount -= 1;
        }

        /* If boolean invincible is true, invincibleTimer begins and becomes a timer due to GetFrameTime().
         * When invincibleTimer becomes equal to or greater than 5.00 seconds, invincibility becomes false.
         * This makes it so the invincible ability is only active for 5 seconds. */
        if(invincible) {
          invincibleTimer += GetFrameTime();
          if(invincibleTimer >= 5.0f) invincible = false;
        }

        /* Loop that checks if player has collided with any of the values in the 'walls' array.
         * If so, boolean gameOver becomes true and the game ends.*/

        for(int i = 0; i < wallArrayLength; i++) {
            if (CheckCollisionRecs(player, walls[i])) {
                gameOver = true;
              }
        }

        /* If player has collided with bigEnemy, boolean gameOver becomes true and the game ends,
         * unless boolean invincible is true. If invincible is true, bigEnemy has no effect on player*/

        if (CheckCollisionRecs(player, bigEnemy) && !invincible) {
            gameOver = true;
        }

        /* Loop that checks if bigEnemy has collided with the first 2 values in the 'walls' array, the
         * walls on the left and right. If so, bigEnemySpeedX is multiplied by -1, making bigEnemy
         * go in the other direction after collision.
        */

        for(int i = 0; i < wallArrayLength/2; i++) {
            if (CheckCollisionRecs(bigEnemy, walls[i])) {
                bigEnemySpeedX *= -1;
            }
        }

        /* Loop that checks if bigEnemy has collided with the last 2 values in the 'walls' array, the
         * walls on the top and bottom. If so, bigEnemySpeedY is multiplied by -1, making bigEnemy
         * go in the other direction after collision.
        */

        for(int i = 2; i < wallArrayLength; i++) {
            if (CheckCollisionRecs(bigEnemy, walls[i])) {
                bigEnemySpeedY *= -1;
            }
        }

        // Float 'timer' is made into an actual timer due to GetFrameTime().
        timer += GetFrameTime();

        /* So long as gameOver is false, if timer becomes equal to or greater than 1.0 seconds,
         * integer count increases by 1, and timer resets. This makes it so count increases every second. */ 
        if(!gameOver) {
            if(timer >= 1.0f) {
            count += 1;
            timer = 0.0f;
            }
        }

        // Float 'timeForSpeed' is made into an actual timer due to GetFrameTime().
        timeForSpeed += GetFrameTime();

        /* So long as gameOver is false, if timeForSpeed becomes equal to or greater than 5.0 seconds,
         * the speeds of bigEnemy are multiplied by 1.25, and timer resets. 
         * This makes it so the speed of bigEnemy increases every five seconds. */ 
        if(!gameOver) {
            if (count <= 50) // Stops increasing speed after 50 seconds {
                if(timeForSpeed >= 5.0f) {
                    bigEnemySpeedX *= 1.25;
                    bigEnemySpeedY *= 1.25;
                    timeForSpeed = 0.0f;
                }  
            }
        

        /* If the player collides with pellet, the position of pellet changes randomly within limits.
         * The 'score' variable increases by 10 every time they collide. */ 

        if(CheckCollisionRecs(player, pellet)) {
            pellet.x = GetRandomValue(80, 700);
            pellet.y = GetRandomValue(-100, 370);
            score += 10;
        }

        /* If statements that changes the texture of the player to different textures depending
                 * on which abilities are or aren't active. The default texture is green.
                */
                
                if(boosting && !shrinking && !invincible) {
                    playerTexture = boostTexture;
                    }
                else if(shrinking && !boosting && !invincible) {
                    playerTexture = shrinkTexture;
                }
                else if(boosting && shrinking && !invincible) {
                    playerTexture = pinkTexture;
                }
                else if((invincible && !shrinking) || (invincible && boosting && !shrinking)) {
                    playerTexture = invincibleTexture;
                }
                else if((invincible && shrinking) || (invincible && shrinking && boosting)) {
                    playerTexture = smallInvincibleTexture;
                }
                else {
                    playerTexture = defaultTexture;
                }

                // If count is less than 30, bigEnemy is drawn as red.

                if(count < 30) {
                    enemyTexture = defaultEnemyTexture;
                }

                else {
                    /* If count is greater than 30 and less than 50, the height and width
                     * of bigEnemy both increase to 100, and bigEnemy's texture is changed. */ 
                    if (count >= 30 && count < 50) {
                        bigEnemy.height = 100;
                        bigEnemy.width = 100;
                        enemyTexture = biggerEnemyTexture;
                    }
                    /* If count is greater than 50, the height and width of bigEnemy both 
                     * increase to 130, and bigEnemy's texture is changed again. */ 
                    else {
                        bigEnemy.height = 130;
                        bigEnemy.width = 130;
                        enemyTexture = massiveEnemyTexture;
                    }
             }

             // enemyFrames is set to accomodate each frame of its texture, with the width of the texture
             // being halved as the texture itself is basically both frames combined into one.
             enemyFrames = (Rectangle){0.0f, 0.0f, (float)enemyTexture.width / 2, enemyTexture.height};

             // The portion of the texture changes depending on the value of currentFrame, which should be
             // changing every 0.5 seconds. 
             // If currentFrame is 0, the first half of the texture is displayed, and if currentFrame is 1,
             // the second hald of the texture is shown.
             enemyFrames.x = (float) currentFrame * (float)(enemyTexture.width / 2);

            }

            // If the game is over
            if(gameOver) { 
                if(IsKeyPressed(KEY_R)) {
                    InitGame(); // Pressing R restarts the game and initialises everything
                    gameOver = false;
                 }
                if(IsKeyPressed(KEY_ESCAPE)) CloseWindow(); // Pressing escape closes the game window
            }
}

void DrawGame(void) {{
    //----------------------------------------------------------------------------------
        // Begins to draw everything
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                /* Loop that draws every building in with size and position of 'buildings' 
                 * and with the colour of buildColors */ 
                
                for (int i = 0; i < MAX_BUILDINGS; i++) DrawRectangleRec(buildings[i], buildColors[i]);    
                
                // The player's texture is drawn in the position of the player.

                DrawTexture(playerTexture, (int)(player.x), (int)(player.y), WHITE);

                // The pellet is drawn as brown
                DrawRectangleRec(pellet, BROWN);

                // Every wall in the 'walls' array is drawn as black
                for(int i = 0; i < wallArrayLength; i++) {
                    DrawRectangleRec(walls[i], BLACK);
                }  

             // The texture for the enemy square is drawn.
             DrawTextureRec(enemyTexture, enemyFrames, enemyPosition, WHITE);
        }
        
            EndMode2D();

            // Creates a red border around the screen

            DrawRectangle(0, 0, screenWidth, 5, RED);
            DrawRectangle(0, 5, 5, screenHeight - 10, RED);
            DrawRectangle(screenWidth - 5, 5, 5, screenHeight - 10, RED);
            DrawRectangle(0, screenHeight - 5, screenWidth, 5, RED);

            // If gameOver is false, control menu and score are drawn on the screen.

            if(!gameOver) {

            // Draws UI
            
            DrawRectangle(5, 635, 790, 160, WHITE);
            DrawRectangle(5, 635, 790, 160, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(5, 635, 790, 160, BLUE);

            DrawText("CONTROLS:", 15, 655, 20, BLACK);
            DrawText("- Right/Left/Up/Down to move", 25, 675, 15, DARKGRAY);
            DrawText("- 'A' to Boost", 25, 690, 15, DARKGRAY);
            DrawText(TextFormat("- Boosts Remaining: %d", boostCount), 25, 705, 15, DARKGRAY);
            DrawText("- 'S' to Shrink", 25, 720, 15, DARKGRAY);
            DrawText(TextFormat("- Shrinks Remaining: %d", shrinkCount), 25, 735, 15, DARKGRAY);
            DrawText("- 'D' for Invincibility", 25, 750, 15, DARKGRAY);
            DrawText(TextFormat("- Invincibility Remaining: %d", invincibleCount), 25, 765, 15, DARKGRAY);
            DrawText(TextFormat("Score: %d", score), 500, 680, 50, BLACK); 
        
        }

            // If gameOver is true, the game over screen is drawn.
            
            if (gameOver) {
                // If the current score is greater than highscore, highscore becomes score
                if(score > highscore) highscore = score;
                ClearBackground(BLACK);
                DrawText("GAME OVER", 250, 350, 50, RED);
                DrawText(TextFormat("Highscore: %d", highscore), 330, 450, 20, RED);
                DrawText("Press R to Restart", 300, 500, 20, BLUE);
                DrawText("Press Esc to Exit", 310, 550, 20, BLUE);
            }

        EndDrawing();

}

void UnloadGame(void) {
    UnloadTexture(playerTexture); 
    UnloadTexture(enemyTexture);
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}


