async function drawBoard() {
    const res = await fetch('/board');
    const board = await res.json();

    for (let row = 0; row < 8; row++) {
        for (let col = 0; col < 8; col++) {
            const coord = String.fromCharCode(97 + col) + (8 - row);
            const cell = document.querySelector(`[data-coord="${coord}"]`);
            const piece = board[row][col];
            cell.textContent = pieceToUnicode(piece);
        }
    }
}

function pieceToUnicode(piece) {
    const map = {
        "bR": "♜", "bN": "♞", "bB": "♝", "bQ": "♛", "bK": "♚", "bP": "♟",
        "wR": "♖", "wN": "♘", "wB": "♗", "wQ": "♕", "wK": "♔", "wP": "♙",
        "": ""
    };
    return map[piece] || "";
}

drawBoard();
