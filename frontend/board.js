// Sakktábla létrehozása
const chessboard = document.getElementById("chessboard");
const columns = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'];

for (let row = 8; row >= 1; row--) {
    for (let col = 0; col < 8; col++) {
        const square = document.createElement("div");
        square.classList.add("square");
        // Színezés
        if ((row + col) % 2 === 0) {
            square.classList.add("white");
        } else {
            square.classList.add("black");
        }
        // Koordináta hozzárendelése
        const coord = columns[col] + row;
        square.dataset.coord = coord;
        square.innerText = coord;

        // Kattintás esemény
        square.addEventListener("click", () => {
            console.log("Kattintott mező:", coord);

            // Adat küldése backendnek
            fetch(`/click/${coord}`, { method: "POST" })
                .then(res => {
                    if (res.ok) {
                        console.log("Koordináta elküldve:", coord);
                    } else {
                        console.error("Hiba a küldés közben");
                    }
                })
                .catch(err => console.error("Fetch hiba:", err));
        });

        chessboard.appendChild(square);
    }
}
