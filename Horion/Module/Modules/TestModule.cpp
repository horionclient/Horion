#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"
#include <deque>
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

	struct tex_t{
		__int64* atlas, *light;
	} textures;
	struct data_t {
		__int64 one;
		tex_t* two;
	} data;
	using renderMesh_t = __int64 (*)(__int64, __int64, mce::MaterialPtr*, data_t*);
	static renderMesh_t renderMesh = reinterpret_cast<renderMesh_t>(FindSignature("40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B F1 4D 8B E0 4C"));

	struct matStack {
		std::deque<glm::mat4x4> stack;
		bool isDirty;

		glm::mat4x4& push() {
			const auto latestAndGreatest = this->stack.back();
			this->stack.push_back(latestAndGreatest);
			return this->stack.back();
		}

		void pop() {
			this->stack.pop_back();
		}
	};

	matStack* matStackPtr = reinterpret_cast<matStack*>(*reinterpret_cast<__int64*>(DrawUtils::get3dScreenContext() + 0x18i64) + 0x30i64);

	matStackPtr->isDirty = 1;
	auto& newMat = matStackPtr->push();

	auto origin = DrawUtils::getOrigin();
	auto temp = DrawUtils::getOrigin().add(pos.toFloatVector()).mul(-1);

	static float t = 0;
	t+=3;
	auto preRotTranslation = glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
	auto afterRotTranslation = glm::translate(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 0.5f));
	auto rotation = afterRotTranslation * glm::rotate(glm::mat4(1.f), glm::radians(t), glm::vec3(0, 1, 0)) * preRotTranslation;

	auto translation = glm::translate(glm::mat4(1.f), glm::vec3(-origin.x, -origin.y, -origin.z));
	newMat = translation * rotation;

	vec3_ti zer = {0, 0, 0};
	auto mesh = blockTess->getMeshForBlockInWorld(tess, block, zer);

	textures.atlas = g_Data.getClientInstance()->levelRenderer->atlasTexture.getClientTexture();
	textures.light = *reinterpret_cast<__int64**>(g_Data.getClientInstance()->getLightTexture() + 0x18);

	data.one = 2;
	data.two = &textures;

	static mce::MaterialPtr defaultMat("moving_block");
	renderMesh(mesh, DrawUtils::get3dScreenContext() + 0x10, &defaultMat, &data);

	matStackPtr->isDirty = 1;
	matStackPtr->pop();
}
