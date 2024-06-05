#include "debugger.h"

Debugger::Debugger()
{
    
}

long Debugger::getPrivateWorkingSet(const wchar_t* process)
{
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
    PDH_FMT_COUNTERVALUE counterVal;
    const wchar_t* name = L"\\Process(%s)\\Working Set - Private";
    wchar_t buf[1024] = L"";
    swprintf_s(buf, 1024, name, process);

    PdhOpenQuery(NULL, NULL, &query);
    PDH_STATUS res = PdhAddEnglishCounter(query, (LPCSTR)buf, NULL, &counter);
    res = PdhCollectQueryData(query);
    PdhGetFormattedCounterValue(counter, PDH_FMT_LONG, NULL, &counterVal);
    long ret = counterVal.longValue;
    PdhCloseQuery(query);

    return ret;
}

void Debugger::printMemory()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    printf("Private committed memory: %u B\nWorking set: %u B\n", (UINT)pmc.PrivateUsage, (UINT)pmc.WorkingSetSize);

    printf("Private working set: %u B\n", (UINT)getPrivateWorkingSet(L"Program"));

    HANDLE heaps[20];
    HEAP_SUMMARY hs;

    DWORD cHeaps = GetProcessHeaps(20, heaps);

    for (DWORD i = 0; i < cHeaps; i++) {
        memset(&hs, 0, sizeof(hs));
        hs.cb = sizeof(hs);
        if (HeapSummary(heaps[i], 0, &hs) == FALSE) printf("HeapSummary failed");
        printf("Heap #%u size: %u B\n", (UINT)i, (UINT)hs.cbAllocated);
    }
    printf("\n");
}
