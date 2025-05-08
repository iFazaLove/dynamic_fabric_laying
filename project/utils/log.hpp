#ifdef ENABLE_CLOTH_LOG
    #include <iostream>
    #define LOG(msg) std::cout << "[Cloth] " << msg << std::endl
#else
    #define LOG(msg)
#endif