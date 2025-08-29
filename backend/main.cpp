#include "crow.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

// 8x8 sakktábla: minden mező egy string (pl. "wP", "bK", "")
std::vector<std::vector<std::string>> board(8, std::vector<std::string>(8, ""));

// Koordináta átalakítása (pl. "e2" → [6][4])
std::pair<int, int> coordToIndex(const std::string& coord) {
    std::unordered_map<char, int> colMap = {
        {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3},
        {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}
    };
    int col = colMap[coord[0]];
    int row = 8 - (coord[1] - '0'); // 0-indexelt sor
    return { row, col };
}

// Alapállás beállítása
void initializeBoard() {
    // Fehér bábuk
    board[7] = { "wR", "wR", "wB", "wQ", "wK", "wB", "wN", "wR" };
    board[6] = std::vector<std::string>(8, "wP");

    // Fekete bábuk
    board[0] = { "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR" };
    board[1] = std::vector<std::string>(8, "bP");

    // Üres sorok középen
    for (int r = 2; r <= 5; r++) {
        board[r] = std::vector<std::string>(8, "");
    }
}

int main()
{
    crow::SimpleApp app;
    initializeBoard();

    // Végpont: teljes tábla JSON-ban
    CROW_ROUTE(app, "/board")
        ([]() {
        crow::json::wvalue jsonBoard;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                jsonBoard[r][c] = board[r][c];
            }
        }
        return jsonBoard;
            });

    // Végpont: kattintás kezelése
    CROW_ROUTE(app, "/click/<string>").methods(crow::HTTPMethod::POST)
        ([](const crow::request& /*req*/, std::string coord) {
        auto [row, col] = coordToIndex(coord);
        std::string piece = board[row][col];

        std::cout << "Kattintott mező: " << coord;
        if (!piece.empty()) {
            std::cout << " → Bábú: " << piece;
        }
        else {
            std::cout << " → Üres mező";
        }
        std::cout << std::endl;

        return crow::response(200);
            });

    // Statikus fájlok kiszolgálása (frontend mappa)
    app.route_dynamic("/frontend/<path>")
        ([](const crow::request& /*req*/, std::string path) {
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
