//
//  native.cpp
//  MacSample
//
//  Created by Zorro Young on 20/12/17.
//  Copyright © 2017 idea4good. All rights reserved.
//

#include "native.hpp"

extern int run(int main_cnt, int main_width, int main_height,
               int sub_cnt, int sub_width, int sub_height,
               int color_bytes);

void run_host_monitor()
{
    run(1, 1024, 768, 0, 1024, 370, 2);
}

///////////////////////////////
void do_assert(const char* file, int line)
{
    printf("assert! file:%s, line:%d", file, line);
}

void log_out(const char* log)
{
    printf("%s", log);
}
