#ifndef OBJECTDOM_H
#define OBJECTDOM_H

#include <SDL.h>
#include <string>

using namespace std;

/**
 * Clase estructural con los datos de un objeto dinamico del navegador
 * 
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx> * 
 */
class ObjectDOM
{
    private:
    ///Atributos
        SDL_Rect area;
        string nombreTag;
        string id;
        string clase;
        string value;
        bool focus;

    public:
    ///Constructor
        ObjectDOM();

    ///Getters
        SDL_Rect getArea();
        string getNombreTag();
        string getId();
        string getClase();
        string getValue();
        bool getFocus();

    ///Setters
        void setArea(SDL_Rect x);
        void setNombreTag(string x);
        void setId(string x);
        void setClase(string x);
        void setValue(string x);
        void setFocus(bool x);
};

#endif // OBJECTDOM_H
