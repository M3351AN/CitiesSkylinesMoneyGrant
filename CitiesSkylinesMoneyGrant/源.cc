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


template<typename T>
T RefPtr(uintptr_t baseAddr, const std::vector<uintptr_t>& offset) {
    uintptr_t addr = baseAddr;
    for (size_t i = 0; i < offset.size(); i++) {

        addr = MEM::ReadAddress<uintptr_t>(addr);
        addr += offset[offset.size() - i - 1];//The ptr offsets in CT tablez are in reverse order.
    }

    return addr;
}

int Mian()
{
    SetConsoleTitle(L"Gimme Money!");

    uintptr_t ptrAddress = MEM::baseAddress + offsets::ctAddress;
    uintptr_t relAddr = RefPtr<uintptr_t>(ptrAddress, offsets::relOffsets);
    uintptr_t viewAddr = RefPtr<uintptr_t>(ptrAddress, offsets::viewOffsets);
    while (true) {
        int iMoney = -1;
        int relVal, viewVal;
        while (iMoney) {
            system("cls");
            std::cout << "剟勻天切及白央件犯奈扑亦件手�垓蕭鵊馱峇縣嶀鋓\n";
            relVal = MEM::ReadAddress<int>(relAddr);
            viewVal = MEM::ReadAddress<int>(viewAddr);
            //std::cout << "relVal:" << relVal << "\n";
            //std::cout << "viewVal:" << viewVal << "\n";
            std::cout << "You have money: " << viewVal / 100 << " now.\n";//Yes 100 times, it's correct.
            iMoney = 0;
        }
        std::cout << "How much u wanna gain?\n";
        std::cin >> iMoney; 
        iMoney = iMoney * 100;
        relVal = MEM::ReadAddress<int>(relAddr);
        viewVal = MEM::ReadAddress<int>(viewAddr);//User may cost / gain money before enter.
        MEM::WriteAddress<int>(relAddr, iMoney + viewVal);
        MEM::WriteAddress<int>(viewAddr, iMoney + viewVal);
        Sleep(250);
    }

    return 0;
}

int main() { return Mian(); }
