#pragma once

#define DEFAULT_PORT 5840
#define STRINGIFY_(X) "#X"
#define STRINGIFY(X) STRINGIFY_(X)

// check if has rio include
#if __has_include(<frc/RobotBase.h>)
    // Make "_WPILIB_CODE"
    #ifndef WPILIB_CODE
        #define _WPILIB_CODE
    #endif
    #include <frc/RobotBase.h>
    //check if is frc simulation
    #ifndef __FRC_ROBORIO__
        #ifndef _IS_SIMULATION
            #define _IS_SIMULATION
        #endif
    #else
        #ifndef _NOT_SIMULATION
            #define _NOT_SIMULATION
        #endif
    #endif
#else
    #ifndef
        #define _ROS2_Code
    #endif
#endif