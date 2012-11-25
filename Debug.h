#ifndef DEBUG_H
#define DEBUG_H

//#define VERBOSE
//#define VERBOSE2

//#define ROTATE
#define ROTATE_FACTOR 0.01
#define ROTATEINITIAL true

#define FPSLIMIT
#define MAXFPS 60

#define MAX_LEAVES 20
#define BRANCH_ROTATION_PER_FRAME M_PI/(6*180.0)
#define BRANCH_BETA_ROTATION_LIMIT_X 5.0
#define BRANCH_BETA_ROTATION_LIMIT_Z 5.0

#define RENDER_BRANCH true
#define RENDER_LEAVES false

#include <chrono>
#define TIME_CURRENT_MILLIS (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();

//#define DEBUG_SINGLE_LEAF
//#define DEBUG_LEAF_BETA

#define ONEFBETA_SCALE 120.000 // sec. between 2 iterations of noise
#define ONEFBETA_2POW 7
#define ONEFBETA_BETA 2

#define LEAF_ROTATION_LIMIT_X 40.0 // Between 30-90
#define LEAF_ROTATION_LIMIT_Y 60.0
#define LEAF_ROTATION_LIMIT_Z 80.0
#define LEAF_ROTATION_X_Z_FACTOR 1.0

//#define PROFILING

#endif // !DEBUG_H
