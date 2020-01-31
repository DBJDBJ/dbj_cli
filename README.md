# dbj_cli

## Generic modern way to obtain CLI data for C++17 clients

Snazzy C++ code actually using properly a proper, std::vector ctor.

DBJ way is not that good, since it depends on the _UNICODE macro.
```cpp

template <typename CT>
inline auto make_cli_vector = [](CT **_arg_p, std::size_t _argc) {
  return std::vector<std::basic_string<CT>>{_arg_p, _arg_p + _argc};
};

auto cli_data = []() {
  try {
#if _UNICODE
    if (wargv_[0] != nullptr ) // not guaranteed
      return make_cli_vector<wchar_t>(wargv_, argc_);
#else
    if (argv_[0] != nullptr) // not guaranteed
      return make_cli_vector<char>(argv_, argc_);
#endif
    // it is not guaranteed by UCRT when command line arguments will be ready
    throw std::runtime_error( "  Command line arguments are not ready yet");
  } catch (...) {
    throw std::runtime_error( "  dbj cli_data() Has failed..." );
  }
};
```

Steve Wishnousky (@stwish) seems better. But it just hides the same issue with _TCHAR macro.

```cpp
	using vector_tstring_type = std::vector<std::basic_string<_TCHAR> >;
	namespace {

		auto cli_data = []() {
			try {
				return
					vector_tstring_type
				{
					__targv,
					(__targv ?
					__targv + __argc :
					__targv)
				};
			}
			catch (...) {
				throw std::runtime_error(__func__);
			}
		};
	}
```
