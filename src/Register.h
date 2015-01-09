#ifndef REG_H
#define REG_H
class Register
{
private:
    int reg[9];
public:
    Register()
    {
        for (int i = 0; i < 9; i++)
            reg[i] = 0;
    }
    bool setRegister(int index, int value)
    {
        if (index >= 0 && index <= 7)
        {
            reg[index] = value;
            return true;
        }
        else
            return false;
    }
    int getRegister(int index)
    {
        if (index >= 0 && index <= 7)
            return reg[index];
        else
            return 0;
    }
    void reset()
    {
        for (int i = 0; i < 9; i++)
            reg[i] = 0;
    }



};

#endif
