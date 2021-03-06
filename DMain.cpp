#include "DMain.h"

DMain::DMain ()
{
    running = true;

    surfDisplay = NULL;
}

int DMain::OnExecute ()
{
    if (OnInit () == false)
    {
        return -1;
    }
	
    SDL_Event Event;

    while (running)
    {
        while (SDL_PollEvent (&Event))
        {
            OnEvent (&Event);
        }

        OnLoop ();
        OnRender ();
    }

    OnCleanUp ();

    return 0;
}

int main (int argc, char* argv[])
{
    DMain theApp;
    return theApp.OnExecute();
}
