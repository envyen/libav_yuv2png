# libav_yuv2png

Encode a YUVNV12 to PNG using libav



![128x64](https://github.com/envyen/libav_yuv2png/raw/master/out/128x64.png
 "128x64")

```cpp
#include <thumbnail.h>

//Initialize once
thumbnail thumb(inWidth,inHeight,thumbWidth,thumbHeight);

//Save PNG thumbs as required

thumb.savepng ("out.png", Ybuffer, UVbuffer);

thumb.savepng ("out.png", YUVbuffer, YUVbuffer + inWidth*inHeight);
                
```
        
