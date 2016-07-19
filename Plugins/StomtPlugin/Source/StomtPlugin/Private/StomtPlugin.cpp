#include "StomtPluginPrivatePCH.h"



class FStomtPlugin : public StomtPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		// @HACK Force classes to be compiled on shipping build
	}

	virtual void ShutdownModule() override
	{

	}
};



IMPLEMENT_MODULE(FStomtPlugin, StomtPlugin);

DEFINE_LOG_CATEGORY(LogStomt);

