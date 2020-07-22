#include <iostream>
#include <string>
#include <cstdio>
#include <cstdint>

#include <thumbnail.h>

using namespace std;

//NV12
#define YW 1920
#define YH 960
//PNG
#define TW 128
#define TH 64

#define CLK_START(var) \
    clock_t var = clock();
#define CLK_STOP(var) \
    printf("[%s] %0.5f ms\n", #var, ((float) (clock() - (var)) / CLOCKS_PER_SEC)*1000 );


int main()
{
    auto* Ybuffer = new uint8_t[YW*YH];
    auto* UVbuffer = new uint8_t[YW*YH/2];

    //DUMMY DATA #############################
    const int bands = 5;
    uint8_t Y[bands] = {111,227,191,111,59};
    uint32_t UV[bands] = {0xCB6CCB6C,
                          0x7D7C7D7C,
                          0x6A886A88,
                          0x66976697,
                          0x73927392};
    int ysz = (YW*YH)/bands;
    int uvsz = (YW*YH/2)/bands;
    uint32_t *p, i;
    for(int b=0;b<bands;b++) {
        memset (Ybuffer+(ysz*b), Y[b], ysz);
        p = (uint32_t *) (UVbuffer + uvsz*b);
        for(i = 0; i < uvsz / sizeof(* p); ++i)
            p[i] = UV[b];
    }
    //DUMMY DATA #############################


    {
        //Initialize once
        CLK_START (init_thumb);
        thumbnail thumb(YW,YH,TW,TH);
        CLK_STOP (init_thumb);

        //Save PNG thumbs as reqd
        CLK_START (save_thumb);
        thumb.savepng ("out.png", Ybuffer, UVbuffer);
        CLK_STOP (save_thumb);
    }

    delete[] Ybuffer;
    delete[] UVbuffer;

    return 0;
}
