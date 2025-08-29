#include "crow.h"
#include "logic/Game.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

json boardToJson(const Board& board) {
    json jBoard = json::array();
    for (int r = 0; r < 8; r++) {
        json row = json::array();
        for (int c = 0; c < 8; c++) {
            const Piece& p = board.getPiece(r, c);
            json cell;
            cell["type"] = static_cast<int>(p.type);
            cell["color"] = static_cast<int>(p.color);
            row.push_back(cell);
        }
        jBoard.push_back(row);
    }
    return jBoard;
}

// Segédfüggvény fájl beolvasásához
crow::response serveFile(const std::string& path, const std::string& contentType = "text/html") {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return crow::response(404);
    }
    std::ostringstream contents;
    contents << in.rdbuf();
    crow::response res(contents.str());
    res.set_header("Content-Type", contentType);
    return res;
}

int main() {
    crow::SimpleApp app;
    Game game;

    // Gyökér oldal (index.html)
    CROW_ROUTE(app, "/")
        ([]() {
        return serveFile("../frontend/index.html");
            });

    CROW_ROUTE(app, "/state")
        ([&]() {
        json response;
        response["board"] = boardToJson(game.getBoard());
        response["currentPlayer"] = static_cast<int>(game.getCurrentPlayer());
        return crow::response{ response.dump() };
            });

    // POST /move → lépés végrehajtása
    CROW_ROUTE(app, "/move").methods("POST"_method)
        ([&](const crow::request& req) {
        auto body = json::parse(req.body);
        int fr = body["fromRow"];
        int fc = body["fromCol"];
        int tr = body["toRow"];
        int tc = body["toCol"];

        Move move(fr, fc, tr, tc);
        bool ok = game.makeMove(move);

        json response;
        response["success"] = ok;
        response["board"] = boardToJson(game.getBoard());
        response["currentPlayer"] = static_cast<int>(game.getCurrentPlayer());

        return crow::response{ response.dump() };
            });

    // Frontend fájlok kiszolgálása (például build után)
    CROW_ROUTE(app, "/frontend/<path>")
        ([](const std::string& filePath) {
        std::string fullPath = "../frontend/" + filePath; // build-ből kilépünk egy szintet
        std::ifstream in(fullPath, std::ios::binary);
        if (!in) return crow::response(404);

        std::ostringstream contents;
        contents << in.rdbuf();
        return crow::response{ contents.str() };
            });

    app.port(18080).multithreaded().run();
}
