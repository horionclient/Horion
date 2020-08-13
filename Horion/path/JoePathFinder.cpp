#include "JoePathFinder.h"

#include <vector>
#include <queue>
#include "../../Utils/Logger.h"
#include "../../Utils/Utils.h"
#include <chrono>
#include <unordered_map>
#include <numeric>
#include "../../Memory/GameData.h"

JoePathFinder::JoePathFinder(vec3_ti start, C_BlockSource* reg, std::shared_ptr<JoeGoal> goal) : startPos(start), region(reg), goal(goal) {
}

struct NodeRef {
	__int64 hash;
	__forceinline bool isInvalid() const{
		return hash == 0xFFFFFFFFFFFFFFFF;
	}
	NodeRef(__int64 hash) : hash(hash) {}
	NodeRef() : hash(-1) {};
};

struct Node {
	vec3_ti pos;
	float fScore; // heuristic
	float gScore; // total cost
	struct {
		NodeRef nodeBefore = { -1 };
		JoeSegmentType edgeType = JoeSegmentType::INVALID;
	} cameFrom;
	bool isClosed;
	bool isInOpenSet;

	Node(const vec3_ti& pos, float fScore, float gScore) : pos(pos), fScore(fScore), gScore(gScore), isClosed(false), isInOpenSet(false) {}
};

struct Edge {
	NodeRef startNode, endNode;
	float cost;
	JoeSegmentType type;

	Edge(const NodeRef& startNode, const NodeRef& endNode, float cost, JoeSegmentType type) : startNode(startNode), endNode(endNode), cost(cost), type(type) {}
};
__forceinline unsigned __int64 rotBy(int in, unsigned int by){
	auto mut = static_cast<unsigned __int64>(in);
	return ((mut & 0x7FFFFFui64) | ((static_cast<unsigned int>(in) >> 8u) & 0x800000u)/*copy sign bit*/) << by;
}

__forceinline unsigned __int64 posToHash(const vec3_ti& pos){
	return rotBy(pos.x, 0) | rotBy(pos.z, 24) | (static_cast<unsigned __int64>(pos.y) << 48u);
}

NodeRef findNode(std::unordered_map<unsigned __int64, Node>& allNodes, vec3_ti& pos){
	auto posHash = posToHash(pos);
	auto res = allNodes.find(posHash);
	if(res != allNodes.end()){
		return NodeRef(posHash);
	}

	allNodes.emplace(posHash, Node(pos, 0.f, 10000000.f));
	return NodeRef(posHash);
}

__forceinline bool isDangerous(const vec3_ti& pos, C_BlockSource* reg, bool allowWater){
	auto obs1 = reg->getBlock(pos)->toLegacy();
	if(obs1->material->isSuperHot)
		return true;
	if(!allowWater && (obs1->material->isLiquid || obs1->hasWater(reg, pos)))
		return true;

	// contact damage based
	{
		// there is a function called dealsContactDamage but it takes in so many parameters + plant growth that its not useful anymore
		static uintptr_t** cactusBlockVtable = nullptr;
		if (cactusBlockVtable == nullptr) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 41 88 9E ?? ?? ?? ?? 41 C6");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			cactusBlockVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		}
		static uintptr_t** cobwebVtable = nullptr;
		if (cobwebVtable == nullptr) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 06 C6 86 ?? 00 00 00 00 0F B6 05 ?? ?? ?? ?? 88 86 ?? ?? ?? ?? C6 86 ?? ?? ?? ?? ?? C7 86 ?? ?? ?? ?? ?? ?? ?? ?? 48");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			cobwebVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		}
		static uintptr_t** witherRoseVtable = nullptr;
		if (witherRoseVtable == nullptr) {
			uintptr_t sigOffset = 0;
			if(g_Data.getVersion() == GAMEVERSION::g_1_16_0)
				sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 48 B9");
			else
				sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 06 48 B9");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			witherRoseVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		}
		static uintptr_t** magmaBlockVtable = nullptr;
		if (magmaBlockVtable == nullptr) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 41 88 9E ?? ?? ?? ?? 41 88 9E ?? ?? ?? ?? 41");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			magmaBlockVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		}

		if(obs1->Vtable == cactusBlockVtable)
			return true;
		if(obs1->Vtable == cobwebVtable)
			return true;
		if(obs1->Vtable == witherRoseVtable)
			return true;
		if(obs1->Vtable == magmaBlockVtable)
			return true;
		// there should be a sweet berry vtable here as well but the vtable was really aids so i resorted to block names
		if(obs1->tileName.getTextLength() > 20 && strcmp(obs1->tileName.getText() + 5 /*cutoff tile. prefix*/, "sweet_berry_bush") == 0)
			return true;
	}
	return false;
}
__forceinline bool isDangerousPlayer(const vec3_ti& pos, C_BlockSource* reg, bool allowWater = false){
	return isDangerous(pos, reg, allowWater) || isDangerous(pos.add(0, 1, 0), reg, allowWater);
}

__forceinline bool canStandOn(const vec3_ti& pos, C_BlockSource* reg, bool inWater = false){
	auto block = reg->getBlock(pos);
	auto standOn = block->toLegacy();
	bool validWater = inWater && standOn->hasWater(reg, pos);
	if(validWater){
		// block above has to be water as well
		auto swimPos = pos.add(0, 1, 0);
		auto swimIn = reg->getBlock(swimPos)->toLegacy();
		validWater = swimIn->hasWater(reg, swimPos);
	}
	if(!standOn->material->isSolid && !validWater)
		return false;
	if(isDangerous(pos, reg, inWater))
		return false;

	if(validWater)
		return true;

	AABB aabb;
	if(!standOn->getCollisionShape(&aabb, block, reg, &pos, nullptr))
		return false;

	auto diff = aabb.lower.sub(aabb.upper);

	if(ceilf(aabb.upper.y) - aabb.upper.y > 0.13f /* 0.125 for soulsand and farmland*/)
		return false;

	if(ceilf(aabb.upper.x) - aabb.upper.x > 0.07f /* 0.0625 for chests*/)
		return false;
	if(ceilf(aabb.upper.z) - aabb.upper.z > 0.07f /* 0.0625 for chests*/)
		return false;
	return fabsf(diff.x) > 0.85f && fabsf(diff.z) > 0.85f;
}
__forceinline bool isObstructed(const vec3_ti& pos, C_BlockSource* reg, bool allowWater = false){
	auto block = reg->getBlock(pos);
	auto obs1 = block->toLegacy();
	if(obs1->material->isBlockingMotion)
		return true;

	AABB aabb{};
	bool hasBox = obs1->getCollisionShape(&aabb, block, reg, &pos, nullptr);

	if(hasBox)
		return true;

	return isDangerous(pos, reg, allowWater);
}
__forceinline bool isObstructedPlayer(const vec3_ti& pos, C_BlockSource* reg, bool allowWater = false){
	return isObstructed(pos, reg, allowWater) || isObstructed(pos.add(0, 1, 0), reg);
}

std::vector<Edge> findEdges(std::unordered_map<unsigned __int64, Node>& allNodes, const Node& startNode, C_BlockSource* reg, NodeRef startNodeRef){
	std::vector<Edge> edges;
	auto startBlock = reg->getBlock(startNode.pos)->toLegacy();
	bool isInWater = startBlock->hasWater(reg, startNode.pos);

	float maxWalkSpeed = isInWater ? WATER_SPEED : SPRINT_SPEED;

	static const float SQRT2 = sqrtf(2);

	const float diagonalSpeed = SQRT2 / maxWalkSpeed;
	const float straightSpeed = 1 / maxWalkSpeed;
	const float diagonalSlowSpeed = SQRT2 / fminf(maxWalkSpeed, WALKING_SPEED);
	const float walkOffBlockTime = 0.8f / maxWalkSpeed;

	if(isInWater){
		{
			auto mod = startNode.pos.add(0, 1, 0);
			auto block = reg->getBlock(mod);
			if (block->toLegacy()->material->isLiquid && !block->toLegacy()->material->isSuperHot) {
				if (!isObstructed(startNode.pos.add(0, 1, 0), reg, true) && !isObstructed(startNode.pos.add(0, 2, 0), reg, true))
					edges.emplace_back(startNodeRef, findNode(allNodes, mod), 1 / WATER_UP_SPEED, JoeSegmentType::WATER_WALK);
			}
		}
		if(!isObstructed(startNode.pos.add(0, -1, 0), reg, true) && canStandOn(startNode.pos.add(0, -2, 0), reg, true)){
			auto mod = startNode.pos.add(0, -1, 0);
			edges.emplace_back(startNodeRef, findNode(allNodes, mod), 1 / WATER_SINK_SPEED, JoeSegmentType::WATER_WALK);
		}
	}

	for(int x = -1; x <= 1; x++){
		for(int z = -1; z <= 1; z++){
			if(x == 0 && z == 0)
				continue;
			bool isDiagonal = x != 0 && z != 0;

			vec3_ti newPos = startNode.pos.add(x,0, z);

			// lower block obstructed
			if(isObstructed(newPos, reg, true)){
				// maybe jump?
				if(isDiagonal)
					continue;

				if(!canStandOn(newPos, reg))
					continue;
				if(isObstructed(startNode.pos.add(0, 2, 0), reg))
					continue; // can we jump straight up?

				newPos = newPos.add(0, 1, 0);
				if(isObstructedPlayer(newPos, reg))
					continue;

				edges.emplace_back(startNodeRef, findNode(allNodes, newPos), JUMP_TIME, isInWater ? JoeSegmentType::WATER_WALK : JoeSegmentType::JUMP);
				continue;
			}

			// Check if we can stand on the block
			if(!canStandOn(newPos.add(0, -1, 0), reg, isInWater)){
				if(isDiagonal || isInWater)
					continue;
				// maybe drop?

				if(isObstructedPlayer(newPos, reg)) // walk to drop
					continue;

				// Drop down
				{
					int numWaterBlocks = 0;
					int dropLength = 0;
					while(dropLength < 3){
						dropLength++;
						auto dropPos = newPos.add(0, -1 * dropLength, 0); // drop down 1 block

						if(isObstructed(dropPos, reg, true)){// block below walk to drop
							dropLength = -1;
							break;
						}

						if(!canStandOn(dropPos.add(0, -1, 0), reg, false)) // block to stand on after drop
							continue;

						int waterDepth = 0;
						while(waterDepth < dropLength){
							auto testPos = dropPos.add(0, waterDepth, 0);
							auto blockTest = reg->getBlock(testPos)->toLegacy();
							if(!blockTest->hasWater(reg, testPos))
								break;

							waterDepth++;
						}
						if(waterDepth > 0){
							dropPos = dropPos.add(0, waterDepth - 1, 0);
						}

						const float dropTime = FALL_N_BLOCKS_COST[dropLength] + walkOffBlockTime;
						edges.emplace_back(startNodeRef, findNode(allNodes, dropPos), dropTime, JoeSegmentType::DROP);
						dropLength = -1;
						break; // Also allow parkour jump
					}
					if(dropLength == 3){ // no drop found, lets try water drops
						auto dropPos = newPos.add(0, -1 * dropLength, 0);

						while(dropPos.y > 1){
							dropPos.y--;
							dropLength++;

							if(isObstructed(dropPos, reg, true))// block below walk to drop
								break;

							auto block = reg->getBlock(dropPos)->toLegacy();
							auto isWaterBlock = block->hasWater(reg, dropPos);
							if(isWaterBlock)
								numWaterBlocks++;
							else{
								numWaterBlocks = 0;
								continue;
							}

							if(!canStandOn(dropPos.add(0, -1, 0), reg) && numWaterBlocks < 17 /*we dont need a block to stand on with that much water*/) // block to stand on after drop
								continue;

							// find out how deep the water is
							int waterDepth = 1;
							while(waterDepth < 19){ // make sure we don't drop too deep
								auto testPos = dropPos.add(0, waterDepth, 0);
								auto blockTest = reg->getBlock(testPos)->toLegacy();
								if(!blockTest->hasWater(reg, testPos))
									break;

								waterDepth++;
							}

							const float dropTime = FALL_N_BLOCKS_COST[dropLength] + walkOffBlockTime;
							dropPos = dropPos.add(0, waterDepth - 1, 0);
							edges.emplace_back(startNodeRef, findNode(allNodes, dropPos), dropTime, JoeSegmentType::DROP);
							break;
						}
					}
				}

				// maybe parkour jump?
				bool canJump = !isObstructed(startNode.pos.add(0, 2, 0), reg);// directly above our head

				if(isObstructed(newPos.add(0, 2, 0), reg)) // above old walk target
					canJump = false;

				newPos = startNode.pos.add(x * 2,0, z * 2); // 2 block distance, 1 block gap
				if(isObstructedPlayer(newPos, reg)){// landing zone
					// maybe jump up?
					if(!canJump)
						continue;
					auto jumpPos = newPos.add(0, 1, 0);

					if(!canStandOn(jumpPos.add(0, -1, 0), reg))
						continue;
					if(isObstructedPlayer(jumpPos, reg, false))
						continue;

					edges.emplace_back(startNodeRef, findNode(allNodes, jumpPos), PARKOUR_JUMP1_TIME, JoeSegmentType::PARKOUR_JUMP_SINGLE);
					continue;
				}

				if(!canStandOn(newPos.add(0, -1, 0), reg)){ // we can't stand on parkour jump landing zone, move it down and walk there?
					for(int dropLength = 1; dropLength <= 3; dropLength++){
						auto dropPos = newPos.add(0, -1 * dropLength, 0);
						if(isObstructed(dropPos, reg)) // we can't move it down, something in the way
							goto tryLargerParkourJump;
						if(isObstructed(startNode.pos.add(x,-1 * dropLength, z), reg, false))
							goto tryLargerParkourJump;

						// can we stand
						if(!canStandOn(dropPos.add(0, -1, 0), reg)) // we can't stand on the lowered landing zone :(
							continue;

						// walk to lower landing zone
						float time = fmaxf((2 + 0.05f /*small penalty*/) / maxWalkSpeed, FALL_N_BLOCKS_COST[dropLength] + walkOffBlockTime);
						edges.emplace_back(startNodeRef, findNode(allNodes, dropPos), time, JoeSegmentType::WALK);
						goto tryLargerParkourJump;
					}
					goto tryLargerParkourJump;
				}

				if(canJump){
					edges.emplace_back(startNodeRef, findNode(allNodes, newPos), PARKOUR_JUMP1_TIME, JoeSegmentType::PARKOUR_JUMP_SINGLE);
					continue; // we don't need to try a larger jump, we could just walk there
				}
			tryLargerParkourJump:
				// TODO

				continue;
			}

			// upper block obstructed?
			if(isObstructed(newPos.add(0, 1, 0), reg))
				continue;

			int isDiagonalObstructed = 0;
			if(isDiagonal){
				// Check if either x or z are obstruction-less
				isDiagonalObstructed += isObstructedPlayer(startNode.pos.add(x, 0, 0), reg, true);
				isDiagonalObstructed += isObstructedPlayer(startNode.pos.add(0, 0, z), reg, true);
				if(isDiagonalObstructed == 2) // both obstructed
					continue;

				// Check if both x and z aren't dangerous (we don't want to run into cacti)
				if(isDangerousPlayer(startNode.pos.add(x, 0, 0), reg) || isDangerousPlayer(startNode.pos.add(0, 0, z), reg))
					continue;
			}

			float cost = isDiagonal ? (isDiagonalObstructed ? diagonalSlowSpeed : diagonalSpeed) : straightSpeed;
			if(isInWater){
				// check if the block is a flowing block
				auto block = reg->getBlock(newPos)->toLegacy();
				if(block->material->isLiquid){
					vec3_t flow{};
					block->liquidGetFlow(&flow, reg, &newPos);
					if(!flow.iszero()){
						auto tangent = newPos.sub(startNode.pos).toVec3t();
						tangent.y = 0;
						tangent = tangent.normalize();

						auto prevCost = cost;
						cost = 1 / fmaxf(0.1f, maxWalkSpeed + tangent.dot(flow) * 0.07f * 20);
					}
				}
			}
			edges.emplace_back(startNodeRef, findNode(allNodes, newPos), cost, isInWater ? JoeSegmentType::WATER_WALK : JoeSegmentType::WALK);
		}
	}

	return edges;
}

std::pair<float, float> getSlope(std::vector<float>& x, std::vector<float>& y){
	const auto n    = x.size();
	const auto s_x  = std::accumulate(x.begin(), x.end(), 0.0);
	const auto s_y  = std::accumulate(y.begin(), y.end(), 0.0);
	const auto s_xx = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
	const auto s_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
	const auto a    = (n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);
	return std::make_pair((float)a, (float)((s_y / n) - (a * (s_x / n))));
}

JoePath JoePathFinder::findPath() {
	if(this->goal->isInGoal(startPos))
		return JoePath();
	std::unordered_map<unsigned __int64, Node> allNodes;

	auto cmp = [&](NodeRef left, NodeRef right) {
		if(left.isInvalid())
			return false;
		if(right.isInvalid())
			return true;
		return allNodes.at(left.hash).fScore > allNodes.at(right.hash).fScore;
	};
	std::priority_queue<NodeRef, std::vector<NodeRef>, decltype(cmp)> openSet(cmp);

	auto startHash = posToHash(startPos);
	allNodes.emplace(startHash, Node(startPos, this->goal->getHeuristicEstimation(startPos), 0.f));
	openSet.emplace(startHash);

	int numNodes = 0;
	int numEdges = 0;

	if(this->pathSearchTimeout < 0 || this->pathSearchTimeout > 50)
		this->pathSearchTimeout = 10;

	auto pathSearchStart = std::chrono::high_resolution_clock::now();

	while(!openSet.empty()){
		auto curRef = openSet.top();
		openSet.pop();
		if(curRef.isInvalid())
			continue;
		Node& cur = allNodes.at(curRef.hash);

		numNodes++;

		if(this->terminateSearch)
			break;

		if(this->goal->isInGoal(cur.pos) || numNodes % 1200 == 0){
			std::vector<JoeSegment> segments;
			auto node = cur;

			while(node.pos != startPos){
				auto prev = node.cameFrom;
				auto prevNode = allNodes.at(prev.nodeBefore.hash);

				segments.emplace_back(prev.edgeType, prevNode.pos, node.pos, node.gScore - prevNode.gScore);
				node = prevNode;
			}
			std::reverse(segments.begin(), segments.end());

			if(this->goal->isInGoal(cur.pos)){
				auto now = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> diff = now - pathSearchStart;
				logF("Found path! Traversal: %.2f Segments: %i Time: %.2fs Total Nodes: %i NodesVisited: %i Edges: %i", cur.gScore, segments.size(), diff.count(), allNodes.size(), numNodes, numEdges);
				return JoePath(segments, false);
			}

			this->currentPath = JoePath(segments, false);

			// check for timeout
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> diff = now - pathSearchStart;
			if(diff.count() > this->pathSearchTimeout)
				break;
		}
		cur.isClosed = true;
		cur.isInOpenSet = false;

		auto edges = findEdges(allNodes, cur, this->region, curRef); // cur gets invalidated here
		cur = allNodes.at(curRef.hash);
		numEdges += (int)edges.size();
		for(auto edge : edges){
			auto& edgeEndNode = allNodes.at(edge.endNode.hash);
			//logF("(%i %i %i) %i -> (%i %i %i)", cur.pos.x, cur.pos.y, cur.pos.z, edge.type, edgeEndNode.pos.x, edgeEndNode.pos.y, edgeEndNode.pos.z);
			if(edgeEndNode.isClosed)
				continue;
			float tentativeScore = cur.gScore + edge.cost;
			if(tentativeScore >= edgeEndNode.gScore)
				continue;
			float heuristic = tentativeScore + this->goal->getHeuristicEstimation(edgeEndNode.pos);

			edgeEndNode.cameFrom.edgeType = edge.type;
			edgeEndNode.cameFrom.nodeBefore = curRef;
			edgeEndNode.gScore = tentativeScore;
			edgeEndNode.fScore = heuristic;

			if(!edgeEndNode.isInOpenSet){ // not in open set
				edgeEndNode.isInOpenSet = true;
				openSet.push(edge.endNode);
			}else{
				// remove from openset
				openSet.push(edge.endNode);
			}
		}
	}
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> diff = now - pathSearchStart;

	if(this->terminateSearch)
		return JoePath();

	const float coefficients[] = { 1.1f, 1.5f, 2.f, 2.5f, 3.f, 3.5f, 4.f, 3.5f, 5.f, 10.f, 20.f, 30.f };
	constexpr auto coefficientSize = 12;
	constexpr float maxCost = 1000000;
	float bestHeuristicSoFar[coefficientSize] = { };
	std::fill_n(bestHeuristicSoFar, coefficientSize, maxCost);
	NodeRef bestSoFar[coefficientSize] = {};

	int numDist[coefficientSize][150] = {};
	float heuristicByDist[coefficientSize][150] = {};
	for(auto& nodeDesc : allNodes){

		auto& node = nodeDesc.second;
		auto dist = (int)roundf(node.pos.toVec3t().dist(startPos.toVec3t()));

		for(int i = 0; i < coefficientSize; i++){

			float heuristic = (node.fScore - node.gScore) + node.gScore / coefficients[i];
			if(dist < 150){
				heuristicByDist[i][dist] += heuristic;
				numDist[i][dist]++;
			}
			if (heuristic < bestHeuristicSoFar[i]) {
				bestHeuristicSoFar[i] = heuristic;
				bestSoFar[i] = NodeRef(static_cast<__int64>(nodeDesc.first));
			}
		}
	}

	auto chosenCoeff = -1;
	float coeffSlope = 10;
	for(int coeff = 0; coeff < coefficientSize; coeff++){
		std::vector<float> xAxis, yAxis;
		for(int i = 0; i < 150; i++){
			if(numDist[coeff][i] == 0)
				continue;
			xAxis.push_back((float)i);
			yAxis.push_back(heuristicByDist[coeff][i] / (float)numDist[coeff][i]);
		}
		auto slope = getSlope(xAxis, yAxis);

		if(slope.first < coeffSlope){
			chosenCoeff = coeff;
			coeffSlope = slope.first;
			if(slope.first < 0)
				break;
		}
	}

	for(int i = 0; i < coefficientSize; i++){
		if(bestHeuristicSoFar[i] == maxCost){
			continue;
		}

		auto& bestNode = allNodes.at(bestSoFar[i].hash);
		float dist = startPos.toFloatVector().dist(bestNode.pos.toFloatVector());
		if((chosenCoeff == i && dist > 5) || dist >= 100){
			// reconstruct path from here
			std::vector<JoeSegment> segments;
			auto node = bestNode;
			while(node.pos != startPos){
				auto prev = node.cameFrom;
				auto prevNode = allNodes.at(prev.nodeBefore.hash);
				segments.emplace_back(prev.edgeType, prevNode.pos, node.pos, node.gScore - prevNode.gScore);
				node = prevNode;
			}
			std::reverse(segments.begin(), segments.end());
			return JoePath(segments, true);
		}
	}

	return JoePath();
}
const JoePath& JoePathFinder::getCurrentPath() const {
	return currentPath;
}
