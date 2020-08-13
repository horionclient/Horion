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
	g_Data.forEachEntity([](auto c, auto _){
	  vec3_t* start = c->getPosOld();
	  vec3_t* end = c->getPos();

	  auto te = DrawUtils::getLerpTime();
	  vec3_t pos = start->lerp(end, te);

	  auto yPos = pos.y;
	  yPos -= 1.62f;
	  yPos += 0.9f + 0.9f * sin(((t + pos.x * 6 + pos.z * 5) / 60) * PI * 2);

	  std::vector<vec3_t> posList;
	  vec3_t lastPos(pos.x + sinf(0), yPos, pos.z + cosf(0));
	  posList.push_back(lastPos);
	  for(auto angle = 0; angle <= 360; angle += 10){
		  if(angle == 0)
			  continue;
		  vec3_t curPos(pos.x + sinf(angle / (180 / PI)), yPos, pos.z + cosf(angle / (180 / PI)));
		  posList.push_back(curPos);
		  //DrawUtils::drawLine3d(lastPos, curPos);
		  //lastPos = curPos;
	  }

	  DrawUtils::drawLinestrip3d(posList);
	});
}
