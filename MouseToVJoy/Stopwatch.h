////////////////////////////////////////////////////////////////////////////////
//
// Stopwatch.h  -- A simple stopwatch implementation, based on Windows
//                 high-performance timers.
//                 Can come in handy when measuring elapsed times of
//                 portions of C++ code.
//
// Copyright (C) 2016 by Giovanni Dicanio <giovanni.dicanio@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include <crtdbg.h>     // For _ASSERTE
#include <Windows.h>    // For high-performance timers


namespace win32
{


	//------------------------------------------------------------------------------
	// Class to measure time intervals, for benchmarking portions of code.
	// It's a convenient wrapper around the Win32 high-resolution timer APIs
	// QueryPerformanceCounter() and QueryPerformanceFrequency().
	//------------------------------------------------------------------------------
	class Stopwatch
	{
	public:
		// Initialize the stopwatch to a safe initial state
		Stopwatch() noexcept;

		// Clear the stopwatch state
		void reset() noexcept;

		// Start measuring time.
		// When finished, call Stop().
		// Can call ElapsedTime() also before calling Stop(): in this case,
		// the elapsed time is measured since the Start() call.
		void start() noexcept;

		// Stop measuring time.
		// Call ElapsedMilliseconds() to get the elapsed time from the Start() call.
		void stop() noexcept;

		// Return elapsed time interval duration, in milliseconds.
		// Can be called both after Stop() and before it. 
		// (Start() must have been called to initiate time interval measurements).
		double elapsedMilliseconds() const noexcept;


		//
		// Ban copy
		//
	private:
		Stopwatch(const Stopwatch&) = delete;
		Stopwatch& operator=(const Stopwatch&) = delete;


		//
		// *** IMPLEMENTATION ***
		//
	private:
		bool _running;                 // is the timer running?
		long long _start;              // start tick count
		long long _finish;             // end tick count
		const long long _frequency;    // cached frequency value

										//
										// According to MSDN documentation:
										// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644905(v=vs.85).aspx
										//
										// The frequency of the performance counter is fixed at system boot and 
										// is consistent across all processors. 
										// Therefore, the frequency need only be queried upon application 
										// initialization, and the result can be cached.
										//

										// Wrapper to Win32 API QueryPerformanceCounter()
		static long long counter() noexcept;

		// Wrapper to Win32 API QueryPerformanceFrequency()
		static long long frequency() noexcept;

		// Calculate elapsed time in milliseconds,
		// given a start tick and end tick counts.
		double elapsedMilliseconds(long long start, long long finish) const noexcept;
	};



	//
	// Inline implementations
	//


	inline Stopwatch::Stopwatch() noexcept
		: _running{ false }
		, _start{ 0 }
		, _finish{ 0 }
		, _frequency{ frequency() }
	{}


	inline void Stopwatch::reset() noexcept
	{
		_finish = _start = 0;
		_running = false;
	}


	inline void Stopwatch::start() noexcept
	{
		_running = true;
		_finish = 0;

		_start = counter();
	}


	inline void Stopwatch::stop() noexcept
	{
		_finish = counter();
		_running = false;
	}


	inline double Stopwatch::elapsedMilliseconds() const noexcept
	{
		if (_running)
		{
			const long long current{ counter() };
			return elapsedMilliseconds(_start, current);
		}

		return elapsedMilliseconds(_start, _finish);
	}


	inline long long Stopwatch::counter() noexcept
	{
		LARGE_INTEGER li;
		::QueryPerformanceCounter(&li);
		return li.QuadPart;
	}


	inline long long Stopwatch::frequency() noexcept
	{
		LARGE_INTEGER li;
		::QueryPerformanceFrequency(&li);
		return li.QuadPart;
	}


	inline double Stopwatch::elapsedMilliseconds(long long start, long long finish) const noexcept
	{
		_ASSERTE(start >= 0);
		_ASSERTE(finish >= 0);
		_ASSERTE(start <= finish);

		return ((finish - start) * 1000.0) / _frequency;
	}


} // namespace win32
