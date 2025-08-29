#pragma once

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;

    Move(int fr, int fc, int tr, int tc)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc) {}
};
