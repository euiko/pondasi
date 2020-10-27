#include "simple_app.hpp"

namespace pondasi {

bool SimpleApp::start() { return run_fn_(); }
bool SimpleApp::stop() { return true; }
}  // namespace pondasi