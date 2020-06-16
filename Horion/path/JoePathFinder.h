#pragma once

#include "../../Utils/HMath.h"
#include "JoePath.h"
#include "JoeSegment.h"
#include "goals/JoeGoal.h"
#include "../../SDK/CBlockLegacy.h"
#include "JoeConstants.h"
#include <memory>

class JoePathFinder {
private:
	vec3_ti startPos;
	JoePath currentPath;
	C_BlockSource* region;
	std::shared_ptr<JoeGoal> goal;
public:
	bool terminateSearch = false;
	float pathSearchTimeout = 5.f;

	JoePathFinder(vec3_ti start, C_BlockSource* reg, std::shared_ptr<JoeGoal> goal);

	JoePath findPath();
	const JoePath& getCurrentPath() const;
};
