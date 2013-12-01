
precision mediump float;

uniform sampler2D tex0;
uniform sampler2D tex1;

varying vec2 uv;

void main()
{
//    gl_FragColor = texture2D(tex0, uv) + texture2D(tex1, uv);
    gl_FragColor = texture2D(tex0, uv);// * vec4(0.0, 0.0, 0.6, 1.0);
//      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}