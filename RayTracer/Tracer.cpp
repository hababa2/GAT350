#include "Tracer.h"
#include "Scene.h"
#include "Camera.h"

#include <iostream>

void Tracer::Trace(const ColorBuffer& colorBuffer, Scene* scene, Camera* camera)
{
	float aspectRatio = colorBuffer.width / (float)colorBuffer.height;
	float invSamples = 1.0f / samples;

	for (int y = 0; y < colorBuffer.height; ++y)
	{
		for (int x = 0; x < colorBuffer.width; ++x)
		{
			glm::vec3 color{ 0, 0, 0 };

			for (int sample = 0; sample < samples; ++sample)
			{
				glm::vec2 viewport = camera->ScreenToViewport({ x + random01(), y + random01() });
				viewport.y = 1 - (viewport.y);
				Ray ray = camera->ViewportToRay(viewport);

				RaycastHit hit;
				color += scene->Trace(ray, 0.0001f, FLT_MAX, hit, depth);
			}

			color.r = sqrt(color.r * invSamples);
			color.g = sqrt(color.g * invSamples);
			color.b = sqrt(color.b * invSamples);

			colorBuffer.SetColor(x, y, Vec3ToColor(color));
		}

		//std::cout << (int)(y / 6.f) << "%" << std::endl;
	}
}
