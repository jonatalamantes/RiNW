#ifndef XMLESTRUCTURA_H
#define XMLESTRUCTURA_H

#include <fstream>
#include "List.h"

/**
 * Clase capaz de analizar un archivo XML con la estructura de Árbol
 *
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 */
class XMLEstructura
{
    private:
    ///Atributos
        string nombre;
        List<string> atributo;
        List<string> valorAtributo;
        List<XMLEstructura*>* hijos;
        XMLEstructura* padre;
        string valor;
        short tipo; //0 inicial, 1 terminal, 2 unitaria, 3 doble, 4 comentario

    ///Funciones privadas

        /**
         * Automata de estado finito para el analisis de las etiquetas
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param  archivo    Contenido de un archivo en memoria
         * @param  pos        Apuntador con la posicion desde donde comenzará
         * @param  valorPadre Valor que tendrá el nodo padre al termina la iteración
         * @return            Retorna la posicion donde se queda el algoritmo
         */
        unsigned automataEtiqueta(string *archivo, unsigned *pos, string *valorPadre);

        /**
         * Vacia la estructura desde el nodo actual.
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         */
        void vaciarEstructura();

    public:
    ///Constructor
        XMLEstructura();

    ///Destructor
        ~XMLEstructura();

    ///Getters
        string getNombre();
        List<string> getAtributo();
        List<string> getValorAtributo();
        List<XMLEstructura*>* getHijos();
        XMLEstructura* getPadre();
        string getValor();
        short getTipo();

    ///Setters
        void setNombre(string x);
        void setAtributo(List<string> x);
        void setValorAtributo(List<string> x);
        void setHijos(List<XMLEstructura*>* x);
        void setPadre(XMLEstructura* x);
        void setValor(string x);
        void setTipo(short x);

    ///Validadores
        bool esBinaria(string tag);
        bool esUnaria(string tag);

    ///Funciones de la clase

        /**
         * Toma la ubicacion de un archivo y genera el arbol desde el nodo actual
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param path  Ubicación del archivo
         */
        void leerArchivo(string path);

        /**
         * Imprime en la consola o en un archivo el contenido del árbol
         *
         * @param profundidad Profundidad desde donde inicia el algoritmo para poner espacios
         * @param flujo       Objeto de archivo donde se guardará el árbol analizado
         */
        void imprimirTerminal(short profundidad, ofstream *flujo);
};

#endif // XMLESTRUCTURA_H
