#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Adran {
	class AR_API Log
	{
	public: 
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

//...和 __VA_ARGS__配用

//用宏来实现Log
#define AR_CORE_ERROR(...)   ::Adran::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AR_CORE_WARN(...)    ::Adran::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AR_CORE_INFO(...)    ::Adran::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AR_CORE_TRACE(...)   ::Adran::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AR_CORE_FATAL(...)   ::Adran::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//用宏来实现Log
#define AR_ERROR(...)        ::Adran::Log::GetClientLogger()->error(__VA_ARGS__)
#define AR_WARN(...)         ::Adran::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AR_INFO(...)         ::Adran::Log::GetClientLogger()->info(__VA_ARGS__)
#define AR_TRACE(...)        ::Adran::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AR_FATAL(...)        ::Adran::Log::GetClientLogger()->fatal(__VA_ARGS__)