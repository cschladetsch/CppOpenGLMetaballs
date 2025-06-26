#version 330 core

in vec2 fragPos;
in vec2 centerPos;

uniform vec4 blobColor;
uniform float blobRadius;

out vec4 FragColor;

void main() {
    float dist = length(fragPos - centerPos);
    
    if (dist > blobRadius) {
        discard;
    }
    
    float edge = smoothstep(blobRadius * 0.95, blobRadius, dist);
    float center = 1.0 - smoothstep(0.0, blobRadius * 0.8, dist);
    
    vec4 color = blobColor;
    color.rgb *= (0.7 + 0.3 * center);
    color.a = 1.0 - edge;
    
    float highlight = pow(max(0.0, 1.0 - (dist / blobRadius)), 3.0) * 0.3;
    color.rgb += vec3(highlight);
    
    FragColor = color;
}