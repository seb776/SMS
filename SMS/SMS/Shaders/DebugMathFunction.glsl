uniform vec2 resolution;
uniform float time;

uniform vec2 offset;
uniform vec2 scale = vec2(10.0, 5.0);
uniform vec4 minmax = vec4(2.0);
uniform const int CURVE_CNT = 1;

const int CURVE_ELEMS = 128;
uniform vec2 curveA[CURVE_ELEMS];
uniform vec2 curveB[CURVE_ELEMS];
uniform vec2 curveC[CURVE_ELEMS];

uniform const int H_LINES_CNT = 0;
uniform float horizontalLines[H_LINES_CNT];

uniform const int V_LINES_CNT = 0;
uniform float verticalLines[V_LINES_CNT];

const vec4 hlf = vec4(0.5);
const vec4 ppoo = vec4(1.0, 1.0, 0.0, 0.0);

float draw0(vec2 uv, vec2 pos)
{
	return 0.0;
}
float sat(float a)
{
	return clamp(a, 0.0, 1.0);
}

float rdrCurve(vec2 uv, vec2 curve[CURVE_ELEMS])
{
	float thick = 0.004*scale.y;
	float col;
	for (int i = 0; i < CURVE_ELEMS; ++i)
	{
		vec2 res = abs(uv - curve[i]);
		col = sat(col + float(res.x < thick && res.y < thick));
	}
	return col;
}

float rdrHoriz(vec2 uv, float height)
{
	uv.y -= height;
	float refv = abs(uv.y);
	float lineThick = 0.001*scale.y;
	float reference = float(refv < lineThick);
	return reference;
}

vec3 rdr(vec2 uv)
{
	vec2 ref2 = abs(uv);
	float lineThick = 0.001*scale.x;
	float reference = float(ref2.x < lineThick || ref2.y < lineThick);
	reference += rdrHoriz(uv, 1.0);
	reference += rdrHoriz(uv, -1.0);
	reference += rdrHoriz(uv, 2.0);
	reference += rdrHoriz(uv, -2.0);
	float ca = (CURVE_CNT > 0 ? rdrCurve(uv, curveA) : 0.0);
	float cb = (CURVE_CNT > 1 ? rdrCurve(uv, curveB) : 0.0);
	float cc = (CURVE_CNT > 2 ? rdrCurve(uv, curveC) : 0.0);
	return vec3(sat(reference)*0.5) +ppoo.xzz * ca + ppoo.zxz * cb + ppoo.zzx * cc;
}

void main(void)
{
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	float ratio = resolution.x / resolution.y;
	uv -= hlf.xy;
	uv.x *= ratio;
	uv *= scale;
	uv -= offset;

	vec3 outCol = rdr(uv);
	gl_FragColor = vec4(outCol, 1.0);
}