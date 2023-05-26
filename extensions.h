/**
 *  Extensions for CProcessing
 *  @author  : Maks Makuta
 *  @version : 1.0 beta 2
 *
 *  Note: include AFTER CProcessing header
 */

bool in(float a, float b, float val) { return val >= a && val <= b; }

bool inRect(float x, float y, float w, float h, float a, float b) {
  return in(x, x + w, a) && in(y, y + h, b);
}

bool inRect(float x, float y, float w, float h, float r, float a, float b) {
  PVector m(a, b);

  PVector c1(x + r, y + r);
  PVector c2(x + r, y + h - r);
  PVector c3(x + w - r, y + h - r);
  PVector c4(x + w - r, y + r);

  return c1.dist(m) <= r | c2.dist(m) <= r | c3.dist(m) <= r | c4.dist(m) <= r |
         inRect(x + r, y, w - 2 * r, h, a, b) |
         inRect(x, y + r, w, h - 2 * r, m.x, m.y);
}
bool inRect(float r1x, float r1y, float r1w, float r1h, float r2x, float r2y,
            float r2w, float r2h) {
  if (r1x + r1w >= r2x && r1x <= r2x + r2w && r1y + r1h >= r2y &&
      r1y <= r2y + r2h) {
    return true;
  }
  return false;
}
