#ifndef LIB_PERLIN_HPP
#define LIB_PERLIN_HPP

// Copyright 2012 Sol from www.solarianprogrammer.com
// The code is licensed under GPL v3

#include <vector>

namespace Perlin {

    class PerlinNoise {
        // The permutation vector
        std::vector<int> p;
    public:
        // Initialize with the reference values for the permutation vector
        PerlinNoise();
        // Generate a new permutation vector based on the value of seed
        PerlinNoise(unsigned int seed);
        // Get a noise value, for 2D images z can have any value
        double noise(double x, double y, double z);
    private:
        double fade(double t);
        double lerp(double t, double a, double b);
        double grad(int hash, double x, double y, double z);
    };

}

#endif
