#version 400 core

in vec2 pass_coord;

out vec4 out_color;

uniform sampler2D u_image;

void main() {
    float col = texture(u_image, pass_coord).r;
    if (col == 0.0) discard;
    out_color = vec4(vec3(col), 1.0);
}