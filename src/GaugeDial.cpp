#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeDial.hpp"

using namespace piZeroDash;

GaugeDial::~GaugeDial()
{
}

GaugeDial::GaugeDial(double radius, double dialCentreX, double dialCentreY, int globalPositionX, int globalPositionY,
	unsigned width, unsigned height) : Gauge(globalPositionX, globalPositionY, width, height)

{
	_radius = radius;
	_dialCentreX = dialCentreX;
	_dialCentreY = dialCentreY;
}

double GaugeDial::_getRadius()
{
	return _radius;
}

double GaugeDial::_getDialCentreX()
{
	return _dialCentreX;
}

double GaugeDial::_getDialCentreY()
{
	return _dialCentreY;
}

void GaugeDial::_drawDefaultBackground(CairoSurface& surface, int startNumber, int endNumber, double markDistance,
	bool showMinor, bool showPriorMinor, bool showLaterMinor, double markedFontSize, colour& markedFontColour,
	unsigned markedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset,
	colour& majorLineColour, colour& minorLineColour, double startAngle, double endAngle)
{
	// The background surface is exclusive to this gauge.

	// A graduation is the "pie slice".
	unsigned numGraduations = (double)(endNumber - startNumber) / markDistance;

	double curIndicatedNumber = startNumber;

	if(showMinor && showPriorMinor)
	{
		curIndicatedNumber -= markDistance;
	}

	double totalAngle = endAngle - startAngle;

	// Only the prior and later minor marks are below the horizontal.
	double stepAngle = totalAngle / (double)(numGraduations);

	// Note: A start angle of M_PI is an equivalent graduation angle of 0 because of how the markers are defined and rotated.
	double curGradAngle = M_PI - startAngle;

	if(showMinor && showPriorMinor) curGradAngle -= stepAngle;

	cairo_t* cr = surface.getContext();

	cairo_text_extents_t textExtents;
	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, markedFontSize);

	char textBuffer[16];

	// Distance from dial centre to number bounds box position.
	double numberStartRadius = _radius - lineLength - (lineLength / 4.0) - lineStartOffset;

	// The number of lines is one more than the number of graduations.
	unsigned numLines = numGraduations + 1;

	// Take into account prior and later minor lines.
	if(showMinor)
	{
		if(showPriorMinor) numLines++;
		if(showLaterMinor) numLines++;
	}

	// Setup snprintf format.
	string numberFormat = "%.";
	numberFormat += to_string(markedFontDecimalPlaces) + "F";

	for(unsigned lineIndex = 0; lineIndex < numLines; lineIndex++)
	{
		bool isMinor = !(lineIndex % 2);

		if(isMinor)
		{
			if(showMinor)
			{
				cairo_set_line_width(cr, minorLineWidth);
				cairo_set_source_rgba(cr, minorLineColour.r, minorLineColour.g, minorLineColour.b, minorLineColour.a);
			}
		}
		else
		{
			cairo_set_line_width(cr, majorLineWidth);
			cairo_set_source_rgba(cr, majorLineColour.r, majorLineColour.g, majorLineColour.b, majorLineColour.a);
		}

		// Cairo transforms appear to be post-multiplied together then pre-multiplied to the geometry to go from user to
		// device coordinates.

		if(!isMinor || showMinor)
		{
			// Rotate about the "dial centre".
			cairo_identity_matrix(cr);
			cairo_translate(cr, _dialCentreX, _dialCentreY);
			cairo_rotate(cr, curGradAngle);
			cairo_translate(cr, -_dialCentreX, -_dialCentreY);

			// Define and draw line.
			cairo_move_to(cr, _dialCentreX - _radius + lineStartOffset, _dialCentreY);
			cairo_line_to(cr, _dialCentreX - _radius + lineLength + lineStartOffset, _dialCentreY);
			cairo_stroke(cr);
		}

		// Draw text.
		if(!isMinor)
		{
			cairo_identity_matrix(cr);

			snprintf(textBuffer, 16, numberFormat.c_str(), curIndicatedNumber);

			cairo_text_extents(cr, textBuffer, &textExtents);

			double b = textExtents.width / 2.0;
			double h = textExtents.height / 2.0;
			double alpha = atan(h/b);
			double gamma = M_PI_2 - alpha;

			// Text reference point in text bounds local coords (origin is centre of bounds).
			double textRefX = -b - textExtents.x_bearing;
			double textRefY = -h - textExtents.y_bearing;

			double boxEdgeX;
			double boxEdgeY;

			// A bump in the number start radius so that number alignment can be tweaked.
			double numberStartRadiusBump = 0.0;

			if(curGradAngle < alpha)
			{
				boxEdgeX = -b;
				boxEdgeY = -b * tan(curGradAngle);
			}
			else if(curGradAngle < M_PI_2)
			{
				boxEdgeX = -h * tan(M_PI_2 - curGradAngle);
				boxEdgeY = -h;

				numberStartRadiusBump = -lineLength / 8.0;
			}
			else if(curGradAngle < M_PI_2 + gamma)
			{
				boxEdgeX = h * tan(curGradAngle - M_PI_2);
				boxEdgeY = -h;

				numberStartRadiusBump = -lineLength / 8.0;
			}
			else
			{
				boxEdgeX = b;
				boxEdgeY = -b * tan(M_PI - curGradAngle);
			}

			double textPosnX = _dialCentreX - (numberStartRadius + numberStartRadiusBump) * cos(curGradAngle) - boxEdgeX + textRefX;
			double textPosnY = _dialCentreY - (numberStartRadius + numberStartRadiusBump) * sin(curGradAngle) - boxEdgeY + textRefY;

			cairo_set_source_rgba(cr, markedFontColour.r, markedFontColour.g, markedFontColour.b, markedFontColour.a);

			cairo_move_to(cr, textPosnX, textPosnY);
			cairo_show_text(cr, textBuffer);
		}

		curGradAngle += stepAngle;

		curIndicatedNumber += markDistance;
	}
}
