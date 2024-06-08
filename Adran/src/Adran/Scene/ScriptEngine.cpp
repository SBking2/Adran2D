#include "AdranPreCompile.h"
#include "ScriptEngine.h"

//#include "mono/jit/jit.h"
//#include "mono/metadata/assembly.h"
//#include "mono/metadata/object.h"

namespace Adran {

	struct ScriptEngineData
	{

	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		//InitMono();
	}

	void ScriptEngine::Shutdown()
	{

	}



	void ScriptEngine::InitMono()
	{
		//mono_set_assemblies_path("mono/lib");
	}

}