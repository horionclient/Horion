#pragma once

#include "Module.h"
#include "../../DrawUtils.h"
#include "../../../Utils/Target.h"

class NameTags : public IModule
{
private:
	bool DisplayHealth = true;
public:
	inline bool shouldDisplayHealth() { return DisplayHealth; };
	NameTags();
	~NameTags();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};

