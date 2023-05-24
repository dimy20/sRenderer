#include "light.h"
#include "fbuffer.h"

Light light(Vec3(0, 0, 1.0), 0xffffffff);

uint32_t light_color_apply_intensity(uint32_t light_color, float intensity){
	if(intensity > 1) intensity = 1.0;
	if(intensity < 0) intensity = 0;

	uint8_t r, g, b, shaded_r, shaded_g, shaded_b;
	mate3d::unpack_color(light_color, &r, &g, &b);

	shaded_r = static_cast<uint8_t>(intensity * r);
	shaded_g = static_cast<uint8_t>(intensity * g);
	shaded_b = static_cast<uint8_t>(intensity * b);

	return mate3d::pack_color(shaded_r, shaded_g, shaded_b);
}
