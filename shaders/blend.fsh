
precision mediump float;

uniform sampler2D texture0;
uniform sampler2D texture1;

varying vec2 uv;

void main()
{
    //    gl_FragColor = vec4(0.0, 0.0,  1.0, 1.0);
    gl_FragColor = texture2D(texture0, uv) + texture2D(texture1, uv);

}