#pragma once

#include "../Memory/GameData.h"
#include "../SDK/CClientInstance.h"
#include "../SDK/CMinecraftUIRenderContext.h"
#include "../Utils/HMath.h"
#include "../Utils/Target.h"
#include "../Utils/Utils.h"

enum class Fonts { DEFAULT,
				   UNICOD,
				   SMOOTH,
				   RUNE };

using mce__VertexFormat__disableHalfFloats_t = void(__fastcall*)(__int64, int, int);
using Tessellator__initializeFormat_t = void(__fastcall*)(__int64, __int64);

extern C_MinecraftUIRenderContext* renderCtx;
extern mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats;
extern Tessellator__initializeFormat_t Tessellator__initializeFormat;

struct MC_Color {
	union {
		struct {
			float r, g, b, a;
		};
		float arr[4];
	};
	bool shouldDelete = true;

	MC_Color() {
		this->r = 1;
		this->g = 1;
		this->b = 1;
		this->a = 1;
	};

	MC_Color(const MC_Color& other) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = other.a;
		this->shouldDelete = other.shouldDelete;
	}

	MC_Color(const float* arr) {
		this->arr[0] = arr[0];
		this->arr[1] = arr[1];
		this->arr[2] = arr[2];
		this->arr[3] = arr[3];
	};

	MC_Color(const float r, const float g, const float b, const float a = 1) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	};

	MC_Color(const int r, const int g, const int b, const int a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};

	MC_Color(const float r, const float g, const float b, const float a, const bool shouldDelete) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		this->shouldDelete = shouldDelete;
	};
};

enum VertexFormat {

};

class DrawUtils {
public:
	static void setCtx(C_MinecraftUIRenderContext* ctx, C_GuiData* guiData);
	static void setGameRenderContext(__int64 ctx);
	static void flush();
	static void setColor(float r, float g, float b, float a);  // rgba, values from 0 to 1
	static inline void tess__begin(__int64 tesselator, int vertexFormat = 3);
	static C_Font* getFont(Fonts font);
	static float getTextWidth(std::string* textStr, float textSize = 1, Fonts font = Fonts::SMOOTH);

	static void drawLine(vec2_t start, vec2_t end, float lineWidth);  // rgba
	static void drawLinestrip3d(const std::vector<vec3_t>& points);
	static void drawLine3d(const vec3_t& start, const vec3_t& end);
	static inline void fillRectangle(vec4_t pos, const MC_Color col, float alpha) {
		float posF[4];  // vec4_t(startX, startY, endX, endY);
		posF[0] = pos.x;
		posF[1] = pos.z;
		posF[2] = pos.y;
		posF[3] = pos.w;
		renderCtx->fillRectangle(posF, reinterpret_cast<const float*>(&col), alpha);
	}
	static inline void drawRectangle(vec4_t pos, MC_Color col, float alpha, float lineWidth = 1.0f) {
		lineWidth /= 2;
		fillRectangle(vec4_t(pos.x - lineWidth, pos.y - lineWidth, pos.z + lineWidth, pos.y + lineWidth), col, alpha);  // TOP
		fillRectangle(vec4_t(pos.x - lineWidth, pos.y, pos.x + lineWidth, pos.w), col, alpha);                          // LEFT
		fillRectangle(vec4_t(pos.z - lineWidth, pos.y, pos.z + lineWidth, pos.w), col, alpha);                          //
		fillRectangle(vec4_t(pos.x - lineWidth, pos.w - lineWidth, pos.z + lineWidth, pos.w + lineWidth), col, alpha);
	}
	static void drawImage(std::string filePath, vec2_t& ImagePos, vec2_t& ImageDimension, vec2_t& idk);

	static void drawText(vec2_t pos, std::string* text, MC_Color color, float textSize = 1, float alpha = 1, Fonts font = Fonts::SMOOTH);
	static void drawBox(vec3_t lower, vec3_t upper, float lineWidth, bool outline = false);
	static void drawEntityBox(C_Entity* ent, float lineWidth);
	static void draw2D(C_Entity* ent, float lineWidth);
	static void drawNameTags(C_Entity* ent, float textSize, bool drawHealth = false, bool useUnicodeFont = false);
	static void drawItem(C_ItemStack* item, vec2_t ItemPos, float opacity, float scale, bool isEnchanted);
	static void drawKeystroke(char key, vec2_t pos);
	static float getLerpTime();

	static vec2_t worldToScreen(const vec3_t& world);
};

