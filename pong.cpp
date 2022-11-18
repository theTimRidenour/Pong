#include "raylib.h"
#include <string>

main()
{
    // initialize window
    const int WIN_WIDTH = 512;
    const int WIN_HEIGHT = 256;
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "PONG");

    // universal timer
    const float updateTime = 1.0/30.0;
    float runningTime = 0.0;

    // player coordinates
    int playerOneX = 30;
    int playerOneY = (WIN_HEIGHT/2)-14;
    int playerTwoX = WIN_WIDTH-33;
    int playerTwoY = playerOneY;
    int playerCenter = 0;
    int playerWidth = 6;
    int playerHeight = 28;

    // player collision
    int l_pOne_x{playerOneX};
    int r_pOne_x(playerOneX + playerWidth);
    int u_pOne_y(playerOneY);
    int b_pOne_y{playerOneY + playerHeight};

    int l_pTwo_x{playerTwoX};
    int r_pTwo_x{playerTwoX + playerWidth};
    int u_pTwo_y{playerTwoY};
    int b_pTwo_y{playerTwoY + playerHeight};

    // player scores
    int playerOneScore = 0;
    char playerOneScoreText[3] = "";
    int playerTwoScore = 0;
    char playerTwoScoreText[3] = "";

    // ball coordinates and direction
    int ballX = (WIN_WIDTH/2);
    int ballY = (WIN_HEIGHT/2);
    int ballMoveX = 1;
    int ballMoveY = 1;
    int ballRadius = 3;

    // ball collisions
    int l_ball_x{ballX - ballRadius};
    int r_ball_x{ballX + ballRadius};
    int u_ball_y{ballY - ballRadius};
    int b_ball_y{ballY + ballRadius};

    bool collision_with_player_one = 
                    (b_pOne_y >= u_ball_y) && 
                    (u_pOne_y <= b_ball_y) && 
                    (r_pOne_x >= l_ball_x) && 
                    (l_pOne_x <= r_ball_x);

    bool collision_with_player_two = 
                    (b_pTwo_y >= u_ball_y) && 
                    (u_pTwo_y <= b_ball_y) && 
                    (r_pTwo_x >= l_ball_x) && 
                    (l_pTwo_x <= r_ball_x);

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // update edges
        l_ball_x = ballX - ballRadius;
        r_ball_x = ballX + ballRadius;
        u_ball_y = ballY - ballRadius;
        b_ball_y = ballY + ballRadius;
        l_pOne_x = playerOneX;
        r_pOne_x = playerOneX + playerWidth;
        u_pOne_y = playerOneY;
        b_pOne_y = playerOneY + playerHeight;
        l_pTwo_x = playerTwoX;
        r_pTwo_x = playerTwoX + playerWidth;
        u_pTwo_y = playerTwoY;
        b_pTwo_y = playerTwoY + playerHeight;

        // update collisions
        collision_with_player_one = 
                    (b_pOne_y >= u_ball_y) && 
                    (u_pOne_y <= b_ball_y) && 
                    (r_pOne_x >= l_ball_x) && 
                    (l_pOne_x <= r_ball_x);

        collision_with_player_two = 
                    (b_pTwo_y >= u_ball_y) && 
                    (u_pTwo_y <= b_ball_y) && 
                    (r_pTwo_x >= l_ball_x) && 
                    (l_pTwo_x <= r_ball_x);

        BeginDrawing();

        const float dT{ GetFrameTime() };
        runningTime += dT;
        
        // draw background
        ClearBackground(BLACK);
        DrawRectangle((WIN_WIDTH/2)-1, 0, 2, WIN_HEIGHT, WHITE);
        std::sprintf(playerOneScoreText, "%d", playerOneScore);
        DrawText(playerOneScoreText, 5, 1, 5, WHITE);
        std::sprintf(playerTwoScoreText, "%d", playerTwoScore);
        DrawText(playerTwoScoreText, WIN_WIDTH-10, 1, 5, WHITE);

        // draw players
        DrawRectangle(playerOneX, playerOneY, playerWidth, playerHeight, WHITE);
        DrawRectangle(playerTwoX, playerTwoY, playerWidth, playerHeight, WHITE);

        // Draw ball
        if (playerOneScore < 7 && playerTwoScore < 7) {
            DrawCircle(ballX, ballY, ballRadius, WHITE);
        }

        // check for winner
        if (playerOneScore >= 7 || playerTwoScore >= 7) {
            ballX=WIN_WIDTH/2;
            ballY=WIN_HEIGHT/2;
            DrawText("Game Over!", (WIN_WIDTH/2) + 5, (WIN_HEIGHT/2) - 23, 20, WHITE);
            if (playerOneScore >= 7) {
                DrawText("Player One Wins!!!", (WIN_WIDTH/2) + 5, WIN_HEIGHT/2, 20, WHITE);
            } else {
                DrawText("Computer Wins!!!", (WIN_WIDTH/2) + 5, WIN_HEIGHT/2, 20 , WHITE);
            }
        }

        if ( runningTime >= updateTime) {
            // player movement
            if (IsKeyDown(KEY_W)) {
                playerOneY -= 7;
                if (playerOneY < 10) {
                    playerOneY = 10;
                }
            } else if (IsKeyDown(KEY_S)) {
                playerOneY += 7;
                if (playerOneY > WIN_HEIGHT - playerHeight) {
                    playerOneY = WIN_HEIGHT - playerHeight;
                }
            }

            // computer movement
            if (ballMoveX > 0) {
                if (ballY > playerTwoY+14 && ballX >= (WIN_WIDTH/2)-10) {
                    playerTwoY += 6;
                    if (playerTwoY > WIN_HEIGHT - playerHeight) {
                        playerTwoY = WIN_HEIGHT - playerHeight;
                    }
                } else if (ballY < playerTwoY+14) {
                    playerTwoY -= 7;
                    if (playerTwoY < 10) {
                        playerTwoY = 10;
                    }
                }
            } 

            // ball movement
            if (collision_with_player_one || collision_with_player_two) {
                if (collision_with_player_one) {
                    playerCenter = u_pOne_y + 14;
                    ballX = r_pOne_x + 8;
                    collision_with_player_one = false;
                } else {
                    playerCenter = u_pTwo_y + 14;
                    ballX = l_pTwo_x - 8;
                    collision_with_player_two = false;
                }
                if (u_ball_y + ballRadius <= playerCenter) {
                    ballMoveY = -1;
                } else {
                    ballMoveY = 1;
                }
                ballMoveX = -ballMoveX;
            } else {
                // ball movement x-coordinate
                ballX += 7 * ballMoveX;
                if (ballX > WIN_WIDTH) {
                    ballX = WIN_WIDTH/2;
                    ballMoveX = -ballMoveX;
                    playerOneScore += 1;
                } else if (ballX < 0) {
                    ballX = WIN_WIDTH/2;
                    ballMoveX = -ballMoveX;
                    playerTwoScore += 1;
                }

                // ball movement y-coordinate
                ballY += 7 * ballMoveY;
                if (ballY > WIN_HEIGHT) {
                    ballY = WIN_HEIGHT;
                    ballMoveY = -1;
                } else if (ballY < 10) {
                    ballY = 10;
                    ballMoveY = 1;
                }
            }
            runningTime = 0.0;
        }
        EndDrawing();
    }
    CloseWindow();
}