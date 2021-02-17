#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/VoxelIntersector.h"
#include "../../DrawUtils.h"
#include "../../../SDK/MatrixStack.h"
#include <deque>
#include <array>
#include <unordered_set>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>			//radians
#include <glm/ext/matrix_transform.hpp>		// perspective, translate, rotate
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../../../Utils/Json.hpp"

using json = nlohmann::json;


TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes") {
	enum1 = SettingEnum(this)
		.addEntry(EnumEntry("1", 1))
		.addEntry(EnumEntry("2", 2))
		.addEntry(EnumEntry("3", 3));
	
	registerFloatSetting("float1", &this->float1, 0, -10, 10);
	registerIntSetting("int1", &this->int1, 0, -10, 10);
	registerEnumSetting("Enum1", &enum1, 0);
	registerBoolSetting("bool1", &this->bool1, true);
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return false;
}

__forceinline unsigned __int64 rotBy(int in, unsigned int by) {
	auto mut = static_cast<unsigned __int64>(in);
	return ((mut & 0x7FFFFFui64) | ((static_cast<unsigned int>(in) >> 8u) & 0x800000u) /*copy sign bit*/) << by;
}

size_t posToHash(const vec3_ti& pos) {
	return rotBy(pos.x, 0) | rotBy(pos.z, 24) | (static_cast<unsigned __int64>(pos.y) << 48u);
}

struct PosHasher {
	size_t operator()(const vec3_ti& i) const { return posToHash(i); };
};

std::unordered_set<vec3_ti, PosHasher> renderPos(8);

void loadFile(std::wstring filePath) {
	std::string contents = Utils::readFileContents(filePath);
	if (contents.size() == 0)
		return;

	logF("Starting geometry import");
	auto mesh = SkinUtil::objToMesh(contents.c_str(), false);
	logF("Mesh created (verts: %i, uvs: %i, normals: %i, faces: %i)", mesh.vertices.size(), mesh.uvs.size(), mesh.normals.size(), mesh.faces.size());
	
	auto posHasher2 = [](const vec3_ti& i) { return posToHash(i); };
	std::unordered_set<vec3_ti, PosHasher> newPoses(8);
	int num = 0;
	for (const auto& face : mesh.faces) {
		
		if (face.facesPresent != 3) {
			//logF("wierd face %i", face.facesPresent);
			continue;
		}
			
		
		const auto& v1 = mesh.vertices[face.indices[0].vertIndex];
		const auto& v2 = mesh.vertices[face.indices[1].vertIndex];
		const auto& v3 = mesh.vertices[face.indices[2].vertIndex];
		// intersect tris
		Voxel::Triangle3 tri = {{v1[0], v1[1], v1[2]},
								{v2[0], v2[1], v2[2]},
								{v3[0], v3[1], v3[2]}};
		AABB bBox{};
		Voxel::getBoundingBox(bBox, tri);
		bBox.lower.y = std::max(0.f, bBox.lower.y);
		bBox.upper.y = std::min(255.f, bBox.upper.y);

		for (float x = floorf(bBox.lower.x); x < ceilf(bBox.upper.x); x++) {
			for (float y = floorf(bBox.lower.y); y < ceilf(bBox.upper.y); y++) {
				for (float z = floorf(bBox.lower.z); z < ceilf(bBox.upper.z); z++) {
					vec3_t voxel(x, y, z);
					if (!Voxel::intersects(voxel, tri))
						continue;
					
					newPoses.insert(vec3_ti((int)x, (int)y, (int)z));
					num++;
				}
			}
		}
	}
	
	renderPos = newPoses;
	logF("replaced renderPos with %i %i", renderPos.size(), num);
}

void TestModule::onEnable() {
	renderPos.clear();

	HorionDataPacket packet;
	packet.cmd = CMD_FILECHOOSER;
	auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
	packet.data.swap(tmp);
	memset(packet.data.get(), 0, 300);
	strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a 3d object\", \"filter\":\"Object Files (*.obj)|*.obj\"}");
	packet.dataArraySize = (int)strlen((char*)packet.data.get());
	packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
		if (pk->params[0] != 1) {  // Dialog Canceled, reset geo
			auto box = g_Data.addInfoBox("File load", "No file loaded");
			box->closeTimer = 1;
			return;
		}

		wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
		std::wstring jsonDataStr(jsonData);

		json parsed = json::parse(jsonDataStr);
		if (parsed["path"].is_string()) {
			auto box = g_Data.addInfoBox("Importing File", "Please wait...");
			std::thread gamer([parsed, box]() {
				loadFile(Utils::stringToWstring(parsed["path"].get<std::string>()));
				box->fadeTarget = 0;
			});
			gamer.detach();
		}
	});

	g_Data.sendPacketToInjector(packet);
}

bool tryPlace(const vec3_ti& blockPos) {
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
	C_BlockLegacy* blockLegacy = block->toLegacy();
	if (blockLegacy->material->isReplaceable) {

		// Find neighbour
		static std::vector<vec3_ti> checklist;
		if (checklist.empty()) {
			checklist.push_back(vec3_ti(0, -1, 0));
			checklist.push_back(vec3_ti(0, 1, 0));

			checklist.push_back(vec3_ti(0, 0, -1));
			checklist.push_back(vec3_ti(0, 0, 1));

			checklist.push_back(vec3_ti(-1, 0, 0));
			checklist.push_back(vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (const auto& current : checklist) {
			
			vec3_ti calc = blockPos.sub(current);
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				g_Data.getCGameMode()->buildBlock(&calc, i);

				return true;
				break;
			}
			i++;
		}
	}
	return false;
}

void TestModule::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (renderPos.size() == 0)
		return;

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))  // Block in hand?
		return;

	int radius = 4;
	int numBlocksPerTick = 1;
	for (int x = -radius; x < radius; x++) {
		for (int y = -radius; y < radius; y++) {
			for (int z = -radius; z < radius; z++) {
				auto pos = g_Data.getLocalPlayer()->eyePos0.add((float)x, (float)y, (float)z).floor();
				if (renderPos.count(pos) == 0)
					continue;
				if (tryPlace(pos) && --numBlocksPerTick == 0)
					return;
			}
		}

	}
}

void TestModule::onMove(C_MoveInputHandler* hand){

}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	DrawUtils::setColor(0.3f, 0.3f, 0.7f, 1);
	
}

void TestModule::onSendPacket(C_Packet* p) {

}

void TestModule::onDisable() {

}
float t = 0;
void TestModule::onLevelRender() {

	DrawUtils::setColor(0.8f, 0.4f, 0.4f, 1);
	int radius = 50;
	
	const auto origin = g_Data.getLocalPlayer()->eyePos0.floor();
	for (auto pos : renderPos) {
		if (fabsf(pos.x - origin.x) > radius)
			continue;
		if (fabsf(pos.z - origin.z) > radius)
			continue;
		if (fabsf(pos.y - origin.y) > radius)
			continue;
		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pos);
		C_BlockLegacy* blockLegacy = block->toLegacy();
		if (!blockLegacy->material->isReplaceable)
			continue;
		DrawUtils::drawBox3d(pos.toFloatVector(), pos.add(1, 1, 1).toFloatVector());
	}
	
}
