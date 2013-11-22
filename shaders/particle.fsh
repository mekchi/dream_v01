
precision mediump float;

uniform sampler2D particle;

varying vec2 uv;

void main()
{
    gl_FragColor = texture2D(particle, uv) * vec4(1.0, 0.0, 0.0, 0.6);
//      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}