uniform mat4 u_mvpMatrix;

attribute vec4 a_position;
attribute vec3 a_texCoord;

varying vec2 texCoord;
varying vec2 videoCoord;

void main()
{
	gl_Position = u_mvpMatrix * a_position;
	texCoord   = a_texCoord.xy;
	videoCoord = a_texCoord.xz;
}
