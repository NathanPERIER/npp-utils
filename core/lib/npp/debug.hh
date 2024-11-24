
#pragma once


namespace npp {

#if defined(NPP_DEBUG_FEATURES) || !defined(NDEBUG)
constexpr const bool debug_mode = true;
#else
constexpr const bool debug_mode = false;
#endif

} // namespace npp
