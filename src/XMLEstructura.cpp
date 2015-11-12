#include <fstream>
#include <cstring>
#include "XMLEstructura.h"

using namespace std;

///Constructor
XMLEstructura::XMLEstructura()
{
    nombre = "";
    padre = NULL;
    valor = "";
    tipo = -1;
    hijos = new List<XMLEstructura*>();
}

///Destructor
XMLEstructura::~XMLEstructura()
{
    vaciarEstructura();
}

///Getters
string XMLEstructura::getNombre()
{
    return XMLEstructura::nombre;
}

List<string> XMLEstructura::getAtributo()
{
    return XMLEstructura::atributo;
}

List<string> XMLEstructura::getValorAtributo()
{
    return XMLEstructura::valorAtributo;
}

List<XMLEstructura*>* XMLEstructura::getHijos()
{
    return XMLEstructura::hijos;
}

XMLEstructura* XMLEstructura::getPadre()
{
    return XMLEstructura::padre;
}

string XMLEstructura::getValor()
{
    return XMLEstructura::valor;
}

short XMLEstructura::getTipo()
{
    return XMLEstructura::tipo;
}

///Setters
void XMLEstructura::setNombre(string x)
{
    XMLEstructura::nombre = x;
}

void XMLEstructura::setAtributo(List<string> x)
{
    XMLEstructura::atributo = x;
}

void XMLEstructura::setValorAtributo(List<string> x)
{
    XMLEstructura::valorAtributo = x;
}

void XMLEstructura::setHijos(List<XMLEstructura*>* x)
{
    XMLEstructura::hijos = x;
}

void XMLEstructura::setPadre(XMLEstructura* x)
{
    XMLEstructura::padre = x;
}

void XMLEstructura::setValor(string x)
{
    XMLEstructura::valor = x;
}

void XMLEstructura::setTipo(short x)
{
    XMLEstructura::tipo = x;
}

/**
 * Automata de estado finito para el analisis de las etiquetas
 *
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 * @param  archivo    Nombre del archivo a analizar
 * @param  pos        Apuntador con la posicion desde donde comenzará
 * @param  valorPadre Valor que tendrá el nodo padre al termina la iteración
 * @return            Retorna la posicion donde se queda el algoritmo
 */
unsigned XMLEstructura::automataEtiqueta(string *archivo, unsigned *pos, string *valorPadre)
{
    unsigned estado = 1;
    unsigned i;
    string aux = "";
    char actual;

    //Comenzamos a iterar desde la posicion
    for (i = *pos; i < archivo->size(); i++)
    {
        actual = archivo->at(i);

        if (estado == 1)
        {
            if (actual == '<')
            {
                *valorPadre = aux;
                aux = "";
                estado = 2;
            }
            else
            {
                aux.push_back(actual);
            }
        }
        else if (estado == 2)
        {
            if (actual == '!')
            {
                tipo = 0;
                estado = 3;
            }
            else if (actual == '/')
            {
                tipo = 1;
                estado = 4;
            }
            else if (isupper(actual) || islower(actual))
            {
                aux.push_back(actual);
                estado = 8;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 3)
        {
            if (actual == '>')
            {
                estado = 7;
            }
            else if (actual == '-')
            {
                estado = 17;
            }
            else
            {
                aux.push_back(actual);
                estado = 3;
            }
        }
        else if (estado == 4)
        {
            if (isupper(actual) || islower(actual))
            {
                estado = 5;
                aux.push_back(actual);
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 5)
        {
            if (isupper(actual) || islower(actual) || isdigit(actual))
            {
                estado = 5;
                aux.push_back(actual);
            }
            else if (isspace(actual))
            {
                estado = 6;
            }
            else if (actual == '>')
            {
                estado = 7;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 6)
        {
            if (actual == '>')
            {
                estado = 7;
            }
            else if (isspace(actual))
            {
                estado = 6;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 7)
        {
            nombre = aux;
            break;
        }
        else if (estado == 8)
        {
            if (isupper(actual) || islower(actual) || isdigit(actual))
            {
                aux.push_back(actual);
                estado = 8;
            }
            else if (isspace(actual))
            {
                nombre = aux;
                estado = 9;
            }
            else if (actual == '>')
            {
                nombre = aux;
                estado = 16;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 9)
        {
            aux = "";

            if (isspace(actual))
            {
                estado = 9;
            }
            else if (isupper(actual) || islower(actual) || actual == '-' || actual == '_')
            {
                aux.push_back(actual);
                estado = 10;
            }
            else if (actual == '<')
            {
                estado = 16;
            }
            else if (actual == '/')
            {
                estado = 15;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 10)
        {
            if (islower(actual) || isupper(actual) || actual == '-' || actual == '_')
            {
                estado = 10;
                aux.push_back(actual);
            }
            else if (isspace(actual))
            {
                estado = 10;
            }
            else if (actual == '=')
            {
                atributo.append(aux);
                aux = "";
                estado = 11;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 11)
        {
            if (isspace(actual))
            {
                estado = 11;
            }
            else if (actual == '"')
            {
                estado = 12;
            }
            else if (isdigit(actual))
            {
                estado = 14;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 12)
        {
            if (actual == '"')
            {
                estado = 13;
            }
            else
            {
                aux.push_back(actual);
            }
        }
        else if (estado == 13)
        {
            valorAtributo.append(aux);
            aux = "";

            if (actual == '>')
            {
                estado = 16;
            }
            else if (actual == '/')
            {
                estado = 15;
            }
            else if (isspace(actual))
            {
                estado = 9;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 14)
        {
            if (isdigit(actual))
            {
                aux.push_back(actual);
                estado = 14;
            }
            else if (actual == '>')
            {
                valorAtributo.append(aux);
                aux = "";
                estado = 16;
            }
            else if (actual == '/')
            {
                valorAtributo.append(aux);
                estado = 15;
            }
            else if (isspace(actual))
            {
                valorAtributo.append(aux);
                estado = 9;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 15)
        {
            tipo = 2;

            if (actual == '>')
            {
                estado = 16;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 16)
        {
            if (tipo == -1)
            {
                tipo = 3;
            }

            break;
        }
        else if (estado == 17)
        {
            if (actual == '-')
            {
                aux = "";
                estado = 18;
            }
            else
            {
                aux.push_back('-');
                aux.push_back(actual);
                estado = 3;
            }
        }
        else if (estado == 18)
        {
            if (actual == '-')
            {
                atributo.append("value");
                valorAtributo.append(aux);
                estado = 19;
            }
            else
            {
                aux.push_back(actual);
                estado = 18;
            }
        }
        else if (estado == 19)
        {
            if (actual == '-')
            {
                estado = 20;
            }
            else
            {
                aux.push_back('-');
                aux.push_back(actual);
                estado = 18;
            }
        }
        else if (estado == 20)
        {
            if (actual == '>')
            {
                tipo = 4;
                aux = "comentario";
                nombre = aux;
                estado = 7;
            }
            else
            {
                estado = 0;
            }
        }
        else //Estado = 0
        {
            break;
            estado = 0;
        }
    }

    *pos = i;
    return estado;
}

void XMLEstructura::vaciarEstructura()
{
    //Creamos un hijo
    XMLEstructura* hijo = NULL;

    if (hijos->getSize() > 0) //Tiene Hijos
    {
        //Vaciamos a los hijos
        for (unsigned i = 0; i < hijos->getSize(); i++)
        {
            hijos->recover(i)->vaciarEstructura();
        }

        //Liberamos a cada hijo
        for (unsigned i = 0; i < hijos->getSize(); i++)
        {
            hijo = hijos->recover(i);
            free(hijo);
        }

        //Liberamos a la lista de hijos
        hijos->nullify();
    }
}

bool XMLEstructura::esUnaria(string tag)
{
    if (tag == "br" || tag == "hr" || tag == "meta" || tag == "link" ||
        tag == "input" || tag == "param" || tag == "raiz")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool XMLEstructura::esBinaria(string tag)
{
    return not XMLEstructura::esUnaria(tag);
}

void XMLEstructura::leerArchivo(string path)
{
    vaciarEstructura();

    //Abrimos un archivo
    ifstream archivo;
    archivo.open(path.c_str());

    string contenido = "";
    string valorPadre = "";
    unsigned pos = 0;

    if (archivo.is_open())
    {
        char letra = archivo.get();
        bool inicio = false;

        //Comenzamos a leer el archivo desde el simbolo '<'
        while (letra != EOF)
        {
            if (not inicio)
            {
                if (letra == '<')
                {
                    contenido.push_back(letra);
                    inicio = true;
                }
            }
            else
            {
                contenido.push_back(letra);
            }

            letra = archivo.get();
        }
    }

    //Apuntamos al nodo actual y lo llamamos raiz
    XMLEstructura* xml = this;
    xml->setNombre("raiz");

    //Recuperamos la estructura de arbol
    while (pos < contenido.size())
    {
        //Creamos un hijo nuevo
        valorPadre = "";
        XMLEstructura* hijo = new XMLEstructura();

        //Le ponemos los datos del automata y le decimos que el padre es el nodo actual
        int estado = hijo->automataEtiqueta(&contenido, &pos, &valorPadre);
        hijo->setPadre(xml);

        //Eliminamos el contenido de espacios del valor del padre
        string val = "";
        bool encontrado = false;
        for (unsigned j = 0; j < valorPadre.size(); j++)
        {
            if (!encontrado)
            {
                if (!isspace(valorPadre[j]))
                {
                    val.push_back(valorPadre[j]);
                    encontrado = true;
                }
            }
            else
            {
                val.push_back(valorPadre[j]);
            }
        }

        valorPadre = val;

        //Revisamos que sea etiqueta valida
        if (estado != 0)
        {
            if (hijo->getTipo() == 3) //Es etiqueta Doble
            {
                //Si el padre ya tiene valor
                if (xml->getValor() != "")
                {
                    //Creamos una etiqueta intermedia con el valor del padre
                    XMLEstructura* nueva1 = new XMLEstructura;
                    nueva1->setNombre("media");
                    nueva1->setTipo(3);
                    nueva1->setValor(valorPadre);

                    XMLEstructura* nueva2 = new XMLEstructura;
                    nueva2->setNombre("media");
                    nueva2->setTipo(1);

                    (*(xml->getHijos())).append(nueva1);
                    (*(xml->getHijos())).append(nueva2);
                }
                else
                {
                    xml->setValor(valorPadre);
                }

                //Agregamos a la lista de hijos del nodo actual el hijo nuevo
                (*(xml->getHijos())).append(hijo);

                if (esBinaria(hijo->getNombre()))
                {
                    //El hijo ahora es el nodo actual
                    xml = hijo;
                }
            }
            else if (hijo->getTipo() == 1) //Etiqueta terinal
            {
                if (xml->getValor() != "")
                {
                    //Creamos una etiqueta intermedia con el valor del padre
                    XMLEstructura* nueva1 = new XMLEstructura;
                    nueva1->setNombre("media");
                    nueva1->setTipo(3);
                    nueva1->setValor(valorPadre);

                    XMLEstructura* nueva2 = new XMLEstructura;
                    nueva2->setNombre("media");
                    nueva2->setTipo(1);

                    (*(xml->getHijos())).append(nueva1);
                    (*(xml->getHijos())).append(nueva2);
                }
                else
                {
                    xml->setValor(valorPadre);
                }

                //Si el abuelo existe
                if (hijo->getPadre()->getPadre() != NULL)
                {
                    //el nodo actual es el abuelo que ahora tiene de hijo al nodo creado
                    xml = hijo->getPadre()->getPadre();
                    (*(xml->getHijos())).append(hijo);
                    hijo->setPadre(xml);
                }
            }
            else if (hijo->getTipo() == 2) //Etiqueta Unitaria
            {
                if (esBinaria(hijo->getNombre()))
                {
                    //Cerramos la etiqueta
                    XMLEstructura* nuevo = new XMLEstructura;
                    nuevo->setNombre(hijo->getNombre());
                    nuevo->setTipo(1);

                    (*(xml->getHijos())).append(hijo);
                    (*(xml->getHijos())).append(nuevo);

                    //Creamos una etiqueta intermedia con el valor del padre
                    if (xml->getValor() != "")
                    {
                        XMLEstructura* nueva1 = new XMLEstructura;
                        nueva1->setNombre("media");
                        nueva1->setTipo(3);
                        nueva1->setValor(valorPadre);

                        XMLEstructura* nueva2 = new XMLEstructura;
                        nueva2->setNombre("media");
                        nueva2->setTipo(1);

                        (*(xml->getHijos())).append(nueva1);
                        (*(xml->getHijos())).append(nueva2);
                    }
                    else
                    {
                        xml->setValor(valorPadre);
                    }
                }
                else
                {
                    //Le decimos al nodo actual que el nuevo nodo es su hijo.
                    (*(xml->getHijos())).append(hijo);

                    //Creamos una etiqueta intermedia con el valor del padre
                    if (xml->getValor() != "")
                    {
                        XMLEstructura* nueva1 = new XMLEstructura;
                        nueva1->setNombre("media");
                        nueva1->setTipo(3);
                        nueva1->setValor(valorPadre);

                        XMLEstructura* nueva2 = new XMLEstructura;
                        nueva2->setNombre("media");
                        nueva2->setTipo(1);

                        (*(xml->getHijos())).append(nueva1);
                        (*(xml->getHijos())).append(nueva2);
                    }
                    else
                    {
                        xml->setValor(valorPadre);
                    }
                }
            }
            else if (hijo->getTipo() == 4) //Comentario
            {
                //Agregamos el hijo
                (*(xml->getHijos())).append(hijo);
            }
            else
            {
                //Agregamos el hijo
                (*(xml->getHijos())).append(hijo);

                //Creamos una etiqueta intermedia con el valor del padre
                if (xml->getValor() != "")
                {
                    XMLEstructura* nueva1 = new XMLEstructura;
                    nueva1->setNombre("media");
                    nueva1->setTipo(3);
                    nueva1->setValor(valorPadre);

                    XMLEstructura* nueva2 = new XMLEstructura;
                    nueva2->setNombre("media");
                    nueva2->setTipo(1);

                    (*(xml->getHijos())).append(nueva1);
                    (*(xml->getHijos())).append(nueva2);
                }
                else
                {
                    xml->setValor(valorPadre);
                }
            }
        }
    }
}

void XMLEstructura::imprimirTerminal(short profundidad, ofstream *flujo)
{
    //Imprimos espacios segun la profundidad para que se vea como un arbol
    for (short i = 0; i < profundidad; i++)
    {
        *flujo << "    ";
    }

    //Si es etiqueta final inicia con "</"
    if (tipo == 1)
    {
        *flujo << "</" << nombre;
    }
    else
    {
        *flujo << "<" << nombre;
    }

    //Imprimos la etiqueta
    for (unsigned int i = 0; i < atributo.getSize(); i++)
    {
        *flujo << " " << atributo[i] << "=\"" << valorAtributo[i] << "\"";
    }

    //La cerramos y ponemos su 'innerHTML'
    *flujo << "> " << valor;
    *flujo << endl;

    //Hacemos lo mismo con los hijos
    for (unsigned int i = 0; i < hijos->getSize(); i++)
    {
        XMLEstructura* puntero = hijos->recover(i);
        puntero->imprimirTerminal(profundidad+1, flujo);
        puntero = NULL;
    }
}
