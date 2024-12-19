#version 330

uniform sampler2D texture;

uniform float zoom;

uniform ivec2 size;

const int indexMatrix4x4[16] = int[](0,  8,  2,  10,
                                     12, 4,  14, 6,
                                     3,  11, 1,  9,
                                     15, 7,  13, 5);

float stepify(float val, float step) {
    return floor(val / step) * step;
}

float indexValue() {
    vec2 coord = gl_TexCoord[0].xy * size * 16;

    coord.x = stepify(coord.x, zoom);
    coord.y = stepify(coord.y, zoom);

    int x = int(mod(coord.x, 4));
    int y = int(mod(coord.y, 4));

    return indexMatrix4x4[(x + y * 4)] / 16.0;
}

float dither(float color, float a) {
    if (a < 0.1) return color;

    float closestColor = (a < 0.5) ? 0 : 1;
    float secondClosestColor = 1 - closestColor;
    float d = indexValue();
    float distance = abs(closestColor - (1 - a));
    return color * ((distance < d) ? closestColor : secondClosestColor);
}

void main()
{
    vec2 pixel_pos = gl_TexCoord[0].xy;
    pixel_pos.x = stepify(pixel_pos.x, 1.f/16/size.x);
    pixel_pos.y = stepify(pixel_pos.y, 1.f/16/size.y);

    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, pixel_pos);

    // Hack, makes everything a bit more bright to match with disabled dithering (kinda)
    pixel.a = pixel.a * pixel.a;

    if (pixel.a < 0.9)
        pixel.a = pixel.a - 0.1;

    // Dither colors based on alpha (which kinda represents light level)
    pixel.r = dither(pixel.r, pixel.a);
    pixel.g = dither(pixel.g, pixel.a);
    pixel.b = dither(pixel.b, pixel.a);
    pixel.a = 1.0; // Remove alpha

    gl_FragColor = gl_Color * pixel;
}
