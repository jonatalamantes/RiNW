#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "FuncionesPantalla.h"
#include "PantallaInicio.h"

/**
 * Clase de partida de la aplicación
 * 
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 */
class Principal
{
    private:
    ///Atributos
        PantallaInicio* pi;

    public:
    ///Constructor
        Principal();

    ///Metodo de inicio
        void desplegar();
};

#endif // PRINCIPAL_H
