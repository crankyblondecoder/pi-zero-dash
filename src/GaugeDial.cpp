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

void GaugeDial::_setStandardProperties(int startNumber, int endNumber, double markDistance, bool showMinor,
	bool showPriorMinor, bool showLaterMinor, double markedFontSize, colour& markedFontColour,
	unsigned markedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
	double lineStartOffset, colour& majorLineColour, colour& minorLineColour, double startAngle, double endAngle)
{
	_startNumber = startNumber;
	_endNumber = endNumber;
	_markDistance = markDistance;
	_showMinor = showMinor;
	_showPriorMinor = showPriorMinor;
	_showLaterMinor = showLaterMinor;
	_markedFontSize = markedFontSize;
	_markedFontColour = markedFontColour;
	_markedFontDecimalPlaces = markedFontDecimalPlaces;
	_lineLength = lineLength;
	_majorLineWidth = majorLineWidth;
	_minorLineWidth = minorLineWidth;
	_lineStartOffset = lineStartOffset;
	_majorLineColour = majorLineColour;
	_minorLineColour = minorLineColour;
	_startAngle = startAngle;
	_endAngle = endAngle;
}

void GaugeDial::_drawDefaultBackground(CairoSurface& surface)
{
	// The background surface is exclusive to this gauge.

	// A graduation is the "pie slice".
	unsigned numGraduations = (double)(_endNumber - _startNumber) / _markDistance;

	double curIndicatedNumber = _startNumber;

	if(_showMinor && _showPriorMinor)
	{
		curIndicatedNumber -= _markDistance;
	}

	double totalAngle = _endAngle - _startAngle;

	// Only the prior and later minor marks are below the horizontal.
	double stepAngle = totalAngle / (double)(numGraduations);

	// Note: A start angle of M_PI is an equivalent graduation angle of 0 because of how the markers are defined and rotated.
	double curGradAngle = M_PI - _startAngle;

	if(_showMinor && _showPriorMinor) curGradAngle -= stepAngle;

	cairo_t* cr = surface.getContext();

	cairo_text_extents_t textExtents;
	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, _markedFontSize);

	char textBuffer[16];

	// Distance from dial centre to number bounds box position.
	double numberStartRadius = _radius - _lineLength - (_lineLength / 4.0) - _lineStartOffset;

	// The number of lines is one more than the number of graduations.
	unsigned numLines = numGraduations + 1;

	// Take into account prior and later minor lines.
	if(_showMinor)
	{
		if(_showPriorMinor) numLines++;
		if(_showLaterMinor) numLines++;
	}

	// Setup snprintf format.
	string numberFormat = "%.";
	numberFormat += to_string(_markedFontDecimalPlaces) + "F";

	for(unsigned lineIndex = 0; lineIndex < numLines; lineIndex++)
	{
		bool isMinor = !(lineIndex % 2);

		if(isMinor)
		{
			if(_showMinor)
			{
				cairo_set_line_width(cr, _minorLineWidth);
				cairo_set_source_rgba(cr, _minorLineColour.r, _minorLineColour.g, _minorLineColour.b, _minorLineColour.a);
			}
		}
		else
		{
			cairo_set_line_width(cr, _majorLineWidth);
			cairo_set_source_rgba(cr, _majorLineColour.r, _majorLineColour.g, _majorLineColour.b, _majorLineColour.a);
		}

		// Cairo transforms appear to be post-multiplied together then pre-multiplied to the geometry to go from user to
		// device coordinates.

		if(!isMinor || _showMinor)
		{
			// Rotate about the "dial centre".
			cairo_identity_matrix(cr);
			cairo_translate(cr, _dialCentreX, _dialCentreY);
			cairo_rotate(cr, curGradAngle);
			cairo_translate(cr, -_dialCentreX, -_dialCentreY);

			// Define and draw line.
			cairo_move_to(cr, _dialCentreX - _radius + _lineStartOffset, _dialCentreY);
			cairo_line_to(cr, _dialCentreX - _radius + _lineLength + _lineStartOffset, _dialCentreY);
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

				numberStartRadiusBump = -_lineLength / 8.0;
			}
			else if(curGradAngle < M_PI_2 + gamma)
			{
				boxEdgeX = h * tan(curGradAngle - M_PI_2);
				boxEdgeY = -h;

				numberStartRadiusBump = -_lineLength / 8.0;
			}
			else
			{
				boxEdgeX = b;
				boxEdgeY = -b * tan(M_PI - curGradAngle);
			}

			double textPosnX = _dialCentreX - (numberStartRadius + numberStartRadiusBump) * cos(curGradAngle) - boxEdgeX + textRefX;
			double textPosnY = _dialCentreY - (numberStartRadius + numberStartRadiusBump) * sin(curGradAngle) - boxEdgeY + textRefY;

			cairo_set_source_rgba(cr, _markedFontColour.r, _markedFontColour.g, _markedFontColour.b, _markedFontColour.a);

			cairo_move_to(cr, textPosnX, textPosnY);
			cairo_show_text(cr, textBuffer);
		}

		curGradAngle += stepAngle;

		curIndicatedNumber += _markDistance;
	}
}
