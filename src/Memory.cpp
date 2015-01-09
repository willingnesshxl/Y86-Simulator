#include"Memory.h"

codeMemory::codeMemory()
{
    for (int i = 0; i < MEM_SIZE; i++)
        memory[i] = 0;
}

bool codeMemory::Write_Byte_Memory(unsigned char Byte, int MemoryAddress)
{
    if (MemoryAddress < 0 || MemoryAddress >= MEM_SIZE)
        return false;
    memory[MemoryAddress] = Byte;
    return true;
}
bool codeMemory::Read_Byte_Memory(int MemoryAddress, unsigned char *Byte)
{
    if (MemoryAddress < 0 || MemoryAddress >= MEM_SIZE)
        return false;
    *Byte = memory[MemoryAddress];
    return true;
}
bool codeMemory::Write_Word_Memory(int Word, int MemoryAddress)
{
    if (MemoryAddress < 0 || MemoryAddress >= MEM_SIZE - 3)
        return false;
    memory[MemoryAddress] = (Word >> 24) & 0xff;
    memory[MemoryAddress + 1] = (Word >> 16) & 0xff;
    memory[MemoryAddress + 2] = (Word >> 8) & 0xff;
    memory[MemoryAddress + 3] = Word & 0xff;
    return true;

}
bool codeMemory::Read_Word_Memory(int MemoryAddress, int *Word)
{
    if (MemoryAddress < 0 || MemoryAddress >= MEM_SIZE - 3)
        return false;
    int temp;
    temp = memory[MemoryAddress];
    temp = (temp << 8) | (((int)memory[MemoryAddress + 1]) & 0xFF);
    temp = (temp << 8) | (((int)memory[MemoryAddress + 2]) & 0xFF);
    temp = (temp << 8) | (((int)memory[MemoryAddress + 3]) & 0xFF);
    *Word = temp;
    return true;
}
void codeMemory::clearMemory()
{
    for (int i = 0; i < MEM_SIZE; i++)
        memory[i] = 0;
}
int codeMemory::setMemory(char *filename)
{
    ifstream fin(filename, ios::binary);
    int temp_Addr = 0x400;
    unsigned char temp_char;
    while (fin.read((char *)&temp_char, sizeof(unsigned char)))
    {

        if (!Write_Byte_Memory(temp_char, temp_Addr))
        {
            fin.close();
            clearMemory();
            return false;
        }
        temp_Addr++;
    }
    fin.close();
    return temp_Addr;
}
