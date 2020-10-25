#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"
#include "../../../SDK/MatrixStack.h"
#include <deque>
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>  //radians
#include <glm/ext/matrix_transform.hpp> // perspective, translate, rotate
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes") {
	registerFloatSetting("float1", &this->float1, 0, -10, 10);
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
	//if (gm->player->velocity.y > 0)
		
	//logF("%.4f %.4f", gm->player->velocity.y, gm->player->aabb.lower.y);
	
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
	DrawUtils::setColor(0.5f, 0.5f, 0.5f, 1);

	auto blockTess = g_Data.getClientInstance()->levelRenderer->blockTessellator;
	auto tess = DrawUtils::get3dTessellator();

	vec3_ti pos(3, 3, 3);
	auto block = g_Data.getLocalPlayer()->region->getBlock(pos);

	MatrixStack* matStackPtr = DrawUtils::getMatrixStack();

	auto& newMat = matStackPtr->push();

	auto origin = DrawUtils::getOrigin();
	auto temp = DrawUtils::getOrigin().add(pos.toFloatVector()).mul(-1);

	static float t = 0;
	t+=3;
	auto preRotTranslation = glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
	auto afterRotTranslation = glm::translate(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 0.5f));
	auto rotation = afterRotTranslation * glm::rotate(glm::mat4(1.f), glm::radians(t), glm::vec3(0, 1, 0)) * preRotTranslation;

	auto translation = glm::translate(glm::mat4(1.f), glm::vec3(-origin.x, -origin.y, -origin.z));
	*newMat = translation * rotation;

	vec3_ti zer = {0, 0, 0};
	auto mesh = blockTess->getMeshForBlockInWorld(tess, block, zer);
	
	static std::array<mce::MaterialPtr, 8> mats = {{mce::MaterialPtr("moving_block_double_side"),
													mce::MaterialPtr("moving_block_blend"),
													mce::MaterialPtr("moving_block"),
													mce::MaterialPtr("moving_block_alpha"),
													mce::MaterialPtr("moving_block_alpha"),
													mce::MaterialPtr("moving_block_seasons"),
													mce::MaterialPtr("moving_block_alpha_seasons"),
													mce::MaterialPtr("moving_block_alpha_single_side")}};

	std::array<__int64*, 2> textures;
	textures[0] = g_Data.getClientInstance()->levelRenderer->atlasTexture.getClientTexture();
	textures[1] = *reinterpret_cast<__int64**>(g_Data.getClientInstance()->getLightTexture() + 0x18);

	mesh->renderMesh(DrawUtils::getScreenContext(), &mats[block->getRenderLayer()], textures);
}
