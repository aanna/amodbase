//
//  Utility.hpp
//  AMODBase
//
//  Created by Harold Soh on 28/3/15.
//  Copyright (c) 2015 Harold Soh. All rights reserved.
//

#ifndef AMODBase_Utility_hpp
#define AMODBase_Utility_hpp

#include <cmath>
#include "Types.hpp"

namespace amod {
    
    template <typename T> int sign(T val) {
        if (val > 0) {
            return 1;
        } else if (val < 0) {
            return -1;
        } else {
            return 0;
        }
    }


};

#endif
