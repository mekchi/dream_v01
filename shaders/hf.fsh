
precision mediump float;

uniform sampler2D particleTexture;
uniform vec3 lightPosition;
uniform float radius;

varying vec2 uv;
varying vec3 N;
varying vec3 L;
varying vec3 H;
varying vec3 P;

void main()
{
    const vec3 la = vec3(0.0, 0.0, 0.0);
    const vec3 ld = vec3(0.2, 0.2, 0.2);
    const vec3 ls = vec3(1.0, 1.0, 1.0);
    const vec3 ma = vec3(0.0, 0.0, 0.0);
    const vec3 md = vec3(0.01, 0.01, 0.01);
    const vec3 ms = vec3(1.0, 1.0, 1.0);
    const float shiness = 50.0;
    const float border = 2.0;
    
    float dp = 0.0;
 
    float dist = distance(P, lightPosition);
    
    if (dist <= radius - border)
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        if (dist <= radius)
        {
            dp = 1.0 - (abs((radius - dist) - 0.5 * border) * 2.0);
//            dp = 1.0 / (50.0 * dp);
//            gl_FragColor = vec4(vec2(0.5 * dp), dp, 1.0);
            //gl_FragColor = vec4(vec3(dp), 1.0);
            gl_FragColor = texture2D(particleTexture, uv) * vec4(dp, dp, 2.0 * dp, 1.0);
        }
        else
        {
            vec3 color = vec3(0.0, 0.0, 0.0);

            vec3 lv, nv;
            
            lv = normalize(L);
            nv = normalize(N);
            dp = max(dot(nv, lv), 0.0);
            
            if (dp > 0.0)
            {
                color = (la * ma) + (ld * md * dp)
                + (ls * ms * pow(
                                 max(
                                     dot(
                                         //normalize(reflect(lv, nv)),
                                         nv,
                                         normalize(H)
                                         ),
                                     0.0),
                                 shiness)
                   );
            }
            
            //gl_FragColor = vec4(P.z, 0.0, 0.0, 1.0);
            gl_FragColor = vec4(color, 1.0);
//            gl_FragColor = vec4(vec3(P.z), 1.0);
            //gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
            //nv.z = 0.0;
            //gl_FragColor = vec4(nv, 1.0);
        }
    }
}

