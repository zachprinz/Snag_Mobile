"                                                                                   \n\
#ifdef GL_ES                                                                        \n\
precision lowp float;                                                               \n\
#endif                                                                              \n\
#define PI 3.14                                                                     \n\
varying vec2 v_texCoord;                                                            \n\
varying vec4 v_color;                                                               \n\
uniform sampler2D u_texture;                                                        \n\
uniform vec2 resolution;                                                            \n\
//for debugging; use a constant value in final release                              \n\
uniform float upScale;                                                              \n\
uniform float accuracy;                                                             \n\
//alpha threshold for our occlusion map                                             \n\
const float THRESHOLD = 0.99;                                                       \n\
void main(void) {                                                                   \n\
float distance = 1.0;                                                           \n\
for (float y=0.0; y<resolution.y; y+=accuracy) {                                \n\
//rectangular to polar filter                                               \n\
vec2 norm = vec2(v_texCoord.s, y/resolution.y) * 2.0 - 1.0;                 \n\
float theta = PI*1.5 + norm.x * PI;                                         \n\
float r = (1.0 + norm.y) * 0.5;                                             \n\
//coord which we will sample from occlude map                               \n\
vec2 coord = vec2(-r * sin(theta), -r * cos(theta))/2.0 + 0.5;              \n\
//sample the occlusion map                                                  \n\
vec4 data = texture2D(u_texture, coord);                                    \n\
//the current distance is how far from the top we've come                   \n\
float dst = y/resolution.y / upScale;                                       \n\
//if we've hit an opaque fragment (occluder), then get new distance         \n\
//if the new distance is below the current, then we'll use that for our ray \n\
float caster = data.a;                                                      \n\
if (caster > THRESHOLD) {                                                   \n\
distance = min(distance, dst);                                              \n\
}                                                                           \n\
}                                                                           \n\
gl_FragColor = vec4(vec3(distance), 1.0);                                   \n\
}                                                                           \n\
";