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
    if (dist > radius * 4.0) return 0.0;
    
    // Smoother metaball equation for organic morphing
    float normalizedDist = dist / radius;
    float influence = 0.0;
    
    if (normalizedDist < 1.0) {
        // Strong core influence
        influence = pow(1.0 - normalizedDist, 2.0);
    } else if (normalizedDist < 3.0) {
        // Extended smooth falloff for better morphing
        float t = (normalizedDist - 1.0) / 2.0;
        influence = 0.5 * pow(1.0 - t, 3.0);
    }
    
    return influence * radius * radius / 20.0; // Normalize influence
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
    
    // Lower threshold for smoother morphing
    float threshold = 1.0;
    
    // Don't discard pixels - use alpha blending for smooth edges
    if (totalInfluence < 0.1) {
        FragColor = vec4(0.0);
        return;
    }
    
    // Normalize color with influence weighting
    totalColor /= totalInfluence;
    
    // Create smooth transparent edges
    float alpha = smoothstep(0.0, threshold, totalInfluence);
    alpha = pow(alpha, 0.5); // Adjust curve for smoother falloff
    
    // Additional smoothing for very soft edges
    float edgeSoftness = smoothstep(threshold * 0.3, threshold * 1.2, totalInfluence);
    alpha = mix(alpha * 0.5, alpha, edgeSoftness);
    
    totalColor.a = alpha * totalColor.a;
    
    // Enhanced 3D effect with more organic gradients
    float centerInfluence = smoothstep(threshold, threshold * 2.5, totalInfluence);
    float rimLight = 1.0 - smoothstep(threshold * 0.9, threshold * 1.1, totalInfluence);
    
    totalColor.rgb *= 0.6 + 0.4 * centerInfluence;
    totalColor.rgb += vec3(0.1) * rimLight; // Subtle rim lighting
    
    FragColor = totalColor;
}