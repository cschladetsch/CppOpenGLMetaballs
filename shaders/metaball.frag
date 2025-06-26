#version 330 core

in vec2 fragCoord;
out vec4 FragColor;

uniform vec2 resolution;
uniform int blobCount;
uniform vec2 blobPositions[100];
uniform float blobRadii[100];
uniform vec4 blobColors[100];

float metaball(vec2 pos, vec2 center, float radius) {
    float dist = length(pos - center);
    if (dist > radius * 2.0) return 0.0;
    
    // Metaball equation: f(x,y) = r^2 / ((x-cx)^2 + (y-cy)^2)
    float influence = (radius * radius) / (dist * dist + 0.001);
    return influence;
}

void main() {
    vec2 uv = fragCoord * resolution;
    
    float totalInfluence = 0.0;
    vec4 totalColor = vec4(0.0);
    
    // Calculate influence from all blobs
    for (int i = 0; i < blobCount; i++) {
        float influence = metaball(uv, blobPositions[i], blobRadii[i]);
        totalInfluence += influence;
        totalColor += blobColors[i] * influence;
    }
    
    // Threshold for metaball surface
    float threshold = 1.0;
    
    if (totalInfluence < threshold) {
        discard;
    }
    
    // Normalize color
    totalColor /= totalInfluence;
    
    // Smooth edges
    float edge = smoothstep(threshold * 0.9, threshold * 1.1, totalInfluence);
    totalColor.a = edge;
    
    // Add slight gradient for 3D effect
    float centerInfluence = smoothstep(threshold, threshold * 2.0, totalInfluence);
    totalColor.rgb *= 0.7 + 0.3 * centerInfluence;
    
    FragColor = totalColor;
}