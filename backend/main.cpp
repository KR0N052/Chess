#include "crow.h"
#include <iostream>

int main()
{
    crow::SimpleApp app;

    // Statikus fájlok kiszolgálása (frontend mappa)
    CROW_ROUTE(app, "/")([]() {
        return crow::response(crow::status::OK, R"(
            <!DOCTYPE html>
            <html>
            <head>
                <meta http-equiv="refresh" content="0; url=/frontend/index.html" />
            </head>
            <body></body>
            </html>
        )");
        });

    // Kattintás kezelése
    CROW_ROUTE(app, "/click/<string>").methods(crow::HTTPMethod::POST)
        ([](const crow::request& req, std::string coord) {
        std::cout << "Kattintott mező: " << coord << std::endl;
        return crow::response(200);
            });

    // Statikus fájl kiszolgálás (pl. JS, HTML)
    app.route_dynamic("/frontend/<path>")
        ([](const crow::request& req, std::string path) {
        std::string fullPath = "../frontend/" + path;
        std::ifstream file(fullPath, std::ios::binary);
        if (!file) {
            return crow::response(404);
        }
        std::ostringstream buf;
        buf << file.rdbuf();
        return crow::response(buf.str());
            });

    app.port(18080).multithreaded().run();
}
