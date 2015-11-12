#ifndef SCROLL_H
#define SCROLL_H

/**
 * Clase estructural con los datos de un scroll para la pantalla
 * 
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 */
class Scroll
{
    private:
    ///Atributos
        float maximoScrollY;
        float maximoScrollX;
        float actualScrollX;
        float actualScrollY;

    public:
    ///Constructor
        Scroll();

    ///Getters
        float getMaximoScrollX();
        float getActualScrollX();
        float getMaximoScrollY();
        float getActualScrollY();

    ///Setters
        void setMaximoScrollX(float x);
        void setActualScrollX(float x);
        void setMaximoScrollY(float x);
        void setActualScrollY(float x);
};

#endif // SCROLL_H
