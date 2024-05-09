#pragma once
#include<string>
enum class ErrorStatus
{
    Ok,
    Error,
    Debug,
};
class Status
{
public:
    ~Status()
    {
        if (nullptr != status_)
        {
            delete status_;
        }
    }
    Status(const Status &other)
    {
        if (status_ == other.status_)
        {
            return;
        }
        if (nullptr != other.status_)
        {
            status_ = new InternalStatus;
            status_->code_ = other.status_->code_;
            status_->info_ = other.status_->info_;
        }
    };
    Status(Status &&other)
    {
        if (nullptr != other.status_)
        {
            std::swap(other.status_, this->status_);
        }
    }
    Status &operator=(const Status &other)
    {
        if (status_ == other.status_)
        {
            return *this;
        }
        if (nullptr != other.status_)
        {
            status_ = new InternalStatus;
            status_->code_ = other.status_->code_;
            status_->info_ = other.status_->info_;
        }
        return *this;
    }
    Status &operator=(Status &&other)
    {
        if (nullptr != other.status_)
        {
            std::swap(other.status_, this->status_);
        }
        return *this;
    }
    std::string to_string()
    {
        if (nullptr == status_)
        {
            return std::string("Just OK");
        }
        return std::to_string(int(status_->code_)) +"->"+ status_->info_;
    }
    Status &append(const std::string &info)
    {
        if (nullptr == status_)
        {
            status_ = new InternalStatus();
        }
        status_->info_.append(info);
        return *this;
    }
    bool isOk(){
        if(status_ == nullptr || status_->code_ == ErrorStatus::Ok){
            return true;
        }
        return false;
    }
#define DEFINE_CODE(CODE)                       \
    static Status CODE(const std::string &info) \
    {                                           \
        Status s;                               \
        s.set_code(ErrorStatus::CODE);          \
        s.set_info(info);                       \
        return s;                               \
    }                                           \
    static Status CODE()                        \
    {                                           \
        return Status();                        \
    }                                           \
    static Status CODE(std::string &&info)      \
    {                                           \
        Status s;                               \
        s.set_code(ErrorStatus::CODE);          \
        s.set_info(std::move(info));            \
        return s;                               \
    }
    DEFINE_CODE(Ok)
    DEFINE_CODE(Debug)
    DEFINE_CODE(Error)

#undef DEFINE_CODE
private:
    void set_code(ErrorStatus code)
    {
        if (nullptr == status_)
        {
            status_ = new InternalStatus;
        }
        status_->code_ = code;
    }
    void set_info(const std::string &info)
    {
        if (nullptr == status_)
        {
            status_ = new InternalStatus;
        }
        status_->info_ = info;
    }
    void set_info(std::string &&info)
    {
        if (nullptr == status_)
        {
            status_ = new InternalStatus;
        }
        status_->info_ = std::move(info);
    }
    Status() : status_{nullptr} {};
    struct InternalStatus
    {
        InternalStatus() : code_(ErrorStatus::Ok){};
        ErrorStatus code_;
        std::string info_;
    };
    InternalStatus *status_{nullptr};
};


#define CHECK_STATUS(exp)\
do{\
    Status s = (exp);\
    if(!status.isOk()){\
        return s;\
    }\
}while(false);


#define DISALLOW_COPY_AND_ASSIGNMENT_CONSTRUCTOR(CLASS) \
    CLASS(const CLASS &) = delete;                      \
    CLASS(CLASS &&) = delete;                           \
    CLASS &operator=(const CLASS &) = delete;           \
    CLASS &operator=(CLASS &&) = delete;


using ID = int32_t;

