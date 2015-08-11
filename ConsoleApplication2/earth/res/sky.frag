uniform sampler2D tex;
varying vec3 normal;
varying vec3 position;
uniform vec3 cameraPosition;
uniform float attInFactor;
uniform float attOutFactor;
uniform float range;
uniform float reflectAttShift;

//uniform vec2 params;
void main()
{
    //float attFactor = params.x;
    //float range = params.y;
    vec3 lightDir = normalize(gl_LightSource[0].position.xyz - position.xyz);

    vec3 eyeDir   = normalize(cameraPosition - position.xyz);
    vec3 r       = normalize(reflect(-lightDir, normal) );

    float l = dot(normal, eyeDir);

    float alpha = 0.0;
    float p = 7.0;
    if (l<range)
        alpha = clamp(pow (l / range, attOutFactor), 0.0, 1.0);
    else
        alpha = clamp(pow ((1.0 - l) / (1.0 - range), attInFactor), 0.0, 1.0);

    //alpha *= dot( r, eyeDir)  - reflectAttShift;

    //DiffuseTerm += pow(max(dot(r, eyeDir), 0.0),   6);

    float DiffuseTerm = clamp(
                clamp(dot(normal, lightDir),0.0, 1.0) +
                clamp(dot( r, eyeDir) / 50.0  - reflectAttShift, 0.0, 1.0),
                    0.0, 1.0);

    vec3 color =  gl_FrontMaterial.diffuse * clamp(DiffuseTerm, 0.0, 1.0);


    // Calculating The Final Color
    gl_FragColor = vec4(color.rgb, gl_FrontMaterial.diffuse.a * alpha);
}

