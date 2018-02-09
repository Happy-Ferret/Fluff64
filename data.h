#ifndef DATA_H
#define DATA_H


class Data
{
public:
    Data();

    bool redrawOutput=false;
    bool redrawInput=false;
    unsigned char currentColor = 0;
    static Data data;
};

#endif // DATA_H
