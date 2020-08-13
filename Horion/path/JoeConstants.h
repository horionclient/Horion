#pragma once

constexpr float WALKING_SPEED = 4.32f;
constexpr float SPRINT_SPEED = 5.61f;
constexpr float WATER_SPEED = 1.8f;
constexpr float JUMP_TIME = 0.6f;
constexpr float PARKOUR_JUMP1_TIME = 0.5f;
constexpr float WATER_UP_SPEED = 2.5f;
constexpr float WATER_SINK_SPEED = 3.5f;

float minecraftFallVel(int ticks);

float distanceToTicks(float dist);

float* generateFallNBlocksCost();

extern const float* FALL_N_BLOCKS_COST;