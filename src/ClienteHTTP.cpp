#include <iostream>
#include <sstream>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <cmath>
#include <cctype>
#include <ctime>
#include <fstream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <csignal>
#include <sys/poll.h>
#include "List.h"
#include "ClienteHTTP.h"
#include "FuncionesPantalla.h"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define redPack 1024

using namespace std;

string intToStr(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

///Constructor
ClienteHTTP::ClienteHTTP()
{
    ClienteHTTP::host = "";
    ClienteHTTP::puerto = "";
    ClienteHTTP::metaCabeceras.nullify();
    ClienteHTTP::datoCabeceras.nullify();
    ClienteHTTP::versionHTTP = "";
    ClienteHTTP::codigoHTTP = "";
    ClienteHTTP::nombreCodigoHTTP = "";
    ClienteHTTP::ubicacionArchivo = "";
    ClienteHTTP::protocolo = "";
    ClienteHTTP::url = "";
    ClienteHTTP::errorCodigo = 0;
}

///Getters
string ClienteHTTP::getProtocolo()
{
    return ClienteHTTP::protocolo;
}

string ClienteHTTP::getUrl()
{
    return ClienteHTTP::url;
}

string ClienteHTTP::getHost()
{
    return ClienteHTTP::host;
}

string ClienteHTTP::getPuerto()
{
    return ClienteHTTP::puerto;
}

string ClienteHTTP::getUbicacionArchivo()
{
    return ClienteHTTP::ubicacionArchivo;
}

string ClienteHTTP::getVersionHTTP()
{
    return ClienteHTTP::versionHTTP;
}

string ClienteHTTP::getCodigoHTTP()
{
    return ClienteHTTP::codigoHTTP;
}

string ClienteHTTP::getNombreCodigoHTTP()
{
    return ClienteHTTP::nombreCodigoHTTP;
}

List<string> ClienteHTTP::getMetaCabeceras()
{
    return ClienteHTTP::metaCabeceras;
}

List<string> ClienteHTTP::getDatoCabeceras()
{
    return ClienteHTTP::datoCabeceras;
}

int ClienteHTTP::getErrorCodigo()
{
    return ClienteHTTP::errorCodigo;
}

///Setters
void ClienteHTTP::setProtocolo(string x)
{
    ClienteHTTP::protocolo = x;
}

void ClienteHTTP::setUrl(string x)
{
    ClienteHTTP::url = x;
}

void ClienteHTTP::setHost(string x)
{
    ClienteHTTP::host = x;
}

void ClienteHTTP::setPuerto(string x)
{
    ClienteHTTP::puerto = x;
}

void ClienteHTTP::setUbicacionArchivo(string x)
{
    ClienteHTTP::ubicacionArchivo = x;
}

void ClienteHTTP::setVersionHTTP(string x)
{
    ClienteHTTP::versionHTTP = x;
}

void ClienteHTTP::setCodigoHTTP(string x)
{
    ClienteHTTP::codigoHTTP = x;
}
void ClienteHTTP::setNombreCodigoHTTP(string x)
{
    ClienteHTTP::nombreCodigoHTTP = x;
}

void ClienteHTTP::setMetaCabeceras(List<string> x)
{
    ClienteHTTP::metaCabeceras = x;
}

void ClienteHTTP::setDatoCabeceras(List<string> x)
{
    ClienteHTTP::datoCabeceras = x;
}

void ClienteHTTP::setErrorCodigo(int x)
{
    ClienteHTTP::errorCodigo = x;
}

/**
 * Automata de Estado Finito para la validación de una URL del tipo
 * protocolo://host:puerto/archivo
 *
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 * @param  url     String con la URL a inspeccionar
 * @return         URL con los datos correcto, sino regresa "incorrecto".
 */
string ClienteHTTP::insertarDatosUrl(string url)
{
    int estado = 1;
    string myUrl = url;

    for (unsigned int i = 0; i < url.size(); i++)
    {
        if (estado == 1)
        {
            if ( isupper(url[i]) || islower(url[i]) || isdigit(url[i]) || url[i] == '[')
            {
                estado = 2;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 2)
        {
            if (url[i] == ':')
            {
                estado = 3;
            }
            else
            {
                estado = 2;
            }
        }
        else if (estado == 3)
        {
            if (url[i] == '/')
            {
                estado = 4;
            }
            else
            {
                estado = 2;
            }
        }
        else if (estado == 4)
        {
            if (url[i] == '/')
            {
                estado = 5;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 5)
        {
            if ( isupper(url[i]) || islower(url[i]) || isdigit(url[i]) )
            {
                estado = 6;
            }
            else if (url[i] == '[')
            {
                estado = 9;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 6)
        {
            if (isupper(url[i]) || islower(url[i]) || isdigit(url[i]) || url[i] == '.'  ||
                url[i] == '-' || url[i] == '_')
            {
                estado = 6;
            }
            else if (url[i] == ':')
            {
                estado = 7;
            }
            else if (url[i] == '/')
            {
                string cad;

                //Obtenemos el nombre del protocolo
                for (unsigned int j = 0; j < url.size(); j++)
                {
                    if (url[j] == ':')
                    {
                        break;
                    }
                    else
                    {
                        cad.push_back(url[j]);
                    }
                }

                //Sacamos el nombre del puerto
                struct servent *tmp = getservbyname(cad.c_str(), "tcp");
                cad = intToStr(ntohs(tmp->s_port));

                myUrl = url.substr(0, i) + ":" + cad + url.substr(i, url.size()-i);
                myUrl = insertarDatosUrl(myUrl);

                estado = 8;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 7)
        {
            if (isdigit(url[i]))
            {
                estado = 7;
            }
            else if (url[i] == '/')
            {
                estado = 8;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 8)
        {
            estado = 8;
        }
        else if (estado == 9)
        {
            if (url[i] == ':' || isdigit(url[i]))
            {
                estado = 10;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 10)
        {
            if (url[i] == ':' || isdigit(url[i]))
            {
                estado = 10;
            }
            else if (url[i] == ']')
            {
                estado = 11;
            }
            else
            {
                estado = 0;
            }
        }
        else if (estado == 11)
        {
            if (url[i] == '/')
            {
                string cad;

                //Obtenemos el nombre del protocolo
                for (unsigned int j = 0; j < url.size(); j++)
                {
                    if (url[j] == ':')
                    {
                        break;
                    }
                    else
                    {
                        cad.push_back(url[j]);
                    }
                }

                //Sacamos el nombre del puerto
                struct servent *tmp = getservbyname(cad.c_str(), "tcp");
                cad = intToStr(ntohs(tmp->s_port));

                myUrl = url.substr(0, i) + ":" + cad + url.substr(i, url.size()-i);
                myUrl = insertarDatosUrl(myUrl);

                estado = 8;
            }
            else if (url[i] == ':')
            {
                estado = 7;
            }
            else
            {
                estado = 0;
            }
        }
        else //Asume 0
        {
            //Incorrecto
        }
    }

    if (estado == 2)
    {
        return insertarDatosUrl("http://" + myUrl);
    }
    else if (estado == 6 || estado == 11)
    {
        string cad;

        //Obtenemos el nombre del protocolo
        for (unsigned int j = 0; j < url.size(); j++)
        {
            if (url[j] == ':')
            {
                break;
            }
            else
            {
                cad.push_back(url[j]);
            }
        }

        //Sacamos el nombre del puerto
        struct servent *tmp = getservbyname(cad.c_str(), "tcp");
        cad = intToStr(ntohs(tmp->s_port));

        myUrl = myUrl + ":" + cad + "/";
        myUrl = insertarDatosUrl(myUrl);

        return myUrl;
    }
    else if (estado == 7)
    {
        return insertarDatosUrl(myUrl + "/");
    }
    else if (estado == 8)
    {
        return myUrl;
    }
    else
    {
        return "incorrecto";
    }
}

/**
 * Guarda los datos de una URL dada, para se dividia del modo
 * protocolo://host:puerto/archivo
 *
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 * @param urlPast  String con la URL que se va a inspeccionar
 * @param protocol Apuntador donde se guardara el 'protocolo' que usa la URL
 * @param hostname Apuntador donde se guardara el 'host' que usa la URL
 * @param puerto   Apuntador donde se guardara el 'puerto' que usa la URL
 * @param filename Apuntador donde se guardara el 'archivo' que usa la URL
 */
void ClienteHTTP::cargarDatosPagina(string urlPast, string *protocol, string *hostname,
                                    string *puerto, string *filename)
{
    //Sacamos el protocolo
    string url = urlPast;
    int posTag = url.find("://");
    *protocol = url.substr(0, posTag);
    url = url.substr(posTag+3, url.size()-posTag);

    //Sacamos el Host
    posTag = url.find('[');

    if (posTag == -1) //IPv4
    {
        posTag = url.find(":");
        *hostname = url.substr(0, posTag);
        url = url.substr(posTag+1, url.size()-posTag);
    }
    else //IPv6
    {
        posTag = url.find(']');
        *hostname = url.substr(1, posTag-1);
        url = url.substr(posTag+2, url.size()-posTag);
    }

    //Sacamos el puerto
    posTag = url.find("/");
    *puerto = url.substr(0, posTag);
    url = url.substr(posTag, url.size()-posTag);

    //Sacamos la url del archivo
    *filename = url;
}

/**
 * Genera una conexion con el host indicado
 *
 * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
 * @param  hostn         Host al que se quiere conectar
 * @param  port          Puerto al que se quiere conectar
 * @param  headerEnviar  String con el header que recibira el servidor
 * @param  tipoHeader    String con el tipo de comando en el header como "GET" o "HEAD"
 * @param  headerState   Booleano que describe si desea guardar el header en el archivo indicado
 * @param  nombreArchivo Archivo donde se guardara el archivo solicitado
 * @param  attrHeader    String con el nombre de una cabecera para procesarla como "Content-Length"
 * @param  useSSL        Booleano que describe si desea usar SSL en la conexion
 * @return               Estado de Error de si se pudo o no realizar la descarga del archivo.
 */
int ClienteHTTP::conectarHeaderHTTP(string hostn, string port, string headerEnviar,
                                    string tipoHeader, bool headerState, string *nombreArchivo,
                                    string attrHeader, bool useSSL)
{
    //Creamos la variables
    int s, res, k;
    uint8_t buffer[redPack];
    char ip_buffer[256], puerto_buffer[64];
    string hostname = hostn;
    string puerto = port;
    string headerResponse = "";
    bool header = headerState;
    long lastRes = 0;

    //Variables para el SSL
    BIO *sbio = NULL;
    SSL *ssl = NULL;
    SSL_CTX *ctx = NULL;

    //Creamos la estructura de para los datos
    struct addrinfo hints, *resStruct, *ressave;

    //Rellenamos los datos con valor 0
    memset(&hints, 0, sizeof(hints));
    memset(&buffer, 0, sizeof(buffer));

    //Rellenamos la estructura de hints
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    //Obtenemos los datos del host
    k = getaddrinfo(hostname.c_str(), puerto.c_str(), &hints, &resStruct);

    if (k < 0)
    {
        return 1;
    }

    //Guardamos el inicio de la estructura
    bool conexion = false;
    ressave = resStruct;

    while (resStruct != NULL && !conexion)
    {
        getnameinfo(resStruct->ai_addr, resStruct->ai_addrlen,
                    ip_buffer, 255,
                    puerto_buffer, 63, NI_NUMERICHOST);

        s = socket(resStruct->ai_family, resStruct->ai_socktype, resStruct->ai_protocol);

        if (s < 0)
        {
            return 2;
        }

        //Nos conectamos con el socket
        res = connect(s, resStruct->ai_addr, resStruct->ai_addrlen);

        if (res >= 0)
        {
            conexion = true;
        }
        else
        {
            close(s);
        }

        resStruct = resStruct->ai_next;
    }

    //Liberamos la informacion
    freeaddrinfo(ressave);

    if (!conexion)
    {
        return 3;
    }

    //Abrimos una archivo para guardar los datos
    ofstream file1;

    int posDiagonal = nombreArchivo->rfind("/");

    if (posDiagonal > 0)
    {
       *nombreArchivo = nombreArchivo->substr(posDiagonal+1, nombreArchivo->size()-(posDiagonal+1));
    }

    //Abrimos el archivo
    FuncionesPantalla f;
    *nombreArchivo = f.getPath() + "cache/" + *nombreArchivo;

    file1.open(nombreArchivo->c_str(), ios::binary & ios::trunc);
    string html = "";

    //Revisamos si se usara SSL
    if (useSSL)
    {
        //Inicializamos la libreria
        SSL_library_init();

        //Inicializamos los algoritmos
        ERR_load_crypto_strings();
        ERR_load_SSL_strings();
        OpenSSL_add_all_algorithms();

        //Cargamos el entorno y la conexión
        ctx = SSL_CTX_new(SSLv23_client_method());
        sbio = BIO_new_ssl_connect(ctx);

        //Guardaos los datos de SSL
        BIO_get_ssl(sbio, &ssl);

        if(!ssl)
        {
            ERR_print_errors_fp(stderr);
            return 2;
        }

        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

        BIO_set_conn_hostname(sbio, (hostname + ":" + puerto).c_str());

        //Conecamos y hacemos el handshake
        if(BIO_do_connect(sbio) <= 0)
        {
            ERR_print_errors_fp(stderr);
            return 2;
        }

        if(BIO_do_handshake(sbio) <= 0)
        {
            ERR_print_errors_fp(stderr);
            return 2;
        }
    }

    //Creamos la estructura del Poll
    struct pollfd revisar;

    revisar.fd = s;
    revisar.events = POLLERR | POLLOUT | POLLRDNORM | POLLHUP | POLLIN;

    int n, i = 0;
    int gtres;

    //0 = inicia, 1 = manda cabezera, 2 = recibe cabezera, 3 = envia contenido, 4 recive contenido.
    int estadoHTTP = 0;

    bool fin = false;

    if (tipoHeader == "HEAD")
    {
        estadoHTTP = 0;
    }
    else if (tipoHeader == "GET")
    {
        estadoHTTP = 2;
    }

    //Creamos las variables para el tiempo
    struct timeval tv;
    struct timezone tz;

    gtres = gettimeofday(&tv, &tz);

    double tiempo = ((double)tv.tv_sec * 1000000 + (double)tv.tv_usec);
    double ptiempo = tiempo;


    while (!fin)
    {
        //Obtenemos la hora actual con los milisegundos
        gtres = gettimeofday(&tv, &tz);

        if (gtres >= 0)
        {
            //Transformamos a un número doble la hora
            tiempo = ((double)tv.tv_sec * 1000000 + (double)tv.tv_usec);

            //Incrementamos a i cada 500 milisegundos
            if (tiempo - ptiempo > 500000)
            {
                ptiempo = tiempo;
                i++;
            }
        }

        //Llamamos a poll para revisar si tenemos un evento en 50 milisegundos
        n = poll(&revisar, 1, 50);

        if (n < 0)
        {
            //cerr << "Error al llamar a poll (n<0): \"" <<  strerror(errno) << "\"" << endl;
        }
        else if (n > 0)
        {
            if ((revisar.revents & POLLERR) == POLLERR)
            {
                return 4;
            }
            else if ((revisar.revents & POLLHUP) == POLLHUP)
            {
                return 4;
            }
            else if ((revisar.revents & POLLRDNORM) == POLLRDNORM ||
                     ((estadoHTTP == 1 || estadoHTTP == 3) && useSSL) ) //Datos recibidos
            {
                if (estadoHTTP == 1) //Recibiendo cabezera
                {
                    memset(&buffer, 0, sizeof(buffer));

                    if (useSSL)
                    {
                        res = BIO_read(sbio, buffer, redPack);
                    }
                    else
                    {
                        res = read(s, buffer, redPack);
                    }

                    if (res < 0)
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK)
                        {
                            //cerr << "Error de read: \"" <<  strerror(errno) << "\"" << endl;
                        }
                        else
                        {
                            //cerr << "Tendra que internarlo más tarde" << endl;
                        }
                    }
                    else if (res == 0)
                    {
                        header = false;
                        fin = true;
                    }
                    else
                    {
                        char myBuffer[redPack];
                        memcpy(myBuffer, buffer, sizeof(myBuffer));

                        //Revisamos que este vacia la estructura de datos
                        if (string(myBuffer).size() == 0)
                        {
                            html = "";
                            header = false;
                            fin = true;
                        }

                        //Setteamos la variable html con los datos de la cabezera
                        html = html + myBuffer;

                        file1 << html << endl;

                        if (attrHeader != "")
                        {
                            //Buscamos la etiqueta Contest-lenght
                            string target = attrHeader + " ";

                            int posLeng = html.find(target) + target.size();
                            html = html.substr(posLeng, html.size() - (posLeng) );

                            //Buscamos el final
                            posLeng = html.find("\n");
                            html = html.substr(0, posLeng);
                        }

                        i = 0;
                    }
                }
                else if (estadoHTTP == 3) //Recibimiendo contenido
                {
                    memset(&buffer, 0, sizeof(buffer));

                    if (useSSL)
                    {
                        res = BIO_read(sbio, buffer, redPack-1);
                    }
                    else
                    {
                        res = read(s, buffer, redPack-1);
                    }

                    if (res < 0)
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK)
                        {
                            //cerr << "Error de read: \"" <<  strerror(errno) << "\"" << endl;
                        }
                        else
                        {
                            //cerr << "Tendra que internarlo más tarde" << endl;
                        }
                    }
                    else if (res == 0)
                    {
                        fin = true;
                    }
                    else
                    {
                        //Buscamos el caracter del final del header
                        if (!header)
                        {
                            int posHeader = 0;
                            bool encontrado = false;

                            for (int i = 0; i < res; i++)
                            {
                                if (buffer[i] == '\n')
                                {
                                    encontrado = true;
                                }
                                else if (buffer[i] == '\r')
                                {
                                    if (encontrado)
                                    {
                                        header = true;
                                        posHeader = i;
                                        break;
                                    }
                                }
                                else
                                {
                                    encontrado = false;
                                }
                            }

                            int j = 0;
                            for (int l = posHeader+2; l < res; l++, j++)
                            {
                                file1.put(buffer[l]);
                            }

                            lastRes = j;
                        }
                        else
                        {
                            //Copiamos los datos
                            for (int j = 0; j < res; j++)
                            {
                                file1.put(buffer[j]);
                            }

                            lastRes = lastRes + res;
                        }

                        i = 0;
                    }
                }
            }
            else if ((revisar.revents & POLLOUT) == POLLOUT) //Datos enviados
            {
                if (estadoHTTP == 0)
                {
                    //Escribimos una cabezera para HTTP
                    string cabezera = headerEnviar;

                    //Leemos los datos del socket
                    if (useSSL)
                    {
                        res = BIO_write(sbio, cabezera.c_str(), cabezera.size());
                    }
                    else
                    {
                        res = write(s, cabezera.c_str(), cabezera.size());
                    }

                    if (res < 0)
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK)
                        {
                            //cerr << "Error de write: \"" <<  strerror(errno) << "\"" << endl;
                        }
                        else
                        {
                            //cerr << "Tendra que internarlo más tarde" << endl;
                        }
                    }
                    else
                    {
                        estadoHTTP = 1;
                    }
                }
                else if (estadoHTTP == 2)
                {
                    //Escribimos una cabezera para HTTP
                    string cabezera = headerEnviar;

                    //Leemos los datos del socket
                    if (useSSL)
                    {
                        res = BIO_write(sbio, cabezera.c_str(), cabezera.size());
                    }
                    else
                    {
                        res = write(s, cabezera.c_str(), cabezera.size());
                    }

                    if (res < 0)
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK)
                        {
                            //cerr << "Error de write: \"" <<  strerror(errno) << "\"" << endl;
                        }
                        else
                        {
                            //cerr << "Tendra que internarlo más tarde" << endl;
                        }
                    }
                    else
                    {
                        estadoHTTP = 3;
                    }
                }
            }
        }

        if (i == 20) //han pasado 10 segundos sin respuesta
        {
            fin = true;
            return 2;
        }
    }

    //Cerramos el archivo
    file1.close();
    close(s);

    if (useSSL)
    {
        BIO_free_all(sbio);
        SSL_CTX_free(ctx);
    }

    return 0;
}

void ClienteHTTP::guardarURL(string *urlRecibida)
{
    //Creamos las variables
    string url = "", pastUrl = "", hostname = "";
    string puerto = "", extension = "", protocol = "", filename = "";
    string defEstado = "";
    string versionHTTP = "", estatusResponse = "", significadoResponse = "";

    //Creamos dos listas para el contenido de cabeceras
    List<string> cabecerasMeta, cabecerasDato;

    bool header, recursive, useSSL;
    short intentosURL = 0;

    url = *urlRecibida;
    url = insertarDatosUrl(url);

    if (url == "incorrecto")
    {
        ClienteHTTP::setErrorCodigo(5);
    }
    else
    {
        do
        {
            //Vaciamos las cabeceras y cargamos los datos
            cabecerasDato.nullify();
            cabecerasMeta.nullify();

            recursive = false;
            cargarDatosPagina(url, &protocol, &hostname, &puerto, &filename);

            if (protocol == "http")
            {
                useSSL = false;
            }
            else if (protocol == "https")
            {
                useSSL = true;
            }
            else
            {
                ClienteHTTP::setErrorCodigo(6);
                break;
            }

            //Generamos las cadenas de la peticion
            string header1 = "HEAD " + filename + " HTTP/1.1" + "\n" +
                             "Host: " + hostname + "\n" +
                             "Connection: close " + "\n" +
                             "User-Agent: RiNW" + "\n" + "\n";

            string header2 = "GET " + filename + " HTTP/1.1" + "\n" +
                             "Host: " + hostname + "\n" +
                             "Connection: close " + "\n" +
                             "User-Agent: RiNW" + "\n" + "\n";

            int posTag = filename.find('?');

            //Revisamoque sea un nombre de archivo valido
            if (filename == "/" || filename[filename.size()-1] == '/')
            {
                header = false;
                extension = "index.jex";
                filename = "index.jex";
            }
            else if (posTag != -1)
            {
                extension = "search.jex";
                filename = "search.jex";
            }
            else
            {
                //Nombre de una extension
                extension = filename.substr(1, filename.size()-1) + extension;
                filename = extension;
            }

            //Obtenemos la cabecera para guardarla en un archivo
            errorCodigo = conectarHeaderHTTP(hostname, puerto, header1, "HEAD", header, &extension,
                                             "HTTP/1.1 ", useSSL);

            if (errorCodigo == 0)
            {
                FuncionesPantalla f;
                extension = f.getPath() + extension;

                //Abrimos el archivo de las cabeceras
                ifstream file2;
                file2.open(extension.c_str());

                if (file2.is_open())
                {
                    //Obtenemos el archivo
                    char actual = file2.get();
                    string aux = "";
                    bool meta = true;

                    //Leemos hasta no encontrar el final de archivo
                    while (actual != EOF)
                    {
                        //Final de linea
                        if (actual == '\r')
                        {
                            if (aux != "")
                            {
                                cabecerasDato.append(aux.substr(1, aux.size()-1));
                                aux = "";
                                meta = true;
                            }
                        }
                        else if (actual == ':') //hubo un separador de metadato
                        {
                            if (meta)
                            {
                                cabecerasMeta.append(aux.substr(1, aux.size()-1));
                                aux = "";
                                meta = false;
                            }
                            else
                            {
                                aux.push_back(actual);
                            }
                        }
                        else
                        {
                            aux.push_back(actual);
                        }

                        actual = file2.get();
                    }
                }

                //Sacamos la primera cabecera de la version
                try
                {
                    defEstado = cabecerasDato.pop(0);
                }
                catch (ListException)
                {
                    defEstado = "";
                }

                //Obtenemos la version
                int posTag = defEstado.find(" ");
                versionHTTP = "H" + defEstado.substr(0, posTag);
                defEstado = defEstado.substr(posTag+1, defEstado.size()-posTag);

                //Obtenemos el codigo de respuesta y el significado
                posTag = defEstado.find(" ");
                estatusResponse = defEstado.substr(0, posTag);
                significadoResponse = defEstado.substr(posTag+1, defEstado.size()-posTag);

                if (estatusResponse.substr(0, 2) == "20")
                {
                    conectarHeaderHTTP(hostname, puerto, header2, "GET", header, &filename, "",
                                       useSSL);
                }
                else if (estatusResponse.substr(0, 2) == "30")
                {
                    //Obtenemos la etiqueta de la ubicacion
                    url = cabecerasDato[cabecerasMeta.searchInList("Location")];
                    *urlRecibida = url;
                    url = insertarDatosUrl(url);

                    //Eliminamos el archivo
                    #ifdef WIN32
                        string arch = "del " + extension;
                        int * x = new int;
                        *x = system(arch.c_str());
                        free(x);
                    #else
                        string arch = "rm " + extension;
                        int * x = new int;
                        *x = system(arch.c_str());
                        free(x);
                    #endif

                    recursive = true;
                    intentosURL++;

                    if (intentosURL == 3)
                    {
                        extension = "30X.html";
                        conectarHeaderHTTP(hostname, puerto, header2, "GET", header, &extension,
                                           "", useSSL);
                    }
                }
                else if (estatusResponse.substr(0, 2) == "40")
                {
                    #ifdef WIN32
                        string arch = "del " + extension;
                        int * x = new int;
                        *x = system(arch.c_str());
                        free(x);
                    #else
                        string arch = "rm " + extension;
                        int * x = new int;
                        *x = system(arch.c_str());
                        free(x);
                    #endif

                    //Obtenemos el archivo
                    extension = "40X.html";
                    conectarHeaderHTTP(hostname, puerto, header2, "GET", header, &extension,
                                       "", useSSL);
                }
            }
        }
        while (recursive && intentosURL < 3);
    }

    //Guardamos cada uno de los datos de la conexion
    ClienteHTTP::host = hostname;
    ClienteHTTP::puerto = puerto;
    ClienteHTTP::metaCabeceras = cabecerasMeta;
    ClienteHTTP::datoCabeceras = cabecerasDato;
    ClienteHTTP::versionHTTP = versionHTTP;
    ClienteHTTP::codigoHTTP = estatusResponse;
    ClienteHTTP::nombreCodigoHTTP = significadoResponse;
    ClienteHTTP::ubicacionArchivo = extension;
    ClienteHTTP::protocolo = protocol;
    ClienteHTTP::url = *urlRecibida;
}

void ClienteHTTP::imprimir()
{
    //Impresion en consola de los datos
    cout << endl;
    cout << "Error:            \t" << ClienteHTTP::errorCodigo << endl;
    cout << "Host:             \t" << ClienteHTTP::host << endl;
    cout << "Puerto:           \t" << ClienteHTTP::puerto << endl;
    cout << "Protocolo:        \t" << ClienteHTTP::protocolo << endl;
    cout << "URL:              \t" << ClienteHTTP::url << endl;
    cout << "Version HTTP:     \t" << ClienteHTTP::versionHTTP << endl;
    cout << "Codigo HTTP:      \t" << ClienteHTTP::codigoHTTP << endl;
    cout << "Signigicado HTTP: \t" << ClienteHTTP::nombreCodigoHTTP << endl;
    cout << "Archivo Guardado: \t" << ClienteHTTP::ubicacionArchivo << endl;
    cout << "Cabeceras (Meta): \t" << ClienteHTTP::metaCabeceras << endl;
    cout << "Cabeceras (Datos):\t" << ClienteHTTP::datoCabeceras << endl;
}
