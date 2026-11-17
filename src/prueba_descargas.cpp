// Programa de prueba: descarga cada libro y muestra cuántos caracteres tiene.
#include "utilidades.hpp"
#include <iostream>

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    std::vector<std::string> urls = leer_urls("libros.txt");
    std::cout << "Libros a descargar: " << urls.size() << "\n";

    for (const auto& url : urls) {
        std::string texto = descargar_libro(url);
        std::cout << url << "  ->  " << texto.size() << " caracteres\n";
    }

    curl_global_cleanup();
    return 0;
}