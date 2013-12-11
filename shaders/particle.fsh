
precision mediump float;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec3 color;

varying vec2 uv;

void main()
{
//    gl_FragColor = texture2D(tex0, uv) + texture2D(tex1, uv);
    
    gl_FragColor = texture2D(tex0, uv) * vec4(color, 1.0);
    
//    float z = texture2D(tex0, uv).a;
//    gl_FragColor = vec4(z, 0.0, 0.0, 1.0);

//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}