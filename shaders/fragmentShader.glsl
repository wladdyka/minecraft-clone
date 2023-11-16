#version 330

in vec2 textureCoords;
in vec4 vertexColour;
in vec3 normal;
in vec3 fragPos;

out vec4 colour;

struct Light {
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform DirectionalLight directionalLight;

uniform sampler2D theTexture;
uniform Material material;
uniform vec3 eyePosition;

float CalcDiffuse(vec3 normal, vec3 lightDir) {
    float diff = max(dot(normal, lightDir), 0.0);
    return diff;
}

vec4 CalcShadows() {
    vec3 lightDir = normalize(directionalLight.direction);

    // Calculate diffuse lighting based on the dot product of normal and light direction
    float diffuseFactor = CalcDiffuse(normal, lightDir);

    // Set different shadow intensity based on normal direction and view direction
    float shadowIntensity = 0.2 + 0.8 * diffuseFactor;

    // Calculate the view direction
    vec3 viewDir = normalize(eyePosition - fragPos);

    // Highlight the difference between back face and side face shadowing
    float viewDotNormal = dot(viewDir, normal);
    if (viewDotNormal < 0.0) {
        // Back face shadowing
        shadowIntensity *= 1.5; // Adjust this factor for emphasis
    } else {
        // Side face shadowing
        shadowIntensity *= 0.8; // Adjust this factor for emphasis
    }

    // Apply ambient, diffuse, and specular components
    vec4 ambientColour = vec4(directionalLight.base.colour, 1.0f) * directionalLight.base.ambientIntensity;
    vec4 diffuseColour = vec4(directionalLight.base.colour, 1.0f) * directionalLight.base.diffuseIntensity * shadowIntensity;
    vec4 specularColour = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = reflect(-lightDir, normal);

        float specularFactor = max(dot(fragToEye, reflectedVertex), 0.0);
        specularFactor = pow(specularFactor, material.shininess);
        specularColour = vec4(directionalLight.base.colour * material.specularIntensity * specularFactor, 1.0f);
    }

    return (ambientColour + diffuseColour + specularColour);
}

void main() {
    vec4 finalColour = CalcShadows();

    colour = texture(theTexture, textureCoords) * finalColour;
}