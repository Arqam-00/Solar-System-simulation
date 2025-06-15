#version 330

uniform vec2 resolution;
uniform vec2 center;
uniform vec3 starColor;
uniform float radius;
uniform float glowStrength;

out vec4 fragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution;
    vec2 pos = uv * resolution - center;

    float dist = length(pos);
    float alpha = 1.0 - smoothstep(radius, radius + glowStrength, dist);

    vec3 color = starColor * alpha;

    fragColor = vec4(color, alpha);
}
