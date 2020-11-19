#include "Module.h"
class Twerk : public IModule {
private:
	bool extra = false;
	int delay1 = 2;
	int delay = 0;

public:
	Twerk();
	~Twerk();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};