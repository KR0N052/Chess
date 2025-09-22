#include "crow.h"
#include "logic/Game.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

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
                cell["color"] = (piece->getColor() == Color::White) ? 0 : 1;
                switch (piece->getType()) {
                case PieceType::King:   cell["type"] = 0; break;
                case PieceType::Queen:  cell["type"] = 1; break;
                case PieceType::Rook:   cell["type"] = 2; break;
                case PieceType::Bishop: cell["type"] = 3; break;
                case PieceType::Knight: cell["type"] = 4; break;
                case PieceType::Pawn:   cell["type"] = 5; break;
                default: cell["type"] = -1; break;
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

std::string coordToAlgebraic(int row, int col) {
    char file = 'a' + col;
    char rank = '8' - row;
    return std::string{ file } + rank;
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
    Game game; // az új Game osztály példánya

    // Gyökér oldal (index.html)
    CROW_ROUTE(app, "/")([]() {
        return serveFile("../frontend/index.html");
        });

    // GET /state
    CROW_ROUTE(app, "/state")([&]() {
        json response;
        response["board"] = boardToJson(game.getBoard());
        response["currentPlayer"] = (game.getCurrentTurn() == Color::White) ? 0 : 1;

        crow::response res(response.dump());
        res.set_header("Content-Type", "application/json; charset=utf-8");
        res.set_header("Cache-Control", "no-store");
        return res;
        });

    // POST /move
    CROW_ROUTE(app, "/move").methods("POST"_method)([&](const crow::request& req) {
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

        if (ok) {
            std::string moveCode = coordToAlgebraic(fr, fc) + coordToAlgebraic(tr, tc);
            response["moveCode"] = moveCode;
        }
        else {
            response["error"] = "Érvénytelen lépés";
        }

        crow::response res(response.dump());
        res.set_header("Content-Type", "application/json; charset=utf-8");
        res.set_header("Cache-Control", "no-store");
        return res;
        });

    // POST /reset → új játék
    CROW_ROUTE(app, "/reset").methods("POST"_method)([&]() {
        game.resetBoard(); // az új osztály metódusát hívjuk
        json response;
        response["board"] = boardToJson(game.getBoard());
        response["currentPlayer"] = (game.getCurrentTurn() == Color::White) ? 0 : 1;

        crow::response res(response.dump());
        res.set_header("Content-Type", "application/json; charset=utf-8");
        res.set_header("Cache-Control", "no-store");
        return res;
        });

    app.port(18080).multithreaded().run();
}
