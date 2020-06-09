#include "JoePathFinder.h"

#include <vector>
#include <queue>
#include "../../Utils/Logger.h"
#include "../../Utils/Utils.h"
#include <chrono>

JoePathFinder::JoePathFinder(vec3_ti start, C_BlockSource* reg, std::unique_ptr<JoeGoal> goal) : startPos(start), region(reg), goal(std::move(goal)) {
}

struct NodeRef {
	int index;
	NodeRef(int index) : index(index) {}
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
	bool isUpToDate;

	Node(const vec3_ti& pos, float fScore, float gScore) : pos(pos), fScore(fScore), gScore(gScore), isClosed(false), isInOpenSet(false), isUpToDate(true) {}
};

struct Edge {
	NodeRef startNode, endNode;
	float cost;
	JoeSegmentType type;

	Edge(const NodeRef& startNode, const NodeRef& endNode, float cost, JoeSegmentType type) : startNode(startNode), endNode(endNode), cost(cost), type(type) {}
};

NodeRef findNode(std::vector<Node>& allNodes, vec3_ti& pos){
	for(int i = 0; i < allNodes.size(); i++){
		auto cur = allNodes.at(i);
		if(cur.pos == pos && cur.isUpToDate)
			return NodeRef(i);
	}

	allNodes.emplace_back(pos, 0.f, 5000.f);
	return NodeRef((int) allNodes.size() - 1);
}

__forceinline bool isDangerous(vec3_ti pos, C_BlockSource* reg, bool allowWater){
	auto obs1 = reg->getBlock(pos)->toLegacy();
	if(obs1->material->isLiquid){
		if(!allowWater || obs1->material->isSuperHot)
			return true;
	}

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

		if(obs1->Vtable == cactusBlockVtable)
			return true;
		if(obs1->Vtable == cobwebVtable)
			return true;
		// there should be a sweet berry vtable here as well but the vtable was really aids so i resorted to block names
		if(obs1->tileName.getTextLength() > 20 && strcmp(obs1->tileName.getText() + 5 /*cutoff tile. prefix*/, "sweet_berry_bush") == 0)
			return true;
	}
	return false;
}
__forceinline bool isDangerousPlayer(vec3_ti pos, C_BlockSource* reg, bool allowWater = false){
	return isDangerous(pos, reg, allowWater) || isDangerous(pos.add(0, 1, 0), reg, allowWater);
}

__forceinline bool canStandOn(vec3_ti pos, C_BlockSource* reg, bool inWater = false){
	auto block = reg->getBlock(pos);
	auto standOn = block->toLegacy();
	bool validWater = inWater && standOn->material->isLiquid && !standOn->material->isSuperHot;
	if(!standOn->material->isSolid && !validWater)
		return false;
	if(isDangerous(pos, reg, inWater))
		return false;

	if(validWater)
		return true;

	AABB aabb;
	if(!standOn->getCollisionShape(&aabb, block, reg, &pos, nullptr))
		return false;

	return aabb.isFullBlock();
}
__forceinline bool isObstructed(vec3_ti pos, C_BlockSource* reg, bool allowWater = false){
	auto block = reg->getBlock(pos);
	auto obs1 = block->toLegacy();
	if(obs1->material->isBlockingMotion)
		return true;

	// Snow blocks
	{
		static uintptr_t** snowBlockVtable = nullptr; // TopSnowBlock
		if (snowBlockVtable == nullptr) {
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 C6 83 ?? ?? ?? ?? 01 F3");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			snowBlockVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
		}

		AABB aabb;
		if(obs1->Vtable == snowBlockVtable && obs1->getCollisionShape(&aabb, block, reg, &pos, nullptr))
			return true;
	}

	return isDangerous(pos, reg, allowWater);
}
__forceinline bool isObstructedPlayer(vec3_ti pos, C_BlockSource* reg, bool allowWater = false){
	return isObstructed(pos, reg, allowWater) || isObstructed(pos.add(0, 1, 0), reg);
}

std::vector<Edge> findEdges(std::vector<Node>& allNodes, Node startNode, C_BlockSource* reg, NodeRef startNodeRef){
	std::vector<Edge> edges;
	auto startBlock = reg->getBlock(startNode.pos)->toLegacy();
	bool isInWater = startBlock->material->isLiquid && !startBlock->material->isSuperHot;
	float maxWalkSpeed = isInWater ? WATER_SPEED : SPRINT_SPEED;

	static const float SQRT2 = sqrtf(2);

	const float diagonalSpeed = SQRT2 / maxWalkSpeed;
	const float straightSpeed = 1 / maxWalkSpeed;
	const float diagonalSlowSpeed = SQRT2 / fminf(maxWalkSpeed, WALKING_SPEED);
	const float walkOffBlockSpeed = 0.8f / maxWalkSpeed;

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

				int dropLength = 0;
				while(dropLength < 3){
					dropLength++;
					auto dropPos = newPos.add(0, -1 * dropLength, 0); // drop down 1 block

					if(isObstructed(dropPos, reg, true)){// block below walk to drop
						dropLength = -1;
						break;
					}

					if(!canStandOn(dropPos.add(0, -1, 0), reg)) // block to stand on after drop
						continue;

					const float dropTime = FALL_N_BLOCKS_COST[dropLength] + walkOffBlockSpeed;
					edges.emplace_back(startNodeRef, findNode(allNodes, dropPos), dropTime, JoeSegmentType::DROP);
					dropLength = -1;
					break; // Also allow parkour jump
				}
				if(dropLength == 3){ // no drop found, lets try water drops
					auto dropPos = newPos.add(0, -1 * dropLength, 0);
					int numWaterBlocks = 0;
					while(dropPos.y > 1){
						dropPos.y--;
						dropLength++;

						if(isObstructed(dropPos, reg, true)){// block below walk to drop
							break;
						}

						auto block = reg->getBlock(dropPos)->toLegacy();
						auto isWaterBlock = block->material->isLiquid && !block->material->isSuperHot;
						if(isWaterBlock)
							numWaterBlocks++;
						else{
							numWaterBlocks = 0;
							continue;
						}

						if(!canStandOn(dropPos.add(0, -1, 0), reg) && numWaterBlocks < 19 /*we dont need a block to stand on with that much water*/) // block to stand on after drop
							continue;

						// find out how deep the water is
						float waterDepth = 1;
						while(waterDepth < 20){ // make sure we don't drop too deep
							auto blockTest = reg->getBlock(dropPos.add(0, waterDepth, 0))->toLegacy();
							if(!blockTest->material->isLiquid || blockTest->material->isSuperHot)
								break;

							waterDepth++;
						}

						const float dropTime = FALL_N_BLOCKS_COST[dropLength] + walkOffBlockSpeed;
						dropPos = dropPos.add(0, waterDepth - 1, 0);
						edges.emplace_back(startNodeRef, findNode(allNodes, dropPos), dropTime, JoeSegmentType::DROP);
						break;
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

				if(!canStandOn(newPos.add(0, -1, 0), reg)){
					// we can't stand on landing zone for jump
					if(isObstructed(newPos.add(0, -1, 0), reg)) // we can't move it down
						goto tryLargerParkourJump;
					if(isDangerous(startNode.pos.add(x,-1, z), reg, true))
						goto tryLargerParkourJump;

					// move landing zone down?
					auto dropPos = newPos.add(0, -1, 0);
					if(!canStandOn(dropPos.add(0, -1, 0), reg)) // we can't stand on the lowered landing zone :(
						goto tryLargerParkourJump;

					// walk to lower landing zone
					constexpr float time = (2 + 0.05f /*small penalty*/) / SPRINT_SPEED;
					edges.emplace_back(startNodeRef, findNode(allNodes, dropPos), time, JoeSegmentType::WALK);
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
			edges.emplace_back(startNodeRef, findNode(allNodes, newPos), cost, isInWater ? JoeSegmentType::WATER_WALK : JoeSegmentType::WALK);

			searchLoop:; // "continue" for nested loops
		}
	}

	return edges;
}

JoePath JoePathFinder::findPath() {
	if(this->goal->isInGoal(startPos))
		return JoePath();
	std::vector<Node> allNodes;

	auto cmp = [&](NodeRef left, NodeRef right) { return allNodes[left.index].fScore > allNodes[right.index].fScore; };
	std::priority_queue<NodeRef, std::vector<NodeRef>, decltype(cmp)> openSet(cmp);

	allNodes.emplace_back(startPos, this->goal->getHeuristicEstimation(startPos), 0.f);
	openSet.emplace(0);

	int numNodes = 0;
	int numEdges = 0;

	if(this->pathSearchTimeout < 0 || this->pathSearchTimeout > 1000)
		this->pathSearchTimeout = 10;

	auto pathSearchStart = std::chrono::high_resolution_clock::now();

	while(!openSet.empty()){
		auto curRef = openSet.top();
		openSet.pop();
		Node& cur = allNodes[curRef.index];
		if(!cur.isUpToDate)
			continue;

		numNodes++;

		if(this->terminateSearch)
			break;

		if(this->goal->isInGoal(cur.pos) || numNodes % 75 == 0){
			std::vector<JoeSegment> segments;
			auto node = cur;
			while(node.pos != startPos){
				auto prev = node.cameFrom;
				auto prevNode = allNodes[prev.nodeBefore.index];
				segments.emplace_back(prev.edgeType, prevNode.pos, node.pos);
				node = prevNode;
			}
			std::reverse(segments.begin(), segments.end());
			if(this->goal->isInGoal(cur.pos)){
				auto now = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> diff = now - pathSearchStart;
				logF("Found path! Traversal: %.2f Segments: %i Time: %.2fs Total Nodes: %i NodesVisited: %i Edges: %i", cur.gScore, segments.size(), diff.count(), allNodes.size(), numNodes, numEdges);
				return JoePath(segments);
			}

			this->currentPath = JoePath(segments);

			// check for timeout
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> diff = now - pathSearchStart;
			if(diff.count() > this->pathSearchTimeout)
				break;
		}
		cur.isClosed = true;
		cur.isInOpenSet = false;

		auto edges = findEdges(allNodes, cur, this->region, curRef); // cur gets invalidated here
		numEdges += (int)edges.size();
		for(auto edge : edges){
			auto& edgeEndNode = allNodes[edge.endNode.index];
			if(edgeEndNode.isClosed)
				continue;
			float tentativeScore = allNodes[curRef.index].gScore + edge.cost;
			if(tentativeScore >= edgeEndNode.gScore)
				continue;
			float heuristic = tentativeScore + this->goal->getHeuristicEstimation(edgeEndNode.pos);

			if(!edgeEndNode.isInOpenSet){ // not in open set
				edgeEndNode.isInOpenSet = true;
				edgeEndNode.cameFrom.edgeType = edge.type;
				edgeEndNode.cameFrom.nodeBefore = curRef;
				edgeEndNode.gScore = tentativeScore;
				edgeEndNode.fScore = heuristic;
				openSet.push(edge.endNode);
			}else{
				edgeEndNode.isUpToDate = false;
				auto& gamer = allNodes.emplace_back(edgeEndNode.pos, heuristic, tentativeScore);
				gamer.cameFrom.edgeType = edge.type;
				gamer.cameFrom.nodeBefore = curRef;
				openSet.push((int)allNodes.size() - 1);
			}
		}
		//Sleep(100);
	}
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> diff = now - pathSearchStart;
	logF("Could not find path! Time: %.2fs Total Nodes: %i NodesVisited: %i Edges: %i term: %i", diff.count(), allNodes.size(), numNodes, numEdges, terminateSearch);

	return JoePath();
}
const JoePath& JoePathFinder::getCurrentPath() const {
	return currentPath;
}
