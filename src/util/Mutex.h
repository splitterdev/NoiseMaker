#pragma once

#include <windows.h>

class Mutex {
    public:
    Mutex() {
        _mutex_h = CreateMutex( NULL, FALSE, NULL );
    }
    ~Mutex() {
        CloseHandle( _mutex_h );
    }

    bool lock() {
        return ( WaitForSingleObject( _mutex_h, INFINITE ) == WAIT_OBJECT_0 );
    }

    void release() {
        ReleaseMutex( _mutex_h );
    }

    private:

    HANDLE _mutex_h;

};


