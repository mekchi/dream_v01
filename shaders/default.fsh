
precision mediump float;

varying vec3 N;
varying vec3 L;
varying vec3 H;

void main()
{
    const vec3 la = vec3(0.9, 0.9, 0.9);
    const vec3 ld = vec3(0.9, 0.9, 0.9);
    const vec3 ls = vec3(1.0, 1.0, 1.0);
    const vec3 ma = vec3(0.1, 0.1, 0.1);
    const vec3 md = vec3(0.1, 0.1, 0.1);
    const vec3 ms = vec3(1.0, 1.0, 1.0);
    const float shiness = 50.0;
    
    vec3 color = vec3(0.0, 0.0, 0.0);
    float dp;
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
    
    gl_FragColor = vec4(color, 1.0);
//    if (nv.z < 1.0)
//    {
//        gl_FragColor = vec4(abs(nv.x), abs(nv.y), abs(nv.z), 1.0);
////        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
////        gl_FragColor = vec4(color, 1.0);
//    }
//    else
//    {
//        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
//        //gl_FragColor = vec4(nv.x, nv.y, 0.0, 1.0);
//        //gl_FragColor = vec4(nv.x, nv.y, 0.0, 1.0);
//    }
}



//varying vec3 pos;
//varying vec3 norm;
//varying vec3 halfVector;
//
//void main()
//{
//    vec3 lightAmbient = vec3(0.4, 0.4, 0.4);
//    vec3 lightDiffuse = vec3(0.9, 0.9, 0.9);
//    vec3 lightSpecular = vec3(1.0, 1.0, 1.0);
//    vec3 materialAmbient = vec3(0.6, 0.6, 0.6);
//    vec3 materialDiffuse = vec3(0.1, 0.1, 0.1);
//    vec3 materialSpecular = vec3(1.0, 1.0, 1.0);
//    
//    vec3 n,halfV,viewV,lightDir;
//    float NdotL,NdotHV;
//    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
//    float att, dist;
//    
//    vec3 lightPosition = vec3(0.0, 0.0, 5.0);
//    
//    n = normalize(norm);
//    
//    // Compute the ligt direction
//    lightDir = vec3(lightPosition - pos);
//    
//    /* compute the distance to the light source to a varying variable*/
//    dist = length(lightDir);
//    
//    
//    /* compute the dot product between normal and ldir */
//    NdotL = max(dot(n,normalize(lightDir)),0.0);
//    
//    if (NdotL > 0.0) {
//        
//        att = 1.0 / (2.0 +
//                     1.0 * dist +
//                     0.5 * dist * dist);
//        color += vec4((lightDiffuse * materialDiffuse) * NdotL + (lightAmbient * materialAmbient), 1.0);
//        
//        
//        halfV = normalize(halfVector);
//        NdotHV = max(dot(n,halfV), 0.0);
//        
//        color += vec4(att * materialSpecular * lightSpecular * pow(NdotHV, 14.0), 1.0);
//    }
//    
//    gl_FragColor = color;
//}
