# dbj_cli

## CLI arguments for C++ clients

#### The History (2017)

I somehow stumbled upon snazzy C++ code, actually just using properly a proper, std::vector ctor in combination with comand line arguments that every terminal, console or gui app contains.

My way is not that good, since it depends on the _UNICODE macro.
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

Steve Wishnousky (@stwish) solution, seems better. But it just hides the same issue with _TCHAR macro.

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

#### The Solution 2020

Probably it might be impossible to solve the logic, when any one of the two might be nullptr or not. So. Why do not we try and receive 
std vector results from both? And this leads to a simple code.
```cpp
	struct cli_data_rettype final 
	{
		using wide_type = std::vector<std::wstring>;
		using narrow_type = std::vector<std::string >;

		wide_type wide{};
		narrow_type narrow{};
	};

	inline cli_data_rettype cli_almost_certain() 
	{
		cli_data_rettype retval{};

		if (wargv_)
			retval.wide = cli_data_rettype::wide_type{ wargv_, wargv_ + argc_ };

		if (argv_)
			retval.narrow = cli_data_rettype::narrow_type{ argv_, argv_ + argc_ };

		return retval;
	}
```
And simple but efective usage

```cpp

	auto [ wide_cl_args, narrow_cl_args   ] = dbj::cli_almost_certain();

	if (wide_cl_args.size())
		dbj::detail::print(w_cli); // vector printing routine

	if (narrow_cl_args.size())
		dbj::detail::print(n_cli);

```
## The 'final solution'
It is only that ["over elaborated"](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0939r4.pdf) std::string is not required at all.
```cpp
///-----------------------------------------------------------------------------///
///	(c) 2020 by dbj.org, CC BY SA 4.0
struct cli_arguments_vectors final
{
	using wide_type = std::vector<wchar_t* >;
	using narrow_type = std::vector<char*  >;

	wide_type wide{};
	narrow_type narrow{};
};

inline cli_arguments_vectors cli_arguments()
{
	cli_arguments_vectors retval{};

	if (wargv_)
		retval.wide = cli_arguments_vectors::wide_type( __wargv, __wargv + argc_ );
    
	if (argv_)
		retval.narrow = cli_arguments_vectors::narrow_type( __argv, __argv + argc_ );

	return retval;
}
```
#### Caution
Of course, calling `cli_arguments()` might be happening anytime, including before and after main. Thus both `__argv` and `__wargv` might be `nullptr`.
Which will produce two empty vectors.
