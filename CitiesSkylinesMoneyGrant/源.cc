#include "memory.h"
#include <iostream>
#include <random>

namespace offsets {
/*<?xml version="1.0" encoding="utf-8"?>
<CheatTable CheatEngineTableVersion="46">
  <CheatEntries>
    <CheatEntry>
      <ID>0</ID>
      <Description>"REL"</Description>
      <VariableType>4 Bytes</VariableType>
      <Address>"mono.dll"+00296BC8</Address>
      <Offsets>
        <Offset>E8</Offset>
        <Offset>0</Offset>
        <Offset>330</Offset>
        <Offset>20</Offset>
      </Offsets>
    </CheatEntry>
    <CheatEntry>
      <ID>1</ID>
      <Description>"VIEW"</Description>
      <VariableType>4 Bytes</VariableType>
      <Address>"mono.dll"+00296BC8</Address>
      <Offsets>
        <Offset>F8</Offset>
        <Offset>0</Offset>
        <Offset>80</Offset>
        <Offset>38</Offset>
        <Offset>F8</Offset>
        <Offset>318</Offset>
        <Offset>1D0</Offset>
      </Offsets>
    </CheatEntry>
  </CheatEntries>
  <UserdefinedSymbols/>
  <Comments>Info about this table:
</Comments>
</CheatTable>
*/
    constexpr std::uintptr_t ctAddress = 0x00296BC8;
    std::vector<uintptr_t> relOffsets = { 0xE8, 0x0, 0x330, 0x20 };
    std::vector<uintptr_t> viewOffsets = { 0xF8, 0x0, 0x80, 0x38, 0xF8, 0x318, 0x1D0 };
}

int Mian()
{
    SetConsoleTitle(L"Gimme Money!");

    DWORD processId = Ukia::GetProc(L"Cities.exe");

    uintptr_t baseAddress = Ukia::GetModuleBaseAddr(processId, L"mono.dll");

    HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId);

    uintptr_t ptrAddress = baseAddress + offsets::ctAddress;
    uintptr_t relAddr = Ukia::DeRefPtr<uintptr_t>(processHandle, ptrAddress, offsets::relOffsets);
    uintptr_t viewAddr = relAddr + 0x10;//Ukia::DeRefPtr<uintptr_t>(processHandle, ptrAddress, offsets::viewOffsets);
    while (true) {
        long long iMoney = -1;
        //long long relVal;
        long long viewVal;
        while (iMoney) {
            system("cls");
            std::cout << "年がら年中リップサービス鈍感カマしては辟易\n";
            //relVal = Ukia::ReadAddr<long long>(processHandle, relAddr);
            viewVal = Ukia::ReadAddr<long long>(processHandle, viewAddr);
            //std::cout << "relVal:" << relVal << "\n";
            //std::cout << "viewVal:" << viewVal << "\n";
            std::cout << "You have money: " << (viewVal > 0 ? "\033[32m" : "\033[31m" ) << viewVal / 100 << "\033[0m" << " now.\n";//Yes 100 times, it's correct.
            iMoney = 0;
        }
        std::cout << "How much u wanna gain?\n";
        std::cin >> iMoney; 
        iMoney = iMoney * 100;
        //relVal = Ukia::ReadAddr<long long>(processHandle, relAddr);
        viewVal = Ukia::ReadAddr<long long>(processHandle, viewAddr);//User may cost / gain money before enter.
        Ukia::WriteAddr<long long>(processHandle, relAddr, iMoney + viewVal);
        Ukia::WriteAddr<long long>(processHandle, viewAddr, iMoney + viewVal);
        Sleep(250);
    }

    return 0;
}

int main() { return Mian(); }
