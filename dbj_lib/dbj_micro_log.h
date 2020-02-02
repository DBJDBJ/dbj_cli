#pragma once
/*
dbj Run Time lib
*/
#include <ctime>
#include <array>
#include <chrono>
#include <vector>
#include <sstream>
// #include <iostream>
#include <string_view>
#include "../dbj_common.h"

// this will do      std::ios::sync_with_stdio(false);
#define DBJ_LOG_EXCLUSIVE


namespace dbj {

	namespace detail {
#ifdef _UNICODE
		constexpr auto unicode = true;
		using outstream_type = std::wostream;
		using stringbuf_type = std::wstringbuf;
		//	inline auto && COUT = std::wcout;
		constexpr auto LEFT_ANGLED = L'[';
		constexpr auto RGHT_ANGLED = L']';
		constexpr auto SPACE = L' ';
		constexpr auto COMMA = L',';
#else
#error "Just UNICODE builds please :)"
#endif

		constexpr auto bufsiz = BUFSIZ * 2;

		using vector_wstrings_type = std::vector<std::wstring>;
		using vector_strings_type = std::vector<std::string >;

	} // ns

	namespace god_of_time {

		using namespace std;

		constexpr auto BUFSIZE = 1024u;
		typedef array<char, BUFSIZE>  char_buffer;
		typedef std::chrono::time_point<std::chrono::system_clock>  system_time_point;


		inline std::time_t systemtime_now()
		{
			system_time_point system_now = std::chrono::system_clock::now();
			return std::chrono::system_clock::to_time_t(system_now);
		}

		/// <summary>
		/// Apparently this is thread safe
		/// </summary>
		/// <param name="time">our own std::time_t buffer</param>
		/// <returns>std::tm snapshot</returns>
		inline tm localtime(std::time_t time = 0)
		{
			std::tm tm_snapshot;
			if (time == 0) time = systemtime_now();
			localtime_s(&tm_snapshot, (&time));
			return tm_snapshot;
		}

		inline char_buffer timestamp()
		{
			// locale::global(locale(nullptr));
			// time_t t = time(nullptr);
			// std::time_t tt_{};
			const auto lt_ = &localtime();
			char_buffer mbstr;
			if (strftime(mbstr.data(), mbstr.size(), "%c", lt_)) {
				return mbstr;
			}
			throw runtime_error("timestamp() -- strftime has failed");
		}
	} // god_of_time
} // dbj

#pragma region micro logging fwork 
// namespace {

inline
std::ostream& operator<<(std::ostream& os, std::wstring_view s_) {
	os << std::string(
		(char*)(s_.data()),
		(char*)(s_.data() + s_.size())
	).c_str();
	return os;
}

inline
std::wostream& operator<<(std::wostream& os, std::string_view s_) {
	os << std::wstring(
		(wchar_t*)(s_.data()),
		(wchar_t*)(s_.data() + s_.size())
	).c_str();
	return os;
}

template<typename  SEQ >
inline void seq_output(dbj::detail::outstream_type& os, SEQ const & sequence_ ) {
	os << dbj::detail::LEFT_ANGLED << dbj::detail::SPACE;
	for (auto& el : sequence_) { os << el << dbj::detail::SPACE; }
	os << dbj::detail::RGHT_ANGLED;
}
template<typename T>
inline dbj::detail::outstream_type& operator<<(dbj::detail::outstream_type& os, const std::vector<T>& vec) {
	seq_output(os,vec);
	return os;
}

template<typename T, size_t N>
inline dbj::detail::outstream_type& operator<<(dbj::detail::outstream_type& os, const std::array<T,N>& arr) {
	seq_output(os,arr);
	return os;
}

// };

namespace dbj {

	namespace detail {

		struct dbj_log_type final {

			static constexpr bool PIPE_OUT{ true };
			static constexpr bool TIMESTAMP{ false };
			/*
			usage: DBJBuf buff;
			std::ostream stream(&buf);
			stream << 1 << true << L"X" << std::flush ;

			DBJBuf is a stream target. On flush it's method sync()
			is called.

			*/
			mutable struct DBJBuf final : public dbj::detail::stringbuf_type
			{
				// called on stream flush
				virtual int sync() {
					// use this->str() here
					auto string_trans = this->str();

					if (dbj_log_type::PIPE_OUT) {
						// if one wants to pipe/redirect the console output
						auto rez = ::_putws(string_trans.data());
						_ASSERTE(EOF != rez);
						(void)rez;
					}

					_RPT0(_CRT_WARN, string_trans.data());
					// clear the buffer afterwards
					this->str(L"");
					return 1 /*true*/;
				}
			} buffer_{};
			// mutable  DBJBuf buffer_{};

		public:

			typedef dbj::detail::outstream_type   stream_type;

			stream_type& stream() noexcept {
				static stream_type single_log_instance__stream_(&buffer_);
				return single_log_instance__stream_;
			}

			void flush() {
				stream_type& stream_ = this->stream();
				_ASSERTE(stream_.good());
				stream_.flush();
			}

			dbj_log_type() {
#ifdef DBJ_LOG_EXCLUSIVE
				std::ios::sync_with_stdio(false);
#endif
				this->flush();
			}

			~dbj_log_type() {
				static bool flushed = false;
				try {
					if (!flushed) {
						this->flush();
						flushed = true;
						// by hand once more
						// for a good measure
						buffer_.sync(); 
					}
				}
				catch (...) {
#if _DEBUG
					DBJ_TRACE(__func__, " Unknown exception");
#endif
				}
			}

		}; // dbj_log_type

		inline dbj_log_type single_log_instance_{}; // dbj_log_type instance made here


	} // detail nspace

		/*
	just print to the stream as ever
	*/
	inline auto print = [](auto ... param)
	{
		constexpr auto no_of_args = sizeof...(param);
		auto& os = detail::single_log_instance_.stream();

		if constexpr (no_of_args > 0) {

			if (detail::dbj_log_type::TIMESTAMP)
				os << "\n" << dbj::god_of_time::timestamp().data() << " : ";

			volatile char dummy[no_of_args] = {
				((
					os << param
				  ), 0)...
			};
			// remove the annoying warning : unused variable 'dummy' [-Wunused-variable]
			(void)dummy;
		}
		os << std::flush; // provokes output from dbj buffer that is os target
#ifndef __clang__
		return print;
#endif
	};

} // dbj

#pragma endregion

#pragma region standard header suffix 
#pragma comment( user, __FILE__ "(c) 2017...2020 by dbj@dbj.org | Version: " __DATE__ __TIME__ ) 

/*
Copyright 2017-2020 by dbj@dbj.org  CC BY SA 4.0
*/
#pragma endregion
