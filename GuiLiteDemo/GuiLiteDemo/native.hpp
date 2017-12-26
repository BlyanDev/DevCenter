//
//  native.hpp
//  MacSample
//
//  Created by Zorro Young on 20/12/17.
//  Copyright © 2017 idea4good. All rights reserved.
//

#ifndef native_hpp
#define native_hpp

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif
    
    void run_host_monitor();
    
    void* get_frame_buffer(int display_id, int* width, int* height);
    
#ifdef __cplusplus
}
#endif
#endif /* native_hpp */
