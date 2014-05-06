varying vec3 norm;
varying vec3 cam_dir;
varying vec3 color;

uniform samplerCube cubemap;

// Declare any additional variables here. You may need some uniform variables.


void main(void)
{
	// Your shader code here.
	// Note that this shader won't compile since gl_FragColor is never set.

	// gl_FragColor = [color]
	
	
	
	vec3 texture_Coordinate = reflect(-cam_dir, normalize(norm));
	color = texture(cubemap, texture_Coordinate);
	gl_FragColor = vec4(color, 0.0);

}
