#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>  // Para manejar JSON

const std::string API_KEY = "TU_CLAVE_DE_API"; // Sustituye con tu clave real

std::string traducirTexto(const std::string& texto, const std::string& origen, const std::string& destino) {
    std::string url = "https://translation.googleapis.com/language/translate/v2?key=" + API_KEY;

    // Cuerpo de la solicitud en formato JSON
    nlohmann::json requestBody = {
        {"q", texto},
        {"source", origen},
        {"target", destino},
        {"format", "text"}
    };

    // Enviar la solicitud POST
    cpr::Response response = cpr::Post(
        cpr::Url{url},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{requestBody.dump()}
    );

    // Manejar la respuesta
    if (response.status_code == 200) {
        auto jsonResponse = nlohmann::json::parse(response.text);
        return jsonResponse["data"]["translations"][0]["translatedText"];
    } else {
        return "Error en la traducción: " + response.text;
    }
}

int main() {
    std::string texto = "Hola, mundo";
    std::string traduccion = traducirTexto(texto, "es", "en");
    std::cout << "Traducción: " << traduccion << std::endl;
    return 0;
}
