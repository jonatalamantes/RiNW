#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "FuncionesPantalla.h"
#include "PantallaInicio.h"
#include "Principal.h"
#include "Constantes.h"

using namespace std;

//Constructor
Principal::Principal()
{
    Principal::pi = NULL;
}

//Metodo inicial de la aplicacion
void Principal::desplegar()
{
    //Creamos las variables de la pantalla
    SDL_Surface* Pantalla;
    FuncionesPantalla* f = new FuncionesPantalla();

    //Inicializamos el servicio de video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Unable to init SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    //Inicializamos el servicio de fuente
    if (TTF_Init() != 0)
    {
    	cout<< "Error letras " << TTF_GetError() << endl;
    	SDL_Quit();
    	exit(1);
    }

    //Establecemos el tamaño de la pantalla
    Pantalla = SDL_SetVideoMode(LARGO_VENTANA, ALTO_VENTANA, 16, SDL_SWSURFACE);

    if (Pantalla == NULL)
    {
       cout << "Unable to set 600x400 video: " << SDL_GetError() << endl;
       exit(1);
    }

    //Creamos la ventana y la desplegamos con los eventos que tiene
    pi = new PantallaInicio(Pantalla, f);
    pi->imprimirPantalla();

    //Salimos de la aplicación
	TTF_Quit();
    SDL_Quit();
}
