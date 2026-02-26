#pragma once

namespace Physics
{
	constexpr float GRAVITY = 1800.f; // downward acceleration
    constexpr float GROUND_Y = 440.f;   // y of the top surface of the ground
    constexpr float WINDOW_WIDTH = 640.f; // 벽 넓이
    constexpr float WINDOW_HEIGHT = 480.f; //벽 높이

    // Player
    constexpr float PLAYER_SPEED = 260.f;
    constexpr float PLAYER_JUMP = -700.f;
    constexpr float KICK_POWER = 3.0f;

    // Ball
    constexpr float BALL_BOUNCE = 0.72f;   // energy kept on ground bounce
    constexpr float BALL_FRICTION = 0.985f;  // horizontal damping multiplier per frame
    constexpr float MIN_BOUNCE_VY = 80.f;    // below this the ball just rests

    // Head-hit impulse
    constexpr float HEAD_BOUNCE_VY = -620.f;  // upward velocity applied to ball
    constexpr float HEAD_HORIZ_MUL = 0.5f;    // fraction of player vx carried to ball
    constexpr float HEAD_HORIZ_ADD = 200.f;   // extra horizontal from contact normal
}