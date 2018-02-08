#ifndef DATA_H
#define DATA_H


class Data
{
public:
    Data();

    bool redraw=false;
    unsigned char currentColor = 0;
    static Data data;
};

#endif // DATA_H
