#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"

TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes") {
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return false;
}

std::vector<vec3_ti> lastPos;
void TestModule::onEnable() {
	lastPos.clear();
}

void TestModule::onTick(C_GameMode* gm) {
	/*auto pPos = gm->player->eyePos0;
	vec3_ti startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));

	if(std::find(lastPos.begin(), lastPos.end(), startNode) == lastPos.end()){
		lastPos.push_back(startNode);
	}*/
	//logF("%.2f", gm->player->velocity.z);
}

void TestModule::onMove(C_MoveInputHandler* hand){

}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	for(auto pos : lastPos){
		DrawUtils::drawBox(pos.toFloatVector(), pos.add(1, 1, 1).toFloatVector(), 1, false);
	}
}

void TestModule::onSendPacket(C_Packet* p) {
}

void TestModule::onDisable() {

}
float t = 0;
void TestModule::onLevelRender() {
	t++;
	DrawUtils::setColor(1, 0.2f, 0.2f, 1);

	vec3_t permutations[36];
	for(int i = 0; i < 36; i++){
		permutations[i] = {sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI))};
	}

	const float coolAnim = 0.9f + 0.9f * sin((t / 60) * PI * 2);

	g_Data.forEachEntity([&](auto c, auto _){
	  vec3_t* start = c->getPosOld();
	  vec3_t* end = c->getPos();

	  auto te = DrawUtils::getLerpTime();
	  vec3_t pos = start->lerp(end, te);

	  auto yPos = pos.y;
	  yPos -= 1.62f;
	  yPos += coolAnim;

	  std::vector<vec3_t> posList;
	  posList.reserve(36);
	  for(auto& perm : permutations){
		  vec3_t curPos(pos.x, yPos, pos.z);
		  posList.push_back(curPos.add(perm));
	  }

	  DrawUtils::drawLinestrip3d(posList);
	});
}
