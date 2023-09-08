# 生成库
list(APPEND _sources
    # 进程相关
    process.cc
    # 线程以及线程安全相关模块
    thread/cond_var.cc
    thread/mutex.cc
    thread/sem.cc
    thread/shared_mutex.cc
    # 日志模块
    log/record.cc
    log/format.cc
    log/append.cc
    log/file_append.cc
    log/stream_append.cc
    log/log.cc
    log/log_stream.cc
    # 网络模块
    network/buffer.cc
    network/address.cc
    network/socket.cc
    # 基础模块
    noncopyable.h
    config.h
    define_config.h
    swapbyte.h
    type_traits.h
)

add_library(yf_obj OBJECT ${_sources})

# 设置位置无关属性
set_target_properties(yf_obj
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
)

# 添加库依赖
target_link_libraries(yf_obj pthread)

# 生成动态库和静态库
add_library(yf_static STATIC $<TARGET_OBJECTS:yf_obj>)
add_library(yf_shared SHARED $<TARGET_OBJECTS:yf_obj>)
