#pragma once

#include "../../Utils/HMath.h"
#include "JoePath.h"
#include "JoeSegment.h"
#include "../../SDK/CBlockLegacy.h"

class JoePathFinder {
private:
	vec3_ti startPos;
	JoePath currentPath;
	C_BlockSource* region;
public:
	JoePathFinder(vec3_ti start, C_BlockSource* reg);

	JoePath findPathTo(vec3_ti endNode);
	const JoePath& getCurrentPath() const;
};
