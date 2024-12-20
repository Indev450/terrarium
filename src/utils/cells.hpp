/*
 * Copyright 2022 Indev
 *
 * This file is part of TerrariumEngine
 *
 * TerrariumEngine is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#ifndef UTIL_CELLS_HPP
#define UTIL_CELLS_HPP

#include <vector>

template <class T>
struct Cells {
    std::vector<T> cells;

    unsigned width = 0, height = 0;

    Cells& operator=(Cells &&moved) {
        cells = std::move(moved.cells);

        width = moved.width;
        moved.width = 0;

        height = moved.height;
        moved.height = 0;

        return *this;
    }

    Cells(Cells &&moved) {
        *this = std::move(moved);
    }

    Cells() = default;

    Cells(const Cells &copy) = delete;
    Cells& operator=(const Cells &copy) = delete;

    inline void create(unsigned _width, unsigned _height, T fill_with) {
        width = _width;
        height = _height;

        cells.resize(width*height);

        std::fill(cells.begin(), cells.end(), fill_with);
    }

    inline bool isInRange(int x, int y) const {
        return x >= 0 && x < int(width) && y >= 0 && y < int(height);
    }

    inline T *get(int x, int y) {
        if (!isInRange(x, y)) {
            return nullptr;
        }

        return &cells[y*width + x];
    }

    inline const T *get(int x, int y) const {
        if (!isInRange(x, y)) {
            return nullptr;
        }

        return &cells[y*width + x];
    }
};

#endif
