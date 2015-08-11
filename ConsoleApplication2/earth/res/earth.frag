uniform sampler2D tex;
varying vec3 normal;
varying vec3 position;
uniform vec3 cameraPosition;

void main()
{
    vec3 lightDir = normalize(gl_LightSource[0].position.xyz - position.xyz);

    vec3 eyeDir   = normalize(cameraPosition - position.xyz);
    vec3 r       = normalize(reflect(-lightDir, normal) );


    float lightSpecular   = pow(max(dot(r, eyeDir), 0.0), gl_FrontMaterial.shininess);

    // Calculating The Diffuse Term And Clamping It To [0;1]
    float DiffuseTerm = clamp(dot(normal, lightDir), 0.0, 1.0);
    //DiffuseTerm += pow(max(dot(r, eyeDir), 0.0),   6);


    vec4 color = gl_FrontMaterial.ambient +  //ambient component

            gl_LightSource[0].diffuse
            * gl_FrontMaterial.diffuse //diffuse component
            * DiffuseTerm
            * texture2D(tex,gl_TexCoord[0].st)

            + gl_FrontMaterial.specular
            * gl_LightSource[0].specular
            * lightSpecular;

    // Calculating The Final Color
    gl_FragColor = color;
}
