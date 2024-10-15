- When entering the "steering" mode the app seems to flush the UDP buffer by sending 1000 zeros...?
- IPv6 support, currently all examples are IPv4 only
- On Linux system, use socat to listen to UDP stuff:  
  sudo socat UDP4-LISTEN:21105,fork,reuseaddr UDP4:192.168.0.111:21105
- Useful wireshark filter:  
  udp && (ip.dst==192.168.0.111 || ip.src==192.168.0.111)
- Users may now overwrite SystemState by implementing
  ```cpp
  virtual SystemState& systemState()
  ```
  This allows to mix implicit and explicit state changes to the users liking. If this function does not get overridden, then the libraries defaults will be used and commands like `LAN_X_SET_TRACK_POWER_ON` simply set or clear their corresponding flags. If this function does get overridden however, users may update any flags to whatever.
- `System::trackPower(bool on)`, `System::stop()` as well as `Programming::cvRead(uint16_t cv_addr)` and `Programming::cvWrite(uint16_t cv_addr, uint8_t byte)` should probably return a bool to indicate success? This would help in accidentally sending any broadcast replies although some command might have failed.

####
Typical CV process for LAN_X_CV_READ:
- LAN_X_CV_READ
- LAN_X_BC_PROGRAMMING_MODE
- LAN_X_CV_RESULT
- CV8 read
- LAN_X_SET_TRACK_POWER_ON

After all service mode read accesses there is a CV8 read (LAN_X_CV_READ->LAN_X_CV_RESULT).
LAN_X_CV_READ and LAN_X_CV_WRITE implicitly turn on track voltage!

Typical CV process for LAN_X_CV_WRITE:
- LAN_X_CV_WRITE
- LAN_X_BC_PROGRAMMING_MODE
- LAN_X_CV_RESULT (ACK with same value)
- LAN_X_SET_TRACK_POWER_ON

Typical CV process for LAN_X_CV_POM_READ_BYTE:
- CV8 read
- LAN_X_CV_POM_READ_BYTE (the actual one)
- LAN_X_CV_RESULT

Typical CV process for LAN_X_CV_POM_WRITE_BYTE:
- CV8 read
- LAN_X_CV_POM_WRITE_BYTE

Prior all POM accesses there is a CV8 read (LAN_X_CV_POM_READ_BYTE->LAN_X_CV_RESULT).
POM accesses do not implicitly turn on track voltage!