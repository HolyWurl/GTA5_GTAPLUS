// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "sigscan.h"
#include "script_global.h"

int* m_bypass_gta_plus;
bool* m_is_session_started;

DWORD Mainthread(LPVOID lp)
{
    pattern_batch main_batch;
    main_batch.add("script_globals", "48 8D 05 ? ? ? ? 4E 89 34 F8 49", [=](ptr_manage ptr)
    {
        m_script_globals = ptr.add(3).rip().as<std::int64_t**>();
    });
    main_batch.add("bypass_gta_plus", "E8 ? ? ? ? 84 C0 74 17 89 F1", [=](ptr_manage ptr)
    {
        m_bypass_gta_plus = ptr.add(1).rip().add(7).rip().as<int*>();
    });
    main_batch.add("is_session_started", "0F B6 05 ? ? ? ? 0A 05 ? ? ? ? 75 2A", [=](ptr_manage ptr)
    {
        m_is_session_started = ptr.add(3).rip().as<bool*>();
    });
    main_batch.run();
    while (true)
    {
        if (*m_is_session_started)
        {
            *script_global(1963593).as<int*>() = 1;
            *m_bypass_gta_plus = 0;
        }
    }
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, static_cast<LPTHREAD_START_ROUTINE>(Mainthread), hModule, NULL, NULL);
        break;
    }
    return TRUE;
}

