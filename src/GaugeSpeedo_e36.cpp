#include "GaugeSpeedo_e36.hpp"

using namespace piZeroDash;

GaugeSpeedo_e36::~GaugeSpeedo_e36()
{
}

GaugeSpeedo_e36::GaugeSpeedo_e36(unsigned maxSpeed, double lineLength, double majorLineWidth, double minorLineWidth,
	int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeSpeedo(maxSpeed, globalPositionX, globalPositionY, width, height)
{
	_lineLength = lineLength;
	_majorLineWidth = majorLineWidth;
	_minorLineWidth = minorLineWidth;
}

void GaugeSpeedo_e36::_drawBackground(CairoSurface& surface)
{
	// The background surface is exclusive to this gauge.

	double radius = (double)(_getWidth()) / 2.0;

	unsigned numGraduations = (_getMaxSpeed() - 20) / 10;

	// The 0 and max speed lines are below the semicircle.
	double stepAngle = M_PI / (double)(numGraduations - 2);

	double curGradAngle = -stepAngle;

	cairo_t* cr = surface.getContext();

	// The number of lines is one more than the number of graduations because 20 has a line.
	// Also a minor line goes below the horizontal before 20 and after max speed.
	unsigned numLines = numGraduations + 3;

	for(unsigned lineIndex = 0; lineIndex <= numLines; lineIndex++)
	{
		bool isMinor = !(lineIndex % 2);

		// For the moment pure white.
		cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);

		if(isMinor)
		{
			cairo_set_line_width(cr, _minorLineWidth);
		}
		else
		{
			cairo_set_line_width(cr, _majorLineWidth);
		}

		// Cairo transforms appear to be post-multiplied together then pre-multiplied to the geometry to go from user to
		// device coordinates.

		// Debug.
		cairo_matrix_t dmat;

		// Rotate about the "dial centre".
		cairo_identity_matrix(cr);
		cairo_translate(cr, radius, radius);
		cairo_rotate(cr, curGradAngle);
		cairo_translate(cr, -radius, -radius);

		cairo_get_matrix(cr, &dmat);

		// Debug.
		double x = 0.0;
		double y = radius;
		cairo_user_to_device(cr, &x, &y);

		// Define and draw line.
		cairo_move_to(cr, 0.0, radius);
		cairo_line_to(cr, _lineLength, radius);
		cairo_stroke(cr);

		curGradAngle += stepAngle;
	}
}

void GaugeSpeedo_e36::_drawForeground(CairoSurface& surface)
{
}

void GaugeSpeedo_e36::test()
{
	_speedoInstr.test(_getMaxSpeed());
}

bool GaugeSpeedo_e36::inTestMode()
{
	return _speedoInstr.inTestMode();
}
