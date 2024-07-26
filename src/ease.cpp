#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

float lerp(float t, float start, float end) {
	return start + (end - start) * t;
}

// Referenced https://easings.net/#
// Some of the code here is rewrite from my lua script libraries

class Ease {
public:

	//
	//	BASIC
	//

	float linear(float t, float start, float end) { return lerp(t, start, end); };

	float byStep(float val, float dest, float step)
	{
		float result = val;
		if (result < dest)
		{
			result += step;
			if (result > dest)
				return dest;
		}
		else if (result > dest)
		{
			result -= step;
			if (result < dest)
				return dest;
		}

		return result;
	}

	//
	//	SINE
	//

	float inSine(float t, float start, float end) {
		return lerp(1.00f - cos((t * M_PI) / 2.00f), start, end);
	}

	float outSine(float t, float start, float end) {
		return lerp(sin((t * M_PI) / 2.00f), start, end);
	}

	float inOutSine(float t, float start, float end) {
		return lerp(-(cos(t * M_PI) - 1) / 2.00f, start, end);
	}

	//
	//	QUAD
	//

	float inQuad(float t, float start, float end) {
		return lerp(t * t, start, end);
	}

	float outQuad(float t, float start, float end) {
		return lerp(1.00f - (1.00f - t) * (1.00f - t), start, end);
	}

	float inOutQuad(float t, float start, float end) {
		float nt = (t < 0.5f) ? 2.00f * t * t : 1.00f - powf(-2.00f * t + 2.00f, 2.00f) / 2;
		
		return lerp(nt, start, end);
	}

	//
	//	CUBIC
	//

	float inCubic(float t, float start, float end) {
		return lerp(t * t * t, start, end);
	}

	float outCubic(float t, float start, float end) {
		return lerp(1.00f - powf(1.00f - t, 3.00f), start, end);
	}

	float inOutQuad(float t, float start, float end) {
		float nt = (t < 0.5f) ? 4.00f * t * t * t : 1.00f - powf(-2.00f * t + 2.00f, 3.00f) / 2;

		return lerp(nt, start, end);
	}

	//
	//	QUART
	//

	float inQuart(float t, float start, float end) {
		return lerp(t * t * t * t, start, end);
	}

	float outQuart(float t, float start, float end) {
		return lerp(1.00f - powf(1.00f - t, 4.00f), start, end);
	}

	float inOutQuart(float t, float start, float end) {
		float nt = (t < 0.5f) ? 8.00f * t * t * t * t : 1.00f - powf(-2.00f * t + 2.00f, 4.00f) / 2;

		return lerp(nt, start, end);
	}

	//
	//	QUINT
	//

	float inQuart(float t, float start, float end) {
		return lerp(t * t * t * t * t, start, end);
	}

	float outQuart(float t, float start, float end) {
		return lerp(1.00f - powf(1.00f - t, 5.00f), start, end);
	}

	float inOutQuart(float t, float start, float end) {
		float nt = (t < 0.5f) ? 16.00f * t * t * t * t * t : 1.00f - powf(-2.00f * t + 2.00f, 5.00f) / 2;

		return lerp(nt, start, end);
	}

};

class Bezier {
	float quad(float t, float p0, float p1, float p2)
	{
		return powf((1.00f - t), 2.00f) * p0 + (1.00f - t) * t * p1 * 2.00f + powf(t, 2.00f) * p2;
	}
};