#pragma once

#include <string>   //String types, character traits and a set of converting functions
#include <fstream>  //Input/Output Stream Class to operate on files
#include <cstdio>   //Standard Input Output Class

#include "macros.h"
#include "lf_queue.h"
#include "thread_utils.h"
#include "time_utils.h"

namespace Common {
    constexpr size_t LOGS_QUEUE_SIZE = 8 * 1024 * 1024;

    //The different types to be logged
    enum class LogType : int8_t {
        CHAR = 0,
        INTEGER = 1,
        LONG_INTEGER = 2,
        LONG_LONG_INTEGER = 3,
        UNSIGNED_INTEGER = 4,
        UNSIGNED_LONG_INTEGER = 5,
        UNSIGNED_LONG_LONG_INTEGER = 6,
        FLOAT = 7,
        DOUBLE = 8
    };

    //Define the value to be pushed to the Queue
    struct LogElement {
        LogType type_ = LogType::CHAR;
        union {
            char c;
            int i;
            long l;
            long long ll;
            unsigned u;
            unsigned long ul;
            unsigned long long ull;
            float f;
            double d;
        } u_;
    };

    class Logger final {
        private :
            const std::string file_name_;
            std::ofstream file_;

            LFQueue<LogElement> queue_;
            std::atomic<bool> running_ = {true};
            std::thread *logger_thread_ = nullptr;

        public :
            //Empty the queue of log data and write to file.
            // While running_ is true :
            // - consume any new element in the queue
            // - write the element to the file
            auto flushQueue() noexcept {
                while (running_) {
                    for (auto next = queue_.getNextToRead(); queue_.size() && next; next = queue_.getNextToRead()) {
                        switch (next->type_) {
                            case LogType::CHAR:
                                file_ << next->u_.c;
                                break;
                            case LogType::INTEGER:
                                file_ << next->u_.i;
                                break;
                            case LogType::LONG_INTEGER:
                                file_ << next->u_.l;
                                break;
                            case LogType::LONG_LONG_INTEGER:
                                file_ << next->u_.ll;
                                break;
                            case LogType::UNSIGNED_INTEGER:
                                file_ << next->u_.u;
                                break;
                            case LogType::UNSIGNED_LONG_INTEGER:
                                file_ << next->u_.ul;
                                break;
                            case LogType::UNSIGNED_LONG_LONG_INTEGER:
                                file_ << next->u_.ull;
                                break;
                            case LogType::FLOAT:
                                file_ << next->u_.f;
                                break;
                            case LogType::DOUBLE:
                                file_ << next->u_.d;
                                break;
                        }
                        queue_.updateReadIndex();
                    }
                    file_.flush();

                    // Queue is empty -> wait for 10ms and try again
                    using namespace std::literals::chrono_literals;
                    std::this_thread::sleep_for(10ms);
                }
            }

            //Initialize the queue with the propper size, and filename to disk
            explicit Logger(const std::string &file_name)
                    : file_name_(file_name), queue_(LOG_QUEUE_SIZE) {
                //Open Filename
                file_.open(file_name);
                //On error Die!
                ASSERT(file_.is_open(), "Could not open log file:" + file_name);
                //Create and launch the logger (queue) thread.
                // -1 to not set CPU affinity
                logger_thread_ = createAndStartThread(-1, "Common/Logger " + file_name_, [this]() { flushQueue(); });
                //On error Die!
                ASSERT(logger_thread_ != nullptr, "Failed to start Logger thread.");
            }

            ~Logger() {
                std::string time_str;
                std::cerr << Common::getCurrentTimeStr(&time_str) << " Flushing and closing Logger for " << file_name_ << std::endl;

                // Wait until the queue is empty
                while (queue_.size()) {
                    using namespace std::literals::chrono_literals;
                    std::this_thread::sleep_for(1s);
                }

                //Set running_ to false to stop FlushQueue()
                running_ = false;
                //Wait for the longer thread to finish execution.
                logger_thread_->join();

                file_.close();
                std::cerr << Common::getCurrentTimeStr(&time_str) << " Logger for " << file_name_ << " exiting." << std::endl;
            }

            //Overloaded methods to Push data to the Queue
            //Writes to the next location of the queue and increments write index
            auto pushValue(const LogElement &log_element) noexcept {
                *(queue_.getNextToWriteTo()) = log_element;
                queue_.updateWriteIndex();
            }

            auto pushValue(const char value) noexcept {
                pushValue(LogElement{LogType::CHAR, {.c = value}});
            }

            auto pushValue(const int value) noexcept {
                pushValue(LogElement{LogType::INTEGER, {.i = value}});
            }

            auto pushValue(const long value) noexcept {
                pushValue(LogElement{LogType::LONG_INTEGER, {.l = value}});
            }

            auto pushValue(const long long value) noexcept {
                pushValue(LogElement{LogType::LONG_LONG_INTEGER, {.ll = value}});
            }

            auto pushValue(const unsigned value) noexcept {
                pushValue(LogElement{LogType::UNSIGNED_INTEGER, {.u = value}});
            }

            auto pushValue(const unsigned long value) noexcept {
                pushValue(LogElement{LogType::UNSIGNED_LONG_INTEGER, {.ul = value}});
            }

            auto pushValue(const unsigned long long value) noexcept {
                pushValue(LogElement{LogType::UNSIGNED_LONG_LONG_INTEGER, {.ull = value}});
            }

            auto pushValue(const float value) noexcept {
                pushValue(LogElement{LogType::FLOAT, {.f = value}});
            }

            auto pushValue(const double value) noexcept {
                pushValue(LogElement{LogType::DOUBLE, {.d = value}});
            }

            auto pushValue(const char *value) noexcept {
                while (*value) {
                    pushValue(*value);
                    ++value;
                }
            }

            auto pushValue(const std::string &value) noexcept {
                pushValue(value.c_str());
            }

            //Method similar to printf() with % as placeholder.
            //i.e. log("Integer:% String:% Double:%", int_val, str_val, dbl_val);
            template<typename T, typename... A>
            auto log(const char *s, const T &value, A... args) noexcept {
                while (*s) {
                    if (*s == '%') {
                        if (UNLIKELY(*(s + 1) == '%')) { // to allow %% -> % escape character.
                            ++s;
                        } else {
                            pushValue(value); // substitute % with the value specified in the arguments.
                            log(s + 1, args...); // pop an argument and call self recursively.
                            return;
                        }
                    }
                    pushValue(*s++);
                }
                FATAL("extra arguments provided to log()");
            }

            // note that this is overloading not specialization. gcc does not allow inline specializations.
            // Handle the case when no arguments are passed
            auto log(const char *s) noexcept {
                while (*s) {
                    if (*s == '%') {
                        if (UNLIKELY(*(s + 1) == '%')) { // to allow %% -> % escape character.
                            ++s;
                        } else {
                            FATAL("missing arguments to log()");
                        }
                    }
                    pushValue(*s++);
                }
            }

            // Deleted default, copy & move constructors and assignment-operators.
            Logger() = delete;

            Logger(const Logger &) = delete;

            Logger(const Logger &&) = delete;

            Logger &operator=(const Logger &) = delete;

            Logger &operator=(const Logger &&) = delete;
    }
}

