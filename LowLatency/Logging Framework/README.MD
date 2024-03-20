# LOGGING FRAMEWORK

For the logging framework :
1. Create a background logging thread. Unique task is to write log lines to disk. Offload slow disk I/O and string formatting from the main performance-critical thread. Disk operations don't have to be instantaneous.
2. Use multi-threading to create the logging thread.
3. Use lock-free data queue to process log data. The main thread will push data to the queue and the logging thread will consume data from the queue to write to disk.

## Definitions

### time_since_epoch

**epoch** consistent with Unix Time. This is defined as the time duration that has elapsed since 00:00:00 Coordinated Universal Time (UTC), Thursday, 1 January 1970, not counting leap seconds.