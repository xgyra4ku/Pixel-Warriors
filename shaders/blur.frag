uniform sampler2D texture;
void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = vec4(0.0);
    float offset = 1.0 / 512.0; // настройка размытия

    for (int i = -4; i <= 4; i++) {
        for (int j = -4; j <= 4; j++) {
            color += texture2D(texture, uv + vec2(i, j) * offset) * 0.0625;
        }
    }
    
    gl_FragColor = color;
}
