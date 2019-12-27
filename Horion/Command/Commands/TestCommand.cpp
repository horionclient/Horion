#include "TestCommand.h"


TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
	if (hMemoryGeometry)
		FreeResource(hMemoryGeometry);
	if (hMemorySteve)
		FreeResource(hMemorySteve);
}

bool TestCommand::execute(std::vector<std::string>* args)
{
	auto hResourceGeometry = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
	hMemoryGeometry = LoadResource(g_Data.getDllModule(), hResourceGeometry);

	auto sizeGeometry = SizeofResource(g_Data.getDllModule(), hResourceGeometry);
	auto ptrGeometry = LockResource(hMemoryGeometry);

	auto hResourceSteve = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_STEVE), (char*)RT_RCDATA);
	hMemorySteve = LoadResource(g_Data.getDllModule(), hResourceSteve);

	auto sizeSteve = SizeofResource(g_Data.getDllModule(), hResourceSteve);
	auto ptrSteve = LockResource(hMemorySteve);

	std::shared_ptr<TextHolder> newGeometryData = std::make_shared<TextHolder>(ptrGeometry, sizeGeometry, true);
	C_SkinData newSkinData;
	newSkinData.SkinWidth = 128;
	newSkinData.SkinHeight = 128;
	newSkinData.skinData = ptrSteve;
	newSkinData.skinSize = sizeSteve;

	std::shared_ptr<TextHolder> newSkinResourcePatch = std::make_shared<TextHolder>(Utils::base64_decode("ewogICAiZ2VvbWV0cnkiIDogewogICAgICAiYW5pbWF0ZWRfZmFjZSIgOiAiZ2VvbWV0cnkuYW5pbWF0ZWRfZmFjZV9wZXJzb25hXzRjZGJiZmFjYTI0YTk2OGVfMF8wIiwKICAgICAgImRlZmF1bHQiIDogImdlb21ldHJ5LnBlcnNvbmFfNGNkYmJmYWNhMjRhOTY4ZV8wXzAiCiAgIH0KfQo="));

	std::shared_ptr<MinecraftSkinData> skinData = std::make_shared<MinecraftSkinData>(newSkinData, newGeometryData, newSkinResourcePatch);
	g_Data.setOverwrittenSkin(skinData);

	return true;
}