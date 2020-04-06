#include "Ninjas.h"

#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogNinjas);

class FNinjasModule : public IModuleInterface
{
public:

	// Begin IModule Interface
	virtual bool IsGameModule() const override
	{
		return true;
	}
	// End IModule Interface
};

IMPLEMENT_PRIMARY_GAME_MODULE(FNinjasModule, Ninjas, "Ninjas");
 