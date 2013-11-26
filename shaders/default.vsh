
uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;

attribute vec4 position;
attribute vec4 normal;

varying vec3 N;
varying vec3 L; // light direction
varying vec3 H; // halfway

void main()
{
//    const vec3 lightPosition = vec3(0.0, 0.0, 5.0);
//    const vec3 viewPosition = vec3(0.0, 0.0, 5.0);
//    
//    vec3 vertex = (modelview * vec4(position, 1.0)).xyz;
//    
//    N = normalize(normalMatrix * vec4(normal, 1.0)).xyz;
//    L = lightPosition + vec3(0.0, 0.0, 2.0);
//    L -= vertex;
//    H = normalize(L + (viewPosition - vertex));
    
//    gl_Position =  projection * vec4(vertex, 1.0);
    
    gl_Position =  projection * modelview * position;
}




// * default code dont change it *

//uniform mat4 modelview;
//uniform mat4 projection;
//uniform mat4 normalMatrix;
//
//attribute vec3 position;
//attribute vec3 normal;
//
//varying vec3 pos;
//varying vec3 norm;
//varying vec3 halfVector;

//void main()
//{
//    vec3 lightPosition = vec3(0.0, 0.0, 5.0);
//    
//    norm = normalize(normalMatrix * vec4(normal, 1.0)).xyz;
//    
//    pos = (modelview * vec4(position, 1.0)).xyz;
//    
//    halfVector = normalize(normalize(lightPosition - position) + vec3(0.0, 0.0, 1.0));
//    
//    gl_Position =  modelview * projection * vec4(position, 1.0);
//}

