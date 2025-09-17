cd .. && rm -rf build && mkdir build && cd ./build
cmake -G "MinGW Makefiles" ..
mingw32-make


cpp_crow_add/ 
├─ include/ 
│ ├─ crow/ 
│ ├─ crow.h 
│ ├─ asio.hpp 
│ └─ asio/
├─ backend/
│ ├─ main.cpp
│ ├─ logic/
│ │ ├─ Board.h
│ │ ├─ game.cpp
│ │ ├─ game.h
│ │ ├─ move.h
│ │ ├─ utils.h
│ │ ├─ utils.cpp
│ │ ├─ pieces/
│ │ │ ├─ Bishop.h
│ │ │ ├─ King.h
│ │ │ ├─ Knight.h
│ │ │ ├─ Pawn.h
│ │ │ ├─ Piece.h
│ │ │ ├─ Queen.h
│ │ └─└─ Rook.h
│ └─ tests/
└─ frontend/
  └─ index.html
  └─ board.js
