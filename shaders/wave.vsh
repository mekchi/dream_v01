
attribute vec4 position;
attribute vec2 textureCoord;

varying vec2 uv;

void main()
{
    uv = textureCoord;
    gl_Position = position;
}