#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

// BB stands for Block Bitmask
static const uint8_t BB_TOP =         0b00000001;
static const uint8_t BB_BOTTOM =      0b00000010; // uwu
static const uint8_t BB_LEFT =        0b00000100;
static const uint8_t BB_RIGHT =       0b00001000;
static const uint8_t BB_TOPLEFT =     0b00010000;
static const uint8_t BB_TOPRIGHT =    0b00100000;
static const uint8_t BB_BOTTOMLEFT =  0b01000000;
static const uint8_t BB_BOTTOMRIGHT = 0b10000000;

// 8-bit masked autotiles have 47 unique visual variants. Extra 2 "variants"
// are just empty.

// Precalculated lookup table for positions in spritesheet.
static const sf::IntRect block_autotile_rects[49] = {
    { 0*16, 0*16, 16, 16 }, { 1*16, 0*16, 16, 16 },
    { 2*16, 0*16, 16, 16 }, { 3*16, 0*16, 16, 16 },
    { 4*16, 0*16, 16, 16 }, { 5*16, 0*16, 16, 16 },
    { 6*16, 0*16, 16, 16 }, { 0*16, 1*16, 16, 16 },
    { 1*16, 1*16, 16, 16 }, { 2*16, 1*16, 16, 16 },
    { 3*16, 1*16, 16, 16 }, { 4*16, 1*16, 16, 16 },
    { 5*16, 1*16, 16, 16 }, { 6*16, 1*16, 16, 16 },
    { 0*16, 2*16, 16, 16 }, { 1*16, 2*16, 16, 16 },
    { 2*16, 2*16, 16, 16 }, { 3*16, 2*16, 16, 16 },
    { 4*16, 2*16, 16, 16 }, { 5*16, 2*16, 16, 16 },
    { 6*16, 2*16, 16, 16 }, { 0*16, 3*16, 16, 16 },
    { 1*16, 3*16, 16, 16 }, { 2*16, 3*16, 16, 16 },
    { 3*16, 3*16, 16, 16 }, { 4*16, 3*16, 16, 16 },
    { 5*16, 3*16, 16, 16 }, { 6*16, 3*16, 16, 16 },
    { 0*16, 4*16, 16, 16 }, { 1*16, 4*16, 16, 16 },
    { 2*16, 4*16, 16, 16 }, { 3*16, 4*16, 16, 16 },
    { 4*16, 4*16, 16, 16 }, { 5*16, 4*16, 16, 16 },
    { 6*16, 4*16, 16, 16 }, { 0*16, 5*16, 16, 16 },
    { 1*16, 5*16, 16, 16 }, { 2*16, 5*16, 16, 16 },
    { 3*16, 5*16, 16, 16 }, { 4*16, 5*16, 16, 16 },
    { 5*16, 5*16, 16, 16 }, { 6*16, 5*16, 16, 16 },
    { 0*16, 6*16, 16, 16 }, { 1*16, 6*16, 16, 16 },
    { 2*16, 6*16, 16, 16 }, { 3*16, 6*16, 16, 16 },
    { 4*16, 6*16, 16, 16 }, { 5*16, 6*16, 16, 16 },
    { 6*16, 6*16, 16, 16 },
};

// When calculating bitmask, value may vary from 0 to 255. This lookup table
// transforms bitmask to index in the rectangle lookup table.
//
// Entries with value 255 should never be picked. Array is used to avoid
// overhead from using actual hash map.
static const uint8_t block_autotile_rects_indices[256] {
    24, 17, 3, 10, 23, 48, 34, 41, 21, 46, 32, 39, 22, 47, 33, 40,
    255, 255, 255, 255, 255, 16, 255, 29, 255, 255, 255, 255, 255, 37, 255, 25,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 14, 255, 28, 255, 38, 255, 19,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 15, 255, 27,
    255, 255, 255, 255, 255, 255, 2, 36, 255, 255, 255, 255, 255, 255, 30, 18,
    255, 255, 255, 255, 255, 255, 255, 9, 255, 255, 255, 255, 255, 255, 255, 20,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 45,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 35, 255, 255, 31, 26,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 44,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 255, 255, 255, 13,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 5,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 6,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 11,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 12,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 8,
};

// Coordinates for parts of input image.
//
// Indices:
// 0 - top left
// 1 - top right
// 2 - bottom left
// 3 - bottom right
static const sf::IntRect hline[4] = {
    {  0,  0,  8,  8 },
    {  8,  0,  8,  8 },
    {  0,  8,  8,  8 },
    {  8,  8,  8,  8 },
};

static const sf::IntRect vline[4] = {
    { 16,  0,  8,  8 },
    { 24,  0,  8,  8 },
    { 16,  8,  8,  8 },
    { 24,  8,  8,  8 },
};

static const sf::IntRect outtercorner[4] = {
    {  0, 16,  8,  8 },
    {  8, 16,  8,  8 },
    {  0, 24,  8,  8 },
    {  8, 24,  8,  8 },
};

static const sf::IntRect innercorner[4] = {
    { 16, 16,  8,  8 },
    { 24, 16,  8,  8 },
    { 16, 24,  8,  8 },
    { 24, 24,  8,  8 },
};

static const sf::IntRect fill = { 0, 32, 16, 16 };

enum QuadType {
    Fill = 0, HLine, VLine, OutterCorner, InnerCorner
};

QuadType decideQuadType(uint8_t vblock, uint8_t hblock, uint8_t dblock) {
    if (!(vblock || hblock || dblock)) { return QuadType::OutterCorner; }

    if (vblock && hblock) {
        return dblock ? QuadType::Fill : QuadType::InnerCorner;
    }

    if (vblock && !hblock) { return QuadType::VLine; }
    if (!vblock && hblock) { return QuadType::HLine; }

    return QuadType::Fill;
}

void decideQuadTypes(QuadType output[4], uint8_t bitmask) {
    output[0] = decideQuadType(bitmask & BB_TOP, bitmask & BB_LEFT, bitmask & BB_TOPLEFT);
    output[1] = decideQuadType(bitmask & BB_TOP, bitmask & BB_RIGHT, bitmask & BB_TOPRIGHT);
    output[2] = decideQuadType(bitmask & BB_BOTTOM, bitmask & BB_LEFT, bitmask & BB_BOTTOMLEFT);
    output[3] = decideQuadType(bitmask & BB_BOTTOM, bitmask & BB_RIGHT, bitmask & BB_BOTTOMRIGHT);
}

sf::IntRect getSourceRect(QuadType type, size_t index) {
    switch (type) {
        case QuadType::Fill: return fill;
        case QuadType::HLine: return hline[index];
        case QuadType::VLine: return vline[index];
        case QuadType::OutterCorner: return outtercorner[index];
        case QuadType::InnerCorner: return innercorner[index];
    }

    // Compiler complains if i don't add this
    return fill;
}

void fillSingleTile(sf::Image &output, const sf::Image &source, sf::Vector2i pos, QuadType quads[4]) {
    sf::Vector2i offsets[4] = {
        { 0, 0 },
        { 8, 0 },
        { 0, 8 },
        { 8, 8 },
    };

    output.copy(source, pos.x, pos.y, fill);

    for (size_t i = 0; i < 4; ++i) {
        if (quads[i] != QuadType::Fill) {
            sf::Vector2i copy_pos = pos + offsets[i];
            output.copy(source, copy_pos.x, copy_pos.y, getSourceRect(quads[i], i));
        }
    }
}

int main(int argc, const char *argv[]) {
    std::string input_fn;
    std::string output_fn;

    if (argc == 1) {
        std::cerr<<"Usage: "<<argv[0]<<" <input> [output]"<<std::endl;
        return EXIT_FAILURE;
    }
    else {
        input_fn = argv[1];

        if (argc > 2) {
            output_fn = argv[2];
        } else {
            output_fn = argv[1];

            if (output_fn.ends_with(".png"))
                output_fn.resize(output_fn.size() - 4); // Remove .png

            output_fn += "_out.png";
        }
    }

    sf::Image source;
    sf::Image result;

    if (!source.loadFromFile(input_fn)) {
        std::cerr<<"Failed to open input file"<<std::endl;

        return EXIT_FAILURE;
    }

    result.create(112, 112, sf::Color::Transparent);

    for (size_t i = 0; i <= 255; ++i) {
        uint8_t index = block_autotile_rects_indices[i];

        if (index != 255) {
            sf::Vector2i pos;
            QuadType quads[4] = { QuadType::Fill, };

            pos.x = block_autotile_rects[index].left;
            pos.y = block_autotile_rects[index].top;

            decideQuadTypes(quads, i);

            fillSingleTile(result, source, pos, quads);
        }
    }

    if (!result.saveToFile(output_fn)) {
        std::cerr<<"Failed to save result"<<std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
