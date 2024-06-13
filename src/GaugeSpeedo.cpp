#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeSpeedo.hpp"

using namespace piZeroDash;

GaugeSpeedo::~GaugeSpeedo()
{
}

GaugeSpeedo::GaugeSpeedo(unsigned maxSpeed, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: Gauge(globalPositionX, globalPositionY, width, height)
{
	_maxSpeed = maxSpeed;

	_addInstrument(&_speedoInstr);
}

unsigned GaugeSpeedo::_getMaxSpeed()
{
	return _maxSpeed;
}

void GaugeSpeedo::test()
{
	_speedoInstr.test(_maxSpeed + 10);
}

bool GaugeSpeedo::inTestMode()
{
	return _speedoInstr.inTestMode();
}

void GaugeSpeedo::_drawDefaultBackground(CairoSurface& surface, double markedSpeedFontSize, colour& markedSpeedFontColour,
	double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset, colour& majorLineColour,
	colour& minorLineColour, colour& preciseSpeedBackgroundColour, double preciseSpeedBackgroundWidth,
	double preciseSpeedBackgroundHeight)
{
	// The background surface is exclusive to this gauge.

	double radius = (double)(_getWidth()) / 2.0;

	unsigned numGraduations = _getMaxSpeed() / 10;

	unsigned curSpeed = 0;

	// The 0 and max speed lines are below the semicircle.
	double stepAngle = M_PI / (double)(numGraduations - 2);

	double curGradAngle = -stepAngle;

	cairo_t* cr = surface.getContext();

	cairo_text_extents_t textExtents;
	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, markedSpeedFontSize);

	// Distance from dial centre to number bounds box position.
	double numberStartRadius = radius - lineLength - (lineLength / 4.0) - lineStartOffset;

	// The number of lines is one more than the number of graduations because 20 has a line.
	// Also a minor line goes below the horizontal before 20 and after max speed.
	unsigned numLines = numGraduations + 1;

	for(unsigned lineIndex = 0; lineIndex < numLines; lineIndex++)
	{
		bool isMinor = !(lineIndex % 2);

		curSpeed += 10;

		if(isMinor)
		{
			cairo_set_line_width(cr, minorLineWidth);
			cairo_set_source_rgba(cr, minorLineColour.r, minorLineColour.g, minorLineColour.b, minorLineColour.a);
		}
		else
		{
			cairo_set_line_width(cr, majorLineWidth);
			cairo_set_source_rgba(cr, majorLineColour.r, majorLineColour.g, majorLineColour.b, majorLineColour.a);
		}

		// Cairo transforms appear to be post-multiplied together then pre-multiplied to the geometry to go from user to
		// device coordinates.

		// Rotate about the "dial centre".
		cairo_identity_matrix(cr);
		cairo_translate(cr, radius, radius);
		cairo_rotate(cr, curGradAngle);
		cairo_translate(cr, -radius, -radius);

		// Define and draw line.
		cairo_move_to(cr, lineStartOffset, radius);
		cairo_line_to(cr, lineLength + lineStartOffset, radius);
		cairo_stroke(cr);

		// Draw text.
		if(!isMinor)
		{
			cairo_identity_matrix(cr);

			string numberText = to_string(curSpeed);
			cairo_text_extents(cr,  numberText.c_str(), &textExtents);

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

			double textPosnX = radius - (numberStartRadius + numberStartRadiusBump) * cos(curGradAngle) - boxEdgeX + textRefX;
			double textPosnY = radius - (numberStartRadius + numberStartRadiusBump) * sin(curGradAngle) - boxEdgeY + textRefY;

			cairo_set_source_rgba(cr, markedSpeedFontColour.r, markedSpeedFontColour.g, markedSpeedFontColour.b,
				markedSpeedFontColour.a);

			cairo_move_to(cr, textPosnX, textPosnY);
			cairo_show_text(cr, numberText.c_str());
		}

		curGradAngle += stepAngle;
	}

	// Draw precise speed background.
	cairo_identity_matrix(cr);

	cairo_set_source_rgba(cr, preciseSpeedBackgroundColour.r, preciseSpeedBackgroundColour.g, preciseSpeedBackgroundColour.b,
		preciseSpeedBackgroundColour.a);

	double cornerRadius = preciseSpeedBackgroundHeight / 4.0;

	double left = radius - preciseSpeedBackgroundWidth / 2.0;
	double right = left + preciseSpeedBackgroundWidth;
	double top = radius - preciseSpeedBackgroundHeight / 2.0;
	double bottom = top + preciseSpeedBackgroundHeight;

	cairo_new_sub_path(cr);
	cairo_arc(cr, left + cornerRadius, top + cornerRadius, cornerRadius, M_PI, 1.5 * M_PI);
	cairo_line_to(cr, right - cornerRadius, top);
	cairo_arc(cr, right - cornerRadius, top + cornerRadius, cornerRadius, 1.5 * M_PI, 2.0 * M_PI);
	cairo_line_to(cr, right, bottom - cornerRadius);
	cairo_arc(cr, right - cornerRadius, bottom - cornerRadius, cornerRadius, 0.0, 0.5 * M_PI);
	cairo_line_to(cr, left - cornerRadius, bottom);
	cairo_arc(cr, left + cornerRadius, bottom - cornerRadius, cornerRadius, 0.5 * M_PI, M_PI);
	cairo_close_path(cr);

	cairo_fill(cr);
}

void GaugeSpeedo::_drawDefaultForeground(CairoSurface& surface, double indicatorLineLength, double indicatorLineWidth,
	colour& indicatorLineColour, double preciseSpeedFontSize, colour& preciseSpeedFontColour)
{
	cairo_t* cr = surface.getContext();

	double radius = (double)(_getWidth()) / 2.0;

	// Draw indicator line.

	cairo_set_source_rgba(cr, indicatorLineColour.r, indicatorLineColour.g, indicatorLineColour.b, indicatorLineColour.a);

	cairo_set_line_width(cr, indicatorLineWidth);

	unsigned curSpeed = _speedoInstr.getSpeed();

	// Indicator angle starts from 20 at 0 degrees and increases clockwise to max speed.
	double indicatorAngle = M_PI * ((double)curSpeed - 20.0) / ((double)_maxSpeed - 20.0);

	// Rotate about the "dial centre".
	cairo_identity_matrix(cr);
	cairo_translate(cr, radius, radius);
	cairo_rotate(cr, indicatorAngle);
	cairo_translate(cr, -radius, -radius);

	// Define and draw line.
	cairo_move_to(cr, 0.0, radius);
	cairo_line_to(cr, indicatorLineLength, radius);
	cairo_stroke(cr);

	// Write current speed to precise speed box.

	cairo_identity_matrix(cr);

	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, preciseSpeedFontSize);

	cairo_text_extents_t textExtents;

	string numberText = to_string(curSpeed);
	string extentString;

	// Use a fixed number string to get text extents. This is so the number doesn't jump around.
	if(curSpeed < 10)
	{
		extentString = "5";
	}
	else if(curSpeed < 100)
	{
		extentString = "55";
	}
	else
	{
		extentString = "555";
	}

	cairo_text_extents(cr,  extentString.c_str(), &textExtents);

	cairo_set_source_rgba(cr, preciseSpeedFontColour.r, preciseSpeedFontColour.g, preciseSpeedFontColour.b,
		preciseSpeedFontColour.a);

	double top = radius - textExtents.height / 2.0;
	double left = radius - textExtents.width / 2.0;

	cairo_move_to(cr, left - textExtents.x_bearing, top - textExtents.y_bearing);
	cairo_show_text(cr, numberText.c_str());
}
