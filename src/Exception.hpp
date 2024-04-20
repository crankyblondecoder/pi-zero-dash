#ifndef PZD_EXCEPTION_H
#define PZD_EXCEPTION_H

#include <string>

using namespace std;

namespace piZeroDash
{
	class Exception
	{
		public:

			enum Error
			{
				UNKNOWN,
				/** Gauge count has exceeded maximum. */
				TOO_MANY_GAUGES
			};

			virtual ~Exception(){}

			/**
			 * @param error Error code.
			 * @param description Description of error.
			 */
			Exception(Error error, string& description);

			/**
			 * Get the error code associated with this exception.
			 */
			Error getErrorCode() const;

			/**
			 * Get the description of the error associated with this exception.
			 */
			const string& getErrorDescr() const;

		private:

			/** Error code. */
			Error _error;

			/** Description of error. */
			string _descr;
	};
}

#endif