#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 projection;
uniform vec2 blobCenter;
uniform float blobRadius;
uniform float distortionFactor;
uniform vec2 distortionDirection;

out vec2 fragPos;
out vec2 centerPos;

void main() {
    vec2 pos = position;
    
    vec2 fromCenter = pos - blobCenter;
    float dist = length(fromCenter);
    
    if (distortionFactor > 0.0 && dist < blobRadius * 1.5) {
        float normalizedDist = dist / blobRadius;
        float stretchAmount = distortionFactor * (1.0 - normalizedDist);
        
        vec2 perpendicular = vec2(-distortionDirection.y, distortionDirection.x);
        float dotProduct = dot(normalize(fromCenter), distortionDirection);
        
        pos += distortionDirection * stretchAmount * blobRadius * 0.5 * dotProduct;
        pos += perpendicular * stretchAmount * blobRadius * 0.2 * (1.0 - abs(dotProduct));
    }
    
    fragPos = pos;
    centerPos = blobCenter;
    
    gl_Position = projection * vec4(pos, 0.0, 1.0);
}