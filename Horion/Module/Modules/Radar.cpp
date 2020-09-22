#include "Radar.h"

static float rcolors[4];
static int size = 100;
static int pixelSize = 2;
static float cent = size / 2;
static float pxSize = pixelSize / 2;
static float topPad = -1;
static float zoom = 1;
static float pxOpacity = 1;
// didn't bother puting this onto the header file and making it non-static...
// it's the only one that could be. since everything else is accessed at renderEntity()
static float bgOpacity = 0.2;

Radar::Radar() : IModule(0x0, Category::VISUAL, "Radar") {
	registerIntSetting("Size", &size, size, 50, 200);
	registerIntSetting("Pixel Size", &pixelSize, pixelSize, 2, 4);
	registerFloatSetting("Zoom", &zoom, zoom, 1.0f / 4.0f, 4.0f);
	registerFloatSetting("Background Opacity", &bgOpacity, bgOpacity, 0.1f, 1.0f);
	registerFloatSetting("Pixel Opacity", &pxOpacity, pxOpacity, 0.1f, 1.0f);
}

Radar::~Radar() {
}

const char* Radar::getModuleName() {
	return "Radar";
}

void recalculateScale() {
	cent = size / 2;
	pxSize = pixelSize / 2;
	topPad = g_Data.getGuiData()->heightGame / 2;
}

void renderEntity(C_Entity* currentEntity, bool isRegularEntity) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (currentEntity == nullptr)
		return;

	if (currentEntity == player)  // Skip Local player
		return;

	if (!player->canAttack(currentEntity, false))
		return;

	if (!currentEntity->isAlive())
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	vec3_t* start = currentEntity->getPosOld();
	vec3_t* end = currentEntity->getPos();

	vec3_t lerped = start->lerp(end, DrawUtils::getLerpTime());

	const float deltaX = lerped.x - player->currentPos.x;
	const float deltaZ = lerped.z - player->currentPos.z;
	const float dist = sqrt(powf(deltaX, 2) + powf(deltaZ, 2));

	float angle = (180.0f - player->yaw) + 180.0f;
	if (angle < 0) angle += 360;
	angle *= RAD_DEG;

	const float s = sin(angle);
	const float c = cos(angle);

	const vec2_t relPos = vec2_t(
		cent - ((deltaX * (c * zoom)) - (deltaZ * (s * zoom))),
		topPad - ((deltaX * (s * zoom)) + (deltaZ * (c * zoom)))
	);
	if (relPos.x > 0 && relPos.x < size && relPos.y > topPad - cent && relPos.y < topPad + cent) {
		DrawUtils::fillRectangle(vec4_t(relPos.x - pxSize, relPos.y - pxSize, relPos.x + pxSize, relPos.y + pxSize), MC_Color(rcolors[0], rcolors[1], rcolors[2]), pxOpacity);
	}
}

void Radar::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();

	if (player == nullptr || !GameData::canUseMoveKeys()) return;

	if (rcolors[3] < 1) {
		rcolors[0] = 0.2f;
		rcolors[1] = 0.2f;
		rcolors[2] = 1.f;
		rcolors[3] = 1;
	}
	recalculateScale();

	Utils::ApplyRainbow(rcolors, 0.0015f);

	DrawUtils::fillRectangle(vec4_t(0, topPad - cent, size, topPad + cent), MC_Color(0, 0, 0), bgOpacity);

	g_Data.forEachEntity(renderEntity);

	const float pxSize = pixelSize / 2;
	DrawUtils::fillRectangle(vec4_t(cent - pxSize, topPad - pxSize, cent + pxSize, topPad + pxSize), MC_Color(255, 255, 255), pxOpacity);
}
