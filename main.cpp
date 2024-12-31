#include <iostream>
#include <cmath>
#include <fstream>

struct Vec3 {
    double x, y, z;

    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    Vec3 add(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 subtract(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 scale(double factor) const {
        return Vec3(x * factor, y * factor, z * factor);
    }

    double dotProduct(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 normalize() const {
        double length = std::sqrt(x * x + y * y + z * z);
        return Vec3(x / length, y / length, z / length);
    }
};

bool isRayHittingSphere(const Vec3& sphereCenter, double sphereRadius, const Vec3& rayOrigin, const Vec3& rayDirection) {
    Vec3 vectorFromRayOriginToCenter = rayOrigin.subtract(sphereCenter);
    double a = rayDirection.dotProduct(rayDirection);
    double b = 2.0 * vectorFromRayOriginToCenter.dotProduct(rayDirection);
    double c = vectorFromRayOriginToCenter.dotProduct(vectorFromRayOriginToCenter) - sphereRadius * sphereRadius;
    double discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

Vec3 getColorForRay(const Vec3& rayOrigin, const Vec3& rayDirection) {
    Vec3 sphereCenter(0, 0, -5);
    double sphereRadius = 1.0;

    if (isRayHittingSphere(sphereCenter, sphereRadius, rayOrigin, rayDirection)) {
        return Vec3(1, 0, 0);
    }

    double t = 0.5 * (rayDirection.y + 1.0);
    Vec3 white(1.0, 1.0, 1.0);
    Vec3 blue(0.5, 0.7, 1.0);
    return white.scale(1.0 - t).add(blue.scale(t));
}

int main() {
    const int imageWidth = 400;
    const int imageHeight = 200;

    std::ofstream imageFile("output.ppm");
    imageFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    Vec3 cameraOrigin(0, 0, 0);
    double viewportHeight = 2.0;
    double viewportWidth = 4.0;
    double focalLength = 1.0;

    for (int y = imageHeight - 1; y >= 0; --y) {
        for (int x = 0; x < imageWidth; ++x) {
            double u = double(x) / (imageWidth - 1);
            double v = double(y) / (imageHeight - 1);

            Vec3 rayDirection(
                u * viewportWidth - viewportWidth / 2,
                v * viewportHeight - viewportHeight / 2,
                -focalLength
            );
            rayDirection = rayDirection.normalize();

            Vec3 pixelColor = getColorForRay(cameraOrigin, rayDirection);

            int red = static_cast<int>(255.999 * pixelColor.x);
            int green = static_cast<int>(255.999 * pixelColor.y);
            int blue = static_cast<int>(255.999 * pixelColor.z);

            imageFile << red << " " << green << " " << blue << "\n";
        }
    }

    imageFile.close();

    std::cout << "Rendering is complete, Check the output in 'output.ppm'.\n";
    return 0;
}
