#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "FuncionesPantalla.h"
#include "PantallaInicio.h"
#include "stringConvert.h"
#include "Constantes.h"

using namespace std;

///Constructor
PantallaInicio::PantallaInicio(SDL_Surface* vent, FuncionesPantalla* fe)
{
    PantallaInicio::ventana = vent;
    PantallaInicio::f = fe;
    PantallaInicio::urlActual = "about";
    PantallaInicio::actualIcono = "img/go.png";
    paginaAbout = true;
    paginaError = false;
    actualHistory = 0;
}

///Destructor
PantallaInicio::~PantallaInicio()
{
    PantallaInicio::ventana = NULL;
}

///Funcion de la pantallaInicio

/**
* Recarga el contenido de la barra de navegación
*
* @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
*/
void PantallaInicio::recargarBarraNavegacion()
{
    if (SDL_MUSTLOCK(ventana))
    {
        SDL_LockSurface(ventana);
    }

    //Creamos la barra del navegador
    SDL_Rect rectangulo;
    rectangulo.x = 0;
    rectangulo.y = 0;
    rectangulo.h = ALTO_BARRA_NAV;
    rectangulo.w = LARGO_BARRA_NAV;

    Uint32 color = SDL_MapRGB(PantallaInicio::ventana->format, 150,150,150); //gris claro
    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

    //Ponemos la barra Interna
    rectangulo.x = TAM_LINEA;
    rectangulo.y = TAM_LINEA;
    rectangulo.h = ALTO_BARRA_NAV-(TAM_LINEA*2);
    rectangulo.w = LARGO_BARRA_NAV-(TAM_LINEA*2);

    color = SDL_MapRGB(PantallaInicio::ventana->format, 230,230,230); //gris
    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

    //Iconos Anterior y siguiente
    f->cargarImagen(ventana, "img/prev.png", 7, 7);
    f->cargarImagen(ventana, "img/go.png", 42, 7);

    //Barra de Icono Anterior
    rectangulo.x = 40-TAM_LINEA;
    rectangulo.y = (TAM_LINEA);
    rectangulo.h = ALTO_BARRA_NAV-(TAM_LINEA*2);
    rectangulo.w = (TAM_LINEA);

    color = SDL_MapRGB(PantallaInicio::ventana->format, 150,150,150); //gris
    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

    //Barra de Icono Siguiente
    rectangulo.x = 70;
    rectangulo.y = TAM_LINEA;
    rectangulo.h = ALTO_BARRA_NAV-(TAM_LINEA*2);
    rectangulo.w = TAM_LINEA;

    color = SDL_MapRGB(PantallaInicio::ventana->format, 150,150,150); //gris
    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

    //Escribimos que es una pagina inicial
    unsigned maxChar = ((LARGO_VENTANA-50)-(70+TAM_LINEA))/(TAM_FUENTE_BARRA-6);

    if (urlActual.size() >= maxChar)
    {
        string aux = urlActual.substr(urlActual.size()-maxChar, urlActual.size());
        f->escribirPalabra(ventana, aux.c_str(), 80, 7, "monospace", TAM_FUENTE_BARRA, 0, 0, 0);
    }
    else
    {
        f->escribirPalabra(ventana, urlActual.c_str(), 80, 7, "monospace", TAM_FUENTE_BARRA, 0, 0, 0);
    }

    //Ponemos la imagen de final
    f->cargarImagen(ventana, actualIcono.c_str(), LARGO_VENTANA-37, 7);

    rectangulo.x = LARGO_VENTANA - 50;
    rectangulo.y = TAM_LINEA;
    rectangulo.h = ALTO_BARRA_NAV-(TAM_LINEA*2);
    rectangulo.w = TAM_LINEA;

    color = SDL_MapRGB(PantallaInicio::ventana->format, 150,150,150); //gris
    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

    SDL_UpdateRect(PantallaInicio::ventana, 0, 0, LARGO_VENTANA, ALTO_BARRA_NAV);

    if (!SDL_MUSTLOCK(ventana))
    {
        SDL_UnlockSurface(ventana);
    }

    SDL_FreeSurface(ventana);
}

/**
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 *
 * Refresca los pixeles de la pantalla especilamente el cuerpo de esta
 */
void PantallaInicio::imprimirPantallaInterna()
{
    if (SDL_MUSTLOCK(ventana))
    {
        SDL_LockSurface(ventana);
    }

    //Establecemos el titulo
    SDL_WM_SetCaption("Navegador Web RiNW", NULL);
    SDL_Rect rectangulo;

    //Generamos el cuerpo de la pagina
    rectangulo.x = 0;
    rectangulo.y = ALTO_BARRA_NAV;
    rectangulo.w = LARGO_VENTANA;
    rectangulo.h = ALTO_VENTANA - ALTO_BARRA_NAV;

    //Rellenamos con blanco
    Uint32 color = SDL_MapRGB(PantallaInicio::ventana->format, 255,255,255);
    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

    //Se desea visitar la pagina de about
    if (paginaAbout)
    {
        f->cargarImagen(ventana, "img/logo.png", 170, 42);
        f->escribirPalabra(ventana, "Cliente HTTP Y Analizador HTML RiNW",
                          190, 490, "monospace", TAM_FUENTE_ERROR, 11,59,3);
        f->escribirPalabra(ventana, "Creado por: Jonathan Elías Sandoval Talamantes",
                          120, 520, "monospace", TAM_FUENTE_ERROR, 11,59,3);
    }
    else if (paginaError) //Es pagina de error
    {
        stringConvert s;

        f->cargarImagen(ventana, "img/error.png", LARGO_VENTANA-300, ALTO_VENTANA-310);

        string cad = "Esto es vergonzoso, Ha ocurrido un Error";
        f->escribirPalabra(ventana, cad.c_str(), 40, 60, "monospace", TAM_FUENTE_ERROR, 11,59,3);

        cad = "Se ha detectado un Error de Tipo: " + s.intToStr(location.getErrorCodigo());
        f->escribirPalabra(ventana, cad.c_str(), 40, 100, "monospace", TAM_FUENTE_ERROR, 11,59,3);

        cad = "Eso significa: ";
        f->escribirPalabra(ventana, cad.c_str(), 40, 140, "monospace", TAM_FUENTE_ERROR, 11,59,3);

        if (location.getErrorCodigo() == 1)
        {
            cad = "Usted como usuario ha escrito un dominio en la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 190, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "pero no se puede acceder a ese dominio";
            f->escribirPalabra(ventana, cad.c_str(), 40, 230, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Asegurece que el dominio este bien escrito";
            f->escribirPalabra(ventana, cad.c_str(), 40, 300, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Verifique que tiene acceso a Internet";
            f->escribirPalabra(ventana, cad.c_str(), 40, 340, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Después intente volver a acceder a la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 380, "monospace", TAM_FUENTE_ERROR, 11,59,3);
        }
        else if (location.getErrorCodigo() == 2)
        {
            cad = "No se ha podido crear un Socket";
            f->escribirPalabra(ventana, cad.c_str(), 40, 190, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "Eso es un error interno del sistema";
            f->escribirPalabra(ventana, cad.c_str(), 40, 230, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Consulte al Administrador del Sistema";
            f->escribirPalabra(ventana, cad.c_str(), 40, 300, "monospace", TAM_FUENTE_ERROR, 11,59,3);
        }
        else if (location.getErrorCodigo() == 4)
        {
            cad = "Usted como usuario ha escrito un dominio en la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 190, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "pero no se puede acceder a ese dominio";
            f->escribirPalabra(ventana, cad.c_str(), 40, 230, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Asegurece que el dominio este bien escrito";
            f->escribirPalabra(ventana, cad.c_str(), 40, 300, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Verifique que el dominio inicie con 'www'";
            f->escribirPalabra(ventana, cad.c_str(), 40, 340, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Después intente volver a acceder a la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 380, "monospace", TAM_FUENTE_ERROR, 11,59,3);
        }
        else if (location.getErrorCodigo() == 3)
        {
            cad = "El dominio al que esta intentando acceder le esta denegando";
            f->escribirPalabra(ventana, cad.c_str(), 40, 190, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "por lo que no se puede acceder a ese dominio";
            f->escribirPalabra(ventana, cad.c_str(), 40, 230, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Verifique que el dominio use el puerto indicado";
            f->escribirPalabra(ventana, cad.c_str(), 40, 300, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Asegurece que el dominio este bien escrito";
            f->escribirPalabra(ventana, cad.c_str(), 40, 340, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Después intente volver a acceder a la URL";
        }
        else if (location.getErrorCodigo() == 5)
        {
            cad = "Usted como usuario ha escrito algo raro en la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 190, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "o la sintaxis de la URL es incorrecta";
            f->escribirPalabra(ventana, cad.c_str(), 40, 230, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Revise que el contenido de la URL no tenga nada raro";
            f->escribirPalabra(ventana, cad.c_str(), 40, 300, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Verifique que no escribio ':/' en el Host";
            f->escribirPalabra(ventana, cad.c_str(), 40, 340, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Después intente volver a acceder a la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 380, "monospace", TAM_FUENTE_ERROR, 11,59,3);
        }
        else if (location.getErrorCodigo() == 6)
        {
            cad = "Usted como usuario ha escrito '" + location.getProtocolo() + "://' en la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 190, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "al inicio puso un protocolo no soportado por el navegador";
            f->escribirPalabra(ventana, cad.c_str(), 40, 230, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Este navegador sólo soporta HTTP y HTTPS";
            f->escribirPalabra(ventana, cad.c_str(), 40, 300, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Cambie el protocolo a HTTP o HTTPS";
            f->escribirPalabra(ventana, cad.c_str(), 40, 340, "monospace", TAM_FUENTE_ERROR, 11,59,3);

            cad = "* Después intente volver a acceder a la URL";
            f->escribirPalabra(ventana, cad.c_str(), 40, 380, "monospace", TAM_FUENTE_ERROR, 11,59,3);
        }

        //Boton de Internet
        rectangulo.x = 60;
        rectangulo.y = ALTO_VENTANA - (ALTO_BARRA_NAV*2.5);
        rectangulo.w = (TAM_FUENTE_ERROR-4) * 18;
        rectangulo.h = 40;

        color = SDL_MapRGB(PantallaInicio::ventana->format, 190,10,0); //Rojizo
        SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

        cad = "Buscar en Internet";
        f->escribirPalabra(ventana, cad.c_str(), rectangulo.x + 10, rectangulo.y + 10, "monospace",
                          TAM_FUENTE_ERROR, 255,255,255);
    }
    else //Pagina de Internet
    {
        //Revisamos la extensión de la pagina
        int posTag = location.getUbicacionArchivo().rfind('.');
        string ext;

        if (posTag >= 0)
        {
            ext = location.getUbicacionArchivo();
            ext = ext.substr(posTag+1, ext.size()-posTag);
        }

        //Es un archivo de imagen
        if ((ext.substr(0, 3) == "png" || ext.substr(0, 3) == "jpg"))
        {
            string x = location.getUbicacionArchivo();

            if (x[0] == '.')
            {
                x = x.substr(6, x.size()-6);
            }

            f->cargarImagen(ventana, x.c_str(), scroll.getActualScrollX(),
                            ALTO_BARRA_NAV + scroll.getActualScrollY());
        }
        else
        {
            //Calculamos el tamaño maximo de la pagina cargandola de internet
            string cadena;
            float actx = 0, acty = 0;
            objetosDOM.nullify();
            procesarXMLPantalla(&raiz, &actx, &acty, false, &cadena);

            scroll.setMaximoScrollY(acty-40);
            scroll.setActualScrollY(40);

            //Imprimimos la pantalla con lo conseguido del scroll
            acty = 40;
            SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);
            objetosDOM.nullify();
            procesarXMLPantalla(&raiz, &actx, &acty, true, &cadena);

            //Recargamos la barra de navegación
            recargarBarraNavegacion();
        }
    }

    SDL_UpdateRect(PantallaInicio::ventana, 0, ALTO_BARRA_NAV, LARGO_VENTANA,
                   ALTO_VENTANA-ALTO_BARRA_NAV);

    if (!SDL_MUSTLOCK(ventana))
    {
        SDL_UnlockSurface(ventana);
    }

    SDL_FreeSurface(ventana);
}

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
void PantallaInicio::imprimirEtiqueta(float *actualX, float *actualY, int tamFuente, string valor,
                                      int col1, int col2, int col3, bool finalEspacio,
                                      bool quitarEspacio, bool bold)
{
    int w = 0, h = 0;

    //Conseguimos el valor a imprimir y lo validamos
    if (valor != "")
    {
        string x = "";
        string newValue = "";

        //Si desea quitar espacios
        if (quitarEspacio)
        {
            //Eliminamos los caracteres basura del inicio
            bool espacio = false;
            for (unsigned j = 0; j < valor.size(); j++)
            {
                if (isspace(valor[j]) || valor[j] == '\t' || valor[j] == '\n')
                {
                    if (!espacio)
                    {
                        espacio = true;
                        newValue.push_back(' ');
                    }
                }
                else
                {
                    espacio = false;
                    newValue.push_back(valor[j]);
                }
            }
        }
        else
        {
            newValue = valor;
        }

        //Procesamos cada uno de los caracteres de la palabra resultante
        for (unsigned i = 0; i < newValue.size(); i++)
        {
            //Si se sale del magen de la ventana o es salto de linea brincamos una linea
            if (!(*actualX >= 0 && *actualX < LARGO_VENTANA))
            {
                *actualX = 0;
                *actualY = *actualY + h;
            }

            if (newValue[i] == '\n')
            {
                *actualX = 0;
                *actualY = *actualY + h;
            }
            else
            {
                //Insertamos la palabra
                x.push_back(newValue[i]);

                //Escribimos en la pantalla
                if (bold)
                {
                    f->escribirPalabra(ventana, x, *actualX, *actualY, "LinLibertine_B",
                              tamFuente, col1, col2, col3, &w, &h);
                }
                else
                {
                    f->escribirPalabra(ventana, x, *actualX, *actualY, "LinLibertine",
                              tamFuente, col1, col2, col3, &w, &h);
                }

                *actualX = *actualX + w;
                x = "";
            }
        }

        //Si desea tener un salto de linea
        if (finalEspacio)
        {
            *actualY = *actualY + h;
            *actualX = 0;
        }

        //Si se sale de la ventana se hace un salto de linea
        if (*actualX > LARGO_VENTANA)
        {
            *actualX = 0;
            *actualY = *actualY + TAM_FUENTE_CUERPO;
        }
    }
}

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
void PantallaInicio::procesarXMLPantalla(XMLEstructura* actual, float *actualX, float *actualY,
                                         bool copiaCache, string *valorListaUl)
{
    XMLEstructura* hijo = actual;

    //Procesamos al actual
    if (actual->getNombre() != "") //Terminamos
    {
        if (actual->getTipo() != 1) //Es etiqueta final
        {
            string myNombre = actual->getNombre();

            if (myNombre == "a")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    //Generamos el cuadrado de la palabra
                    SDL_Rect rect;

                    rect.x = *actualX;
                    rect.y = *actualY;

                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO, actual->getValor(),
                                     0, 0, 255, false, true, false);

                    rect.w = *actualX - rect.x;
                    rect.h = *actualY + TAM_FUENTE_CUERPO;

                    //Creamos el objeto DOM para el vinculo
                    ObjectDOM *obj = new ObjectDOM();

                    obj->setArea(rect);
                    obj->setFocus(false);
                    obj->setNombreTag("a");

                    int posLink = actual->getAtributo().searchInList("href");

                    //Si tiene un vinculo agregamos el evento y sacamos la ubicación
                    if (posLink != -1)
                    {
                        string nombreAttr = actual->getValorAtributo()[posLink];

                        if (nombreAttr[0] == '.' && nombreAttr[0] == '.')
                        {
                            nombreAttr = location.getProtocolo() + "://" + location.getHost() +
                                        ":" + location.getPuerto() + "/" + nombreAttr;
                        }
                        else if (nombreAttr.substr(0, 4) == "http")
                        {
                            //Nombre de Internet
                        }
                        else
                        {
                            //Cortamos hasta encontrar el nombre del archivo
                            while (nombreAttr[0] == '.' || nombreAttr[0] == '/')
                            {
                                nombreAttr = nombreAttr.substr(1, nombreAttr.size()-1);
                            }

                            nombreAttr = location.getProtocolo() + "://" + location.getHost() +
                                        ":" + location.getPuerto() + "/" +  nombreAttr;
                        }

                        obj->setValue(nombreAttr);
                    }

                    objetosDOM.append(obj);
                }
            }
            else if (myNombre == "br")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }
            }
            else if (myNombre == "hr")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;

                    //Insertamos los guiones tan caracteristicos de un hr
                    string guiones = "";

                    for (unsigned i = 0; i < LARGO_VENTANA*2; i+=TAM_FUENTE_CUERPO)
                    {
                        guiones.push_back('-');
                    }

                    f->escribirPalabra(ventana, guiones, *actualX, *actualY, "LinLibertine",
                                       TAM_FUENTE_CUERPO, 0, 0, 0);

                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }
            }
            else if (myNombre == "h1")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO + (TAM_FUENTE_CUERPO*6),
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "h2")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO + (TAM_FUENTE_CUERPO*5),
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "h3")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO + (TAM_FUENTE_CUERPO*4),
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "h4")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO + (TAM_FUENTE_CUERPO*3),
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "h5")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO + (TAM_FUENTE_CUERPO*2),
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "h6")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO + (TAM_FUENTE_CUERPO*1),
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "img")
            {
                //Buscamos el atributo 'src'
                int pos = actual->getAtributo().searchInList("src");

                if (pos != -1)
                {
                    string nombreAttr = actual->getValorAtributo().recover(pos);

                    //Conseguimos la imagen para ponerla en pantalla
                    if (copiaCache)
                    {
                        string nuevoAttr = "";
                        string x = "";
                        char letraActual;

                        //Como esta en nuestro cache buscamos hasta el punto inverso el archivo
                        for (int a = nombreAttr.size()-1; a >= 0; a--)
                        {
                            letraActual = nombreAttr[a];

                            if (letraActual == '/')
                            {
                                break;
                            }
                            else
                            {
                                x.push_back(letraActual);
                                nuevoAttr = x + nuevoAttr;
                                x = "";
                            }
                        }

                        int tamx = 0, tamy = 0;
                        bool posible;

                        *actualX = 0;
                        *actualY = tamy + *actualY + TAM_FUENTE_CUERPO;

                        //Cargamos la imagen
                        posible = f->cargarImagen(ventana, "cache/" + nuevoAttr, *actualX,
                                                  *actualY, &tamx, &tamy);

                        //Si no fue posible cargamos la imagen de broken
                        if (!posible)
                        {
                            f->cargarImagen(ventana, "img/broken.png", *actualX, *actualY,
                                            &tamx, &tamy);
                        }

                        *actualX = 0;
                        *actualY = tamy + *actualY + TAM_FUENTE_CUERPO;
                    }
                    else
                    {
                        //Creamos un cliente de internet para conseguir la imagen
                        ClienteHTTP c;

                        if (nombreAttr[0] == '.' && nombreAttr[0] == '.')
                        {
                            nombreAttr = location.getProtocolo() + "://" + location.getHost() +
                                        ":" + location.getPuerto() + "/" + nombreAttr;
                        }
                        else if (nombreAttr.substr(0, 4) == "http")
                        {
                            //Nombre de Internet
                        }
                        else
                        {
                            //Cortamos hasta encontrar el nombre del archivo
                            while (nombreAttr[0] == '.')
                            {
                                nombreAttr = nombreAttr.substr(1, nombreAttr.size()-1);
                            }

                            nombreAttr = location.getProtocolo() + "://" + location.getHost() +
                                        ":" + location.getPuerto() + "/" + nombreAttr;
                        }

                        c.guardarURL(&nombreAttr);

                        //Imagen descargada correctamente
                        if (c.getErrorCodigo() == 0)
                        {
                            if (c.getCodigoHTTP().substr(0,2) == "20")
                            {
                                //Ponemos la imagen en pantalla
                                int tamx = 0, tamy = 0;
                                bool posible;
                                *actualX = 0;
                                *actualY = tamy + *actualY + TAM_FUENTE_CUERPO;

                                //Ponemos la imagen
                                posible = f->cargarImagen(ventana, c.getUbicacionArchivo(),
                                                          *actualX, *actualY, &tamx, &tamy);

                                //De no haber podido ponemos la imagen de 'broken'
                                if (!posible)
                                {
                                    f->cargarImagen(ventana, "img/broken.png", *actualX,
                                                    *actualY, &tamx, &tamy);
                                }

                                *actualX = 0;
                                *actualY = tamy + *actualY + TAM_FUENTE_CUERPO;
                            }
                            else
                            {
                                //Si la imagen no fue encontrada se pone la imagen de 'broken'
                                int tamx = 0, tamy = 0;
                                f->cargarImagen(ventana, "img/broken.png", *actualX, *actualY,
                                                &tamx, &tamy);

                                *actualX = 0;
                                *actualY = tamy + *actualY + TAM_FUENTE_CUERPO;
                            }
                        }
                    }
                }
            }
            else if (myNombre == "p")
            {
                //Insertamos un salto de linea
                if (*actualX != 0)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }

                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                     actual->getValor(), 0, 0, 0, false, true, false);
                }
            }
            else if (myNombre == "input")
            {
                //Buscamos el tipo de input
                int posAttr = actual->getAtributo().searchInList("type");

                if (posAttr != -1)
                {
                    string nombreTipo = actual->getValorAtributo().recover(posAttr);

                    if (nombreTipo == "radio")
                    {
                        int tamRadio = 20;

                       //Revisamos que entra a lo largo de la pantalla
                        if (not copiaCache || *actualY < ALTO_VENTANA)
                        {
                            int posAttr = actual->getAtributo().searchInList("selected");

                            ObjectDOM *obj = new ObjectDOM();

                            //Generamos los circulos
                            SDL_Rect rect;
                            rect.x = *actualX;
                            rect.y = *actualY;
                            rect.w = tamRadio;
                            rect.h = tamRadio;

                            f->dibujarCirculo(ventana, tamRadio, *actualX, *actualY, 0, 0, 0);

                            f->dibujarCirculo(ventana, tamRadio-(TAM_LINEA_OPTION*2),
                                             *actualX + TAM_LINEA_OPTION,
                                             *actualY + TAM_LINEA_OPTION, 255, 255, 255);

                            if (posAttr != -1)
                            {
                                //Si tiene valor se dibuja el nuevo circulo
                                f->dibujarCirculo(ventana, tamRadio-(TAM_LINEA_OPTION*4),
                                                 *actualX + (TAM_LINEA_OPTION*2),
                                                 *actualY + (TAM_LINEA_OPTION*2), 0, 0, 0);

                                obj->setValue("true");
                            }
                            else
                            {
                                obj->setValue("false");
                            }

                            *actualX = *actualX + tamRadio + 2;

                            //Imprimimos el valor de la etiqueta
                            imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                             actual->getValor(), 0, 0, 0, false, false, false);

                            //Creamos el objeto del DOM
                            obj->setArea(rect);
                            obj->setFocus(false);
                            obj->setNombreTag("radio");

                            objetosDOM.append(obj);
                        }
                    }
                    else if (nombreTipo == "checkbox")
                    {
                        int tamRadio = 20;

                       //Revisamos que entra a lo largo de la pantalla
                        if (not copiaCache || *actualY < ALTO_VENTANA)
                        {
                            ObjectDOM *obj = new ObjectDOM();
                            int posAttr = actual->getAtributo().searchInList("checked");

                            //Generamos el cuadrado
                            SDL_Rect rect;

                            rect.x = *actualX;
                            rect.y = *actualY;
                            rect.w = tamRadio;
                            rect.h = tamRadio;

                            Uint32 color = SDL_MapRGB(ventana->format, 0,0,0);

                            SDL_FillRect(ventana, &rect, color);

                            rect.x = *actualX + TAM_LINEA_OPTION;
                            rect.y = *actualY + TAM_LINEA_OPTION;
                            rect.w = tamRadio - (TAM_LINEA_OPTION*2);
                            rect.h = tamRadio - (TAM_LINEA_OPTION*2);

                            color = SDL_MapRGB(ventana->format, 255,255,255);

                            SDL_FillRect(ventana, &rect, color);

                            //Ponemos el valor que tenga si esta checkeado o no
                            if (posAttr != -1)
                            {
                                *actualX = *actualX + 4;
                                imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO+5,
                                             "X", 0, 0, 0, false, true, false);

                                obj->setValue("true");
                            }
                            else
                            {
                                obj->setValue("false");
                            }

                            //Creamos el objeto del DOM
                            obj->setArea(rect);
                            obj->setFocus(false);
                            obj->setNombreTag("checkbox");

                            objetosDOM.append(obj);

                            *actualX = rect.x + rect.w + TAM_LINEA_OPTION*2 + 5;
                        }
                    }
                    else if (nombreTipo == "submit")
                    {
                        //Revisamos que entra a lo largo de la pantalla
                        if (not copiaCache || *actualY < ALTO_VENTANA)
                        {
                            SDL_Rect rect;

                            //Creamos el boton
                            rect.x = *actualX;
                            rect.y = *actualY;
                            rect.h = TAM_INPUT_H;
                            rect.w = TAM_INPUT_W;

                            Uint32 color = SDL_MapRGB(ventana->format, 0,0,0);

                            SDL_FillRect(ventana, &rect, color);

                            rect.x = *actualX + (TAM_ANCHO_LINEA_INPUT/2);
                            rect.y = *actualY + (TAM_ANCHO_LINEA_INPUT/2);
                            rect.h = TAM_INPUT_H - (TAM_ANCHO_LINEA_INPUT);
                            rect.w = TAM_INPUT_W - (TAM_ANCHO_LINEA_INPUT);

                            color = SDL_MapRGB(PantallaInicio::ventana->format, 230,230,230); //gris

                            SDL_FillRect(ventana, &rect, color);

                            *actualX = *actualX+TAM_ANCHO_LINEA_INPUT;
                            *actualY = *actualY+TAM_ANCHO_LINEA_INPUT;

                            //Imprimos el valor de la etiqueta
                            imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO, actual->getValor(),
                                             0, 0, 0, false, true, false);

                            //Ponemos el nuevo objeto del DOM
                            ObjectDOM *obj = new ObjectDOM();

                            obj->setNombreTag("button");
                            obj->setFocus(false);
                            obj->setValue(actual->getValor());
                            obj->setArea(rect);

                            objetosDOM.append(obj);

                            *actualX = rect.x + rect.w + 5;
                        }
                    }
                    else if (nombreTipo == "text")
                    {
                       //Revisamos que entra a lo largo de la pantalla
                        if (not copiaCache || *actualY < ALTO_VENTANA)
                        {
                            //Creamos el espacio para insertar el texto
                            SDL_Rect rect;
                            ObjectDOM *obj = new ObjectDOM();

                            int posAttr = actual->getAtributo().searchInList("value");

                            rect.x = *actualX;
                            rect.y = *actualY;
                            rect.h = TAM_INPUT_H;
                            rect.w = TAM_INPUT_W;

                            Uint32 color = SDL_MapRGB(ventana->format, 0,0,0);

                            SDL_FillRect(ventana, &rect, color);

                            rect.x = *actualX + (TAM_ANCHO_LINEA_INPUT/2);
                            rect.y = *actualY + (TAM_ANCHO_LINEA_INPUT/2);
                            rect.h = TAM_INPUT_H - (TAM_ANCHO_LINEA_INPUT);
                            rect.w = TAM_INPUT_W - (TAM_ANCHO_LINEA_INPUT);

                            color = SDL_MapRGB(PantallaInicio::ventana->format, 255,255,255);

                            SDL_FillRect(ventana, &rect, color);

                            *actualX = *actualX+TAM_ANCHO_LINEA_INPUT;
                            *actualY = *actualY+TAM_ANCHO_LINEA_INPUT;

                            //Buscamos el atributo para el valor
                            if (posAttr != -1)
                            {
                                imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                                 actual->getValorAtributo()[posAttr], 0, 0, 0,
                                                 false, true, false);

                                obj->setValue(actual->getValorAtributo()[posAttr]);
                            }

                            //Insertamos el objeto del DOM
                            obj->setArea(rect);
                            obj->setFocus(false);
                            obj->setNombreTag("text");

                            objetosDOM.append(obj);

                            *actualX = rect.x + rect.w + 5;
                        }
                    }
                }
            }
            else if (myNombre == "div" || myNombre == "body")
            {
                //Generamos un salto de linea
                if (*actualX != 0)
                {
                    *actualX = 0;
                    *actualY = *actualY + (TAM_FUENTE_CUERPO*2);
                }

               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    int pastX = *actualX, pastY = *actualY;

                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                    actual->getValor(), 0, 0, 0, true, true, false);

                    if (*actualY == pastY)
                    {
                        *actualX = pastX + (TAM_FUENTE_CUERPO * actual->getValor().size() + 2);
                    }
                }
            }
            else if (myNombre == "table")
            {
                //Generamos un salto de linea
                if (*actualX != 0)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }

               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "td")
            {
               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    int pastX = *actualX, pastY = *actualY;

                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                    actual->getValor(), 0, 0, 0, false, true, false);

                    //Imprimos en relacion del tamaño de la tabla
                    if (*actualY == pastY)
                    {
                        *actualX = pastX + (TAM_FUENTE_CUERPO * MAX_CHAR_TABLE);
                    }
                }
            }
            else if (myNombre == "tr")
            {
                //Generamos un salto de linea
                if (*actualX != 0)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }

               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                     actual->getValor(), 0, 0, 0, true, true, false);
                }
            }
            else if (myNombre == "th")
            {
               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    int pastX = *actualX, pastY = *actualY;

                    //Imprimimos en relacion del tamaño de la tabla y en negritas
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                    actual->getValor(), 0, 0, 0, false, true, true);

                    if (*actualY == pastY)
                    {
                        *actualX = pastX + (TAM_FUENTE_CUERPO * MAX_CHAR_TABLE);
                    }
                }
            }
            else if (myNombre == "label" || myNombre == "tt"
                     || myNombre == "span"  || myNombre == "media")
            {
                //Imprimimos las etiquetas de texto
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                     actual->getValor(), 0, 0, 0, false, true, false);
                }
            }
            else if (myNombre == "pre")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                     actual->getValor(), 0, 0, 0, false, false, false);
                }
            }
            else if (myNombre == "title")
            {
                //Establecemos el nombre de la ventana
                string x = "Navegador Web RiNW - " + actual->getValor();
                SDL_WM_SetCaption(x.c_str() , NULL);
            }
            else if (myNombre == "textarea")
            {
               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    //Generamos el cuadrado del area
                    SDL_Rect rect;

                    rect.x = *actualX;
                    rect.y = *actualY;
                    rect.h = TAM_TEXTAREA_H;
                    rect.w = TAM_TEXTAREA_W;

                    Uint32 color = SDL_MapRGB(ventana->format, 0,0,0);

                    SDL_FillRect(ventana, &rect, color);

                    rect.x = *actualX + (TAM_ANCHO_LINEA_TEXTAREA/2);
                    rect.y = *actualY + (TAM_ANCHO_LINEA_TEXTAREA/2);
                    rect.h = TAM_TEXTAREA_H - (TAM_ANCHO_LINEA_TEXTAREA);
                    rect.w = TAM_TEXTAREA_W - (TAM_ANCHO_LINEA_TEXTAREA);

                    color = SDL_MapRGB(PantallaInicio::ventana->format, 255,255,255);

                    SDL_FillRect(ventana, &rect, color);

                    //Ponemos el valor en la pantalla
                    if (actual->getValor() != "")
                    {
                        *actualX = *actualX + 5;
                        *actualY = *actualY + 2;
                        imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                         actual->getValor(), 0, 0, 0, false, true, false);

                    }

                    //Actualziamos el objeto en el DOM
                    ObjectDOM *obj = new ObjectDOM();

                    obj->setNombreTag("textarea");
                    obj->setFocus(false);
                    obj->setValue(actual->getValor());
                    obj->setArea(rect);

                    objetosDOM.append(obj);

                    *actualX = rect.x + rect.w + 5;
                    *actualY = rect.y + rect.h - TAM_INPUT_H;
                }
            }
            else if (myNombre == "select")
            {
               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    SDL_Rect rect;

                    //Generamos el espacio del select
                    rect.x = *actualX;
                    rect.y = *actualY;
                    rect.h = TAM_INPUT_H;
                    rect.w = TAM_INPUT_W;

                    Uint32 color = SDL_MapRGB(ventana->format, 0,0,0);

                    SDL_FillRect(ventana, &rect, color);

                    rect.x = *actualX + (TAM_ANCHO_LINEA_INPUT/2);
                    rect.y = *actualY + (TAM_ANCHO_LINEA_INPUT/2);
                    rect.h = TAM_INPUT_H - (TAM_ANCHO_LINEA_INPUT);
                    rect.w = TAM_INPUT_W - (TAM_ANCHO_LINEA_INPUT);

                    color = SDL_MapRGB(PantallaInicio::ventana->format, 230,230,230); //gris

                    SDL_FillRect(ventana, &rect, color);

                    //Agregamos el objeto al DOM
                    ObjectDOM* obj = new ObjectDOM();

                    obj->setNombreTag("select");
                    obj->setFocus(false);
                    obj->setValue("");
                    obj->setArea(rect);

                    objetosDOM.append(obj);

                    *actualX = rect.x + rect.w + 5;
                    *actualY = rect.y;
                }
            }
            else if (myNombre == "option")
            {
               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    SDL_Rect rect;

                    rect.x = *actualX;
                    rect.y = *actualY;
                    rect.h = TAM_INPUT_H;
                    rect.w = TAM_INPUT_W;

                    //Agregamos el objeto al DOM
                    ObjectDOM *obj = new ObjectDOM();

                    obj->setNombreTag("option");
                    obj->setFocus(false);
                    obj->setValue(actual->getValor());
                    obj->setArea(rect);

                    objetosDOM.append(obj);
                }
            }
            else if (myNombre == "button" || myNombre == "submit")
            {
               //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    //Creamos la superficie del boton
                    SDL_Rect rect;

                    rect.x = *actualX;
                    rect.y = *actualY;
                    rect.h = TAM_INPUT_H;
                    rect.w = TAM_INPUT_W;

                    Uint32 color = SDL_MapRGB(ventana->format, 0,0,0);

                    SDL_FillRect(ventana, &rect, color);

                    rect.x = *actualX + (TAM_ANCHO_LINEA_INPUT/2);
                    rect.y = *actualY + (TAM_ANCHO_LINEA_INPUT/2);
                    rect.h = TAM_INPUT_H - (TAM_ANCHO_LINEA_INPUT);
                    rect.w = TAM_INPUT_W - (TAM_ANCHO_LINEA_INPUT);

                    color = SDL_MapRGB(PantallaInicio::ventana->format, 230,230,230); //gris

                    SDL_FillRect(ventana, &rect, color);

                    *actualX = *actualX+TAM_ANCHO_LINEA_INPUT;
                    *actualY = *actualY+TAM_ANCHO_LINEA_INPUT;

                    //Imprimos el valor del dato
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO, actual->getValor(),
                                     0, 0, 0, false, true, false);

                    //Actualizamos el objeto en el DOM
                    ObjectDOM *obj = new ObjectDOM();

                    obj->setNombreTag("button");
                    obj->setFocus(false);
                    obj->setValue(actual->getValor());
                    obj->setArea(rect);

                    objetosDOM.append(obj);

                    *actualX = rect.x + rect.w + 5;
                }
            }
            else if (myNombre == "ul")
            {
                //Insertamos un espacio y decimos que los li usaran asteriscos
                if (*actualX != 0)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }

                *valorListaUl = "*";
            }
            else if (myNombre == "ol")
            {
                //Insertamos un espacio y decimos que los li usaran numeros
                if (*actualX != 0)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }

                *valorListaUl = "1";
            }
            else if (myNombre == "li")
            {
                if (*actualX != 0)
                {
                    *actualX = 0;
                    *actualY = *actualY + TAM_FUENTE_CUERPO;
                }

                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    //Sacamos el valor del li con su tipo de lista
                    if (*valorListaUl == "*")
                    {
                        imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                        "* " + actual->getValor(), 0, 0, 0, false, true, false);
                    }
                    else
                    {
                        string valUL = *valorListaUl + " " + actual->getValor();

                        imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                         valUL, 0, 0, 0, false, true, false);

                        //Aumentamos el contador de la lista
                        stringConvert ss;
                        int val = ss.strToInt(*valorListaUl);
                        val++;
                        cout << val << endl;
                        *valorListaUl = ss.intToStr(val);
                    }
                }
            }
            else if (myNombre == "strong" || myNombre == "b")
            {
                //Revisamos que entra a lo largo de la pantalla
                if (not copiaCache || *actualY < ALTO_VENTANA)
                {
                    //Imprimos en negritas
                    imprimirEtiqueta(actualX, actualY, TAM_FUENTE_CUERPO,
                                 actual->getValor(), 0, 0, 0, false, true, true);
                }
            }
        }
    }

    //Procesamos cada uno de los hijos que tenga
    for (unsigned i = 0; i < hijo->getHijos()->getSize(); i++) //Procesamos a los hijos
    {
        procesarXMLPantalla(hijo->getHijos()->recover(i), actualX, actualY,
                            copiaCache, valorListaUl);
    }
}

/**
 * Permite la carga de la pagina en relación del cuadro de la URL
 *
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 * @param paginaNormal booleano para saber si la pagina es normal o una carga
 */
void PantallaInicio::irPagina(bool paginaNormal)
{
    bool change = false;

    //Revisamos si tiene el simbolo '|' la url actual y de tenerlo lo quitamos
    if (urlActual[urlActual.size()-1] == '|')
    {
        urlActual = urlActual.substr(0, urlActual.size()-1);
        change = true;
    }

    if (paginaNormal)
    {
        //Actualizamos el objeto history
        if (history.getSize() != 0)
        {
            while (history.getSize() != actualHistory)
            {
                history.deleteForPosition(actualHistory);
            }
        }

        history.append(urlActual);
        actualHistory++;
    }

    //Si es la pagina de About
    if (urlActual == "about" || urlActual == "about|")
    {
        paginaAbout = true;
        paginaError = false;
    }
    else
    {
        paginaAbout = false;

        //Abrimos una url
        location.guardarURL(&urlActual);

        if (location.getErrorCodigo() != 0)
        {
            paginaError = true;
        }
        else
        {
            int posTag = location.getUbicacionArchivo().rfind('.');

            //Conseguimos la extension de dicha página
            if (posTag > 0)
            {
                int posAux = location.getUbicacionArchivo().size()-posTag;
                string ext = location.getUbicacionArchivo().substr(posTag+1, posAux);

                //Si es un archivo html lo procesamos
                if (!(ext.substr(0, 3) == "png" || ext.substr(0, 3) == "jpg"))
                {
                    string name = f->getPath() + "cache/procesado.jex";
                    ofstream outfile(name.c_str(), ios::trunc);

                    short deep = 0;
                    paginaError = false;

                    raiz.leerArchivo(location.getUbicacionArchivo());
                    raiz.imprimirTerminal(deep, &outfile);

                    outfile.close();
                }
                else //Es una imagen
                {
                    int x, y;
                    f->cargarImagen(ventana, location.getUbicacionArchivo(), 0,0, &x, &y);

                    //Actualizamos el scroll
                    scroll.setMaximoScrollX(x);
                    scroll.setMaximoScrollY(y);
                    scroll.setActualScrollX(0);
                    scroll.setActualScrollY(0);
                }
            }
            else
            {
                //Tomamos Creamos el archivo donde se procesará el documento html
                string name = f->getPath() + "cache/procesado.jex";
                ofstream outfile(name.c_str(), ios::trunc);

                short deep = 0;
                paginaError = false;

                raiz.leerArchivo(location.getUbicacionArchivo());
                raiz.imprimirTerminal(deep, &outfile);

                outfile.close();
            }
        }
    }

    //Reinsertamos el caret si lo tenia
    if (change)
    {
        urlActual.push_back('|');
    }
}

/**
 * Actualiza al objeto dinamicos que haya en la pantalla como las areas de texto
 *
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 * @param obj        Apuntador con el objeto a modificar
 * @param quitarFoco Booleano que sirve para quitar o poner el foco del objeto
 */
void PantallaInicio::updateDomObject(ObjectDOM *obj, bool quitarFoco)
{
    //Buscamos el nombre de la etiqueta que se un cuadro de texto
    if (obj->getNombreTag().substr(0,4) == "text")
    {
        //Sacamos el area y el valor
        SDL_Rect recta = obj->getArea();
        string value = obj->getValue();

       if (quitarFoco)
        {
            //Le quitamos el foco
            obj->setFocus(false);

            if (value[value.size()-1] == '|')
            {
                value = value.substr(0, value.size()-1);
                obj->setValue(value);
            }
        }
        else //Agrega el Foco
        {
            obj->setFocus(true);

            if (value[value.size()-1] != '|')
            {
                value.push_back('|');
                obj->setValue(value);
            }
        }

        //Redibujamos el cuadro con el dato nuevo
        Uint32 color = SDL_MapRGB(PantallaInicio::ventana->format, 255,255,255);
        SDL_FillRect(ventana, &recta, color);

        float actualX = recta.x, actualY = recta.y;

        imprimirEtiqueta(&actualX, &actualY, TAM_FUENTE_CUERPO, value, 0, 0, 0,
                         false, true, false);

        SDL_UpdateRect(ventana, recta.x, recta.y, recta.w, recta.h);
    }
    else if (obj->getNombreTag() == "radio")
    {
        //Actualizamos el radio redibujando los circulos
        int tamRadio = 20;

        int *actualX = new int, *actualY = new int;

        *actualX = obj->getArea().x;
        *actualY = obj->getArea().y;

        f->dibujarCirculo(ventana, tamRadio, *actualX, *actualY, 0, 0, 0);

        f->dibujarCirculo(ventana, tamRadio-(TAM_LINEA_OPTION*2),
                         *actualX + TAM_LINEA_OPTION,
                         *actualY + TAM_LINEA_OPTION, 255, 255, 255);

        if (obj->getValue() != "true")
        {
            obj->setValue("true");
        }

        f->dibujarCirculo(ventana, tamRadio-(TAM_LINEA_OPTION*4),
                         *actualX + (TAM_LINEA_OPTION*2),
                         *actualY + (TAM_LINEA_OPTION*2), 0, 0, 0);

        SDL_Rect rect = obj->getArea();
        SDL_UpdateRect(ventana, rect.x, rect.y, rect.w, rect.h);

        free(actualX);
        free(actualY);
    }
    else if (obj->getNombreTag() == "checkbox")
    {
        //Redibujamos el cuadro del checkbox
        SDL_Rect rect = obj->getArea();

        Uint32 color = SDL_MapRGB(ventana->format, 255,255,255);
        SDL_FillRect(ventana, &rect, color);

        if (obj->getValue() == "true")
        {
            obj->setValue("false");
        }
        else
        {
            obj->setValue("true");

            float actualX = obj->getArea().x + 1, actualY = obj->getArea().y - 2;

            imprimirEtiqueta(&actualX, &actualY, TAM_FUENTE_CUERPO+5,
                         "X", 0, 0, 0, false, true, false);
        }

        SDL_UpdateRect(ventana, rect.x, rect.y, rect.w, rect.h);
    }
    else if (obj->getNombreTag() == "select")
    {
        //Buscamos el select
        bool selectEncontrado = false;
        int contadorObjetos = 0;
        Uint32 color = SDL_MapRGB(ventana->format, 230,230,230);
        Uint32 color2 = SDL_MapRGB(ventana->format, 255,255,255);

        for (unsigned i = 0; i < objetosDOM.getSize(); i++)
        {
            if (!selectEncontrado)
            {
                if (objetosDOM[i] == obj)
                {
                    selectEncontrado = true;

                    obj->setFocus(!obj->getFocus());
                }
            }
            else //buscamos los option para hacerlos como una lista
            {
                if (objetosDOM[i]->getNombreTag() != "option")
                {
                    break;
                }
                else
                {
                    contadorObjetos++;

                    SDL_Rect newRect = obj->getArea();
                    newRect.y = newRect.y + (newRect.h * contadorObjetos);

                    objetosDOM[i]->setArea(newRect);

                    //Si tiene el foto para desplegar las opciones
                    if (obj->getFocus() == true)
                    {
                        float x = newRect.x, y = newRect.y;
                        SDL_FillRect(ventana, &newRect, color);
                        imprimirEtiqueta(&x, &y,  TAM_FUENTE_CUERPO, objetosDOM[i]->getValue(),
                                         0,0,0, false, true, false);
                    }
                    else
                    {
                        SDL_FillRect(ventana, &newRect, color2);
                    }

                    SDL_UpdateRect(ventana, newRect.x, newRect.y, newRect.w, newRect.h);
                }
            }
        }
    }
    else if (obj->getNombreTag() == "option")
    {
        //Buscamos el select
        bool selectEncontrado = false;
        int contadorObjetos = 0;
        Uint32 color = SDL_MapRGB(ventana->format, 230,230,230);
        Uint32 color2 = SDL_MapRGB(ventana->format, 255,255,255);

        for (unsigned i = 0; i < objetosDOM.getSize(); i++)
        {
            if (!selectEncontrado)
            {
                if (objetosDOM[i]->getNombreTag() == "select" && objetosDOM[i]->getFocus())
                {
                    //Quitamos las opciones de los option y lo ponemos en su select
                    selectEncontrado = true;
                    objetosDOM[i]->setValue(obj->getValue());
                    objetosDOM[i]->setFocus(false);

                    SDL_Rect newRect = objetosDOM[i]->getArea();

                    float x = newRect.x, y = newRect.y;
                    SDL_FillRect(ventana, &newRect, color);
                    imprimirEtiqueta(&x, &y,  TAM_FUENTE_CUERPO, objetosDOM[i]->getValue(),
                                     0,0,0, false, true, false);

                    SDL_UpdateRect(ventana, newRect.x, newRect.y, newRect.w, newRect.h);

                    obj = objetosDOM[i];
                }
            }
            else //buscamos los option para hacerlos como una lista
            {
                if (objetosDOM[i]->getNombreTag() != "option")
                {
                    break;
                }
                else
                {
                    contadorObjetos++;

                    SDL_Rect newRect = obj->getArea();
                    newRect.y = newRect.y + (newRect.h * contadorObjetos);

                    objetosDOM[i]->setArea(newRect);
                    SDL_FillRect(ventana, &newRect, color2);

                    SDL_UpdateRect(ventana, newRect.x, newRect.y, newRect.w, newRect.h);
                }
            }
        }
    }
}

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
string PantallaInicio::cargarLetra(string palabra, int letra, bool control, bool shift)
{
    string cad = palabra;
    cad = cad.substr(0, cad.size()-1);

    if (control)
    {
        if (letra == 'v' || letra == 'V')
        {
            //Futuro para Control+V
        }
        else if (letra == 'c' || letra == 'C')
        {
            //Futuro para Control+C
        }
    }
    else if (shift)
    {
        if (isupper(letra) || islower(letra))
        {
            cad.push_back(toupper(letra));
        }
        else if (letra == '.')
        {
            cad.push_back(':');
        }
        else if (letra == '1')
        {
            cad.push_back('!');
        }
        else if (letra == '2')
        {
            cad.push_back('"');
        }
        else if (letra == '3')
        {
            cad.push_back('#');
        }
        else if (letra == '4')
        {
            cad.push_back('$');
        }
        else if (letra == '5')
        {
            cad.push_back('%');
        }
        else if (letra == '6')
        {
            cad.push_back('&');
        }
        else if (letra == '7')
        {
            cad.push_back('/');
        }
        else if (letra == '8')
        {
            cad.push_back('(');
        }
        else if (letra == '9')
        {
            cad.push_back(')');
        }
        else if (letra == '0')
        {
            cad.push_back('=');
        }
        else if (letra == '\'')
        {
            cad.push_back('?');
        }
        else if (letra == '+')
        {
            cad.push_back('*');
        }
        else if (letra == ',')
        {
            cad.push_back(';');
        }
        else if (letra == '-')
        {
            cad.push_back('_');
        }
        else if (letra == '{')
        {
            cad.push_back('[');
        }
        else if (letra == '}')
        {
            cad.push_back(']');
        }
        else
        {
            cad.push_back(letra);
        }
    }
    else
    {
        if (letra >= 256 && letra <= 265) //Letras del teclado derechas del 1-9
        {
            cad.push_back('0' + (letra-256));
        }
        else if (letra == 266)
        {
            cad.push_back('.');
        }
        else if (letra == 267)
        {
            cad.push_back('/');
        }
        else if (letra == 268)
        {
            cad.push_back('*');
        }
        else if (letra == 269)
        {
            cad.push_back('-');
        }
        else if (letra == 270)
        {
            cad.push_back('+');
        }
        else if (letra == 0)
        {
            cad.push_back('1');
        }
        else
        {
            cad.push_back(letra);
        }
    }

    cad.push_back('|');

    return cad;
}

/**
 * Gestina los eventos en la ventana
 *
 * Imprime el contenido de una etiqueta en una ubicacion especifica
 */
void PantallaInicio::imprimirPantalla()
{
    //Creamos las variables y banderas
    SDL_Event Evento1;
    bool Fin = false;
    bool direcionActive = false;
    bool shift = false;
    bool control = false;

    //Recargamos la barra de navegación y la ventana con la pantalla de about
    irPagina("normal");
    PantallaInicio::recargarBarraNavegacion();
    imprimirPantallaInterna();

    //Iteramos para los eventos
    while (!Fin)
    {
        //Esperamos un neuvo evento
        SDL_WaitEvent(&Evento1);

        //Apreto el boton del ratón
        if (Evento1.type == SDL_MOUSEBUTTONDOWN)
        {
            int x = Evento1.motion.x, y = Evento1.motion.y;

            if (y > 0 && y < 35) //Dentro de barra de Navegación
            {
                if (x > 5 && x < 35) //Boton Atras
                {
                    //Se carga el history anterior
                    if (direcionActive)
                    {
                        direcionActive = false;
                        urlActual = urlActual.substr(0, urlActual.size()-1);
                    }

                    if (actualHistory > 1)
                    {
                        actualHistory--;
                        urlActual = history[actualHistory-1];
                        irPagina(false);
                    }

                    //Recargamos la barra de navegacion y la pantalla interna
                    imprimirPantallaInterna();
                    recargarBarraNavegacion();
                }
                else if (x > 40 && x < 75) //Boton Adelante
                {
                    //Obtenemos el siguiente History
                    if (direcionActive)
                    {
                        direcionActive = false;
                        urlActual = urlActual.substr(0, urlActual.size()-1);
                    }

                    if (actualHistory < history.getSize())
                    {
                        actualHistory++;
                        urlActual = history[actualHistory-1];
                        irPagina(false);
                    }

                    //Recargamos la barra de navegación y la pantalla interna
                    imprimirPantallaInterna();
                    recargarBarraNavegacion();
                }
                else if (x > 750) //Boton del final
                {
                    //Recargamos el contenido de la ventana con la pagina de internet nueva
                    if (direcionActive)
                    {
                        direcionActive = false;
                        urlActual = urlActual.substr(0, urlActual.size()-1);
                    }

                    actualIcono = "img/cancel.png";
                    recargarBarraNavegacion();

                    irPagina(true);

                    imprimirPantallaInterna();
                    actualIcono = "img/refresh.png";
                    recargarBarraNavegacion();
                }
                else //Barra de Direccion
                {
                    //Quita el foto de los objetos del dom
                    if (!direcionActive)
                    {
                        for (unsigned i = 0; i < objetosDOM.getSize(); i++)
                        {
                            if (objetosDOM.recover(i)->getNombreTag().substr(0,4) == "text")
                            {
                                updateDomObject(objetosDOM.recover(i), true);
                            }
                        }

                        //Se agrega el foco a la barra de navegacion
                        direcionActive = true;
                        urlActual.push_back('|');
                        actualIcono = "img/go.png";
                    }

                    recargarBarraNavegacion();
                }
            }
            else //Dentro de la pagina
            {
                //Creamos la ubicación de la pagina interna
                SDL_Rect miRect;

                miRect.x = 60;
                miRect.y = ALTO_VENTANA - (ALTO_BARRA_NAV*2.5);
                miRect.w = (TAM_FUENTE_ERROR-4) * 18;
                miRect.h = 40;

                //Si es pagina de error y apreto el boton de busqueda
                if (paginaError && x >= miRect.x && x <= miRect.x + miRect.w
                                && y >= miRect.y && y <= miRect.y + miRect.h)
                {
                    paginaError = false;

                    string aux;

                    //Recargamos la ubicación con la pagina de Yahoo
                    for (unsigned int i = 0; i < urlActual.size(); i++)
                    {
                        if (urlActual[i] == ' ')
                        {
                            aux.push_back('+');
                        }
                        else
                        {
                            aux.push_back(urlActual[i]);
                        }
                    }

                    urlActual = "https://mx.search.yahoo.com/search?p=" + aux;
                    actualIcono = "img/cancel.png";
                    recargarBarraNavegacion();

                    irPagina(true);

                    imprimirPantallaInterna();
                    actualIcono = "img/refresh.png";
                    recargarBarraNavegacion();
                }
                else //Buscando Eventos del DOM
                {
                    SDL_Rect recta;

                    //Recorremos todos los objetos existentes
                    for (unsigned i = 0; i < objetosDOM.getSize(); i++)
                    {
                        recta = objetosDOM[i]->getArea();

                        string tagname = objetosDOM[i]->getNombreTag();

                        //Sacamos el nombre de las etiquetas con eventos
                        if (tagname == "a")
                        {
                            //Revisamos que el evento este en el area de la figura
                            if (recta.x <= x && x <= (recta.x+recta.w) && recta.y <= y
                                && y <= (recta.y+recta.h))
                            {
                                //Como es un 'a' redireciona a la pagina del objeto DOM
                                if (direcionActive)
                                {
                                    direcionActive = false;
                                    urlActual = urlActual.substr(0, urlActual.size()-1);
                                }

                                urlActual = objetosDOM[i]->getValue();

                                actualIcono = "img/cancel.png";
                                recargarBarraNavegacion();

                                irPagina(true);

                                actualIcono = "img/refresh.png";
                                recargarBarraNavegacion();
                                imprimirPantallaInterna();
                            }
                        }
                        else if (tagname == "textarea" || tagname == "text")
                        {
                            //Revisamos que el evento este en el area de la figura
                            if (recta.x <= x && x <= (recta.x+recta.w) && recta.y <= y &&
                                y <= (recta.y+recta.h))
                            {
                                //Como es un text solo recupera el foco
                                updateDomObject(objetosDOM[i], false);
                                direcionActive = false;

                                if (urlActual[urlActual.size()-1] == '|')
                                {
                                    urlActual = urlActual.substr(0, urlActual.size()-1);
                                    recargarBarraNavegacion();
                                }
                            }
                            else
                            {
                                updateDomObject(objetosDOM[i], true);
                            }
                        }
                        else if (tagname == "checkbox" || tagname == "radio")
                        {
                            //Revisamos que el evento este en el area de la figura y recupera foco
                            if (recta.x <= x && x <= (recta.x+recta.w) && recta.y <= y &&
                                y <= (recta.y+recta.h))
                            {
                                updateDomObject(objetosDOM[i], false);
                            }
                        }
                        else if (tagname == "select" || tagname == "option")
                        {
                            //Revisamos que el evento este en el area de la figura y recupera foco
                            if (recta.x <= x && x <= (recta.x+recta.w) && recta.y <= y &&
                                y <= (recta.y+recta.h))
                            {
                                updateDomObject(objetosDOM[i], false);
                            }
                        }
                    }
                }

                //Actualizamos el cursor de la barra de navegacion
                if (direcionActive)
                {
                    direcionActive = false;

                    if (urlActual[urlActual.size()-1] == '|')
                    {
                        urlActual = urlActual.substr(0, urlActual.size()-1);
                        recargarBarraNavegacion();
                    }
                }
            }
        }
        else if (Evento1.type == SDL_KEYDOWN) //Se apreto una tecla
        {
            int letra = Evento1.key.keysym.sym;

            if (Evento1.key.keysym.sym == SDLK_RSHIFT || Evento1.key.keysym.sym == SDLK_LSHIFT)
            {
                shift = true;
            }
            else if (Evento1.key.keysym.sym == SDLK_RCTRL || Evento1.key.keysym.sym == SDLK_LCTRL)
            {
                control = true;
            }
            else if (Evento1.key.keysym.sym == SDLK_LEFT)
            {
                //Si es una imagen, entonces se puede mover la imagen a la izquierda
                if (!paginaError && (scroll.getActualScrollX() < 0) )
                {
                    scroll.setActualScrollX(scroll.getActualScrollX() + SALTO_SCROLL);
                    imprimirPantallaInterna();
                    recargarBarraNavegacion();
                    SDL_UpdateRect(ventana, 0,0,0,0);
                }
            }
            else if (Evento1.key.keysym.sym == SDLK_RIGHT)
            {
                //Si es una imagen, entonces se puede mover la imagen a la derecha
                if (!paginaError &&
                    ((-1)*scroll.getActualScrollX() < scroll.getMaximoScrollX() - LARGO_VENTANA) )
                {
                    scroll.setActualScrollX(scroll.getActualScrollX() - SALTO_SCROLL);
                    imprimirPantallaInterna();
                    recargarBarraNavegacion();
                    SDL_UpdateRect(ventana, 0,0,0,0);
                }
            }
            else if (Evento1.key.keysym.sym == SDLK_UP)
            {
                //Si entra en los limites de la ventana, se recorre la pagina para arriba
                if ((scroll.getActualScrollY() < ALTO_BARRA_NAV) && !paginaError)
                {
                    //Se crea un cuadrado con los datos del scroll
                    SDL_Rect rectangulo;

                    scroll.setActualScrollY(scroll.getActualScrollY() + SALTO_SCROLL);
                    float acty = scroll.getActualScrollY(), actx = 0;

                    rectangulo.x = 0;
                    rectangulo.y = ALTO_BARRA_NAV;
                    rectangulo.w = LARGO_VENTANA;
                    rectangulo.h = ALTO_VENTANA - ALTO_BARRA_NAV;

                    Uint32 color = SDL_MapRGB(PantallaInicio::ventana->format, 255,255,255);
                    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

                    //Se busca una extension en el archivo
                    int posTag = location.getUbicacionArchivo().rfind('.');
                    string cad;

                    if (posTag > 0)
                    {
                        int tamAux = location.getUbicacionArchivo().size()-posTag;
                        string ext = location.getUbicacionArchivo().substr(posTag+1, tamAux);

                        //Si es imagen se imprime la pantalla
                        if ((ext.substr(0, 3) == "png" || ext.substr(0, 3) == "jpg"))
                        {
                            imprimirPantallaInterna();
                        }
                        else
                        {
                            //Si es documento, se eliminan los objetos
                            //y se reprocesa con una copia en la carpeta cache
                            objetosDOM.nullify();
                            procesarXMLPantalla(&raiz, &actx, &acty, true, &cad);
                        }
                    }
                    else
                    {
                        //Si es documento, se eliminan los objetos
                        //y se reprocesa con una copia en la carpeta cache
                        objetosDOM.nullify();
                        procesarXMLPantalla(&raiz, &actx, &acty, true, &cad);
                    }

                    //Recargamos la barra de navegacion
                    recargarBarraNavegacion();
                    SDL_UpdateRect(ventana, 0,0,0,0);
                }
            }
            else if (Evento1.key.keysym.sym == SDLK_DOWN)
            {
                //Si entra en los limites de la ventana, se recorre la pagina para arriba
                if (( (-1)*(scroll.getActualScrollY() - 40) < scroll.getMaximoScrollY() - ALTO_VENTANA/2)
                    && !paginaError)
                {
                    //Generamos el espacio para la pagina nueva
                    SDL_Rect rectangulo;
                    scroll.setActualScrollY(scroll.getActualScrollY() - SALTO_SCROLL);
                    float acty = scroll.getActualScrollY(), actx = 0;

                    rectangulo.x = 0;
                    rectangulo.y = ALTO_BARRA_NAV;
                    rectangulo.w = LARGO_VENTANA;
                    rectangulo.h = ALTO_VENTANA - ALTO_BARRA_NAV;

                    Uint32 color = SDL_MapRGB(PantallaInicio::ventana->format, 255,255,255);
                    SDL_FillRect(PantallaInicio::ventana, &rectangulo, color);

                    //Buscamos la extension en la pagina
                    int posTag = location.getUbicacionArchivo().rfind('.');
                    string cad;

                    if (posTag > 0)
                    {
                        int tamAux = location.getUbicacionArchivo().size()-posTag;
                        string ext = location.getUbicacionArchivo().substr(posTag+1, tamAux);

                        //Si es imagen se actualiza la pantalla
                        if ((ext.substr(0, 3) == "png" || ext.substr(0, 3) == "jpg"))
                        {
                            imprimirPantallaInterna();
                        }
                        else
                        {
                            //Si es documento, se eliminan los objetos
                            //y se reprocesa con una copia en la carpeta cache
                            objetosDOM.nullify();
                            procesarXMLPantalla(&raiz, &actx, &acty, true, &cad);
                        }
                    }
                    else
                    {
                        //Si es documento, se eliminan los objetos
                        //y se reprocesa con una copia en la carpeta cache
                        objetosDOM.nullify();
                        procesarXMLPantalla(&raiz, &actx, &acty, true, &cad);
                    }

                    recargarBarraNavegacion();
                    SDL_UpdateRect(ventana, 0,0,0,0);
                }
            }
            else if (letra == 8) //Backspace
            {
                //Se recarga la barra de navegacion eliminando un caracter del final
                if (direcionActive)
                {
                    if (urlActual.size() != 1)
                    {
                        urlActual = urlActual.substr(0, urlActual.size()-2);
                        urlActual.push_back('|');
                    }

                    recargarBarraNavegacion();
                }
                else
                {
                    //Se elimina un caracter del final de un objeto con el foto activo en el DOM
                    for (unsigned i = 0; i < objetosDOM.getSize(); i++)
                    {
                        if (objetosDOM[i]->getNombreTag().substr(0,4) == "text" &&
                            objetosDOM[i]->getFocus())
                        {
                            string x = objetosDOM[i]->getValue();
                            x = x.substr(0, x.size()-2);
                            x.push_back('|');

                            objetosDOM[i]->setValue(x);

                            updateDomObject(objetosDOM[i], false);
                        }
                    }
                }
            }
            else if (letra == 13 || letra == 271) //Enter
            {
                //Se oprimio el enter en la barra de navegacion por lo que nos movemos a la
                //pagina de la barra de navegacion
                if (direcionActive)
                {
                    actualIcono = "img/cancel.png";
                    recargarBarraNavegacion();

                    irPagina(true);

                    actualIcono = "img/refresh.png";
                    imprimirPantallaInterna();
                    recargarBarraNavegacion();
                }
            }
            else
            {
                //Se genera oprimio otra cosa del teclado
                if (direcionActive)
                {
                    //Se agrega a la barra de navegacion el caracter
                    urlActual = cargarLetra(urlActual, letra, control, shift);
                    PantallaInicio::recargarBarraNavegacion();
                }
                else
                {
                    //Si hay un objeto con el foco activo, se agrega la letra al final
                    for (unsigned i = 0; i < objetosDOM.getSize(); i++)
                    {
                        if (objetosDOM[i]->getNombreTag().substr(0,4) == "text" &&
                            objetosDOM[i]->getFocus())
                        {
                            objetosDOM[i]->setValue(cargarLetra(objetosDOM[i]->getValue(),
                                                    letra, control, shift));

                            updateDomObject(objetosDOM[i], false);
                        }
                    }
                }
            }
        }
        else if (Evento1.type == SDL_KEYUP)
        {
            //Si son teclas de liberacion se eliminan las banderas
            if (Evento1.key.keysym.sym == SDLK_RSHIFT || Evento1.key.keysym.sym == SDLK_LSHIFT)
            {
                shift = false;
            }
            else if (Evento1.key.keysym.sym == SDLK_RCTRL || Evento1.key.keysym.sym == SDLK_LCTRL)
            {
                control = false;
            }
        }
        else if (Evento1.type == SDL_QUIT)
        {
            //Cerrar la aplicación
            Fin = true;
        }
    }
}
