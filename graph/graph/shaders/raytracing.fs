#version 420 core
out vec4 FragColor;

layout (location = 0) in vec2 pixelCoords;

uniform vec3 cameraPos;
uniform vec3 cameraDirection;
uniform vec3 cameraFOV;

struct Ray() {
    vec3 origin;
    vec3 direction;
};

struct Hit() {
    float distance;
    vec3 position;
    vec3 normal;
    vec3 color;
};

Hit solveSphere(vec3 origin, float radius, Ray ray)
{
    vec3 oc = ray.origin - origin;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0) {
        return Hit(-1.0f, vec3(0.0), vec3(0.0), vec3(0.0));
    }
    float t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t < 0.0) {
        t = (-b + sqrt(discriminant)) / (2.0 * a);
    }
    vec3 hitPos = ray.origin + ray.direction * t;
    vec3 hitNormal = normalize(hitPos - origin);
    return Hit(hitPos, hitNormal, vec3(1.0));
}



Ray rayFromPixelCoords(vec2 pixelCoords) {
    vec3 pixelRealPos = cameraPosition + cameraDirection
        + vec3(cos(cameraFOV.x) * pixelCoords.x, sin(cameraFOV.y) * pixelCoords.y, 0.0f);
    return Ray(cameraPosition, pixelRealPos);
}

void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
