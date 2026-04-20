#pragma once

/*
    auto postTask = []<typename Func>(Func&& func) {
        using return_type = typename std::invoke_result<Func>::type;

        auto task{
            std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<Func>(func))

                                                                    )};

        std::future<return_type> taskFuture = task->get_future();

        std::thread taskThread{[task]() {
            (*task)();
        }};

        taskThread.detach();

        return taskFuture;
    };
*/

#include <functional>
#include <future>
#include <memory>
#include <type_traits>

template <typename Func, typename... Args>
auto DoTask(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>> {
    using returnType = std::invoke_result_t<Func, Args...>;

    // 直接在 Lambda 里捕获参数，不再使用 std::bind
    // 我们用 std::tuple 存参数，或者直接按值/引用捕获
    auto task = std::make_shared<std::packaged_task<returnType()>>(
        [f = std::forward<Func>(func), ... args = std::forward<Args>(args)]() mutable {
            return std::invoke(std::move(f), std::move(args)...);
        });

    auto future = task->get_future();

    std::thread([task]() {
        (*task)();
    }).detach();

    return future;
}
