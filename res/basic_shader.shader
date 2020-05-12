#shader vertex
#version 400
in vec4 position;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main() {
    // By using position.xyz we have ignored the intensity value that we don't need to
    // draw an image.
    vec4 bPos = vec4(position.xyz, 1.0);
  gl_Position = projection * view * model * bPos;
}
#shader fragment
#version 400
out vec4 frag_colour;
uniform vec4 u_Colour;
void main() {
  frag_colour = u_Colour;
}
