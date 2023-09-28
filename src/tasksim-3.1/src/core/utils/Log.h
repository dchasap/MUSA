/*
 * Copyright (c) 2015, Barcelona Supercomputing Center
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer; redistributions in binary form
 * must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with
 * the distribution; neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CORE_UTILS_LOG_H_
#define CORE_UTILS_LOG_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

// Remove possible interference from nanos' debug.hpp
// Include it here to set its define guards and make sure it's not coming back to bite us in the ass later.
// The general rule should be to include all nanox dependencies before tasksim headers, in which case this is harmless.
#include <debug.hpp>

#undef warning
#undef warning0
#undef verbose
#undef verbose0
#undef debug
#undef debug0
#undef message
#undef message0


#ifndef SIMLOG_LEVEL
#define SIMLOG_LEVEL 0
#endif

namespace sim {
namespace utils {

class Log
{
public:
	enum Level {ERROR, WARNING, INFO, DEBUG, DEBUG2, DEBUG3, DEBUG4};

private:
	static const Level ReportingLevel = static_cast<Level>(SIMLOG_LEVEL);

	static std::unique_ptr<std::ostream> out_;

	/** Class doing nothing, obviously enough for the compiler to remove any NOPLog() << "message";
	 */
	class NOPLog
	{
	public:
		inline NOPLog() {}
		inline ~NOPLog() {}

		template <typename T>
		inline NOPLog &operator <<(T const& t) { return *this; }
	};



	/** Class wrapping an ostringstream and a static ostream&, building a string with all the << it is given
	 *  and then atomically printing everything to the ostream when the destructor is called.
	 */
	template <Level L>
	class OSSLog
	{
	protected:
		std::ostringstream os;

	public:
		inline OSSLog()
		{
			os << "[LOG LEVEL=" << L << "] ";
		}

		inline ~OSSLog()
		{
			os << std::endl;
			(out_ ? *out_ : std::cerr) << os.str();
		}

		template <typename T>
		inline OSSLog &operator <<(T const& t)
		{
			os << t;
			return *this;
		}
	};


public:
	// Log::some_level is either of NOPLog or OSSLog based on logging settings: 0 only ERROR, etc.
	typedef typename std::conditional<ReportingLevel >= DEBUG4 , OSSLog<DEBUG4> , NOPLog>::type debug4;
	typedef typename std::conditional<ReportingLevel >= DEBUG3 , OSSLog<DEBUG3> , NOPLog>::type debug3;
	typedef typename std::conditional<ReportingLevel >= DEBUG2 , OSSLog<DEBUG2> , NOPLog>::type debug2;
	typedef typename std::conditional<ReportingLevel >= DEBUG  , OSSLog<DEBUG>  , NOPLog>::type debug;
	typedef typename std::conditional<ReportingLevel >= INFO   , OSSLog<INFO>   , NOPLog>::type info;
	typedef typename std::conditional<ReportingLevel >= WARNING, OSSLog<WARNING>, NOPLog>::type warning;
	typedef typename std::conditional<ReportingLevel >= ERROR  , OSSLog<ERROR>  , NOPLog>::type error;

	// for dynamorio's C-style prints
	static inline bool should_print(Level verbosity) {
		return ReportingLevel >= verbosity;
	}
};


}  // namespace utils
}  // namespace sim

#endif  // CORE_UTILS_LOG_H_
