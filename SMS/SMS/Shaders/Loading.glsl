uniform vec2 resolution;
uniform float time;
uniform float progress;

float sat(float a)
{
    return clamp(a, 0.,1.);
}

vec3 drawFlare(vec2 uv, float radius, vec3 rgb)
{
    float coef = 1.0-sat(length(uv) / radius);
    vec3 col = vec3(coef);
    return col * rgb;
}

bool isIn(float a, float mi, float ma)
{
    return a > mi && a < ma;
}

float drawGrid(vec2 uv, float sz, float thick)
{
    return float(mod((uv.x), sz) < thick) * float(mod((uv.y), sz) < thick);;
}

float drawRect(vec2 uv, float sz, float thick)
{
   	float hth = thick * .5;
    return float((isIn(abs(uv.x) - sz, -hth, hth) && isIn(abs(uv.y), 0., sz + hth)) ||
                 (isIn(abs(uv.y) - sz, -hth, hth) && isIn(abs(uv.x), 0., sz + hth))
                );
}

vec3 drawJauge(vec2 uv, float level)
{
    float waveLvl = 0.05;
    float sz = 0.21;
    level += waveLvl*sin(time+uv.x*10.) - waveLvl;
    level = sat(level);
    
    bool inWidth = abs(uv.x) < sz;
    bool inHeight = uv.y > -sz && uv.y < mix(-sz, sz+waveLvl, level) && uv.y < sz;
    
	float coef = float(inWidth && inHeight);
    
    vec3 col;
    
    float cGrad = pow(uv.x+0.5,.5);
    
    float power = 1.3;
    vec3 light = vec3(204., 255., 204.)/255.0;
    light.x = pow(light.x, power);
    light.y = pow(light.x, power);
    light.z = pow(light.x, power);
    
    col = mix(vec3(204., 51., 153.)/255.0*0.6, light, cGrad) * (2.*uv.y+0.9);
    return abs(col*sat(coef));
}

void main(void)
{

    vec2 uv = gl_FragCoord.xy/resolution.xx;
    

    uv -= vec2(0.5)*(resolution.xy / resolution.xx);

    vec3 colFlare = 0.4*drawFlare(uv, 0.5, vec3(0.423,0.13, 0.78));
    colFlare += 0.4*drawFlare(uv, 0.9, vec3(0.623,0.13, 0.78));
    
    vec3 col = vec3(0.);
    
    col += drawJauge(uv, progress);
    
    col += 0.5*vec3(0.523,0.53, 0.88)*drawRect(uv, 0.25,0.0015);
    col += 0.2*vec3(0.823,0.43, 0.48)*drawRect(uv, 0.23,0.0013);
    
    col += drawGrid(uv, 0.05,0.0015)*vec3(153, 204, 255)/255.0*0.6*sat(1.-(length(uv)/1.));

    col = col+ colFlare;
    gl_FragColor = vec4(col,1.0);
}

