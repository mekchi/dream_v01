
precision mediump float;

uniform sampler2D textureBuffer;

varying vec2 uv;

void main()
{
//    vec4 color = texture2D(textureBuffer, uv);
//    gl_FragColor = vec4(0.0, 0.0,  1.0, 1.0);
    gl_FragColor = texture2D(textureBuffer, uv);
//    gl_FragColor = vec4(uv, 0.0, 1.0);
}