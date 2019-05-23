uniform vec2 resolution;
uniform float time;
uniform int pointerCount = 1;



void main(void) {
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	float ratio = resolution.x / resolution.y;
	uv -= hlf.xy;
	uv.x *= ratio;

	gl_FragColor *= vec4(float(length(uv) < 0.1));
}