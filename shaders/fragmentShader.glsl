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

vec4 CalcLightByDirection(Light light, vec3 direction) {
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;
    vec4 specularColour = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f)
    {
    	vec3 fragToEye = normalize(eyePosition - fragPos);
    	vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

    	float specularFactor = dot(fragToEye, reflectedVertex);
    	if (specularFactor > 0.0f)
    	{
    		specularFactor = pow(specularFactor, material.shininess);
    		specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
    	}
    }

    return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight() {
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

void main() {
    vec4 finalColour = CalcDirectionalLight();

    colour = texture(theTexture, textureCoords) * finalColour;
}