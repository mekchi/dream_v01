
uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 normalMatrix;
uniform mediump vec3 lightPosition;

attribute vec4 position;
attribute vec3 normal;

varying vec2 uv;
varying vec3 N;
varying vec3 L; // light direction
varying vec3 H; // halfway
varying vec3 P;


void main()
{
    //vec3 lightPosition = vec3(0.0, 0.0, 13.0);
    const vec3 viewPosition = vec3(0.0, 0.0, 5.0);
    vec3 vertex = (modelview * position).xyz;

    uv = vec2((position.x + 17.0) / 34.0, (position.y + 25.0) / 50.0);

    P = position.xyz;
    N = normalize(normalMatrix * vec4(normal, 0.0)).xyz;
    L = lightPosition + vec3(0.0, 0.0, 2.0);
    L -= vertex;
    H = normalize(L + (viewPosition - vertex));
    
    
    gl_Position = projection * vec4(vertex, 1.0);
}