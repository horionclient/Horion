#include "JoeConstants.h"

#include <math.h>
float distanceToTicks(float dist) {
	if(dist == 0)
		return 0;

	float tmp = dist;
	int tickCnt = 0;
	while(true){
		float fallDist = minecraftFallVel(tickCnt);
		if(tmp <= fallDist){
			return tickCnt + tmp / fallDist;
		}
		tmp -= fallDist;
		tickCnt++;
	}
}
float minecraftFallVel(int ticks) {
	return (powf(0.98f, (float)ticks) - 1) * -3.92f;
}
float* generateFallNBlocksCost() {
	float* costs = new float[257];
	for(int i = 0; i < 257; i++){
		costs[i] = distanceToTicks((float)i) * (1.f / 20);
	}
	return costs;
}

const float* FALL_N_BLOCKS_COST = generateFallNBlocksCost();