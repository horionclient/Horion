#pragma once

#include "../Memory/GameData.h"
#include "../SDK/CClientInstance.h"
#include "../SDK/CMinecraftUIRenderContext.h"
#include "../Utils/HMath.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
enum class Fonts { DEFAULT,
				   UNICOD,
				   SMOOTH,
				   RUNE };

struct MC_Color {
	union {
		struct {
			float r, g, b, a;
		};
		float arr[4];
	};
	bool shouldDelete = true;

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

	MC_Color(const float r, const float g, const float b, const float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	};

	MC_Color(const float r, const float g, const float b, const float a, const bool shouldDelete) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		this->shouldDelete = shouldDelete;
	};
};

class DrawUtils {
public:
	static void setCtx(C_MinecraftUIRenderContext* ctx, C_GuiData* guiData);
	static void flush();
	static void setColor(float r, float g, float b, float a);  // rgba, values from 0 to 1
	static inline void tess__begin(__int64 tesselator);
	static C_Font* getFont(Fonts font);
	static float getTextWidth(std::string* textStr, float textSize = 1, Fonts font = Fonts::SMOOTH);

	static void drawLine(vec2_t start, vec2_t end, float lineWidth);  // rgba
	static void fillRectangle(vec4_t pos, const MC_Color col, float alpha);
	static void drawRectangle(vec4_t pos, MC_Color col, float alpha, float lineWidth = 1.0f);

	static void drawText(vec2_t pos, std::string* text, MC_Color* color = nullptr, float textSize = 1, Fonts font = Fonts::SMOOTH);
	static void rainbow(float* rcolors);
	static void drawBox(vec3_t lower, vec3_t upper, float lineWidth);
	static void drawEntityBox(C_Entity* ent, float lineWidth);
	static void drawNameTags(C_Entity* ent, float textSize, bool drawHealth = false, bool useUnicodeFont = false);
	static void wirebox(AABB aabb);

	static vec2_t worldToScreen(vec3_t world);
};
