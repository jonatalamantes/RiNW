#ifndef PANTALLAINICIO_H
#define PANTALLAINICIO_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "FuncionesPantalla.h"
#include "ClienteHTTP.h"
#include "XMLEstructura.h"
#include "ObjectDOM.h"
#include "Scroll.h"

using namespace std;

/**
 * Clase encargada de ofrecer la ventana de la aplicación
 * 
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 */
class PantallaInicio
{
    private:
    ///Atributos
        FuncionesPantalla* f;
        string urlActual;
        string actualIcono;
        List<string> history;
        List<ObjectDOM*> objetosDOM;
        ClienteHTTP location;
        SDL_Surface* ventana;
        bool paginaAbout;
        bool paginaError;
        unsigned int actualHistory;
        XMLEstructura raiz;
        Scroll scroll;

    ///Funcion de la pantallaInicio

        /**
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * 
         * Refresca los pixeles de la pantalla especilamente el cuerpo de esta
         */
        void imprimirPantallaInterna();

        /**
         * Permite la carga de la pagina en relación del cuadro de la URL
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param paginaNormal booleano para saber si la pagina es normal o una carga
         */
        void irPagina(bool paginaNormal);

        /**
         * Actualiza al objeto dinamicos que haya en la pantalla como las areas de texto
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param obj        Apuntador con el objeto a modificar
         * @param quitarFoco Booleano que sirve para quitar o poner el foco del objeto
         */
        void updateDomObject(ObjectDOM *obj, bool quitarFoco);

        /**
         * Toma un objeto XMLEstructura para ponerlo en la pantalla
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param actual       Objecto actual desde el cual analizar
         * @param actualX      Ubicacion X resultante de la pantalla
         * @param actualY      Ubicación Y resultante de la pantalla
         * @param copiaCache   Booleano que describe si desea cargar una página del cache
         * @param valorListaOL Valor recursivo de las listas en etiquetas 'ol' y 'ul'
         */
        void procesarXMLPantalla(XMLEstructura* actual, float *actualX, float *actualY, 
                                 bool copiaCache,  string * valorListaOL);

        /**
         * Toma una referencia de una letra y la agrega a la palabra
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param  palabra  Palabra a la que se le agregará la letra
         * @param  letra    Letra que se agregará a la palabra
         * @param  control  Booleano para saber si se esta pulsando la tecla 'control'
         * @param  shift    Booleano para saber si se esta pulsando la tecla 'shift'
         * @return          Palabra compuesta
         */
        string cargarLetra(string palabra, int letra, bool control, bool shift);

        /**
         * Imprime el contenido de una etiqueta en una ubicacion especifica
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param actualX       Posicion X donde se hará la operación
         * @param actualY       Posicion Y donde se hará la operación
         * @param tamFuente     Tamaño de la fuente en caso de escribir palabras
         * @param valor         Valor que tiene la etiqueta internamente
         * @param col1          color de la palabra 'R'
         * @param col2          color de la palabra 'G'
         * @param col3          color de la palabra 'B'
         * @param finalEspacio  Booleano para saber si desea quitar espacios al inicio y final
         * @param quitarEspacio Booleano para saber si desea quitar espacios internamente
         * @param bold          Si desea que la palabra sea en negritas o no.
         */
        void imprimirEtiqueta(float *actualX, float *actualY, int tamFuente, string valor, int col1,
                              int col2, int col3, bool finalEspacio, bool quitarEspacio, bool bold);

    public:
    ///Constructor
        PantallaInicio(SDL_Surface* v, FuncionesPantalla* f);

    ///Destructor
        ~PantallaInicio();

    ///Funcion de la pantallaInicio
    
        /**
         * Gestina los eventos en la ventana
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         */
        void imprimirPantalla();

        /**
         * Recarga el contenido de la barra de navegación
         * 
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         */
        void recargarBarraNavegacion();
};

#endif
