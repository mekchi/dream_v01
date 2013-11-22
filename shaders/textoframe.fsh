
precision mediump float;

uniform sampler2D textureBuffer;

varying vec2 uv;

void main()
{
    gl_FragColor = texture2D(textureBuffer, uv);
    //gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}