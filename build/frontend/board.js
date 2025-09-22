let selectedCell = null;

function drawBoard(board) {
    const table = document.getElementById("chessboard");
    table.innerHTML = "";

    for (let row = 0; row < 8; row++) {
        const tr = document.createElement("tr");
        for (let col = 0; col < 8; col++) {
            const td = document.createElement("td");
            td.className = (row + col) % 2 === 0 ? "white" : "black";
            td.dataset.row = row;
            td.dataset.col = col;

            const type = board[row][col].type;
            const color = board[row][col].color;
            const key = `${type}_${color}`;
            td.textContent = pieceToUnicode({ type, color });

            td.onclick = () => handleClick(row, col, td);
            tr.appendChild(td);
        }
        table.appendChild(tr);
    }
}

async function loadBoard() {
    const res = await fetch("/state", { cache: "no-store" });
    const data = await res.json();
    drawBoard(data.board);
    updateStatus(data.currentPlayer);
}

async function handleClick(row, col, cell) {
    if (!selectedCell) {
        selectedCell = { row, col, cell };
        cell.style.outline = "3px solid red";
    } else {
        const move = {
            fromRow: selectedCell.row,
            fromCol: selectedCell.col,
            toRow: row,
            toCol: col
        };
        selectedCell.cell.style.outline = "";
        selectedCell = null;

        const res = await fetch("/move", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(move)
        });
        const data = await res.json();

        if (!data.success) {
            logMessage("Hiba: " + (data.error || "Érvénytelen lépés"), "error");
        } else {
            logMessage(data.moveCode || "lépés", "move");
        }

        drawBoard(data.board);
        updateStatus(data.currentPlayer);
    }
}

function pieceToUnicode(piece) {
    const map = {
        "0_1": "♚", "1_1": "♛", "2_1": "♜", "3_1": "♝", "4_1": "♞", "5_1": "♟",
        "0_0": "♔", "1_0": "♕", "2_0": "♖", "3_0": "♗", "4_0": "♘", "5_0": "♙",
        "None_None": ""
    };
    return map[`${piece.type}_${piece.color}`] || "";
}

function logMessage(message, type = "move") {
    const panel = document.getElementById("log-panel");
    const entry = document.createElement("div");
    entry.className = type;
    entry.textContent = message;
    panel.appendChild(entry);
    panel.scrollTop = panel.scrollHeight;

    const logs = JSON.parse(localStorage.getItem("chessLogs") || "[]");
    logs.push({ message, type });
    localStorage.setItem("chessLogs", JSON.stringify(logs));
}

function updateStatus(currentPlayer) {
    document.getElementById("status").textContent =
        currentPlayer === 0 ? "Fehér következik" : "Fekete következik";
}

window.addEventListener("DOMContentLoaded", () => {
    const logs = JSON.parse(localStorage.getItem("chessLogs") || "[]");
    logs.forEach(log => logMessage(log.message, log.type));
    loadBoard();
});

document.getElementById("resetBtn").addEventListener("click", async () => {
    const res = await fetch("/reset", { method: "POST" });
    const data = await res.json();

    drawBoard(data.board);
    updateStatus(data.currentPlayer);

    document.getElementById("log-panel").innerHTML = "";
    localStorage.removeItem("chessLogs");
    logMessage("Új játék kezdődött", "move");
});
