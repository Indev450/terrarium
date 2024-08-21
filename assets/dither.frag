#version 330

uniform sampler2D texture;

const int indexMatrix4x4[16] = int[](0,  8,  2,  10,
                                     12, 4,  14, 6,
                                     3,  11, 1,  9,
                                     15, 7,  13, 5);

float indexValue() {
    int x = int(mod(gl_FragCoord.x, 4));
    int y = int(mod(gl_FragCoord.y, 4));
    return indexMatrix4x4[(x + y * 4)] / 16.0;
}

float dither(float color, float a) {
    float closestColor = (a < 0.5) ? 0 : 1;
    float secondClosestColor = 1 - closestColor;
    float d = indexValue();
    float distance = abs(closestColor - color);
    return (distance < d) ? closestColor : secondClosestColor;
}

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Dither colors based on alpha (which kinda represents light level)
    pixel.r = dither(pixel.r, pixel.a);
    pixel.g = dither(pixel.g, pixel.a);
    pixel.b = dither(pixel.b, pixel.a);
    pixel.a = 1.0; // Remove alpha

    gl_FragColor = gl_Color * pixel;
}
