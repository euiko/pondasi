#include "simple_app.hpp"

namespace pondasi {

bool SimpleApp::run() { return run_fn_(); }
bool SimpleApp::stop() { return true; }
}  // namespace pondasi