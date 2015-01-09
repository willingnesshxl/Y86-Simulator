#ifndef CC_H
#define CC_H
class CC
{
private:
    int ZF;
    int SF;
    int OF;
public:
    CC();
    void reset();
    void setZF(int zf);
    void setSF(int sf);
    void setOF(int of);
    int getZF();
    int getSF();
    int getOF();
};
#endif
