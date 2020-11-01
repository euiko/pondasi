#include "json.hpp"
#include <bits/stdint-intn.h>

namespace pondasi::json::detail {

void marshal(JsonWriter& writer, bool i) {
  writer.Bool(i);
}
void marshal(JsonWriter& writer, uint i) {
  writer.Uint(i);
}
void marshal(JsonWriter& writer, uint64_t i) {
  writer.Uint64(i);
}
void marshal(JsonWriter& writer, int i) {
  writer.Int(i);
}
void marshal(JsonWriter& writer, int64_t i) {
  writer.Int64(i);
}
void marshal(JsonWriter& writer, double d) {
  writer.Double(d);
}

}