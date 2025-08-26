#include <dshow.h>
#include <iostream>
#include "videoplayback.h"

using namespace std;
#pragma comment(lib, "Strmiids.lib")

// Macro to report success or failure, and return on failure (lazy shorthand... bad habit! :-)
#define REPORT_OUTCOME(hr) if (FAILED(hr)) { cout << "failed" << endl; return false; } cout << "success" << endl;

// Data
IGraphBuilder* pGraph = 0; // Filter graph manager
IMediaControl* pControl = 0; // Media control interface
IMediaEvent* pEvent = 0; // Media event interface
IVideoWindow* pVidWin = nullptr;

//Display video
void video::display(string s, HWND hwnd)
{
    if (init(s, hwnd))
    {
        run();
        clearup();
    }
}

//Initialise video
bool video::init(string t, HWND hwnd)
{
    hWnd = hwnd;
    // Attempt to initialise the COM
    HRESULT hr = CoInitialize(NULL);
    REPORT_OUTCOME(hr);

    // Create the filter graph manager
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
    REPORT_OUTCOME(hr);

    // Get addition interfaces for it
    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    REPORT_OUTCOME(hr);

    hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
    REPORT_OUTCOME(hr);

    // Attempt to build the graph for file playback
    std::wstring wideFilename(t.begin(), t.end());
    hr = pGraph->RenderFile(wideFilename.c_str(), NULL);
    hr = pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin);
    REPORT_OUTCOME(hr);
    pVidWin->put_Owner((OAHWND)hWnd); // hWnd is your existing window handle
    pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

    RECT rc;
    GetClientRect(hWnd, &rc);
    pVidWin->SetWindowPosition(0, 0, rc.right, rc.bottom); // Fill the window

    return true;
}

//Run the video
void video::run()
{
    HRESULT hr = pControl->Run();
    if (SUCCEEDED(hr))
    {
        long evCode = 0;
        LONG_PTR param1, param2;

        bool running = true;

        while (running)
        {
            // Check for media events (e.g., playback ended)
            while (SUCCEEDED(pEvent->GetEvent(&evCode, &param1, &param2, 0)))
            {
                if (evCode == EC_COMPLETE) {
                    running = false;
                }
                pEvent->FreeEventParams(evCode, param1, param2);
            }

            // Check if spacebar is pressed
            if (GetAsyncKeyState(VK_SPACE) & 0x8000)
            {
                // Skip to end
                IMediaSeeking* pSeek = nullptr;
                if (SUCCEEDED(pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeek)))
                {
                    LONGLONG duration = 0;
                    pSeek->GetDuration(&duration);
                    pSeek->SetPositions(&duration, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
                    pSeek->Release();
                } 
                running = false;
            }
        }
    }
}



void video::clearup()
{
    // Release our interfaces
    pControl->Release();
    pEvent->Release();
    pGraph->Release();
    if (pVidWin) {
        pVidWin->put_Visible(OAFALSE);
        pVidWin->put_Owner(NULL);
        pVidWin->Release();
    }


    // Shutdown COM
    CoUninitialize();
}
