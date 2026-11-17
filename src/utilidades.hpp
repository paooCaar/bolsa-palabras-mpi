// utilidades.hpp
// Funciones compartidas por la versión serial y la paralela.
#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

// libcurl llama a esta función cada vez que llega un pedazo del archivo.
// Nosotros solo vamos pegando cada pedazo al final de un string.
static size_t escribir_datos(void* contenido, size_t tam, size_t n, void* destino) {
    std::string* texto = static_cast<std::string*>(destino);
    texto->append(static_cast<char*>(contenido), tam * n);
    return tam * n;
}

// Descarga el contenido de una URL y lo regresa como string.
inline std::string descargar_libro(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("No se pudo iniciar curl");

    std::string texto;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);          // seguir redirecciones
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escribir_datos);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &texto);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "bolsa-palabras-mpi");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        throw std::runtime_error("Error al descargar " + url + ": " + curl_easy_strerror(res));
    return texto;
}

// Lee el archivo de URLs: una por línea. Ignora líneas vacías y las que empiezan con '#'.
inline std::vector<std::string> leer_urls(const std::string& archivo) {
    std::ifstream f(archivo);
    if (!f) throw std::runtime_error("No se pudo abrir " + archivo);

    std::vector<std::string> urls;
    std::string linea;
    while (std::getline(f, linea)) {
        // quitar espacios y '\r' al final (por si el archivo viene de Windows)
        while (!linea.empty() && (linea.back() == '\r' || linea.back() == ' '))
            linea.pop_back();
        if (!linea.empty() && linea[0] != '#')
            urls.push_back(linea);
    }
    return urls;
}