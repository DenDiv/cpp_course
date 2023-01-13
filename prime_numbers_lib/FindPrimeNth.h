#ifndef FINDPRIMENTH_H
    #define FINDPRIMENTH_H

    #ifdef _WIN32
        #ifdef FINDPRIMENTH_EXPORTS
            #define FINDPRIMENTHAPI __declspec(dllexport)
        #else
            #define FINDPRIMENTHAPI __declspec(dllimport)
        #endif
        #define FINDPRIMENTHCALL __cdecl
    #else
        #define FINDPRIMENTHAPI
        #define FINDPRIMENTHCALL
    #endif

    #ifdef __cplusplus
        extern "C" {
    #endif

    FINDPRIMENTHAPI unsigned long long FINDPRIMENTHCALL FindPrimeDummy(int n);
    FINDPRIMENTHAPI unsigned long long FINDPRIMENTHCALL FindPrimeErat(int n);

    #ifdef __cplusplus
        }
    #endif
    
#endif