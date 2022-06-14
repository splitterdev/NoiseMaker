#pragma once

#include <windows.h>

#define THREAD_KILL_TIME_MS 3000

class Thread {
    public:
    Thread() {
        _done = false;
        _thread = NULL;
        _started = NULL;
        _event = CreateEvent( NULL, FALSE, FALSE, NULL );
        ResetEvent( _event );
    }
    virtual ~Thread() {
        finish();
        alert();
        join();
        CloseHandle( _thread );
    }
    virtual void run() = 0;
    bool isStarted() {
        return _started;
    }
    bool isDone() {
        return _done;
    }
    void start() {
        if ( _thread != NULL ) {
            return;
        }
        _done = false;
        _thread = CreateThread( NULL, 0, _static_run, this, 0, NULL );
        if ( _thread != NULL ) {
            _started = true;
        }
    }
    void finish() {
        _done = true;
    }
    void kill() {
        if ( _thread != NULL ) {
            TerminateThread( _thread, 0 );
            _thread = NULL;
        }
    }
    void join() {
        if ( WaitForSingleObject( _thread, THREAD_KILL_TIME_MS ) == WAIT_TIMEOUT ) {
            TerminateThread( _thread, 0 );
        }
    }
    void suspend() {
        SuspendThread( _thread );
    }
    void wake() {
        ResumeThread( _thread );
    }
    bool sleep( int ms ) {
        if ( _done ) {
            return false;
        }
        ResetEvent( _event );
        return ( WaitForSingleObject( _event, ms ) == WAIT_TIMEOUT );
    }
    void alert() {
        SetEvent( _event );
    }
    private:
    static DWORD WINAPI _static_run( LPVOID param ) {
        Thread* caller = ( Thread* )( param );
        caller -> run();
        return 0;
    }
    volatile bool _started;
    volatile bool _done;
    HANDLE _thread;
    HANDLE _event;
};
