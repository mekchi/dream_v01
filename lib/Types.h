//
//  Types.h
//  
//
//  Created by Yermek Garifullanov on 10/20/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef dream_Types_h
#define dream_Types_h

#if defined(__LP64__) && __LP64__
#define FLOAT_TYPE double
#define FLOAT_IS_DOUBLE 1
#else
#define FLOAT_TYPE float
#define FLOAT_IS_DOUBLE 0
#endif

typedef FLOAT_TYPE float_t;

#endif
