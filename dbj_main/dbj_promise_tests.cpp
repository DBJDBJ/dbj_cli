#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <chrono>

#include "../dbj_lib/dbj_micro_log.h"
namespace {
	void accumulate(std::vector<int>::iterator first,
		std::vector<int>::iterator last,
		std::promise<int> accumulate_promise)
	{
		int sum = std::accumulate(first, last, 0);
		accumulate_promise.set_value(sum);  // Notify future
	}

	void do_work(std::promise<void> barrier)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		barrier.set_value();
	}

	extern "C" int promise_tests(int argc, wchar_t * argv[])
	{
		using namespace std;
		// Demonstrate using promise<int> to transmit a result between threads.
		vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
		promise<int> accumulate_promise;
		future<int> accumulate_future = accumulate_promise.get_future();
		thread work_thread(accumulate, numbers.begin(), numbers.end(),
			move(accumulate_promise));
		accumulate_future.wait();  // wait for result
		dbj::print(__func__ ,  " -- result = ", accumulate_future.get(), '\n');
		work_thread.join();  // wait for thread completion

	// Demonstrate using promise<void> to signal state between threads.
		promise<void> barrier;
		future<void> barrier_future = barrier.get_future();
		thread new_work_thread(do_work, move(barrier));
		barrier_future.wait();
		new_work_thread.join();

		return EXIT_SUCCESS;
	}
}