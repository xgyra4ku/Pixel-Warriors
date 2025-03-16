// blur.frag
uniform sampler2D texture;
uniform vec2 blurRadius; // Радиус размытия

void main() {
    vec2 texCoord = gl_TexCoord[0].xy;

    // Простое размытие по горизонтали
    vec4 color = vec4(0.0);
    for (int i = -5; i <= 5; ++i) {
        vec2 offset = vec2(float(i) * blurRadius.x, 0.0);
        color += texture2D(texture, texCoord + offset);
    }
    color /= 11.0; // Нормализация цвета

    gl_FragColor = color;
}