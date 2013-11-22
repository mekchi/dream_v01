
precision mediump float;

uniform sampler2D screenTexture;
uniform sampler2D particleTexture;
// 0 and 1 touch coordinates, x and y
// 2 - time step
// 3 - radius
// 4 - ratio
uniform float params[5];

varying vec2 uv;

void main()
{
    vec2 center = vec2(0.5, 0.5);
    float a = 0.25;
    float b = a * 640.0 / 960.0;
    vec2 h = uv - center;
//    float dist = sqrt(dot(h, h));
    
    float dist = (h.x * h.x) / (a * a) + (h.y * h.y) / (b * b);
    
    if (dist > 1.0)
        gl_FragColor = texture2D(screenTexture, uv);
    else if (dist < 0.9)
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
//        gl_FragColor = texture2D(screenTexture, uv) + vec4(0.1, 0.0, 0.0, 0.1);
        gl_FragColor = texture2D(screenTexture, uv) + texture2D(particleTexture, uv);
//        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}