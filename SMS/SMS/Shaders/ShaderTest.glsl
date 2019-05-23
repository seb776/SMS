void main()
{
	vec2 v=-1.+gl_FragCoord.xy/vec2(640.,360.);

	float r=pow(pow(abs(v.x),18.)+pow(abs(v.y),10.),1.8/28.);
	vec2 t=vec2(gl_Color.x*64.+1./r,atan(v.x,v.y));

	t=fract(2.*t)-.5;
	vec4 col=(1.-pow(dot(t.xy,t.xy),.3))*vec4(2,1.8,1.6,0)+vec4(.3,.2,.1,0)+.12*vec4(v,0,0);

	gl_FragColor=col/(2.*r);
};