
attribute vec4 position;
attribute vec2 textureCoord;

varying vec2 uv;
varying vec2 blurUV[14];

void main()
{
    gl_Position = position;
    uv = textureCoord;
    blurUV[ 0] = textureCoord + vec2(-0.028, 0.0);
    blurUV[ 1] = textureCoord + vec2(-0.024, 0.0);
    blurUV[ 2] = textureCoord + vec2(-0.020, 0.0);
    blurUV[ 3] = textureCoord + vec2(-0.016, 0.0);
    blurUV[ 4] = textureCoord + vec2(-0.012, 0.0);
    blurUV[ 5] = textureCoord + vec2(-0.008, 0.0);
    blurUV[ 6] = textureCoord + vec2(-0.004, 0.0);
    blurUV[ 7] = textureCoord + vec2( 0.004, 0.0);
    blurUV[ 8] = textureCoord + vec2( 0.008, 0.0);
    blurUV[ 9] = textureCoord + vec2( 0.012, 0.0);
    blurUV[10] = textureCoord + vec2( 0.016, 0.0);
    blurUV[11] = textureCoord + vec2( 0.020, 0.0);
    blurUV[12] = textureCoord + vec2( 0.024, 0.0);
    blurUV[13] = textureCoord + vec2( 0.028, 0.0);
}