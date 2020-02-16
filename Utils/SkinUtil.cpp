#include "SkinUtil.h"

namespace MeshStructs {
	void to_json(json& j, const face& f) {
		std::vector<std::array<int, 3>> partArray;

		for (int i = 0; i < f.facesPresent; i++) {
			auto ind = f.indices[i];
			partArray.push_back({ind.vertIndex, ind.normalIndex, ind.uvIndex});
		}
		j = partArray;
	}
}