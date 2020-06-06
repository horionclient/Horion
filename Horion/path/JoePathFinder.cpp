#include "JoePathFinder.h"

#include <vector>
#include <queue>
#include "../../Utils/Logger.h"

constexpr float WALKING_SPEED = 4.32f;
constexpr float SPRINT_SPEED = 5.61f;
constexpr float JUMP_TIME = 0.6f;
constexpr float DROP1_TIME = 0.4f;
constexpr float DROP2_TIME = 0.55f;
constexpr float DROP3_TIME = 0.65f;

JoePathFinder::JoePathFinder(vec3_ti start, C_BlockSource* reg) : startPos(start), region(reg) {
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

	allNodes.emplace_back(pos, 0, 500000);
	return NodeRef(allNodes.size() - 1);
}

__forceinline bool canStandOn(vec3_ti pos, C_BlockSource* reg){
	auto standOn = reg->getBlock(pos)->toLegacy();
	return standOn->material->isSolid;
}
__forceinline bool isObstructed(vec3_ti pos, C_BlockSource* reg){
	auto obs1 = reg->getBlock(pos)->toLegacy();
	return obs1->material->isBlockingMotion;
}
__forceinline bool isObstructedPlayer(vec3_ti pos, C_BlockSource* reg){
	return isObstructed(pos, reg) || isObstructed(pos.add(0, 1, 0), reg);
}

std::vector<Edge> findEdges(std::vector<Node>& allNodes, Node startNode, C_BlockSource* reg, NodeRef startNodeRef){
	std::vector<Edge> edges;

	for(int x = -1; x <= 1; x++){
		for(int z = -1; z <= 1; z++){
			if(x == 0 && z == 0)
				continue;
			bool isDiagonal = x != 0 && z != 0;

			vec3_ti newPos = startNode.pos.add(x,0, z);

			if(isObstructed(newPos, reg)){
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

				edges.emplace_back(startNodeRef, findNode(allNodes, newPos), JUMP_TIME, JoeSegmentType::JUMP);
				continue;
			}

			// Check if we can stand on the block
			if(!canStandOn(newPos.add(0, -1, 0), reg)){
				if(isDiagonal)
					continue;
				// maybe drop?

				if(isObstructedPlayer(newPos, reg)) // walk to drop
					continue;

				newPos = newPos.add(0, -1, 0);

				if(isObstructed(newPos, reg)) // block below walk to drop
					continue;

				if(!canStandOn(newPos.add(0, -1, 0), reg)) { // stand on block for drop1
					newPos = newPos.sub(0, 1, 0);
					if(isObstructed(newPos, reg))
						continue;

					if(!canStandOn(newPos.add(0, -1, 0), reg)){ // stand on block for drop2
						newPos = newPos.sub(0, 1, 0);
						if(isObstructed(newPos, reg))
							continue;

						if(!canStandOn(newPos.add(0, -1, 0), reg)) // stand on block for drop3
							continue;

						// 3 Block drop
						edges.emplace_back(startNodeRef, findNode(allNodes, newPos), DROP3_TIME, JoeSegmentType::DROP);
						continue;
					}

					// 2 Block drop
					edges.emplace_back(startNodeRef, findNode(allNodes, newPos), DROP2_TIME, JoeSegmentType::DROP);
					continue;
				}

				edges.emplace_back(startNodeRef, findNode(allNodes, newPos), DROP1_TIME, JoeSegmentType::DROP);
				continue;
			}

			if(isObstructed(newPos.add(0, 1, 0), reg))
				continue;

			if(isDiagonal){ // We need to check if either x or z are obstruction-less
				if(isObstructedPlayer(startNode.pos.add(x, 0, 0), reg) && isObstructedPlayer(startNode.pos.add(0, 0, z), reg))
					continue;
			}

			static const float diagonalSpeed = sqrt(1 + 1) / WALKING_SPEED;
			static const float straightSpeed = 1 / WALKING_SPEED;
			float cost = isDiagonal ? diagonalSpeed : straightSpeed;
			edges.emplace_back(startNodeRef, findNode(allNodes, newPos), cost, JoeSegmentType::WALK);
		}
	}

	return edges;
}

float costHeuristic(vec3_ti current, vec3_ti end){ // The cost heuristic always has to be lower or equal to the actual cost
	return current.toVec3t().dist(end.toVec3t()) / (WALKING_SPEED * 1.1f);
}

JoePath JoePathFinder::findPathTo(vec3_ti endNode) {
	std::vector<Node> allNodes;

	auto cmp = [&](NodeRef left, NodeRef right) { return allNodes[left.index].fScore > allNodes[right.index].fScore; };
	std::priority_queue<NodeRef, std::vector<NodeRef>, decltype(cmp)> openSet(cmp);

	allNodes.emplace_back(startPos, costHeuristic(startPos, endNode), 0);
	openSet.emplace(0);

	while(!openSet.empty()){
		auto curRef = openSet.top();
		openSet.pop();
		Node& cur = allNodes[curRef.index];
		if(!cur.isUpToDate)
			continue;

		if(cur.pos == endNode || true){
			std::vector<JoeSegment> segments;
			auto node = cur;
			while(node.pos != startPos){
				auto prev = node.cameFrom;
				auto prevNode = allNodes[prev.nodeBefore.index];
				segments.emplace_back(prev.edgeType, prevNode.pos, node.pos);
				node = prevNode;
			}
			std::reverse(segments.begin(), segments.end());
			if(cur.pos == endNode){
				logF("Time for traversal: %f", cur.gScore);
				return JoePath(segments);
			}

			this->currentPath = JoePath(segments);
		}
		cur.isClosed = true;
		cur.isInOpenSet = false;

		auto edges = findEdges(allNodes, cur, this->region, curRef); // cur gets invalidated here
		for(auto edge : edges){
			auto& edgeEndNode = allNodes[edge.endNode.index];
			if(edgeEndNode.isClosed)
				continue;
			float tentativeScore = allNodes[curRef.index].gScore + edge.cost;
			if(tentativeScore >= edgeEndNode.gScore)
				continue;
			float heuristic = tentativeScore + costHeuristic(edgeEndNode.pos, endNode);

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
				openSet.push(allNodes.size() - 1);
			}
		}
		//Sleep(100);
	}

	return JoePath();
}
const JoePath& JoePathFinder::getCurrentPath() const {
	return currentPath;
}
