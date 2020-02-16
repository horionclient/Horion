#include "TestCommand.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
}

void listEnts(C_Entity* ent, bool isValid) {
	int id = ent->getEntityTypeId();
	char* name = ent->getNameTag()->getText();
	logF("---------------");
	logF("Entity Name: %s", name);
	logF("Entity ID: %d", id);
	logF("---------------");
}

void showAimedBlockInfo() {
	PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pointingStruct->block);
	int id = block->toLegacy()->blockId;
	char* name = block->toLegacy()->name.getText();
	logF("---------------");
	logF("Block Name: %s", name);
	logF("Block ID: %d", id);
	logF("---------------");
}

void importGeo(std::wstring filePath) {
	std::ifstream fileStr(filePath, std::ios::in | std::ios::binary);
	if (fileStr) {
		std::string contents;
		fileStr.seekg(0, std::ios::end);
		contents.resize(fileStr.tellg());
		fileStr.seekg(0, std::ios::beg);
		fileStr.read(&contents[0], contents.size());
		fileStr.close();

		auto hResourceGeometry = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
		auto hMemoryGeometry = LoadResource(g_Data.getDllModule(), hResourceGeometry);

		auto sizeGeometry = SizeofResource(g_Data.getDllModule(), hResourceGeometry);
		auto ptrGeometry = LockResource(hMemoryGeometry);
		logF("Starting geometry import");
		std::string moddedGeo = SkinUtil::modGeometry(reinterpret_cast<char*>(ptrGeometry), SkinUtil::objToMesh(contents.c_str()));
		g_Data.setCustomGeometryOverride(true, std::make_shared<std::string>(moddedGeo));
		logF("Geometry import done");

		if (hMemoryGeometry)
			FreeResource(hMemoryGeometry);
	}
}

bool TestCommand::execute(std::vector<std::string>* args) {
	HorionDataPacket packet;
	packet.cmd = CMD_FILECHOOSER;
	packet.data.swap(std::shared_ptr<unsigned char[]>(new unsigned char[100]));
	strcpy_s((char*)packet.data.get(), 100, "{\"title\": \"gaming\", \"filter\":\"Object Files (*.obj)|*.obj\"}");
	packet.dataArraySize = (int)strlen((char*)packet.data.get());
	packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
		wchar_t* filePath = reinterpret_cast<wchar_t*>(pk->data.get());
		std::wstring filePathStr(filePath);
		std::thread gamer(importGeo, filePathStr);
		gamer.detach();
	});
	logF("Sent packet");
	g_Data.sendPacketToInjector(packet);
	return true;
}
