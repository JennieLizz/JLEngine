#version 330 core

out vec4 fragColor;

uniform vec2 u_Resolution;
uniform float u_Time;

vec4 dcAdd( vec4 a, vec4 b )
{
    return a + b;
}

// complex multiplication, and derivatives
vec4 dcMul( vec4 a, vec4 b )
{
    return vec4( a.x*b.x - a.y*b.y, 
				 a.x*b.y + a.y*b.x,
				 a.x*b.z + a.z*b.x - a.y*b.w - a.w*b.y,
				 a.x*b.w + a.w*b.x + a.z*b.y + a.y*b.z );
}

// complex squaring, and derivatives
vec4 dcSqr( vec4 a )
{
    return vec4( a.x*a.x - a.y*a.y, 
				 2.0*a.x*a.y,
				 2.0*(a.x*a.z - a.y*a.w),
				 2.0*(a.x*a.w + a.y*a.z) );
}

//--------------------------------------------------

void main()
{
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / u_Resolution.xy;
    p.x *= u_Resolution.x/u_Resolution.y;

    // animation	
	float tz = 0.5 - 0.5*cos(0.225*u_Time);
    float zo = pow( 0.5, 13.0*tz );
	
    vec4 c = vec4( vec2(-0.05,.6805) + p*zo, 1.0, 0.0 );

    // compute orbit
	float m2 = 0.0;
    float co = 0.0;
	vec4 z = vec4( 0.0, 0.0, 0.0, 0.0 );
    for( int i=0; i<256; i++ )
    {
        if( m2>1024.0 ) break;
			
        // Z -> Z² + c		
		z = dcAdd( dcSqr(z), c );
		
		m2 = dot( z.xy, z.xy );
        co += 1.0;
    }

    // distance	
	// d(c) = |Z|·log|Z|/|Z'|
	float d = 0.0;
	if( co<256.0 ) d = sqrt( dot(z.xy,z.xy)/dot(z.zw,z.zw) )*log(dot(z.xy,z.xy));

	
    // do some soft coloring based on distance
	d = clamp( 4.0*d/zo, 0.0, 1.0 );
	d = pow( d, 0.25 );
    vec3 col = vec3( d );
    
    // output color
    fragColor = vec4( col, 1.0 );
}