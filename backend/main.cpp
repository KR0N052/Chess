#include "crow.h"
#include "logic/Game.h"
#include "logic/pieces/King.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

// Tábla → JSON
json boardToJson(const Board& board) {
    json jBoard = json::array();
    for (int r = 0; r < 8; r++) {
        json row = json::array();
        for (int c = 0; c < 8; c++) {
            auto piece = board.getPiece(r, c);
            json cell;
            if (piece) {
                // Szín
                cell["color"] = (piece->getColor() == Color::White) ? 0 : 1;
                // Típus — egyelőre csak a királyt különböztetjük meg
                if (dynamic_cast<const King*>(piece.get())) {
                    cell["type"] = 0; // 0 = King
                }
                else {
                    cell["type"] = -1; // egyéb bábu (még régi logika)
                }
            }
            else {
                cell["color"] = 2; // None
                cell["type"] = -1; // üres
            }
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

    // GET /state → tábla állása
    CROW_ROUTE(app, "/state")
        ([&]() {
        json response;
        response["board"] = boardToJson(game.getBoard());
        response["currentPlayer"] = (game.getCurrentTurn() == Color::White) ? 0 : 1;
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

        bool ok = game.makeMove(fr, fc, tr, tc);

        json response;
        response["success"] = ok;
        response["board"] = boardToJson(game.getBoard());
        response["currentPlayer"] = (game.getCurrentTurn() == Color::White) ? 0 : 1;

        return crow::response{ response.dump() };
            });

    app.port(18080).multithreaded().run();
}
