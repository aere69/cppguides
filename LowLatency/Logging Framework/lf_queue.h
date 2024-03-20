#pragma once

#include <iostream>     //Standard Input Output Streams
#include <vector>       //Vector container class
#include <atomic>       //types that encapsulate a value whose access is guaranteed 
                        //to not cause data races and can be used to synchronize 
                        //memory accesses among different threads.

#include "macros.h"

namespace Common {
    template<typename T>
    class LFQueue final {   // Lock-Free Queue
        private :
            std::vector<T> store_;      //Queue of data

            //atomic (thread safe) members because reading and writing can be 
            //performed from different threads
            std::atomic<size_t> next_write_index_ = {0};    //Next element to be written to
            std::atomic<size_t> next_read_index_ = {0};     //Next element to be read

            std::atomic<size_t> num_elements_ = {0};
        public :
            LFQueue(std::size_t num_elements){
                store_(num_elements, T()); //Pre-allocated vector storage in heap.
            }

            //--- Add elements to de Queue ---

            //Return a pointer to the next element to write new data to.
            auto getNextToWriteTo() noexcept {
                return &store_[next_write_index_];
            }

            //Increment the write index after a record has been written.
            auto updateWriteIndex() noexcept {
                next_write_index_ = (next_write_index_ + 1) % store_.size();
                num_elements_++;
            }

            //-- END - Add elements ---

            //-- Consume elements from the Queue ---

            // Pointer to the next element to consume from the Queue
            // Returns null if there is no element to be consumed
            auto getNextToRead() noexcept {
                return (size() ? &store_[next_read_index_] : nullptr)
            }

            //Update the read index after a record has been consumed
            auto updateReadIndex() noexcept {
                next_read_index_ = (next_read_index_ + 1) % store_.size();
                ASSERT(num_elements_ != 0, "Read an invalid element in : " std::to_string(pthread_self()));
                num_elements_--;
            }

            //-- END - Add elements ---

            // Return the number of elements in the Queue
            auto size() const noexcept {
                return num_elements_.load();    //Return num_elements_ contained value
            }

            // Delete default, copy & move constructors and assignment operators.
            LFQueue() = delete;

            LFQueue(const LFQueue &) = delete;

            LFQueue(const LFQueue &&) = delete;

            LFQueue &operator=(const LFQueue &) = delete;

            LFQueue &operator=(const LFQueue &&) = delete;
    }
}