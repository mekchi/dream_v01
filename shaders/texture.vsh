
uniform mat4 modelview;
uniform mat4 projection;
//uniform mat4 normalMatrix;

attribute vec4 position;
attribute vec4 normal;
attribute vec2 textureCoord;

varying vec2 uv;

void main()
{
    uv = textureCoord;
    gl_Position = (projection * modelview) * position;
}