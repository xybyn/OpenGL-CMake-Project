#version 330 core


uniform float time;
uniform float yaw;
uniform float pitch;
uniform vec2 resolution;
uniform vec3 camera_position;
uniform vec3 camera_front;
uniform mat4 view_matrix;
out vec4 color;
in vec2 point_position;
#define MAX_STEPS 50
#define MAX_DIST 100.
#define ACCURACY 0.01



float sphere(vec3 p)
{
    float r = 1;
    vec3 position = vec3(0, 0,0);
    return length(p-position)-r;
}

float plane(vec3 p)
{
    return p.y;
}

float get_dist(vec3 p)
{
    return min(sphere(p), plane(p));
}

float ray_march(vec3 ro, vec3 rd)
{
    float dO = 0.0;

    for (int i = 0;i < MAX_STEPS;i++)
    {
        vec3 p = ro + rd * dO;
        float d = get_dist(p);
        dO += d;
        if(d < ACCURACY || dO > MAX_DIST)
            break;
    }
    return dO;

} 

vec3 get_normal(vec3 p)
{
    float d = get_dist(p);
    vec2 e = vec2(0.01, 0);

    vec3 n = vec3(d) - vec3(
        get_dist(p-e.xyy),
        get_dist(p-e.yxy),
        get_dist(p-e.yyx));

    return normalize(n);
}

float get_light(vec3 p)
{
    vec3 light_pos = vec3(4*sin(time), 5, 4*cos(time));
    vec3 l = normalize(light_pos - p);
    vec3 n = get_normal(p);
    
    float d = ray_march(p+n*0.05, l);
    float diff = clamp(dot(n, l), 0., 1.0);
    if(d<length(light_pos-p))
        diff *=0.1;
    return diff;
}

vec3 ray_dir( float fov, vec2 size, vec2 pos ) {
	 vec2 xy = pos - size / 2.0;
    float z = size.y / tan(radians(fov) / 2.0);
    return normalize(vec3(xy, z));
}

void main()
{
    vec3 ro = camera_position;

    vec3 rd =  (view_matrix * vec4(ray_dir(45,  resolution.xy, gl_FragCoord.xy), 0)).xyz;
    float d = ray_march(ro, rd);
    
    vec3 p = ro + rd * d;
    
    float diffuse = get_light(p);
    
    color = vec4(vec3(1,1, 1)*diffuse, 1);
      
}