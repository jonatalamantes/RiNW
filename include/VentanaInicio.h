#ifndef VENTANAINICIO_H
#define VENTANAINICIO_H

#include <SDL2/SDL.h>

class VentanaInicio
{
    private:
        static SDL_Window window;

    public:
        VentanaInicio();

        virtual ~VentanaInicio();
    protected:
    private:
};

#endif // VENTANAINICIO_H
