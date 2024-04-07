#pragma once

#include <memory>

//宏定义
#ifdef AR_PLATFORM_WINDOWS
#if AR_DYNAMIC_LINK
	#ifdef AR_BUILD_DLL
		#define AR_API __declspec(dllexport)
	#else
		#define  AR_API __declspec(dllimport)
	#endif
#else
	#define AR_API
#endif
#else
	#error Adran only support on Windows!
#endif

#ifdef AR_DEBUG
	#define AR_ENABLED_ASSERTS
#endif

//是否可用断点
#ifdef AR_ENABLED_ASSERTS
	#define AR_ASSERT(x,...) {if(!(x)){ AR_ERROR("Assertion Failed:{0}", __VA_ARGS__);__debugbreak();}}
	#define AR_CORE_ASSERT(x,...){if(!(x)){AR_CORE_ERROR("Core Assertion Failed:{0}",__VA_ARGS__);__debugbreak();}}
#else
	#define AR_ASSERT(x,...)
	#define AR_CORE_ASSERT(x,...)
#endif

//<<:右移运算符，such as:BIT(3) as 00000100
#define BIT(x) (1<<x)

#define AR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Adran
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args) ...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args) ...);
	}
}