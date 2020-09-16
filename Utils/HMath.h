#pragma once
#define PI (3.1415927f)

#include <math.h>

static constexpr float DEG_RAD2 = PI / 360.0f;
static constexpr float DEG_RAD = 180.0f / PI;
static constexpr float RAD_DEG = PI / 180.f;

inline float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

struct vec2_t {

	float x, y;
	vec2_t() { x = y = 0; }
	vec2_t(float a, float b) : x(a), y(b) {}
	vec2_t &operator=(const vec2_t &copy) {
		x = copy.x;
		y = copy.y;
		return *this;
	}

	static vec2_t fromAngle(float angle){
		return vec2_t((float)-sin(angle), (float)cos(angle));
	}

	bool operator==(const vec2_t &o) const { return x == o.x && y == o.y; }
	bool operator!=(const vec2_t &o) const { return x != o.x || y != o.y; }

	vec2_t sub(float f) {
		return vec2_t(x - f, y - f);
	}
	vec2_t div(float f) {
		return vec2_t(x / f, y / f);
	}

	vec2_t div(const vec2_t &o) {
		return vec2_t(x / o.x, y / o.y);
	}
	vec2_t mul(const vec2_t &o) {
		return vec2_t(x * o.x, y * o.y);
	}
	vec2_t mul(float f) {
		return vec2_t(x * f, y * f);
	}
	vec2_t sub(const vec2_t &o) {
		return vec2_t(x - o.x, y - o.y);
	}
	vec2_t add(const vec2_t &o) {
		return vec2_t(x + o.x, y + o.y);
	}
	vec2_t add(float ox, float oy) {
		return vec2_t(x + ox, y + oy);
	}

	vec2_t normalized() {
		return div(magnitude());
	}

	vec2_t cross(){
		return vec2_t(-y, x);
	}

	float dot(const vec2_t &o) const { return x * o.x + y * o.y; }

	vec2_t normAngles() {
		float x = this->x;
		float y = this->y;
		while (x > 90.f)
			x -= 180.0f;
		while (x < -90.f)
			x += 180.0f;

		while (y > 180.0f)
			y -= 360.0f;
		while (y < -180.0f)
			y += 360.0f;
		return vec2_t(x, y);
	}

	float squaredlen() const { return x * x + y * y; }
	float magnitude() const { return sqrtf(squaredlen()); }
};

struct vec3_t {
	union {
		struct {
			float x, y, z;
		};
		float floatArr[3];
	};

	vec3_t() { x = y = z = 0; }
	vec3_t(int a, int b, int c) : x((float)a), y((float)b), z((float)c) {}
	vec3_t(double a, double b, double c) : x((float)a), y((float)b), z((float)c) {}
	vec3_t(float a, float b, float c) : x(a), y(b), z(c) {}
	vec3_t(float a, float b) : x(a), y(b), z(0) {}
	vec3_t(const vec2_t &copy, float zP) {
		x = copy.x;
		y = copy.y;
		z = zP;
	}
	vec3_t(const vec3_t &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	vec3_t &operator=(const vec3_t &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;

		return *this;
	}

	vec3_t(float *v) : x(v[0]), y(v[1]), z(v[2]) {}

	bool iszero() const { return x == 0 && y == 0 && z == 0; }

	bool operator==(const vec3_t &o) const { return x == o.x && y == o.y && z == o.z; };
	bool operator!=(const vec3_t &o) const { return x != o.x || y != o.y || z != o.z; };
	vec3_t operator-() const { return vec3_t(-x, -y, -z); };

	vec3_t mul(float f) {
		return vec3_t(x * f, y * f, z * f);
	};
	vec3_t mul(float x1, float y1, float z1) {
		return vec3_t(x * x1, y * y1, z * z1);
	};
	vec3_t div(float f) {
		return vec3_t(x / f, y / f, z / f);
	};
	vec3_t add(float f) {
		return vec3_t(x + f, y + f, z + f);
	};
	vec3_t add(float x1, float y1, float z1) {
		return vec3_t(x + x1, y + y1, z + z1);
	};
	vec3_t sub(float f) {
		return vec3_t(x - f, y - f, z - f);
	};

	vec3_t floor() {
		return vec3_t(floorf(x), floorf(y), floorf(z));
	};

	vec3_t add(const vec3_t &o) const {
		return vec3_t(x + o.x, y + o.y, z + o.z);
	}
	vec3_t sub(const vec3_t &o) const {
		return vec3_t(x - o.x, y - o.y, z - o.z);
	}

	float squaredlen() const { return x * x + y * y + z * z; }
	float squaredxzlen() const { return x * x + z * z; }

	vec3_t lerp(const vec3_t& other, float tx, float ty, float tz) const {
		vec3_t ne;
		ne.x = x + tx * (other.x - x);
		ne.y = y + ty * (other.y - y);
		ne.z = z + tz * (other.z - z);
		return ne;
	}

	vec3_t lerp(const vec3_t other, float val) const {
		vec3_t ne;
		ne.x = x + val * (other.x - x);
		ne.y = y + val * (other.y - y);
		ne.z = z + val * (other.z - z);
		return ne;
	}

	vec3_t lerp(const vec3_t *other, float val) const {
		vec3_t ne;
		ne.x = x + val * (other->x - x);
		ne.y = y + val * (other->y - y);
		ne.z = z + val * (other->z - z);
		return ne;
	}

	float sqrxy() const { return x * x + y * y; }

	float dot(const vec3_t &o) const { return x * o.x + y * o.y + z * o.z; }
	float dotxy(const vec3_t &o) const { return x * o.x + y * o.y; }

	float magnitude() const { return sqrtf(squaredlen()); }

	vec3_t normalize() {
		return div(magnitude());
	}

	float dist(const vec3_t &e) const {
		return sub(e).magnitude();
	}

	float Get2DDist(const vec3_t &e) const {
		float dx = e.x - x, dy = e.y - y;
		return sqrtf(dx * dx + dy * dy);
	}

	float magnitudexy() const { return sqrtf(x * x + y * y); }
	float magnitudexz() const { return sqrtf(x * x + z * z); }

	vec3_t cross(const vec3_t &b) {
		return vec3_t(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
	float cxy(const vec3_t &a) { return x * a.y - y * a.x; }

	vec2_t CalcAngle(vec3_t dst) {
		vec3_t diff = dst.sub(*this);

		diff.y = diff.y / diff.magnitude();
		vec2_t angles;
		angles.x = asinf(diff.y) * -DEG_RAD;
		angles.y = (float)-atan2f(diff.x, diff.z) * DEG_RAD;

		return angles;
	}
};

struct vec3_ti {
	int x, y, z;

	vec3_ti() { x = y = z = 0; }
	vec3_ti(int a, int b, int c) : x(a), y(b), z(c) {}
	vec3_ti(int a, int b) : x(a), y(b), z(0) {}
	vec3_ti(const vec3_ti &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	vec3_ti(const vec3_t &copy) {
		x = (int)copy.x;
		y = (int)copy.y;
		z = (int)copy.z;
	}

	vec3_ti &operator=(const vec3_ti &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;

		return *this;
	}

	vec3_ti *operator=(const vec3_ti *copy) {
		x = copy->x;
		y = copy->y;
		z = copy->z;

		return this;
	}

	vec3_ti(int *v) : x(v[0]), y(v[1]), z(v[2]) {}
	
	vec3_t toVec3t() const {
		return vec3_t(x, y, z);
	}

	bool iszero() const { return x == 0 && y == 0 && z == 0; }

	bool operator==(const vec3_ti &o) const { return x == o.x && y == o.y && z == o.z; }
	bool operator!=(const vec3_ti &o) const { return x != o.x || y != o.y || z != o.z; }

	vec3_ti add(vec3_ti o) const {
		return vec3_ti(x + o.x, y + o.y, z + o.z);
	}

	vec3_ti add(int f) const {
		return vec3_ti(x + f, y + f, z + f);
	}

	vec3_ti add(int a, int b, int c) const {
		return vec3_ti(x + a, y + b, z + c);
	};

	vec3_ti sub(int ox, int oy, int oz) const {
		return vec3_ti(x - ox, y - oy, z - oz);
	}

	vec3_ti sub(const vec3_ti& o) const {
		return vec3_ti(x - o.x, y - o.y, z - o.z);
	}

	void set(vec3_ti *o) {
		x = o->x;
		y = o->y;
		z = o->z;
	};

	vec3_t toFloatVector() const {
		vec3_t vec;
		vec.x = (float)x;
		vec.y = (float)y;
		vec.z = (float)z;
		return vec;
	}
};
struct vec4_t {
	union {
		struct {
			float x, y, z, w;
		};
		float v[4];
	};
	vec4_t() { x = 0, y = 0, z = 0, w = 0; }
	explicit vec4_t(const vec3_t &p, float w = 0) : x(p.x), y(p.y), z(p.z), w(w){};
	vec4_t(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){};
	float &operator[](int i) { return v[i]; };
	float operator[](int i) const { return v[i]; };

	__forceinline bool contains(vec2_t *point) {
		if (point->x <= x || point->y <= y)
			return false;

		if (point->x >= z || point->y >= w)
			return false;
		return true;
	};
};

struct glmatrixf {
	union {
		float v[16];
		float v_nested[4][4];
	};

	__forceinline float operator[](int i) const { return v[i]; }
	__forceinline float &operator[](int i) { return v[i]; }

#define MULMAT(row, col) v[col + row] = x[row] * y[col] + x[row + 4] * y[col + 1] + x[row + 8] * y[col + 2] + x[row + 12] * y[col + 3];

	template <class XT, class YT>
	void mul(const XT x[16], const YT y[16]) {
		MULMAT(0, 0);
		MULMAT(1, 0);
		MULMAT(2, 0);
		MULMAT(3, 0);
		MULMAT(0, 4);
		MULMAT(1, 4);
		MULMAT(2, 4);
		MULMAT(3, 4);
		MULMAT(0, 8);
		MULMAT(1, 8);
		MULMAT(2, 8);
		MULMAT(3, 8);
		MULMAT(0, 12);
		MULMAT(1, 12);
		MULMAT(2, 12);
		MULMAT(3, 12);
	}

#undef MULMAT

	glmatrixf *correct() {
		glmatrixf *newMatPtr = new glmatrixf;

		for (int i = 0; i < 4; i++) {
			newMatPtr->v[i * 4 + 0] = v[0 + i];
			newMatPtr->v[i * 4 + 1] = v[4 + i];
			newMatPtr->v[i * 4 + 2] = v[8 + i];
			newMatPtr->v[i * 4 + 3] = v[12 + i];
		}
		return newMatPtr;
	};

	inline bool OWorldToScreen(vec3_t origin, vec3_t pos, vec2_t &screen, vec2_t fov, vec2_t displaySize) {
		pos = pos.sub(origin);

		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0)
			return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	inline void mul(const glmatrixf &x, const glmatrixf &y) {
		mul(x.v, y.v);
	}

	inline void translate(float x, float y, float z) {
		v[12] += x;
		v[13] += y;
		v[14] += z;
	}

	inline void translate(const vec3_t &o) {
		translate(o.x, o.y, o.z);
	}

	inline void scale(float x, float y, float z) {
		v[0] *= x;
		v[1] *= x;
		v[2] *= x;
		v[3] *= x;
		v[4] *= y;
		v[5] *= y;
		v[6] *= y;
		v[7] *= y;
		v[8] *= z;
		v[9] *= z;
		v[10] *= z;
		v[11] *= z;
	}

	inline void invertnormal(vec3_t &dir) const {
		vec3_t n(dir);
		dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
		dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
		dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
	}

	inline void invertvertex(vec3_t &pos) const {
		vec3_t p(pos);
		p.x -= v[12];
		p.y -= v[13];
		p.z -= v[14];
		pos.x = p.x * v[0] + p.y * v[1] + p.z * v[2];
		pos.y = p.x * v[4] + p.y * v[5] + p.z * v[6];
		pos.z = p.x * v[8] + p.y * v[9] + p.z * v[10];
	}

	inline void transform(const vec3_t &in, vec4_t &out) const {
		out.x = transformx(in);
		out.y = transformy(in);
		out.z = transformz(in);
		out.w = transformw(in);
	}

	__forceinline float transformx(const vec3_t &p) const {
		return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
	}

	__forceinline float transformy(const vec3_t &p) const {
		return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
	}

	__forceinline float transformz(const vec3_t &p) const {
		return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
	}

	__forceinline float transformw(const vec3_t &p) const {
		return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
	}

	__forceinline vec3_t gettranslation() const {
		return vec3_t(v[12], v[13], v[14]);
	}

	//assault cube world2screen
	vec3_t transform(glmatrixf *matrix, vec3_t &totransform) {
		return vec3_t(matrix->transformx(totransform),
					  matrix->transformy(totransform),
					  matrix->transformz(totransform))
			.div(matrix->transformw(totransform));
	}

	///pos should be the exact center of the enemy model for scaling to work properly
	vec3_t WorldToScreen(vec3_t pos, int width, int height) {
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		vec4_t clipCoords;
		clipCoords.x = pos.x * v[0] + pos.y * v[4] + pos.z * v[8] + v[12];
		clipCoords.y = pos.x * v[1] + pos.y * v[5] + pos.z * v[9] + v[13];
		clipCoords.z = pos.x * v[2] + pos.y * v[6] + pos.z * v[10] + v[14];
		clipCoords.w = pos.x * v[3] + pos.y * v[7] + pos.z * v[11] + v[15];

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		vec3_t NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		//viewport tranform to screenCooords

		vec3_t playerscreen;
		playerscreen.x = (width / 2 * NDC.x) + (NDC.x + width / 2);
		playerscreen.y = -(height / 2 * NDC.y) + (NDC.y + height / 2);

		return playerscreen;
	}
};

struct AABB {
	vec3_t lower;
	vec3_t upper;
	bool isZero = false;
	char padding[3];
	AABB() {}
	AABB(vec3_t l, vec3_t h) : lower(l), upper(h){};
	AABB(const AABB &aabb) {
		lower = vec3_t(aabb.lower);
		upper = vec3_t(aabb.upper);
	}
	AABB(vec3_t lower, float width, float height, float eyeHeight) {
		lower = lower.sub(vec3_t(width, eyeHeight * 2, width).div(2));
		this->lower = lower;
		this->upper = {lower.x + width, lower.y + height, lower.z + width};
	}

	bool operator==(const AABB &rhs) const {
		return lower == rhs.lower && upper == rhs.upper;
	}

	bool isFullBlock(){
		auto diff = lower.sub(upper);
		return fabsf(diff.y) == 1 && fabsf(diff.x) == 1 && fabsf(diff.z) == 1;
	}

	AABB expanded(float amount) {
		return AABB(lower.sub(amount), upper.add(amount));
	}

	bool intersects(AABB aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			   aabb.upper.y > lower.y && upper.y > aabb.lower.y &&
			   aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}
};

/*
inline int random(int start, int end) {
	return rand() % (end - start + 1) + start;
}

inline float randomf(int start, int end) {
	return (float)random(start, end);
}
*/