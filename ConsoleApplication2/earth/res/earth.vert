varying vec3 normal;
varying vec3 position;

void main() {
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = ftransform();
    // Transforming The Vertex Position To ModelView-Space
    position = (gl_ModelViewMatrix * gl_Vertex).xyz;

    // Transforming The Normal To ModelView-Space
    normal = normalize(gl_NormalMatrix * gl_Normal);
}
