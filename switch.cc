#include "switch.h"

SwitchBase* CreateSwitchObject() {
  return new EthernetSwitch();
}