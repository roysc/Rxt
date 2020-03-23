#define Rxt_CTORS_MOVE_ONLY(name_)              \
    name_(const name_&) = delete;               \
    name_& operator=(const name_&) = delete;    \
    name_(name_&&) = default;                   \
    name_& operator=(name_&&) = default         \

