// volume explosion shader
// simon green / nvidia 2012
// http://developer.download.nvidia.com/assets/gamedev/files/gdc12/GDC2012_Mastering_DirectX11_with_Unity.pdf
#version 460
// sorry, port from HLSL!
//#define float3 vec3
//#define float4 vec4

uniform float iTime;
uniform vec2 iResolution;
uniform vec2 iMouse;

// parameters
// be nice if we had sliders for these!
const int _MaxSteps = 32;
const float _StepDistanceScale = 0.5;
const float _MinStep = 0.001;
const float _DistThreshold = 0.005;

const int _VolumeSteps = 32;
const float _StepSize = 0.02; 
const float _Density = 0.1;

const float _SphereRadius = 0.5;
const float _NoiseFreq = 4.0;
const float _NoiseAmp = -0.5;
const vec3 _NoiseAnim = vec3(0, -1, 0);

// iq's nice integer-less noise function

// matrix to rotate the noise octaves
mat3 m = mat3( 0.00,  0.80,  0.60,
              -0.80,  0.36, -0.48,
              -0.60, -0.48,  0.64 );

float hash( float n )
{
    return fract(sin(n)*43758.5453);
}


float noise(vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0 + 113.0*p.z;

    float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                        mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
                    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                        mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
    return res;
}

float fbm( vec3 p )
{
    float f;
    f = 0.5000*noise( p ); p = m*p*2.02;
    f += 0.2500*noise( p ); p = m*p*2.03;
    f += 0.1250*noise( p ); p = m*p*2.01;
    f += 0.0625*noise( p );
    p = m*p*2.02; f += 0.03125*abs(noise( p ));	
    return f/0.9375;
}



// returns signed distance to nearest surface
// displace is displacement from original surface (0, 1)
float distanceFunc(vec3 p, out float displace)
{	
	//float d = length(p) - _SphereRadius;	// distance to sphere
	float d = length(p) - (sin(iTime*0.5)) * _SphereRadius;	// animated radius
	
	// offset distance with pyroclastic noise
	//p = normalize(p) * _SphereRadius;	// project noise point to sphere surface
	displace = fbm(p*_NoiseFreq + _NoiseAnim*iTime);
	d += displace * _NoiseAmp;
	
	return d;
}

// calculate normal from distance field
vec3 dfNormal(vec3 pos)
{
    float eps = 0.001;
    vec3 n;
    float s;
#if 0
    // central difference
    n.x = distanceFunc( vec3(pos.x+eps, pos.y, pos.z), s ) - distanceFunc( vec3(pos.x-eps, pos.y, pos.z), s );
    n.y = distanceFunc( vec3(pos.x, pos.y+eps, pos.z), s ) - distanceFunc( vec3(pos.x, pos.y-eps, pos.z), s );
    n.z = distanceFunc( vec3(pos.x, pos.y, pos.z+eps), s ) - distanceFunc( vec3(pos.x, pos.y, pos.z-eps), s );
#else
    // forward difference (faster)
    float d = distanceFunc(pos, s);
    n.x = distanceFunc( vec3(pos.x+eps, pos.y, pos.z), s ) - d;
    n.y = distanceFunc( vec3(pos.x, pos.y+eps, pos.z), s ) - d;
    n.z = distanceFunc( vec3(pos.x, pos.y, pos.z+eps), s ) - d;
#endif

    return normalize(n);
}

// color gradient 
// this should be in a 1D texture really
vec4 gradient(float x)
{
	const vec4 c0 = vec4(4, 4, 4, 1);	// hot white
	const vec4 c1 = vec4(1, 1, 0, 1);	// yellow
	const vec4 c2 = vec4(1, 0, 0, 1);	// red
	const vec4 c3 = vec4(0.4, 0.4, 0.4, 4);	// grey
	
	float t = fract(x*3.0);
	vec4 c;
	if (x < 0.3333) {
		c =  mix(c0, c1, t);
	} else if (x < 0.6666) {
		c = mix(c1, c2, t);
	} else {
		c = mix(c2, c3, t);
	}
	//return float4(x);
	//return float4(t);
	return c;
}

// shade a point based on position and displacement from surface
vec4 shade(vec3 p, float displace)
{	
	// lookup in color gradient
	displace = displace*1.5 - 0.2;
	displace = clamp(displace, 0.0, 0.99);
	vec4 c = gradient(displace);
	//c.a *= smoothstep(1.0, 0.8, length(p));
	
	// lighting
	vec3 n = dfNormal(p);
	float diffuse = n.z*0.5+0.5;
	//float diffuse = max(0.0, n.z);
	c.rgb = mix(c.rgb, c.rgb*diffuse, clamp((displace-0.5)*2.0, 0.0, 1.0));
	
	//return float4(float3(displace), 1);
	//return float4(dfNormal(p)*float3(0.5)+float3(0.5), 1);
	//return float4(diffuse);
	//return gradient(displace);
	return c;
}

// procedural volume
// maps position to color
vec4 volumeFunc(vec3 p)
{
	float displace;
	//float d = distanceFunc(p, displace);
	vec4 c = shade(p, displace);
	return c;
}

// sphere trace
// returns hit position
vec3 hitPos;
vec3 sphereTrace(vec3 rayOrigin, vec3 rayDir, inout bool hit, inout float displace)
{
	vec3 pos = rayOrigin;
	hit = false;
	displace = 0.0;	
	float d;
	//float3 hitPos;
	float disp;
	for(int i=0; i<_MaxSteps; i++) {
		d = distanceFunc(pos, disp);
        	if (d < _DistThreshold) {
			hit = true;
			displace = disp;
			hitPos = pos;
        		break;	// early exit from loop doesn't work in ES?
        	}
		//d = max(d, _MinStep);
		pos += rayDir*d*_StepDistanceScale;
	}
	
	return pos;
	//return hitPos;
}


// ray march volume from front to back
// returns color
vec4 rayMarch(vec3 rayOrigin, vec3 rayStep, inout vec3 pos)
{
	vec4 sum = vec4(0, 0, 0, 0);
	pos = rayOrigin;
	for(int i=0; i<_VolumeSteps; i++) {
		vec4 col = volumeFunc(pos);
		col.a *= _Density;
		col.a = min(col.a, 1.0);
		
		// pre-multiply alpha
		col.rgb *= col.a;
		sum = sum + col*(1.0 - sum.a);	
#if 0
		// exit early if opaque
        	if (sum.a > _OpacityThreshold)
            		break;
#endif		
		pos += rayStep;
	}
	return sum;
}


out vec4 FragColor;
uniform float timer;

void main(){
	if(timer > 25.0f){
		vec2 q = gl_FragCoord.xy  / iResolution.xy;
		vec2 p = q*2.0-1.0;
		p.x *= iResolution.x / iResolution.y;
	
		float rotx = 0.0f;//(iMouse.y / iResolution.y)*4.0;
		float roty = 0.0f;//0.2*iTime - (iMouse.x / iResolution.x)*4.0;
	
		// camera
		vec3 ro = 2.0*normalize(vec3(cos(roty), cos(rotx), sin(roty)));
		vec3 ww = normalize(vec3(0.0,0.0,0.0) - ro);
		vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));
		vec3 vv = normalize(cross(ww,uu));
		vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );

		// sphere trace distance field
		bool hit;
		float displace;
		hitPos = sphereTrace(ro, rd, hit, displace);

		vec4 col = vec4(0, 0, 0, 0.01);
		if (hit) {
			//shade
   			col = shade(hitPos, displace);	// opaque version
			//col = rayMarch(hitPos, rd*_StepSize, hitPos);	// volume render
		}
		if(col.a < 0.1f)
			discard;
		else{
			FragColor = col;
		}
	}

	else{
	FragColor = vec4(vec3(1.0f), 1.0f);
	}
}
