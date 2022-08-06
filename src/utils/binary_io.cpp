/*
 * Copyright 2022 Inner Devil
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

#include <string>

#include "binary_io.hpp"

////////////////////////////////////////////////////////////////////////
// Basic types                                                        //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
template <>
uint8_t read<uint8_t>(std::istream &s) {
    char byte;

    s.read(&byte, 1);

    return byte;
}

template <>
void write<uint8_t>(std::ostream &s, uint8_t value) {
    s.write((char*)(&value), 1);
}

////////////////////////////////////////////////////////////////////////
template <>
uint16_t read<uint16_t>(std::istream &s) {
    uint16_t result = 0;
    uint8_t bytes[2];

    s.read((char*)bytes, 2);

    result = bytes[0] | (bytes[1] << 8);

    return result;
}

template <>
void write<uint16_t>(std::ostream &s, uint16_t value) {
    uint8_t bytes[2];

    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;

    s.write((char*)bytes, 2);
}

////////////////////////////////////////////////////////////////////////
template <>
uint32_t read<uint32_t>(std::istream &s) {
    uint32_t result = 0;
    uint8_t bytes[4];

    s.read((char*)bytes, 4);

    result = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);

    return result;
}

template <>
void write<uint32_t>(std::ostream &s, uint32_t value) {
    uint8_t bytes[4];

    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;

    s.write((char*)bytes, 4);
}

////////////////////////////////////////////////////////////////////////
template <>
int32_t read<int32_t>(std::istream &s) {
    int32_t result = 0;
    uint8_t bytes[4];

    s.read((char*)bytes, 4);

    result = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);

    return result;
}

template <>
void write<int32_t>(std::ostream &s, int32_t value) {
    uint8_t bytes[4];

    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;

    s.write((char*)bytes, 4);
}

////////////////////////////////////////////////////////////////////////
// Complex types                                                      //
////////////////////////////////////////////////////////////////////////


template <>
Terrarium::Tile read<Terrarium::Tile>(std::istream &s) {
    Terrarium::Tile tile;

    tile.fg = read<Terrarium::blockid>(s);
    tile.bg = read<Terrarium::blockid>(s);

    return tile;
}

template <>
void write<Terrarium::Tile>(std::ostream &s, Terrarium::Tile tile) {
    write(s, tile.fg);
    write(s, tile.bg);
}

////////////////////////////////////////////////////////////////////////
template <>
std::string read<std::string>(std::istream &s) {
    uint32_t size = read<uint32_t>(s);

    char *buff = new char[size];
    s.read(buff, size);

    std::string str(buff, size);
    delete[] buff;

    return str;
}

template <>
void write<const std::string&>(std::ostream &s, const std::string &string) {
    uint32_t size = string.length();

    write(s, size);

    s.write(string.c_str(), size);
}
