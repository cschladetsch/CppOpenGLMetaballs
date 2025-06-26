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
    if (dist > radius * 3.0) return 0.0;
    
    // Modified metaball equation for more organic shapes
    // Using a higher power for smoother, more blob-like transitions
    float normalizedDist = dist / radius;
    float influence = 0.0;
    
    if (normalizedDist < 1.0) {
        // Core influence with smooth falloff
        influence = pow(1.0 - normalizedDist * normalizedDist, 3.0);
    } else if (normalizedDist < 2.5) {
        // Extended influence for better merging
        float t = (normalizedDist - 1.0) / 1.5;
        influence = 0.2 * pow(1.0 - t, 4.0);
    }
    
    return influence * radius;
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
    
    // Dynamic threshold based on blob density for more organic shapes
    float threshold = 15.0 + sin(totalInfluence * 0.1) * 2.0;
    
    if (totalInfluence < threshold * 0.8) {
        discard;
    }
    
    // Normalize color with influence weighting
    totalColor /= totalInfluence;
    
    // Create organic edge with multiple smoothstep layers
    float edge1 = smoothstep(threshold * 0.7, threshold * 0.9, totalInfluence);
    float edge2 = smoothstep(threshold * 0.85, threshold * 1.15, totalInfluence);
    float edge = edge1 * 0.7 + edge2 * 0.3;
    
    // Add noise-like variation for organic feel
    float variation = sin(totalInfluence * 0.5) * 0.05 + 1.0;
    edge *= variation;
    
    totalColor.a = edge;
    
    // Enhanced 3D effect with more organic gradients
    float centerInfluence = smoothstep(threshold, threshold * 2.5, totalInfluence);
    float rimLight = 1.0 - smoothstep(threshold * 0.9, threshold * 1.1, totalInfluence);
    
    totalColor.rgb *= 0.6 + 0.4 * centerInfluence;
    totalColor.rgb += vec3(0.1) * rimLight; // Subtle rim lighting
    
    FragColor = totalColor;
}