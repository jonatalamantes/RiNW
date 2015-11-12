#ifndef CLIENTEHTTP_H
#define CLIENTEHTTP_H

#include "List.h"

/**
* Clase encargada de la conexion HTTP entre las paginas
*
* @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
**/
class ClienteHTTP
{
    private:
    ///Atributos
        string protocolo;
        string url;
        string host;
        string puerto;
        string ubicacionArchivo;
        string versionHTTP;
        string codigoHTTP;
        string nombreCodigoHTTP;
        List<string> metaCabeceras;
        List<string> datoCabeceras;
        int errorCodigo;

    ///Metodos privados

        /**
         * Automata de Estado Finito para la validación de una URL del tipo
         * protocolo://host:puerto/archivo
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param  url     String con la URL a inspeccionar
         * @return         URL con los datos correcto, sino regresa "incorrecto".
         */
        string insertarDatosUrl(string url);

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
        void cargarDatosPagina(string urlPast, string *protocol,
                               string *hostname, string *puerto, string *filename);

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
        int  conectarHeaderHTTP(string hostn, string port, string headerEnviar, string tipoHeader,
                                bool headerState, string *nombreArchivo, string attrHeader, bool useSSL);

    public:
    ///Constructor
        ClienteHTTP();

    ///Getters
        string getProtocolo();
        string getUrl();
        string getHost();
        string getPuerto();
        string getUbicacionArchivo();
        string getVersionHTTP();
        string getCodigoHTTP();
        string getNombreCodigoHTTP();
        List<string> getMetaCabeceras();
        List<string> getDatoCabeceras();
        int getErrorCodigo();

    ///Setters
        void setProtocolo(string x);
        void setUrl(string x);
        void setHost(string x);
        void setPuerto(string x);
        void setUbicacionArchivo(string x);
        void setVersionHTTP(string x);
        void setCodigoHTTP(string x);
        void setNombreCodigoHTTP(string x);
        void setMetaCabeceras(List<string> x);
        void setDatoCabeceras(List<string> x);
        void setErrorCodigo(int x);

    ///Metodos publicos

        /**
         * Guarda los datos de una URL en el objeto actual
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         * @param url   URL a conectarse y sacar datos
         */
        void guardarURL(string *url);

        /**
         * Imprime los datos del objeto actual
         *
         * @author Jonathan Sandoval <jonathan_s_pisis@yahoo.com.mx>
         */
        void imprimir();
};

#endif // CLIENTEHTTP_H
