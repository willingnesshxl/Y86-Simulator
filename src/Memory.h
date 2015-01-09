#ifndef MEMORY_H
#define MEMORY_H

#include<iostream>
#include<string>
#include<fstream>
using namespace std;
#define MEM_SIZE 0x800
class codeMemory
{
private:
    unsigned char memory[MEM_SIZE];
public:
    codeMemory();

    bool Write_Byte_Memory(unsigned char Byte, int MemoryAddress);

    bool Read_Byte_Memory(int MemoryAddress, unsigned char *Byte);

    bool Write_Word_Memory(int Word, int MemoryAddress);

    bool Read_Word_Memory(int MemoryAddress, int *Word);

    void clearMemory();

    int setMemory(char *filename);

};
#endif
